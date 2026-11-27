$env:REPS = "1"
"fichier,n,m,bt_seq,bt_omp1" | Out-File results/scan.csv -Encoding ascii
foreach ($n in 110,130,150) { foreach ($r in 0.12,0.135) { foreach ($s in 1,2,3) {
  $f = "data/random/c_${n}_${r}_${s}.txt"
  python scripts/gen_graph.py $n $r $s $f | Out-Null
  $proc = Start-Process -FilePath ".\coloring.exe" -ArgumentList "$f 1 csv" -RedirectStandardOutput results/tmp_s.txt -NoNewWindow -PassThru
  if (-not $proc.WaitForExit(40000)) { $proc.Kill(); "$f,$n,?,TIMEOUT,TIMEOUT" | Add-Content results/scan.csv; Write-Host "TIMEOUT $f" }
  else { $p = (Get-Content results/tmp_s.txt).Split(","); "$f,$($p[1]),$($p[2]),$($p[6]),$($p[7])" | Add-Content results/scan.csv; Write-Host "OK $f bt_seq=$($p[6])" }
}}}
Remove-Item results/tmp_s.txt -ErrorAction SilentlyContinue
