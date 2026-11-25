// Fichier: greedy.h. Il declare le coloriage glouton seq et omp.
// Appelant: main.c et les fichiers greedy_seq.c / greedy_omp.c.
// order mawjoud bech on change l ordre des sommets, NULL = ordre 0,1,2...

#ifndef GREEDY_H
#define GREEDY_H
#include "graph.h"
// order = ordre de parcours des sommets (NULL = 0,1,2,...)
// Recoit graphe, ordre (ou NULL) et color. Retourne le nb de couleurs. Remplit color.
int greedy_seq(const Graph *g, const int *order, int *color);
// Memes entrees que greedy_seq. Retourne nb couleurs. Parallelise l examen des voisins.
int greedy_omp(const Graph *g, const int *order, int *color);
#endif
