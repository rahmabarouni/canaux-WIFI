#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "graph.h"
#include "backtracking.h"

/* Programme de test: lance SEULEMENT le backtracking naif (ancienne version).
   Sortie: chi,temps */
int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    if (argc < 2) return 1;
    Graph *g = graph_load(argv[1]);
    if (!g) return 1;
    int *color = malloc(g->n * sizeof(int));
    double t = omp_get_wtime();
    int chi = chromatic_seq(g, color);
    printf("%d,%.6f\n", chi, omp_get_wtime() - t);
    free(color); graph_free(g);
    return 0;
}
