# monta a distribuicao do Nanobo em dist\Nanobo (issues #154 a #158)
# uso, a partir da raiz do repositorio:
#   powershell -NoProfile -ExecutionPolicy Bypass -File instalador\montar_distribuicao.ps1
# opcoes: -SemBuild reaproveita o x64\Release que ja existe (so para depurar este script)
param([switch]$SemBuild)

$ErrorActionPreference = 'Stop'

$raiz = Split-Path -Parent $PSScriptRoot
$release = Join-Path $raiz 'x64\Release'
$dist = Join-Path $raiz 'dist'
$destino = Join-Path $dist 'Nanobo'
$vs = 'C:\Program Files\Microsoft Visual Studio\18\Community'

function Falha($msg) { Write-Host "ERRO: $msg" -ForegroundColor Red; exit 1 }

# acha uma ferramenta do Visual Studio: caminho conhecido primeiro, vswhere depois
function Achar-Msbuild
{
    $fixo = Join-Path $vs 'MSBuild\Current\Bin\MSBuild.exe'
    if (Test-Path $fixo) { return $fixo }
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path $vswhere)
    {
        $achado = & $vswhere -latest -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
        if ($achado) { return $achado }
    }
    Falha 'MSBuild nao encontrado'
}

function Achar-Dumpbin
{
    $base = Join-Path $vs 'VC\Tools\MSVC'
    $achado = Get-ChildItem $base -Directory | Sort-Object { [version]$_.Name } -Descending |
        ForEach-Object { Join-Path $_.FullName 'bin\Hostx64\x64\dumpbin.exe' } |
        Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $achado) { Falha 'dumpbin.exe nao encontrado' }
    return $achado
}

# pasta x64 do runtime redistribuivel mais novo (ignora onecore e debug_nonredist)
function Achar-Redist
{
    $base = Join-Path $vs 'VC\Redist\MSVC'
    $achado = Get-ChildItem $base -Directory | Where-Object { $_.Name -match '^\d+\.\d+\.\d+' } |
        Sort-Object { [version]$_.Name } -Descending |
        ForEach-Object { Get-ChildItem (Join-Path $_.FullName 'x64') -Directory -Filter 'Microsoft.VC*.CRT' -ErrorAction SilentlyContinue } |
        Select-Object -First 1
    if (-not $achado) { Falha 'runtime redistribuivel (Microsoft.VC*.CRT) nao encontrado' }
    return $achado.FullName
}

# 1. fecha o jogo, senao o link falha com LNK1104
Get-Process -Name Nanobo -ErrorAction SilentlyContinue | Stop-Process -Force
Start-Sleep -Milliseconds 500

# 2. rebuild Release x64, sem tolerar nenhum aviso
if (-not $SemBuild)
{
    # apaga os intermediarios antigos (ignorados pelo git): restos de outro projeto na pasta geram o aviso MSB8028
    $intermediarios = Join-Path $raiz 'Nanobo\x64\Release'
    if (Test-Path $intermediarios) { Remove-Item $intermediarios -Recurse -Force }
    $msbuild = Achar-Msbuild
    Write-Host "MSBuild: $msbuild"
    Push-Location $raiz
    try
    {
        $saida = & $msbuild Nanobo.vcxproj -p:Configuration=Release -p:Platform=x64 -t:Rebuild -v:minimal -nologo 2>&1 | ForEach-Object { "$_" }
        $codigo = $LASTEXITCODE
    }
    finally { Pop-Location }
    $saida | ForEach-Object { Write-Host $_ }
    $problemas = $saida | Where-Object { $_ -match '\b(warning|error)\s+[A-Za-z]+\d+' }
    if ($codigo -ne 0) { Falha "MSBuild terminou com codigo $codigo" }
    if ($problemas) { Falha "o build gerou $(@($problemas).Count) aviso(s)/erro(s); a distribuicao exige 0" }
    Write-Host 'Build Release x64 sem avisos.' -ForegroundColor Green
}
$exe = Join-Path $release 'Nanobo.exe'
if (-not (Test-Path $exe)) { Falha "$exe nao existe" }

