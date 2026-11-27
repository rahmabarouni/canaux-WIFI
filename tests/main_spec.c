#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "graph.h"
#include "greedy.h"
/* Compare greedy_seq, greedy_omp (naif) et greedy_spec. Une ligne CSV:
   fichier,n,threads,t_seq,t_omp,t_spec,k_seq,k_omp,k_spec,valide
   REPS (variable d'env) = nombre de repetitions, 20 par defaut. */
int main(int argc, char **argv) {
    if (argc < 3) return 1;
    Graph *g = graph_load(argv[1]);
    if (!g) return 1;
    int p = atoi(argv[2]);
    omp_set_num_threads(p);
    const char *e = getenv("REPS");
    int reps = e ? atoi(e) : 20;
    int *color = malloc(g->n * sizeof(int));
    int ks = 0, ko = 0, kp = 0, ok = 1;
    double ts = 0, to = 0, tp = 0, t;
    greedy_seq(g, NULL, color);   /* echauffement (cache) */
    for (int r = 0; r < reps; r++) {
        t = omp_get_wtime(); ks = greedy_seq(g, NULL, color);
        ts += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);
        t = omp_get_wtime(); ko = greedy_omp(g, NULL, color);
        to += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);
        t = omp_get_wtime(); kp = greedy_spec(g, color);
        tp += omp_get_wtime() - t; ok &= is_valid_coloring(g, color);
    }
    printf("%s,%d,%d,%.6f,%.6f,%.6f,%d,%d,%d,%d\n", argv[1], g->n, p,
           ts / reps, to / reps, tp / reps, ks, ko, kp, ok);
    free(color); graph_free(g);
    return 0;
}
