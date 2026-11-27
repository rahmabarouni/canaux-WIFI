# Usage: python scripts/table.py   (lit results/times.csv, affiche S(p) et E(p))
import csv
rows = list(csv.DictReader(open("results/times.csv")))
print("fichier,p,seq,omp,S,E")
for r in rows:
    ts = float(r["bt_seq"]); tp = float(r["bt_omp"]); p = int(r["threads"])
    if ts < 0.01 or tp == 0: continue
    print(f'{r["fichier"]},{p},{ts:.3f},{tp:.3f},{ts/tp:.2f},{ts/tp/p:.2f}')
