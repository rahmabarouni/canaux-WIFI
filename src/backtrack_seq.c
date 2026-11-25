// Fichier: backtrack_seq.c. Backtracking exact pour trouver chi.
// Il essaie les couleurs, et quand ca bloque il fait un retour en arriere.
// Appelant: main.c via chromatic_seq et colorable_seq.

#include <stdlib.h>
#include "backtracking.h"

// Recoit graphe, color, v et c. Retourne 1 si aucun voisin n a deja la couleur c.
int is_safe(const Graph *g, const int *color, int v, int c) {
    for (int u = 0; u < g->n; u++)
        if (g->adj[v][u] && color[u] == c) return 0;
    return 1;
}

// Recoit graphe et order. Ne retourne rien. Met devant le sommet avec le plus de liens.
/* Ordre : le sommet qui a le plus de voisins deja places, puis le plus grand degre. */
void compute_order(const Graph *g, int *order) {
    int n = g->n;
    int *used = calloc(n, sizeof(int));
    int *links = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        int best = -1;
        for (int v = 0; v < n; v++) {
            // used[v]=1 ya3ni v est deja dans l ordre
            if (used[v]) continue;
            if (best == -1 || links[v] > links[best] ||
               (links[v] == links[best] && graph_degree(g, v) > graph_degree(g, best)))
                best = v;
        }
        order[i] = best;
        used[best] = 1;
        // links[v] = nb de voisins deja places
        for (int u = 0; u < n; u++) if (g->adj[best][u]) links[u]++;
    }
    free(used); free(links);
}

// Recoit graphe, K, color, order, idx et maxused. Retourne 1 si succes. Essaie puis revient en arriere.
/* maxused = plus grande couleur deja utilisee (-1 au debut).
   Cassure de symetrie : on n'essaie que les couleurs 0..maxused+1. */
static int solve(const Graph *g, int K, int *color, const int *order, int idx, int maxused) {
    if (idx == g->n) return 1;   // tous les sommets sont colories
    int v = order[idx];
    int limit = maxused + 1;   // maxused = plus grande couleur deja utilisee, -1 au debut
    if (limit > K - 1) limit = K - 1;   // on ne depasse pas K-1
    // cassure de symetrie: on n essaie que 0..maxused+1
    for (int c = 0; c <= limit; c++) {
        if (is_safe(g, color, v, c)) {
            color[v] = c;
            if (solve(g, K, color, order, idx + 1, c > maxused ? c : maxused)) return 1;
            color[v] = -1;   // retour en arriere: on annule et on essaie la couleur suivante
        }
    }
    return 0;
}

// Recoit graphe, K et color. Retourne 1 si K couleurs suffisent. Initialise tout a -1.
int colorable_seq(const Graph *g, int K, int *color) {
    int *order = malloc(g->n * sizeof(int));
    compute_order(g, order);
    for (int i = 0; i < g->n; i++) color[i] = -1;
    int r = solve(g, K, color, order, 0, -1);
    free(order);
    return r;
}

// Recoit graphe et color. Retourne chi. K-1 a echoue, donc le premier K est minimal.
int chromatic_seq(const Graph *g, int *color) {
    for (int K = 1; K <= g->n; K++)
        if (colorable_seq(g, K, color)) return K;
    return g->n;
}
