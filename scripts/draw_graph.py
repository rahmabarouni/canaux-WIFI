# Usage: python scripts/draw_graph.py data/app_graph.txt results/plots/graph_app.png
# Dessine le graphe en cercle, colorie par le glouton (gauche) et par le backtracking exact (droite).
# hadi l script yrsm l graphe f dira, khater l couleur tban b 3in w l resultat kayfham b surzi (darija tunisienne)
# nlaoun b glouton w b backtracking bech nchouf l farq: l heuristique t3ti ch7al, w l exact ya3ni l chi s7i7 (darija tunisienne)
import sys, math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

src, out = sys.argv[1], sys.argv[2]
with open(src) as f:
    n, m = map(int, f.readline().split())
    edges = [tuple(map(int, f.readline().split())) for _ in range(m)]
adj = [[] for _ in range(n)]
for u, v in edges:
    adj[u].append(v); adj[v].append(u)

def greedy():
    col = [-1] * n
    for v in range(n):
        used = {col[u] for u in adj[v] if col[u] != -1}
        c = 0
        while c in used: c += 1
        col[v] = c
    return col

def exact():
    for K in range(1, n + 1):
        col = [-1] * n
        def solve(v):
            if v == n: return True
            for c in range(K):
                if all(col[u] != c for u in adj[v]):
                    col[v] = c
                    if solve(v + 1): return True
                    col[v] = -1
            return False
        if solve(0): return col

pos = [(math.cos(2 * math.pi * i / n), math.sin(2 * math.pi * i / n)) for i in range(n)]
palette = ["tab:red", "tab:green", "tab:blue", "gold", "tab:purple", "tab:orange", "tab:cyan", "tab:pink"]
fig, axes = plt.subplots(1, 2, figsize=(11, 5))
for ax, (title, col) in zip(axes, [("Glouton", greedy()), ("Exact (backtracking)", exact())]):
    for u, v in edges:
        ax.plot([pos[u][0], pos[v][0]], [pos[u][1], pos[v][1]], "k-", lw=0.8, zorder=1)
    for i in range(n):
        ax.scatter(pos[i][0], pos[i][1], s=500, color=palette[col[i] % len(palette)], edgecolor="black", zorder=2)
        ax.text(pos[i][0], pos[i][1], str(i), ha="center", va="center", zorder=3)
    ax.set_title(f"{title}: {max(col) + 1} couleurs")
    ax.axis("off"); ax.set_aspect("equal")
fig.savefig(out, dpi=150)
print(out, "glouton:", greedy(), "exact:", exact())
