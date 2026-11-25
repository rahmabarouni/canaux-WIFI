# Fichier: gen_graph.py. Il genere un graphe Wi-Fi aleatoire.
# Deux bornes proches ont une arete car elles peuvent interferer.
# Appelant: run_bench.ps1 ou l utilisateur depuis la ligne de commande.

# Graphe Wi-Fi aléatoire : bornes placées au hasard sur un plan,
# arête si deux bornes sont à distance < rayon (interférence).
# Usage: python scripts/gen_graph.py n rayon seed sortie.txt
import sys, random, math
n = int(sys.argv[1]); r = float(sys.argv[2]); seed = int(sys.argv[3]); out = sys.argv[4]
random.seed(seed)   # meme graine = meme graphe, pour refaire les experiences
pts = [(random.random(), random.random()) for _ in range(n)]   # chaque borne a une position (x,y)
edges = [(i, j) for i in range(n) for j in range(i + 1, n)
         if math.dist(pts[i], pts[j]) < r]   # arete si distance < rayon, donc interference
with open(out, "w") as f:
    f.write(f"{n} {len(edges)}\n")   # format lu par graph_load: n m puis les aretes
    for u, v in edges:
        f.write(f"{u} {v}\n")
print(f"{out}: n={n} m={len(edges)}")   # petit resume affiche dans le terminal
