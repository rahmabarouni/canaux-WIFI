# Usage: python scripts/plot_size.py  (lit results/naive_vs_improved.csv, ecrit results/plots/temps_taille_naif.png)
# Courbe: temps du backtracking naif vs ameliore en fonction de n (echelle log).
# TIMEOUT>60 est trace a 60 s ; les temps nuls (< 1 ms, resolution du chronometre) sont traces a 1 ms.
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
rows = list(csv.DictReader(open("results/naive_vs_improved.csv")))
rows = [r for r in rows if int(r["n"]) >= 12]
rows.sort(key=lambda r: int(r["n"]))
n = [int(r["n"]) for r in rows]
def val(s):
    if s.startswith("TIMEOUT"): return 60.0
    return max(float(s), 0.001)
naif = [val(r["naif_s"]) for r in rows]
amel = [val(r["ameliore_s"]) for r in rows]
plt.figure()
plt.plot(n, naif, marker="o", label="naif (60 s = arret)")
plt.plot(n, amel, marker="s", label="ameliore (< 1 ms trace a 1 ms)")
plt.yscale("log"); plt.xlabel("n sommets"); plt.ylabel("temps (s)")
plt.legend(); plt.grid(True)
plt.savefig("results/plots/temps_taille_naif.png", dpi=150)
for a, b, c in zip(n, naif, amel): print(a, b, c)