# 3. dependencias reais: fecho transitivo do dumpbin /dependents a partir do exe
$dumpbin = Achar-Dumpbin
$visitados = @{}
$fila = [System.Collections.Queue]::new()
$fila.Enqueue('Nanobo.exe')
$locais = @()        # dlls que estao no x64\Release (Allegro)
$runtime = @()       # dlls do runtime do Visual C++, vindas do redistribuivel
while ($fila.Count -gt 0)
{
    $nome = $fila.Dequeue()
    if ($visitados.ContainsKey($nome.ToLower())) { continue }
    $visitados[$nome.ToLower()] = $true
    $caminho = Join-Path $release $nome
    if (-not (Test-Path $caminho)) { continue }
    $deps = & $dumpbin /dependents $caminho | ForEach-Object { $_.Trim() } | Where-Object { $_ -match '^[\w\-\.]+\.dll$' }
    foreach ($d in $deps)
    {
        if (Test-Path (Join-Path $release $d))
        {
            if ($locais -notcontains $d) { $locais += $d }
            $fila.Enqueue($d)
        }
        elseif ($d -match '^(vcruntime140(_1)?|msvcp140.*|concrt140|vcomp140)\.dll$')
        {
            if ($runtime -notcontains $d) { $runtime += $d }
        }
        # o resto (api-ms-win-crt-*, KERNEL32, USER32, OPENGL32, DSOUND...) vem com o Windows
    }
}
Write-Host ('DLLs Allegro usadas: ' + (($locais | Sort-Object) -join ', '))
Write-Host ('DLLs do runtime C++: ' + (($runtime | Sort-Object) -join ', '))

# 4. recursos: todo "assets/..." citado entre aspas no codigo-fonte
$citados = Get-ChildItem (Join-Path $raiz 'src') -Recurse -Include *.c, *.h |
    Select-String -Pattern '"(assets/[^"]+)"' -AllMatches |
    ForEach-Object { $_.Matches } | ForEach-Object { $_.Groups[1].Value } | Sort-Object -Unique
if (-not $citados) { Falha 'nenhum recurso citado no codigo' }
foreach ($a in $citados)
{
    if (-not (Test-Path (Join-Path $raiz $a))) { Falha "recurso citado e ausente no disco: $a" }
}

# 5. monta dist\Nanobo do zero
if (Test-Path $dist) { Remove-Item $dist -Recurse -Force }
New-Item -ItemType Directory -Path $destino | Out-Null
Copy-Item $exe $destino
foreach ($d in $locais) { Copy-Item (Join-Path $release $d) $destino }
if ($runtime.Count -gt 0)
{
    $redist = Achar-Redist
    Write-Host "Runtime: $redist"
    foreach ($d in $runtime)
    {
        $origem = Join-Path $redist $d
        if (-not (Test-Path $origem)) { Falha "$d nao esta no redistribuivel" }
        Copy-Item $origem $destino
    }
}
foreach ($a in $citados)
{
    $alvo = Join-Path $destino ($a -replace '/', '\')
    New-Item -ItemType Directory -Path (Split-Path $alvo) -Force | Out-Null
    Copy-Item (Join-Path $raiz $a) $alvo
}

# 6. confere que nada proibido entrou
$proibidos = Get-ChildItem $destino -Recurse -File |
    Where-Object { $_.Extension -in '.pdb', '.aseprite', '.bak', '.tmp', '.log', '.gitkeep' -or $_.Name -like 'Nanobo-DESKTOP-*' -or $_.Name -like 'nanobo_ranking*' -or $_.Name -like 'nanobo_hiscore*' }
if ($proibidos) { Falha ('arquivo proibido na distribuicao: ' + ($proibidos.FullName -join ', ')) }

# 7. manifesto (fora de dist\Nanobo, para nao entrar no instalador) e resumo
$arquivos = Get-ChildItem $destino -Recurse -File | Sort-Object FullName
$base = (Resolve-Path $destino).Path.TrimEnd('\') + '\'
$linhas = $arquivos | ForEach-Object {
    $rel = $_.FullName.Substring($base.Length) -replace '\\', '/'
    '{0}  {1}  {2}' -f (Get-FileHash $_.FullName -Algorithm SHA256).Hash.ToLower(), $_.Length, $rel
}
Set-Content -Path (Join-Path $dist 'manifesto.txt') -Value $linhas -Encoding ascii
$total = ($arquivos | Measure-Object Length -Sum).Sum
Write-Host ('dist\Nanobo: {0} arquivos, {1:N2} MB ({2} bytes)' -f $arquivos.Count, ($total / 1MB), $total) -ForegroundColor Green
Write-Host "Manifesto: $(Join-Path $dist 'manifesto.txt')"
