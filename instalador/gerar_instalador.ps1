# gera o instalador .msi do Nanobo em dist\ (issues #159 a #161)
# uso, a partir da raiz do repositorio:
#   powershell -NoProfile -ExecutionPolicy Bypass -File instalador\gerar_instalador.ps1
# 1. roda o montar_distribuicao.ps1 (Release x64 sem avisos + dist\Nanobo)
# 2. confere que a lista de arquivos do Nanobo.Instalador.vdproj e igual a dist\Nanobo
# 3. compila o projeto de instalacao com o devenv (Visual Studio Installer Projects)
# 4. copia o .msi para dist\Nanobo-<versao>.msi
# opcoes: -SemBuild repassa ao montar_distribuicao.ps1 (reaproveita o x64\Release)
param([switch]$SemBuild)

$ErrorActionPreference = 'Stop'

$raiz = Split-Path -Parent $PSScriptRoot
$dist = Join-Path $raiz 'dist'
$vdproj = Join-Path $PSScriptRoot 'Nanobo.Instalador.vdproj'
$vs = 'C:\Program Files\Microsoft Visual Studio\18\Community'

function Falha($msg) { Write-Host "ERRO: $msg" -ForegroundColor Red; exit 1 }

function Achar-Devenv
{
    $fixo = Join-Path $vs 'Common7\IDE\devenv.com'
    if (Test-Path $fixo) { return $fixo }
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path $vswhere)
    {
        $base = & $vswhere -latest -property installationPath | Select-Object -First 1
        if ($base)
        {
            $achado = Join-Path $base 'Common7\IDE\devenv.com'
            if (Test-Path $achado) { return $achado }
        }
    }
    Falha 'devenv.com nao encontrado'
}

# 1. distribuicao
$montar = Join-Path $PSScriptRoot 'montar_distribuicao.ps1'
if ($SemBuild) { & powershell -NoProfile -ExecutionPolicy Bypass -File $montar -SemBuild }
else { & powershell -NoProfile -ExecutionPolicy Bypass -File $montar }
if ($LASTEXITCODE -ne 0) { Falha "montar_distribuicao.ps1 terminou com codigo $LASTEXITCODE" }

# 2. o vdproj lista os arquivos um a um: tem de bater com dist\Nanobo
$texto = Get-Content $vdproj -Raw -Encoding UTF8
$prefixo = '..\dist\Nanobo\'
$noProjeto = [regex]::Matches($texto, '"SourcePath" = "8:([^"]+)"') |
    ForEach-Object { $_.Groups[1].Value -replace '\\\\', '\' } |
    Where-Object { $_.StartsWith($prefixo) } |
    ForEach-Object { $_.Substring($prefixo.Length) } | Sort-Object
$destino = Join-Path $dist 'Nanobo'
$base = (Resolve-Path $destino).Path.TrimEnd('\') + '\'
$naDist = Get-ChildItem $destino -Recurse -File | ForEach-Object { $_.FullName.Substring($base.Length) } | Sort-Object
$diferenca = Compare-Object @($noProjeto) @($naDist)
if ($diferenca)
{
    $diferenca | ForEach-Object {
        $lado = if ($_.SideIndicator -eq '=>') { 'so em dist\Nanobo' } else { 'so no vdproj' }
        Write-Host "  $lado : $($_.InputObject)"
    }
    Falha 'a lista de arquivos do Nanobo.Instalador.vdproj nao bate com dist\Nanobo; atualize o projeto de instalacao no Visual Studio'
}
Write-Host "vdproj e dist\Nanobo conferem: $(@($naDist).Count) arquivos."

# 3. compila o .msi
$versao = [regex]::Match($texto, '"ProductVersion" = "8:([^"]+)"').Groups[1].Value
if (-not $versao) { Falha 'ProductVersion nao encontrada no vdproj' }
$nomeMsi = "Nanobo-$versao.msi"
$saidaMsi = Join-Path $PSScriptRoot "Release\$nomeMsi"
if (Test-Path $saidaMsi) { Remove-Item $saidaMsi -Force }
$devenv = Achar-Devenv
Write-Host "devenv: $devenv"
Push-Location $raiz
try
{
    $saida = & $devenv Nanobo.sln /Build 'Release|x64' /Project 'instalador\Nanobo.Instalador.vdproj' 2>&1 | ForEach-Object { "$_" }
    $codigo = $LASTEXITCODE
}
finally { Pop-Location }
$saida | Where-Object { $_ -notmatch '^Packaging file ' } | ForEach-Object { Write-Host $_ }
# o erro conhecido de build fora do processo e 'HRESULT = 8000000A' (ver docs/planejamento/release.md)
$problemas = $saida | Where-Object { $_ -match '^\s*(ERROR|WARNING):' }
if ($codigo -ne 0) { Falha "devenv terminou com codigo $codigo" }
if ($problemas) { Falha "o projeto de instalacao gerou $(@($problemas).Count) aviso(s)/erro(s)" }
if (-not (Test-Path $saidaMsi)) { Falha "$saidaMsi nao foi gerado" }

# 4. copia para dist
$final = Join-Path $dist $nomeMsi
Copy-Item $saidaMsi $final -Force
$tamanho = (Get-Item $final).Length
$hash = (Get-FileHash $final -Algorithm SHA256).Hash.ToLower()
Write-Host ('Instalador: {0} ({1:N2} MB, {2} bytes)' -f $final, ($tamanho / 1MB), $tamanho) -ForegroundColor Green
Write-Host "SHA-256: $hash"
