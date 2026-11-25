# Fichier: run_bench.ps1. Il lance les mesures sur tous les graphes.
# Il ecrit une ligne par graphe et par nombre de threads dans times.csv.
# Appelant: l utilisateur avec powershell -File scripts/run_bench.ps1.

$graphs = "data/example5.txt","data/app_graph.txt","data/random/g30.txt","data/random/g60.txt","data/random/g120.txt"   # liste des graphes a mesurer
"fichier,n,m,threads,greedy_seq,greedy_omp,bt_seq,bt_omp,k_greedy,chi" | Out-File results/times.csv -Encoding ascii   # entete du csv une seule fois
foreach ($g in $graphs) {
  foreach ($p in 1,2,4,8) {   # on teste 1, 2, 4 et 8 threads
    $tmp = "results/tmp.txt"
    $proc = Start-Process -FilePath ".\coloring.exe" -ArgumentList "$g $p csv" -RedirectStandardOutput $tmp -NoNewWindow -PassThru   # on lance coloring.exe en mode csv
    if (-not $proc.WaitForExit(120000)) { $proc.Kill(); Write-Host "TIMEOUT $g p=$p" }   # 120 s max, sinon on tue
    else { Get-Content $tmp | Add-Content results/times.csv; Write-Host "OK $g p=$p" }   # on ajoute la ligne dans times.csv
  }
}
Remove-Item results/tmp.txt -ErrorAction SilentlyContinue   # on supprime le fichier temporaire
