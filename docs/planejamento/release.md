# Release e distribuição do Nanobô

Procedimento e resultados do build Release reproduzível, da pasta de distribuição (issues #154 a #158) e do instalador `.msi` (#159 a #161, seção [Instalador](#instalador-msi-159-a-161)). Data: 26/11/2026.

## Como gerar

Da raiz do repositório (o script fecha o Nanobô se ele estiver aberto):

```
powershell -NoProfile -ExecutionPolicy Bypass -File instalador\montar_distribuicao.ps1
```

O script `instalador/montar_distribuicao.ps1`:

1. fecha o `Nanobo.exe`, se estiver rodando (senão o link falha com `LNK1104`);
2. apaga os intermediários antigos de `Nanobo\x64\Release\` (ignorados pelo git; restos de outro projeto nessa pasta geravam o aviso `MSB8028`);
3. faz `Rebuild` de **Release | x64** com o MSBuild e **falha** se houver qualquer aviso ou erro (`warning`/`error` com código) ou se o código de saída não for 0;
4. descobre as DLLs que o jogo realmente usa: fecho transitivo de `dumpbin /dependents` a partir do `Nanobo.exe`. DLLs que estão em `x64\Release` (Allegro) são copiadas dali; DLLs do runtime do Visual C++ (`vcruntime140*`, `msvcp140*`, `concrt140`, `vcomp140`) vêm do redistribuível do Visual Studio (`VC\Redist\MSVC\<versão mais nova>\x64\Microsoft.VC*.CRT`); o resto (`api-ms-win-crt-*`, `KERNEL32`, `USER32`, `OPENGL32`, `DSOUND`...) é do Windows e não é copiado;
5. lê do código-fonte todo caminho `"assets/..."` entre aspas, confere que cada arquivo existe e copia só esses para `dist\Nanobo\assets\`;
6. recusa a montagem se entrar `.pdb`, `.aseprite`, `.bak`, `.tmp`, `.log`, `.gitkeep`, `Nanobo-DESKTOP-*`, `nanobo_ranking*` ou `nanobo_hiscore*`;
7. grava `dist\manifesto.txt` (SHA-256, tamanho e caminho de cada arquivo, fora de `dist\Nanobo` para não entrar no instalador) e imprime o total.

A pasta `dist/` está no `.gitignore`.

### Build reproduzível

O `Nanobo.vcxproj` ganhou, só para `Release|x64`, `/Brepro` no compilador e no linker. Sem ele, o `Nanobo.exe` mudava de hash a cada build porque o PE carrega a data do link. Com a opção, o carimbo vira um hash do conteúdo.

## O que entra em `dist\Nanobo` (46 arquivos, 17,84 MB)

| Grupo | Arquivos | Tamanho (bytes) |
|---|---|---|
| `Nanobo.exe` | 1 | 117.760 |
| DLLs do Allegro | 7 | 14.906.368 |
| `VCRUNTIME140.dll` (app-local) | 1 | 176.488 |
| `assets/audio` (6 SFX + música) | 7 | 750.006 |
| `assets/backgrounds` (3 mapas) | 3 | 2.729.434 |
| `assets/sprites` | 27 | 22.403 |
| **Total** | **46** | **18.702.459 (17,84 MB)** |

DLLs do Allegro enviadas (todas as que o `Nanobo.exe` importa, direta ou indiretamente): `allegro-5.2`, `allegro_image-5.2`, `allegro_font-5.2`, `allegro_ttf-5.2`, `allegro_audio-5.2`, `allegro_acodec-5.2`, `allegro_primitives-5.2`.

### Resultado do `dumpbin /dependents`

- `Nanobo.exe` importa: as 7 DLLs do Allegro acima, `VCRUNTIME140.dll`, `api-ms-win-crt-*` (utility, stdio, math, time, string, filesystem, convert, runtime, locale, heap) e `KERNEL32.dll`.
- As DLLs do Allegro importam só outras DLLs do Allegro e DLLs do sistema (`USER32`, `GDI32`, `ole32`, `WINMM`, `SHLWAPI`, `SHELL32`, `OPENGL32`, `DSOUND`, `KERNEL32`). Elas **não** importam o runtime do Visual C++ (CRT estático no pacote NuGet).
- Só `VCRUNTIME140.dll` falta num Windows 10/11 limpo, e vai ao lado do exe. `vcruntime140_1.dll` **não** é importada por nenhum binário e por isso não vai; se o toolset mudar e ela passar a ser importada, o script a inclui sozinho. As `api-ms-win-crt-*` são do Windows 10+ e não são enviadas.

### DLLs do Allegro que existem em `x64\Release` e não vão

`allegro_color-5.2`, `allegro_dialog-5.2`, `allegro_memfile-5.2`, `allegro_physfs-5.2` e `allegro_video-5.2` (o projeto liga os addons no `.vcxproj`, mas o executável não importa nenhum deles). Também ficam de fora `Nanobo.pdb`, `Nanobo-DESKTOP-FGNKQDA.exe/pdb` e `Nanobo-DESKTOP-FGNKQDA-2.pdb`.

### Assets que não vão

O jogo só abre os arquivos citados em `src/` (37 PNG/WAV). Ficam de fora, por não serem carregados por `load_resources()`: todos os `.aseprite`; `backgrounds/fundo_microscopico.png`; `sprites/nanobo.png`; `bacteria_gram_*` (4 PNG), `celula_propria*` (2), `virus_fase2*` (2) e `icone_laser_*` (2), que o `catalogo-recursos.md` lista como histórico; os `.gitkeep`, `assets/README.md` e `assets/catalogo-recursos.md`. Não há fonte em arquivo (o jogo usa a fonte embutida do Allegro), e `assets/fonts` e `assets/data` estão vazias.

### Arquivos gravados em tempo de execução

O jogo grava `nanobo_ranking.txt` (e `.tmp`/`.bak` durante a troca) na pasta onde roda, por caminho relativo, como os assets. Por isso o instalador precisa instalar numa pasta onde o usuário escreve, por exemplo `%LOCALAPPDATA%\Programs\Nanobo`, e **não** em `Program Files`. O arquivo não faz parte da distribuição.

## Verificação de reprodutibilidade (#154)

O script rodou duas vezes seguidas (cada uma com `Rebuild` completo e a pasta `dist` recriada do zero). Os dois `manifesto.txt` foram comparados com `diff`: **idênticos, inclusive o hash do `Nanobo.exe`** (`8d4dcd8c...59f7c8`). Antes do `/Brepro`, a mesma comparação mostrava só a linha do `Nanobo.exe` diferente (`4dc611c2...` contra `bad7a817...`), com o mesmo tamanho, 117.760 bytes; os outros 45 arquivos já eram idênticos, porque são cópias de entradas fixas.

## Execução fora do ambiente de desenvolvimento (#156, #157)

Teste de 26/11/2026:

- `dist\Nanobo` copiada para uma pasta fora do repositório e do `x64\Release`, sem `nanobo_ranking.txt`;
- `PATH` do processo reduzido a `C:\WINDOWS\system32;C:\WINDOWS` (sem o repositório, sem as pastas do Allegro, sem o Visual Studio);
- jogo aberto com `Start-Process -WorkingDirectory` nessa pasta, pelo `qa_jogo.ps1` (numa cópia que usa a pasta do exe como diretório de trabalho, porque o original deriva a raiz como avô do exe, para o layout `x64\Release`).

Resultado: o jogo abre o **menu** (título, "nenhum recorde ainda"), `ENTER` leva ao briefing, `ESC` pula e a **Fase 1 carrega** com mapa, Nanobô, agentes, ícones, HUD e texto (todos os assets achados). A lista de módulos do processo mostrou as 7 DLLs do Allegro e `VCRUNTIME140.dll` vindas da pasta do jogo, e as demais do `System32`. Memória: working set de 76,6 MB na Fase 1 (meta: até 100 MB). As capturas ficam em `.hermes/release-evidencia/` (menu, briefing e fase 1; pasta ignorada pelo git).

Observação sobre `d3dx9_43.dll`: o Allegro abriu o display por Direct3D 9 e carregou `d3d9.dll` e `d3dx9_43.dll` do `System32` desta máquina. A `d3dx9_43.dll` pertence ao DirectX End-User Runtime e pode **não** existir num Windows limpo. É o primeiro ponto a conferir no teste em máquina limpa.

## Instalador `.msi` (#159 a #161)

### Como gerar

Da raiz do repositório, um comando monta a distribuição e gera o instalador:

```
powershell -NoProfile -ExecutionPolicy Bypass -File instalador\gerar_instalador.ps1
```

O script `instalador/gerar_instalador.ps1`:

1. roda o `montar_distribuicao.ps1` (Rebuild Release x64 sem avisos e `dist\Nanobo` do zero); `-SemBuild` é repassado a ele;
2. confere que os arquivos listados no `instalador/Nanobo.Instalador.vdproj` são exatamente os de `dist\Nanobo` e **falha** se faltar ou sobrar algum (o `.vdproj` lista arquivo por arquivo; se um asset novo entrar no jogo, o projeto de instalação tem de ganhar o arquivo no Visual Studio, na pasta certa do *File System*);
3. compila o projeto de instalação com o `devenv.com` (o Installer Projects não roda no MSBuild):
   `devenv.com Nanobo.sln /Build "Release|x64" /Project instalador\Nanobo.Instalador.vdproj`
   e falha se o código de saída não for 0 ou se aparecer `ERROR:`/`WARNING:`;
4. copia `instalador\Release\Nanobo-1.0.0.msi` para `dist\Nanobo-1.0.0.msi` e imprime tamanho e SHA-256.

Requisito: a extensão **Microsoft Visual Studio Installer Projects 2022** (3.0.0) instalada no Visual Studio 18. Se o build pela linha de comando falhar com `ERROR: An error occurred while validating. HRESULT = '8000000A'`, a extensão traz o `DisableOutOfProcBuild.exe` (em `Common7\IDE\CommonExtensions\Microsoft\VSI\DisableOutOfProcBuild`), que muda uma chave de registro do Visual Studio do usuário. Nesta máquina o build funcionou **sem** essa mudança.

O projeto de instalação entra no `Nanobo.sln` como `Nanobo.Instalador`, só com `ActiveCfg` (sem `Build.0`): o *Build Solution* do Visual Studio continua compilando só o jogo, e o mapeamento `Debug|x64` do `Nanobo` não mudou. `instalador\Release\` e `instalador\Debug\` (saída do `devenv`) são ignorados pelo `.gitignore`.

### O que o instalador faz

| Item | Valor |
|---|---|
| Formato | `.msi` (Windows Installer), sem `setup.exe` e sem pré-requisitos |
| Assistente | clássico, em português do Brasil (1046): Bem-vindo → Pasta de instalação → Confirmar → Progresso → Concluído |
| Produto / fabricante / versão | `Nanobô` / `Nanobô` / `1.0.0`, plataforma x64 |
| Instalação | **por usuário** (`InstallAllUsers = False`, sem `ALLUSERS`), sem pedido de administrador; a opção "para todos" fica escondida no assistente |
| Pasta padrão | `[LocalAppDataFolder]Programs\Nanobo` = `%LOCALAPPDATA%\Programs\Nanobo` (o jogo grava o `nanobo_ranking.txt` ali) |
| Conteúdo | os 46 arquivos de `dist\Nanobo` (exe, 7 DLLs do Allegro, `VCRUNTIME140.dll` app-local, `assets\` com a mesma árvore); nada de `.pdb`, ranking, hiscore ou `Nanobo-DESKTOP-*` |
| Atalhos | "Nanobô" no menu Iniciar (raiz de *Programas*) e na Área de Trabalho, ambos com pasta de trabalho = pasta de instalação (os assets abrem por caminho relativo) |
| Ícone | o padrão de executável: o repositório não tem `.ico` nem recurso de ícone no `Nanobo.exe` |
| Desinstalação | entrada "Nanobô 1.0.0" em *Aplicativos instalados*; `RemovePreviousVersions = True` e `DetectNewerInstalledVersion = True` (mesmo `UpgradeCode`) |
| Tamanho | `dist\Nanobo-1.0.0.msi` com 8.886.272 bytes (8,47 MB; os 17,84 MB vão comprimidos num `.cab` embutido) |

Os atalhos gerados pelo Installer Projects são *anunciados* (advertised): o alvo aparece como um ícone guardado em `%APPDATA%\Microsoft\Installer\{ProductCode}\`, e o Windows Installer confere a instalação antes de abrir o `Nanobo.exe`. É o comportamento padrão da extensão.

### Verificação nesta máquina (26/11/2026)

Sem administrador, num prompt comum:

- `msiexec /i dist\Nanobo-1.0.0.msi /qn /l*v install.log` terminou com código **0** sem elevação (o log registra `no credential elevation is possible`, e a instalação conclui mesmo assim, porque nada nela exige administrador). `AssignmentType = 0` (por usuário), `TARGETDIR = C:\Users\<usuário>\AppData\Local\Programs\Nanobo\`.
- Os 46 arquivos instalados têm SHA-256 **idêntico** aos de `dist\Nanobo` (18.702.459 bytes).
- Atalhos `Nanobô.lnk` na Área de Trabalho e em `Start Menu\Programs`, ambos com `WorkingDirectory = ...\AppData\Local\Programs\Nanobo\`.
- Entrada de desinstalação `{F91E5166-9927-5AEA-8D40-B63C2748DD3F}`: `DisplayName` Nanobô, `DisplayVersion` 1.0.0, `Publisher` Nanobô.
- O jogo aberto **pelo atalho do menu Iniciar** mostrou o menu ("nenhum recorde ainda"), o briefing e a Fase 1 com mapa, sprites e HUD. Os módulos carregados da pasta de instalação foram o exe, as 7 DLLs do Allegro e o `VCRUNTIME140.dll`; a `d3dx9_43.dll` veio de novo do `System32`.
- Gravar e apagar um arquivo de teste na pasta de instalação funcionou sem permissão extra (é onde o ranking vai).
- `msiexec /x dist\Nanobo-1.0.0.msi /qn /l*v uninstall.log` terminou com código **0**: a pasta `%LOCALAPPDATA%\Programs\Nanobo`, os dois atalhos e a entrada de desinstalação sumiram. Nesse teste nenhuma partida foi jogada até a Vitória, então não havia `nanobo_ranking.txt`. Se houver, ele **fica** na pasta depois da desinstalação (o arquivo não é do instalador), e a pasta também fica por causa dele. É aceitável: preserva os recordes numa reinstalação.
- O assistente abre em português ("O Installer vai guiá-lo...", "Avançar >", "Cancelar"). No tema escuro do Windows 11 desta máquina, o título branco do banner não aparece e o botão "Avançar >" quebra a linha; são as telas padrão da extensão, só cosméticas.

Logs e capturas (menu, briefing, fase 1 e o assistente) ficam em `.hermes/instalador-evidencia/` (ignorada pelo git).

## Pendências

- Teste em máquina limpa (#162 a #164), adiado pelo dono. Instalar o `dist\Nanobo-1.0.0.msi` e conferir: se o Allegro abre o display sem `d3dx9_43.dll` (nesta máquina ela veio do `System32`, do DirectX End-User Runtime, que pode faltar num Windows limpo; se não abrir, avaliar forçar OpenGL com um `allegro5.cfg` ao lado do exe, `[graphics] driver=opengl`, ou distribuir a DLL); se o `VCRUNTIME140.dll` app-local basta; se o ranking é gravado na pasta de instalação; se o assistente abre sem pedir administrador.
- O script usa o Visual Studio 18 Community no caminho padrão (como o `AGENTS.md`); em outra máquina o `vswhere` acha o MSBuild, mas o `dumpbin` e o redistribuível usam o mesmo caminho base.
- O runtime copiado vem do redistribuível instalado com o Visual Studio (`14.52.37025`, `Microsoft.VC145.CRT`), mais novo que o toolset `v143` do projeto, o que é compatível.
