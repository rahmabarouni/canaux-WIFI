# Coloriage de graphes (glouton + backtracking, sequentiel et OpenMP)
Projet ISIMa - Complexite algorithmique / Calcul parallele. Application: affectation de canaux Wi-Fi.

## Structure
- include/, src/ : code C (graph, greedy_seq/omp/spec, backtrack_seq/omp)
- tests/ : main_naive.c (backtracking naif), main_spec.c (bench glouton)
- scripts/ : gen_graph.py, run_bench.ps1, plot.py, table.py, plot_gantt.py, draw_graph.py, trace.py, plot_greedy.py
- data/ : example5.txt, app_graph.txt (Wi-Fi ISIMa), random/ (graphes generes)
- results/ : times.csv, greedy_bench.csv, naive_vs_improved.csv, plots/
- report/ : ancienne version naive du backtracking

## Compilation et execution (Windows)
mingw32-make
.\coloring.exe data/app_graph.txt 4        (fichier, nombre de threads)
Variables d'environnement: REPS (repetitions), CUTOFF (profondeur de creation des taches, 12 par defaut), SKIP_BT=1 (saute le backtracking).

## Executables de test
gcc -O2 -fopenmp -Iinclude tests/main_naive.c report/backtrack_seq_naive.c src/graph.c -o naive.exe
gcc -O2 -fopenmp -Iinclude tests/main_spec.c src/graph.c src/greedy_seq.c src/greedy_omp.c src/greedy_spec.c -o spec.exe
.\spec.exe data/random/big4000.txt 4        (glouton: seq, omp naif, speculatif)

## Format d'un graphe
Premiere ligne: n m. Puis m lignes: u v (une arete).

## Benchmarks
python scripts/gen_graph.py n rayon seed sortie.txt
powershell -File scripts/run_bench.ps1 ; python scripts/plot.py ; python scripts/table.py
Gantt: .\coloring.exe graphe p csv fichier.csv puis python scripts/plot_gantt.py fichier.csv sortie.png

## Resultats principaux
- app_graph: glouton 4 canaux, exact 3 (chi=3).
- Backtracking ameliore (ordre + symetrie): s24 de 8,1 s a moins de 1 ms.
- Backtracking OpenMP: jusqu'a 3,9x avec 8 threads (CUTOFF=12).
- Glouton parallele: naif plus lent que le sequentiel; speculatif jusqu'a 1,5x.
