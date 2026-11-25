# Fichier: plot.py. Il lit times.csv et dessine 4 courbes.
# Il trace temps, speedup, efficacite et temps selon la taille.
# Appelant: l utilisateur avec python scripts/plot.py.

# Usage: python scripts/plot.py   (lit results/times.csv, ecrit results/plots/*.png)
import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

rows = list(csv.DictReader(open("results/times.csv")))   # on lit toutes les mesures du csv
graphs = []
for r in rows:
    if r["fichier"] not in graphs: graphs.append(r["fichier"])
os.makedirs("results/plots", exist_ok=True)   # on cree le dossier des images si besoin

# Recoit un nom de fichier graphe. Retourne ses lignes triees par threads.
def rows_of(g):
    return sorted([r for r in rows if r["fichier"] == g], key=lambda r: int(r["threads"]))

# 1. temps en fonction des threads
plt.figure()
for g in graphs:
    rs = rows_of(g)
    plt.plot([int(r["threads"]) for r in rs], [float(r["bt_omp"]) for r in rs], marker="o", label=os.path.basename(g))
plt.xlabel("threads"); plt.ylabel("temps backtracking (s)"); plt.yscale("log"); plt.legend(); plt.grid(True)
plt.savefig("results/plots/temps_threads.png", dpi=150)

# 2 et 3. speedup et efficacite (Ts = bt_seq mesure avec 1 thread)
plt.figure()
for g in graphs:
    rs = rows_of(g)
    ts = float(rs[0]["bt_seq"])
    if ts < 0.01: continue   # trop petit pour etre significatif
    # si c est trop petit, le speedup ne veut rien dire
    p = [int(r["threads"]) for r in rs]
    s = [ts / float(r["bt_omp"]) for r in rs]
    plt.plot(p, s, marker="o", label=os.path.basename(g))
plt.plot([1, 2, 4, 8], [1, 2, 4, 8], "k--", label="ideal S(p)=p")   # droite de reference S(p)=p
plt.xlabel("threads"); plt.ylabel("speedup S(p)"); plt.legend(); plt.grid(True)
plt.savefig("results/plots/speedup.png", dpi=150)

plt.figure()
for g in graphs:
    rs = rows_of(g)
    ts = float(rs[0]["bt_seq"])
    if ts < 0.01: continue
    p = [int(r["threads"]) for r in rs]
    e = [ts / float(r["bt_omp"]) / int(r["threads"]) for r in rs]
    plt.plot(p, e, marker="o", label=os.path.basename(g))
plt.xlabel("threads"); plt.ylabel("efficacite E(p)"); plt.legend(); plt.grid(True)
plt.savefig("results/plots/efficacite.png", dpi=150)

# 4. temps en fonction de la taille du graphe (8 threads)
plt.figure()
sel = sorted([r for r in rows if r["threads"] == "8"], key=lambda r: int(r["n"]))
plt.plot([int(r["n"]) for r in sel], [float(r["bt_seq"]) for r in sel], marker="o", label="seq")
plt.plot([int(r["n"]) for r in sel], [float(r["bt_omp"]) for r in sel], marker="s", label="omp 8 threads")
plt.xlabel("n sommets"); plt.ylabel("temps (s)"); plt.yscale("log"); plt.legend(); plt.grid(True)
plt.savefig("results/plots/temps_taille.png", dpi=150)
print("OK: 4 courbes dans results/plots/")   # confirmation dans le terminal
