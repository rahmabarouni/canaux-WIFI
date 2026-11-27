# Usage: python scripts/trace.py data/example5.txt
# Affiche la trace d'execution du glouton puis du backtracking (ordre naturel 0,1,2...).
# hadi l script ywari l trace mta3 glouton w backtracking, khater l trace kayfhamlek lin kaykharej couleur w lin kayrja3 l werra (darija tunisienne)
# nebda men 0 w nemchi w7ed w7ed m3a l ordre mta3 les sommets, khater l trace kamlha tji mretba (darija tunisienne)
import sys
src = sys.argv[1]
with open(src) as f:
    n, m = map(int, f.readline().split())
    edges = [tuple(map(int, f.readline().split())) for _ in range(m)]
adj = [[] for _ in range(n)]
for u, v in edges:
    adj[u].append(v); adj[v].append(u)

print("=== TRACE GLOUTON ===")
col = [-1] * n
for v in range(n):
    used = sorted({col[u] for u in adj[v] if col[u] != -1})
    c = 0
    while c in used: c += 1
    col[v] = c
    print(f"sommet {v}: voisins {adj[v]}, couleurs interdites {used} -> couleur {c}")
print("resultat:", col, "| couleurs:", max(col) + 1)

def bt(K):
    col = [-1] * n
    def solve(v, d):
        if v == n:
            print("  " * d + "SUCCES"); return True
        for c in range(K):
            if all(col[u] != c for u in adj[v]):
                col[v] = c
                print("  " * d + f"sommet {v} <- couleur {c}")
                if solve(v + 1, d + 1): return True
                col[v] = -1
                print("  " * d + f"retour en arriere: sommet {v} perd la couleur {c}")
            else:
                print("  " * d + f"sommet {v}: couleur {c} refusee (un voisin l'a deja)")
        print("  " * d + f"impasse au sommet {v}")
        return False
    return solve(0, 0), col

for K in range(1, n + 1):
    print(f"=== TRACE BACKTRACKING K={K} ===")
    ok, col = bt(K)
    if ok:
        print("chi =", K, "| coloration:", col); break
    print(f"K={K} impossible")
