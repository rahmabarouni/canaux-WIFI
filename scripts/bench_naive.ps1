$graphs = "data/example5.txt","data/app_graph.txt","data/random/s12.txt","data/random/s16.txt","data/random/s20.txt","data/random/s24.txt","data/random/g30.txt"
"fichier,n,chi,naif_s,ameliore_s" | Out-File results/naive_vs_improved.csv -Encoding ascii
$env:REPS = "1"
foreach ($g in $graphs) {
  $n = (Get-Content $g -TotalCount 1).Split(" ")[0]
  $imp = (& .\coloring.exe $g 1 csv).Split(",")
  $chi = $imp[9]; $ameliore = $imp[6]
  $proc = Start-Process -FilePath ".\naive.exe" -ArgumentList $g -RedirectStandardOutput results/tmp_n.txt -NoNewWindow -PassThru
  if (-not $proc.WaitForExit(60000)) { $proc.Kill(); $naif = "TIMEOUT>60" }
  else { $naif = (Get-Content results/tmp_n.txt).Split(",")[1] }
  "$g,$n,$chi,$naif,$ameliore" | Add-Content results/naive_vs_improved.csv
  Write-Host "OK $g"
}
Remove-Item results/tmp_n.txt -ErrorAction SilentlyContinue
