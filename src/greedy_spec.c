// greedy_spec.c : glouton parallele speculatif. Appelant: tests/main_spec.c
// Idee: phase 1 on colorie TOUS les sommets a faire en parallele (conflits possibles),
// phase 2 on detecte les conflits, on recommence seulement avec les sommets en conflit.
#include <stdlib.h>
#include <omp.h>
#include "greedy.h"

int greedy_spec(const Graph *g, int *color) {
    int n = g->n;
    for (int i = 0; i < n; i++) color[i] = -1;
    int *todo = malloc(n * sizeof(int));   // sommets a colorier dans cette ronde
    int *conf = malloc(n * sizeof(int));   // sommets en conflit (prochaine ronde)
    int ntodo = n;
    for (int i = 0; i < n; i++) todo[i] = i;

    while (ntodo > 0) {
        // PHASE 1: une seule region parallele pour toute la ronde
        #pragma omp parallel
        {
            int *forb = malloc((n + 1) * sizeof(int));   // forb prive a chaque thread
            #pragma omp for schedule(static)
            for (int i = 0; i < ntodo; i++) {
                int v = todo[i];
                for (int c = 0; c <= n; c++) forb[c] = 0;
                for (int u = 0; u < n; u++) {
                    int cu;
                    #pragma omp atomic read   // un autre thread peut ecrire color[u]
                    cu = color[u];
                    if (g->adj[v][u] && cu != -1) forb[cu] = 1;
                }
                int c = 0;
                while (forb[c]) c++;
                #pragma omp atomic write
                color[v] = c;
            }
            free(forb);
        }

        // PHASE 2: detecter les conflits (lecture seule de color)
        int nconf = 0;
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < ntodo; i++) {
            int v = todo[i];
            for (int u = 0; u < v; u++) {   // seul le sommet d'index le plus grand se recolorie
                if (g->adj[v][u] && color[u] == color[v]) {
                    int k;
                    #pragma omp atomic capture   // index unique dans conf
                    k = nconf++;
                    conf[k] = v;
                    break;
                }
            }
        }
        // reset sequentiel des sommets en conflit, puis nouvelle ronde
        for (int i = 0; i < nconf; i++) { color[conf[i]] = -1; todo[i] = conf[i]; }
        ntodo = nconf;
    }
    free(todo); free(conf);
    return count_colors(g, color);
}
