// Fichier: graph.c. Il charge le graphe depuis un fichier texte.
// La matrice d adjacence est une table n x n: adj[u][v]=1 si arete.
// Appelant: main.c via graph_load et graph_free.

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// Recoit un nom de fichier. Retourne un Graph* ou NULL. Lit n, m et les aretes.
Graph *graph_load(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen"); return NULL; }

    Graph *g = malloc(sizeof(Graph));
    if (fscanf(f, "%d %d", &g->n, &g->m) != 2) { fclose(f); free(g); return NULL; }

    g->adj = malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
        g->adj[i] = calloc(g->n, sizeof(int));   // calloc = tout à 0
        // calloc met tous les 0, donc aucune arete au depart

    for (int i = 0; i < g->m; i++) {
        int u, v;
        if (fscanf(f, "%d %d", &u, &v) != 2) break;
        // matrice symetrique: on marque les deux sens
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;   // graphe non orienté = symétrique
    }
    fclose(f);
    return g;
}

// Recoit un Graph*. Ne retourne rien. Libere la memoire du graphe.
void graph_free(Graph *g) {
    for (int i = 0; i < g->n; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}

// Recoit le graphe et un sommet v. Retourne son degre. Compte ses voisins.
int graph_degree(const Graph *g, int v) {
    int d = 0;
    for (int u = 0; u < g->n; u++) d += g->adj[v][u];
    return d;
}

// Vérifie qu'aucune arête n'a ses 2 extrémités de même couleur
// Recoit le graphe et color. Retourne 1 si aucun voisin n a la meme couleur.
int is_valid_coloring(const Graph *g, const int *color) {
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            // adj[u][v]=1 et meme couleur = coloriage faux
            if (g->adj[u][v] && color[u] == color[v]) return 0;
    return 1;
}

// Recoit color. Retourne le nombre de couleurs utilisees (max + 1).
int count_colors(const Graph *g, const int *color) {
    int max = -1;
    for (int i = 0; i < g->n; i++)
        if (color[i] > max) max = color[i];
    // max=-1 au depart, donc on ajoute 1
    return max + 1;
}

// Recoit le graphe et color. Ne retourne rien. Affiche chaque sommet avec sa couleur.
void print_coloring(const Graph *g, const int *color) {
    for (int i = 0; i < g->n; i++)
        printf("Sommet %d -> couleur %d\n", i, color[i]);
}
