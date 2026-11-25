// Fichier: graph.h. Il definit la struct Graph et les fonctions du graphe.
// La matrice adj[u][v]=1 ya3ni il y a une arete entre u et v.
// Appelant: graph.c, main.c et les algorithmes qui lisent le graphe.

#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int n;          // nombre de sommets
    int m;          // nombre d'arêtes
    int **adj;      // matrice d'adjacence n x n
} Graph;

// Recoit un nom de fichier. Retourne un Graph* ou NULL. Lit n, m et les aretes.
Graph *graph_load(const char *filename);
// Recoit un Graph*. Ne retourne rien. Libere la memoire du graphe.
void   graph_free(Graph *g);
// Recoit le graphe et un sommet v. Retourne son degre. Compte ses voisins.
int    graph_degree(const Graph *g, int v);
// Recoit le graphe et color. Retourne 1 si aucun voisin n a la meme couleur.
int    is_valid_coloring(const Graph *g, const int *color);
// Recoit color. Retourne le nombre de couleurs utilisees (max + 1).
int    count_colors(const Graph *g, const int *color);
// Recoit le graphe et color. Ne retourne rien. Affiche chaque sommet avec sa couleur.
void   print_coloring(const Graph *g, const int *color);

#endif
