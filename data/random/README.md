# Graphes aleatoires

Les graphes de ce dossier sont des graphes geometriques (modele Wi-Fi): n points tires au hasard dans un carre unite, une arete entre deux points si leur distance est inferieure a un rayon.

Generation:
python scripts/gen_graph.py n rayon seed sortie.txt
Exemple: python scripts/gen_graph.py 120 0.12 1 data/random/g120.txt
Meme seed = meme graphe (reproductible).

Familles de fichiers:
- g30, g60, g120, g150, g200, g250, g300 : graphes de tailles croissantes (g120 sert au benchmark principal)
- s12, s16, s20, s24 : petits graphes pour comparer backtracking naif et ameliore
- c_<n>_<rayon>_<seed> : candidats du scan des graphes difficiles (scripts/find_hard.ps1)
- big2000, big4000, big6000 : grands graphes pour le glouton (lancer avec SKIP_BT=1)

Format: premiere ligne "n m", puis m lignes "u v" (sommets numerotes de 0 a n-1).
