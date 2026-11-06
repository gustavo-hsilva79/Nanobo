# Baseline de build da refatoração — 06/11/2026

> **Commit:** `e99a8f76c59aec1bc46abdfb67157e6588f37a09` (`fix: grava o recorde apenas na vitória`).
> **Branch:** `refactor/wave1-harness`, criada de `master` nesse mesmo commit.
> **Data:** 06/11/2026, 15:17 (UTC-03:00).
> **Ferramenta:** MSBuild 18.11.0.47511 (Visual Studio 18 Community), toolset `v143`, C11 (`stdc11`), WarningLevel 4, plataforma x64.
> **Objetivo:** provar que o ponto de partida compila limpo, com 0 avisos, antes de qualquer mudança da refatoração. Nenhum arquivo em `src/` foi tocado.

## 1. Árvore limpa

`git status --short` não retornou nada antes dos builds, então o baseline vale para o commit acima, sem alteração pendente.

## 2. Resultados

| Configuração | Comando | Última linha da saída | Avisos | Log |
|---|---|---|---|---|
| Debug \| x64 | `-t:Rebuild -p:Configuration=Debug -p:Platform=x64` | `Nanobo.vcxproj -> ...\x64\Debug\Nanobo.exe` | 0 | `build-debug.log` |
| Release \| x64 | `-t:Rebuild -p:Configuration=Release -p:Platform=x64` | `Nanobo.vcxproj -> ...\x64\Release\Nanobo.exe` | 0 | `build-release.log` |

Os quatro arquivos do build (`main.c`, `game.c`, `entities.c`, `gameplay.c`) foram compilados e linkados nas duas configurações, e os dois comandos saíram com código 0.

Execução usada, a partir da raiz do repositório:

```bash
"/c/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/MSBuild.exe" Nanobo.vcxproj \
  -p:Configuration=Debug -p:Platform=x64 -t:Rebuild -v:minimal -nologo \
  > docs/auditoria/evidencias/refatoracao/build-debug.log 2>&1

"/c/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/MSBuild.exe" Nanobo.vcxproj \
  -p:Configuration=Release -p:Platform=x64 -t:Rebuild -v:minimal -nologo \
  > docs/auditoria/evidencias/refatoracao/build-release.log 2>&1
```

## 3. Verificação

| Checagem | Resultado |
|---|---|
| `grep -ci "warning" build-debug.log` | `0` |
| `grep -ci "warning" build-release.log` | `0` |
| `grep -c "Nanobo.exe" build-debug.log` | `1` |
| `grep -c "Nanobo.exe" build-release.log` | `1` |
| `git status --short` depois do commit | vazio |

## 4. Observação: MSB8028 na primeira execução

O primeiro `Rebuild` Debug emitiu `warning MSB8028`, apontando arquivos compartilhados com o projeto `Nanobo-DESKTOP-FGNKQDA.vcxproj` no diretório intermediário. A causa é estado de build antigo, não código: um `Nanobo-DESKTOP-FGNKQDA.lastbuildstate` de 05/11 sobrou em `Nanobo/x64/Debug/Nanobo.tlog/` (`IntDir`, ignorado pelo git), e o `.vcxproj` com esse nome não existe mais no repositório.

Removido só esse arquivo de estado, o `Rebuild` seguinte saiu com 0 avisos, e é esse log limpo que está registrado aqui. Nada de fonte, projeto ou asset foi alterado — e o aviso voltaria a aparecer para qualquer clone que ainda tivesse o `.tlog` do nome antigo.

## 5. Escopo

- Baseline **visual** (capturas de tela) fica na Task 7 (Wave 2); aqui só o build.
- Nenhum arquivo de `src/`, `Nanobo.vcxproj` ou `assets/` foi alterado.
- `packages/` e `x64/` são ignorados pelo git. Os dois logs desta pasta entram no commit à força (`git add -f`), porque o `.gitignore` ignora `*.log`; sem isso a evidência não chegaria ao repositório.
