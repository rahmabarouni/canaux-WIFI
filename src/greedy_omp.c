// Fichier: greedy_omp.c. Version OpenMP du glouton.
// La boucle principale reste seq, seul l examen des voisins est parallelise.
// Appelant: main.c via greedy_omp.

#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "greedy.h"

/* Glouton OpenMP : la boucle principale reste séquentielle (dépendance entre
   sommets), seul l'examen des voisins est parallélisé. */
// Recoit graphe, ordre et color. Retourne nb couleurs. Boucle principale seq.
int greedy_omp(const Graph *g, const int *order, int *color) {
    int n = g->n;
    for (int i = 0; i < n; i++) color[i] = -1;
    int *forbidden = malloc((n + 1) * sizeof(int));

    for (int i = 0; i < n; i++) {   // la boucle sur i reste seq: couleur de v depend des deja traites
        int v = order ? order[i] : i;
        memset(forbidden, 0, (n + 1) * sizeof(int));
        // forbidden[c]=1 ya3ni couleur c mawjouda 3and un voisin, donc mamnou3a

        /* color, adj, v : partagés en lecture seule. u : privé (indice de boucle).
           forbidden : partagé, écriture protégée par atomic pour éviter la race condition. */
        #pragma omp parallel for schedule(static)   // seul l examen des voisins est parallelise
        for (int u = 0; u < n; u++) {
            if (g->adj[v][u] && color[u] != -1) {
                #pragma omp atomic write   // atomic write evite la race condition sur forbidden
                forbidden[color[u]] = 1;
            }
        }

        int c = 0;
        while (forbidden[c]) c++;   // on prend la 1ere couleur libre
        color[v] = c;   // decision prise apres que tous les voisins sont fahsouhom
    }
    free(forbidden);
    return count_colors(g, color);
}
