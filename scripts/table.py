# Usage: python scripts/table.py   (lit results/times.csv, affiche S(p) et E(p))
# Ts = moyenne de bt_seq sur tous les nombres de threads du meme graphe.
import csv
rows = list(csv.DictReader(open("results/times.csv")))
ts_mean = {}
for r in rows:
    ts_mean.setdefault(r["fichier"], []).append(float(r["bt_seq"]))
ts_mean = {k: sum(v) / len(v) for k, v in ts_mean.items()}
print("fichier,p,Ts_moyen,omp,S,E")
for r in rows:
    ts = ts_mean[r["fichier"]]; tp = float(r["bt_omp"]); p = int(r["threads"])
    if ts < 0.01 or tp == 0: continue
    print(f'{r["fichier"]},{p},{ts:.3f},{tp:.3f},{ts/tp:.2f},{ts/tp/p:.2f}')
