#include <stdlib.h>
#include "backtracking.h"

// La couleur c est-elle permise pour v ? (aucun voisin déjà colorié avec c)
int is_safe(const Graph *g, const int *color, int v, int c) {
    for (int u = 0; u < g->n; u++)
        if (g->adj[v][u] && color[u] == c) return 0;
    return 1;
}

static int solve(const Graph *g, int K, int *color, int v) {
    if (v == g->n) return 1;                 // tous coloriés = succès

    for (int c = 0; c < K; c++) {
        if (is_safe(g, color, v, c)) {
            color[v] = c;
            if (solve(g, K, color, v + 1)) return 1;
            color[v] = -1;                   // BACKTRACK: on annule
        }
    }
    return 0;                                // impasse
}

int colorable_seq(const Graph *g, int K, int *color) {
    for (int i = 0; i < g->n; i++) color[i] = -1;
    return solve(g, K, color, 0);
}

int chromatic_seq(const Graph *g, int *color) {
    for (int K = 1; K <= g->n; K++)
        if (colorable_seq(g, K, color)) return K;   // 1er K qui marche = χ(G)
    return g->n;
}
