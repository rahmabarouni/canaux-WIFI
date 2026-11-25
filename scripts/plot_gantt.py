# Fichier: plot_gantt.py. Il dessine le diagramme de Gantt.
# Il lit un csv de taches et montre qui travaille sur chaque thread.
# Appelant: l utilisateur avec python scripts/plot_gantt.py entree.csv sortie.png.

# Usage: python scripts/plot_gantt.py entree.csv sortie.png
import csv, sys
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

src, out = sys.argv[1], sys.argv[2]
rows = list(csv.DictReader(open(src)))   # on lit le csv ecrit par gantt_dump
tot = {}   # duree totale par valeur de K
for r in rows:
    tot[r["K"]] = tot.get(r["K"], 0) + float(r["end"]) - float(r["start"])
K = max(tot, key=tot.get)               # le K le plus couteux
# on garde le K qui a pris le plus de temps
sel = [r for r in rows if r["K"] == K]
t0 = min(float(r["start"]) for r in sel)
t1 = max(float(r["end"]) for r in sel)
threads = sorted({int(r["thread"]) for r in sel})   # ensemble des threads qui ont travaille

fig, ax = plt.subplots(figsize=(10, 1.5 + len(threads)))
for r in sel:
    s = float(r["start"]) - t0
    d = float(r["end"]) - float(r["start"])
    ax.broken_barh([(s, d)], (int(r["thread"]) - 0.4, 0.8), edgecolor="black", linewidth=0.2)   # une barre = une tache sur un thread
ax.set_yticks(threads)
ax.set_xlabel("temps (s)"); ax.set_ylabel("thread")
ax.set_title(f"Gantt backtracking OpenMP (K={K}, {len(threads)} threads, {len(sel)} taches)")
fig.savefig(out, dpi=150)

span = t1 - t0
print(f"K={K} duree={span:.3f}s taches={len(sel)}")
for t in threads:
    busy = sum(float(r["end"]) - float(r["start"]) for r in sel if int(r["thread"]) == t)   # temps occupe par ce thread
    print(f"thread {t}: occupe {busy:.3f}s sur {span:.3f}s ({100*busy/span:.0f}%)")
