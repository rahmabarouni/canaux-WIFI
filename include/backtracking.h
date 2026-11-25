// Fichier: backtracking.h. Il declare le backtracking seq et omp.
// Appelant: main.c, backtrack_seq.c et backtrack_omp.c.
// Il declare aussi gantt_dump bech on ecrit le journal des taches.

#ifndef BACKTRACKING_H
#define BACKTRACKING_H
#include "graph.h"
// Recoit graphe, color, v et c. Retourne 1 si c est permise pour v.
int is_safe(const Graph *g, const int *color, int v, int c);
// Recoit graphe et order. Ne retourne rien. Range les sommets du plus contraint au moins contraint.
void compute_order(const Graph *g, int *order);
// Recoit graphe, K et color. Retourne 1 si K couleurs suffisent. Remplit color.
int colorable_seq(const Graph *g, int K, int *color);
// Recoit graphe et color. Retourne chi. Teste K=1,2,3... jusqu au premier qui marche.
int chromatic_seq(const Graph *g, int *color);
// Recoit graphe, K et color. Retourne 1 si K suffisent. Utilise des taches OpenMP.
int colorable_omp(const Graph *g, int K, int *color);
// Recoit graphe et color. Retourne chi. Cherche le plus petit K en parallele.
int chromatic_omp(const Graph *g, int *color);
// Recoit un nom de fichier. Ne retourne rien. Ecrit le journal des taches pour le Gantt.
void gantt_dump(const char *file);
#endif
