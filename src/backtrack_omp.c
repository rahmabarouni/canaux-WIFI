// Fichier: backtrack_omp.c. Backtracking exact avec taches OpenMP.
// Chaque branche de l arbre est une tache avec sa propre copie du tableau.
// Appelant: main.c via chromatic_omp et gantt_dump.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "backtracking.h"

/* ---- Journal des taches (pour le diagramme de Gantt) ---- */
#define GCAP 200000
typedef struct { int thread, K; double start, end; } TaskLog;
static TaskLog glog[GCAP];
static int gcount = 0;
static double gt0 = 0;
static int cutoff = 8;   /* profondeur max de creation de taches (variable d'env CUTOFF) */
    // cutoff = profondeur max avant de continuer en seq, on change avec CUTOFF

// Ne recoit rien. Ne retourne rien. Remet le compteur de taches a zero.
static void gantt_reset(void) { gcount = 0; gt0 = omp_get_wtime(); }

// Recoit K, debut et fin. Ne retourne rien. Ajoute une ligne dans le journal.
static void gantt_add(int K, double s, double e) {
    int i;
    #pragma omp atomic capture   // chaque tache prend un index unique, sinon race condition
    i = gcount++;
    if (i < GCAP) {
        glog[i].thread = omp_get_thread_num();
        glog[i].K = K;
        glog[i].start = s - gt0;
        glog[i].end = e - gt0;
    }
}

// Recoit un nom de fichier. Ne retourne rien. Ecrit le csv du Gantt.
void gantt_dump(const char *file) {
    FILE *f = fopen(file, "w");   // on ecrit le csv utilise par plot_gantt.py
    if (!f) return;
    fprintf(f, "thread,K,start,end\n");
    int n = gcount < GCAP ? gcount : GCAP;
    for (int i = 0; i < n; i++)
        fprintf(f, "%d,%d,%.6f,%.6f\n", glog[i].thread, glog[i].K, glog[i].start, glog[i].end);
    fclose(f);
}

/* ---- Backtracking parallele ---- */
// Recoit un pointeur vers found. Retourne sa valeur avec atomic read.
static int read_found(int *found) {
    int f;
    #pragma omp atomic read   // lecture sure pendant que d autres threads ecrivent
    f = *found;
    return f;
}

// Recoit graphe, K, color, order, idx, maxused et found. Retourne 1 si solution. Backtracking seq.
static int solve_local(const Graph *g, int K, int *color, const int *order,
                       int idx, int maxused, int *found) {
    if (read_found(found)) return 0;   // si found deja 1, on arrete cette branche
    if (idx == g->n) return 1;
    int v = order[idx];
    int limit = maxused + 1;   // maxused = plus grande couleur deja utilisee
    if (limit > K - 1) limit = K - 1;
    for (int c = 0; c <= limit; c++) {
        if (is_safe(g, color, v, c)) {
            color[v] = c;
            if (solve_local(g, K, color, order, idx + 1, c > maxused ? c : maxused, found)) return 1;
            color[v] = -1;   // retour en arriere local sur la copie de la tache
        }
    }
    return 0;
}

// Recoit graphe, color, found et result. Ne retourne rien. Garde la 1ere solution trouvee.
static void record(const Graph *g, int *color, int *found, int *result) {
    #pragma omp critical(record_solution)   // critical = un seul thread ecrit result et found
    {
        if (!*found) {
            memcpy(result, color, g->n * sizeof(int));
            #pragma omp atomic write   // atomic write pose found=1 pour arreter les autres
            *found = 1;
        }
    }
}

/* Chaque appel possede son tableau 'color' (copie privee). */
// Recoit graphe, K, color, order, idx, maxused, depth, found et result. Cree des taches.
static void solve_par(const Graph *g, int K, int *color, const int *order,
                      int idx, int maxused, int depth, int *found, int *result) {
    if (read_found(found)) return;   // found=1 ya3ni deja solution, on sort
    if (idx == g->n) { record(g, color, found, result); return; }
    if (depth >= cutoff) {   // apres cutoff on continue en seq pour eviter trop de taches
        double s = omp_get_wtime();
        if (solve_local(g, K, color, order, idx, maxused, found)) record(g, color, found, result);
        gantt_add(K, s, omp_get_wtime());
        return;
    }
    int v = order[idx];
    int limit = maxused + 1;   // symetrie: on n essaie que 0..maxused+1
    if (limit > K - 1) limit = K - 1;
    for (int c = 0; c <= limit; c++) {
        if (is_safe(g, color, v, c)) {
            int *copy = malloc(g->n * sizeof(int));
            // memcpy = nensakh el tableau bech kol task ykoun 3andha copie mte3ha
            memcpy(copy, color, g->n * sizeof(int));
            copy[v] = c;   // chaque branche met sa couleur dans sa copie
            int nm = c > maxused ? c : maxused;
            /* copy, nm, idx, depth : firstprivate (propres a la tache).
               g, K, order, found, result : donnees partagees (lecture seule ou protegees). */
            #pragma omp task firstprivate(copy, nm, idx, depth)   // task = khedma sghira ta5ouha ay thread libre
            {
                solve_par(g, K, copy, order, idx + 1, nm, depth + 1, found, result);
                free(copy);
            }
            // firstprivate donne une copie de copy, nm, idx et depth a la tache
        }
    }
    #pragma omp taskwait   /* attend la fin des taches enfants (different de la protection des acces) */
        // taskwait attend les enfants, mais ne protege pas les donnees partagees
}

// Recoit graphe, K et color. Retourne 1 si K suffisent. Un seul thread lance la racine.
int colorable_omp(const Graph *g, int K, int *color) {
    int found = 0;   // found=1 ya3ni on a deja trouve un coloriage
    const char *e = getenv("CUTOFF");
    cutoff = e ? atoi(e) : 8;   // on peut changer cutoff avec la variable CUTOFF
    int *order = malloc(g->n * sizeof(int));
    int *init = malloc(g->n * sizeof(int));
    compute_order(g, order);
    for (int i = 0; i < g->n; i++) init[i] = -1;

    #pragma omp parallel   // parallel ouvre l equipe de threads
    {
        #pragma omp single   /* un seul thread lance la racine */
        // single = un seul thread lance l arbre, les autres attendent les taches
        solve_par(g, K, init, order, 0, -1, 0, &found, color);
    }
    free(init); free(order);
    return found;
}

// Recoit graphe et color. Retourne chi. Teste K croissants en parallele.
int chromatic_omp(const Graph *g, int *color) {
    gantt_reset();
    for (int K = 1; K <= g->n; K++)   // premier K qui marche = chi, K-1 a echoue
        if (colorable_omp(g, K, color)) return K;
    return g->n;
}
