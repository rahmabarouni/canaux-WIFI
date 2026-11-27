# Usage: python scripts/plot_greedy.py  (lit results/greedy_bench.csv, ecrit results/plots/greedy_*.png)
# Ts = moyenne des t_seq sur tous les threads (le seq ne depend pas de p, on reduit le bruit).
import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
rows = list(csv.DictReader(open("results/greedy_bench.csv")))
graphs = []
for r in rows:
    if r["fichier"] not in graphs: graphs.append(r["fichier"])
os.makedirs("results/plots", exist_ok=True)
def rs(g): return sorted([r for r in rows if r["fichier"] == g], key=lambda r: int(r["threads"]))
print("fichier,p,Ts,T_omp,T_spec,S_omp,S_spec,E_spec")
fig1, a1 = plt.subplots(); fig2, a2 = plt.subplots()
for g in graphs:
    r = rs(g)
    ts = sum(float(x["t_seq"]) for x in r) / len(r)
    p = [int(x["threads"]) for x in r]
    so = [ts / float(x["t_omp"]) for x in r]
    sp = [ts / float(x["t_spec"]) for x in r]
    name = os.path.basename(g)
    a1.plot(p, sp, marker="o", label=name + " spec")
    a1.plot(p, so, marker="x", linestyle=":", label=name + " naif")
    a2.plot(p, [s / q for s, q in zip(sp, p)], marker="o", label=name)
    for i, x in enumerate(r):
        print(f"{name},{p[i]},{ts:.5f},{float(x['t_omp']):.5f},{float(x['t_spec']):.5f},{so[i]:.2f},{sp[i]:.2f},{sp[i]/p[i]:.2f}")
a1.plot([1, 2, 4, 8], [1, 2, 4, 8], "k--", label="ideal S(p)=p")
a1.set_xlabel("threads"); a1.set_ylabel("speedup glouton"); a1.legend(fontsize=7); a1.grid(True)
a2.set_xlabel("threads"); a2.set_ylabel("efficacite glouton spec"); a2.legend(); a2.grid(True)
fig1.savefig("results/plots/greedy_speedup.png", dpi=150)
fig2.savefig("results/plots/greedy_efficacite.png", dpi=150)
