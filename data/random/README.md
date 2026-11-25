# Graphes aléatoires

Ce dossier contient les graphes générés par `scripts/gen_graph.py` (modèle $G(n,p)$).

Exemple :

```bash
python3 scripts/gen_graph.py -n 12 -p 0.35 -s 7 -o data/random/g12_p35_s7.txt
python3 scripts/gen_graph.py -n 16 -p 0.25 -s 11 -o data/random/g16_p25_s11.txt
python3 scripts/gen_graph.py -n 20 -p 0.20 -s 23 -o data/random/g20_p20_s23.txt
```

Le nom de fichier encode : nombre de sommets `n`, probabilité `p` et graine `seed`.
`run_bench.sh` parcourt automatiquement tous les fichiers `.txt` présents ici.
