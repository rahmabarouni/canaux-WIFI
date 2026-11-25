// Fichier: greedy_seq.c. Il colorie le graphe avec la methode gloutone.
// Il parcourt les sommets un par un et prend la premiere couleur libre.
// Appelant: main.c via greedy_seq.

#include <stdlib.h>
#include "greedy.h"

// Recoit graphe, ordre et color. Retourne le nombre de couleurs. Prend la 1ere couleur libre.
int greedy_seq(const Graph *g, const int *order, int *color) {
    int n = g->n;
    for (int i = 0; i < n; i++) color[i] = -1;       // -1 = pas colorié
        // -1 ya3ni pas encore colorie

    int *forbidden = malloc((n + 1) * sizeof(int));
        // forbidden[c]=1 ya3ni couleur c mawjouda 3and un voisin, donc mamnou3a

    for (int i = 0; i < n; i++) {
        int v = order ? order[i] : i;

        for (int c = 0; c <= n; c++) forbidden[c] = 0;
        // on remet tout a 0 avant chaque sommet

        // couleurs déjà prises par les voisins
        // on regarde seulement les voisins deja colories
        for (int u = 0; u < n; u++)
            if (g->adj[v][u] && color[u] != -1)
                forbidden[color[u]] = 1;

        // première couleur libre
        // on avance jusqu a la 1ere couleur libre
        int c = 0;
        while (forbidden[c]) c++;
        color[v] = c;
    }
    free(forbidden);
    // resultat = max + 1
    return count_colors(g, color);
}
