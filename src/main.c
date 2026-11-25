// Fichier: main.c. C est le point d entree du programme.
// Il lit un graphe, lance glouton et backtracking, et mesure le temps.
// Appelant: le systeme quand on tape ./coloring.exe fichier [threads] [csv] [gantt].

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "graph.h"
#include "greedy.h"
#include "backtracking.h"

#define REPS 3

// Recoit argv: fichier graphe, nb threads, csv optionnel, fichier Gantt optionnel. Retourne 0. Lance les 4 algorithmes.
int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);   // affichage direct, utile si on coupe apres timeout
    if (argc < 2) { printf("Usage: %s fichier_graphe [threads] [csv]\n", argv[0]); return 1; }
    Graph *g = graph_load(argv[1]);
    if (!g) return 1;
    int threads = (argc >= 3) ? atoi(argv[2]) : 1;
    int csv = (argc >= 4);   // si 4e arg present, on affiche une ligne csv
    omp_set_num_threads(threads);   // nombre de threads demande par l utilisateur
    int *color = malloc(g->n * sizeof(int));
    double t, ts_g = 0, tp_g = 0, ts_b = 0, tp_b = 0;
    int kg = 0, kgp = 0, chi = 0, chip = 0, ok = 1;

    for (int r = 0; r < REPS; r++) {   // on repete REPS fois et on fait la moyenne
        t = omp_get_wtime(); kg = greedy_seq(g, NULL, color);
        ts_g += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);

        t = omp_get_wtime(); kgp = greedy_omp(g, NULL, color);
        tp_g += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);

        t = omp_get_wtime(); chi = chromatic_seq(g, color);
        ts_b += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);

        t = omp_get_wtime(); chip = chromatic_omp(g, color);
        tp_b += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);
    }
    if (argc >= 5) gantt_dump(argv[4]);   // 5e arg = ou ecrire le journal Gantt
    ts_g /= REPS; tp_g /= REPS; ts_b /= REPS; tp_b /= REPS;

    if (csv) {
        /* fichier,n,m,threads,greedy_seq,greedy_omp,bt_seq,bt_omp,k_greedy,chi */
        printf("%s,%d,%d,%d,%.6f,%.6f,%.6f,%.6f,%d,%d\n", argv[1], g->n, g->m,
               threads, ts_g, tp_g, ts_b, tp_b, kg, chi);
    } else {
        printf("[Glouton seq] couleurs=%d temps=%.6fs\n", kg, ts_g);
        printf("[Glouton omp] couleurs=%d temps=%.6fs (%d threads)\n", kgp, tp_g, threads);
        printf("[Backtr. seq] chi=%d temps=%.6fs\n", chi, ts_b);
        printf("[Backtr. omp] chi=%d temps=%.6fs (%d threads)\n", chip, tp_b, threads);
        printf("Tous colorings valides: %d | chi seq == chi omp: %d\n", ok, chi == chip);   // seq et omp doivent donner le meme chi
    }
    free(color); graph_free(g);
    return 0;
}
