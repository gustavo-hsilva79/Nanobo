# Nanobô — cenário de referência de medição de memória (#114)

> **Issue:** #114 (fecha).
>
> **Consumido por:** #115 (medição datada) e #119 (revisão do limite de ≤ 100 MB).
>
> Este documento fixa **como** medir; os números datados saem na #115. Medir antes das Tarefas 3–5 produz um número que envelhece, porque áudio e spritesheet mudam o consumo.

## Configuração

- **Sistema:** Windows 11 Pro x64 (build 26300), sempre na mesma máquina. Comparações só valem dentro dela.
- **Build de referência:** `Release|x64` — é a configuração que vale para o limite de ≤ 100 MB.
- **Build comparativo:** `Debug|x64` — não é comparável ponto a ponto: nesta medição ficou em 77,3 MB numa janela de 10 s contra 79,4 MB do Release em 20 s. O runtime de depuração muda o consumo, então o Debug serve só como ordem de grandeza, nunca para aprovar o limite.
- **Executável:** `x64/Release/Nanobo.exe` (referência) e `x64/Debug/Nanobo.exe` (comparativo), compilados a partir do mesmo commit. Se o commit mudou, recompile os dois antes de medir.
- **Diretório de trabalho:** a raiz do repositório (`Nanobo/`, onde está `Nanobo.sln`). Os assets são abertos por caminho relativo (`assets/...`); rodar de outra pasta falha no carregamento e o jogo não abre.
- **Uma instância só:** o app é console-subsystem e mostra duas janelas do mesmo processo (`Nanobo.exe` e a janela do jogo). Não deixe outra cópia aberta: o script pega o primeiro processo encontrado.
- **Áudio ativo:** a música deve estar audível no menu. Sem dispositivo de som o jogo abre mudo, não carrega os WAVs e o consumo não é o do cenário.

## Sequência

Roteiro único, do MENU até a nova partida, executado com o script amostrando em paralelo:

1. Abrir o jogo a partir da raiz do repositório: `./x64/Release/Nanobo.exe` (ou `./x64/Debug/Nanobo.exe` no comparativo).
2. **MENU — 15 s parado**, sem clicar. É o estado de repouso da primeira amostra.
3. `ENTER` (ou clique em `INICIAR`) → BRIEFING.
4. `ESC` (ou `PULAR`) para pular o briefing → GAME, Fase 1.
5. Na Fase 1: andar com as setas; **botão direito** sobre cada um dos 9 agentes (3 Gram+, 3 Gram− e 3 células) até identificá-los; trocar de arma com `1`/`2` (ou a roda do mouse) e **clique esquerdo** para neutralizar as 6 bactérias — `Ruptura` nas Gram+ e `Dissolução` nas Gram−; não atirar nas células; ir até o destino para concluir a fase. Não usar `ESC` aqui (pausa).
6. RESULTS: `ENTER` (ou `CONTINUAR`) → Fase 2.
7. Fase 2: neutralizar os 3 vírus com a arma certa e ir até o destino; as 3 células não são alvo.
8. RESULTS: `ENTER` (ou `CONTINUAR`) → VICTORY.
9. VICTORY: `ENTER` (ou `NOVA PARTIDA`) → briefing de uma nova partida. O ciclo termina aqui.

A sequência existe para passar por todos os recursos em uso: menu, briefing, as duas fases, os dois RESULTS e a VICTORY. Qualquer desvio (pausa, GAME_OVER, alt-tab) invalida a execução e ela é refeita.

## Recursos carregados

Tudo entra em `load_resources()` (`src/main.c`) uma única vez, antes do loop de eventos; nenhuma cena recarrega recurso. Se um arquivo for renomeado ou removido, o carregamento falha e o jogo não abre — a lista abaixo é a que a medição deve encontrar carregada.

**Os 11 bitmaps:**

| # | Recurso | Arquivo |
|---|---|---|
| 1 | Nanobô (spritesheet animado) | `assets/sprites/nanobo_spritesheet.png` |
| 2 | Bactéria Gram+ | `assets/sprites/bacteria_gram_positiva.png` |
| 3 | Bactéria Gram− | `assets/sprites/bacteria_gram_negativa.png` |
| 4 | Célula própria | `assets/sprites/celula_propria.png` |
| 5 | Vírus da Fase 2 | `assets/sprites/virus_fase2.png` |
| 6 | Ícone de vida | `assets/sprites/icone_coracao.png` |
| 7 | Ícone do scanner | `assets/sprites/icone_scanner.png` |
| 8 | Ícone da arma Ruptura | `assets/sprites/icone_ruptura.png` |
| 9 | Ícone da arma Dissolução | `assets/sprites/icone_dissolucao.png` |
| 10 | Ícone de defesa | `assets/sprites/icone_defesa.png` |
| 11 | Mapa da Fase 1 | `assets/backgrounds/mapa_fase1.png` |

**A fonte:** a fonte embutida do Allegro (`al_create_builtin_font()`), criada em memória, sem arquivo. O título `NANOBÔ` é um bitmap montado a partir dela em `load_resources()` (`make_text_bitmap`), então conta como bitmap em memória mesmo sem PNG.

**Os 6 efeitos sonoros** (`assets/audio/sfx/`): `scanner.wav`, `disparo.wav`, `impacto.wav`, `neutralizacao.wav`, `penalidade.wav`, `defesa.wav`.

**A música** (`assets/audio/music/`): `fundo.wav`, tocada em loop durante o jogo e parada na pausa.

Ficam em `assets/` sem entrar no carregamento: `sprites/nanobo.png` (o PNG parado, que o spritesheet substituiu), `sprites/icone_laser_vermelho.png`, `sprites/icone_laser_roxo.png` e `backgrounds/fundo_microscopico.png`. Não contam nesta medição.

## Momento e método

Com o jogo aberto e a sequência em andamento, rodar `medir_memoria.ps1 -Seconds 60 -EveryMs 250`. Em 60 s saem **~228 amostras**, não 240: o `-EveryMs` é um piso e o custo do próprio laço de amostragem come ~6% do intervalo (76 em 20 s, 151 em 40 s, 38 em 10 s).

- **O que é amostrado:** `WorkingSet64` (working set, o que o Gerenciador de Tarefas mostra) e `PrivateMemorySize64` (memória privada comprometida), arredondados a 0,01 MB. Uma linha de CSV por amostra, no formato `t_s,working_set_mb,private_mb`.
- **Pico:** a maior amostra de `working_set_mb` da execução — o `max` que o script imprime. É o número que encosta no limite.
- **Estável:** a última amostra (`ultimo`) de uma janela de **20 s com o jogo já aberto**, parado no MENU e sem partida iniciada (`-Seconds 20`). O `t` do CSV conta a partir do início do **script**, não do processo: com o jogo aberto antes, a janela cobre **o menu, não a carga**. Para medir a carga, inicie o script **antes** do jogo — o laço tolera o processo ausente (`if ($p)`) e só começa a amostrar quando ele aparece, então a janela pega o pico do carregamento.
- **Limite:** julgar o pico do `Release|x64` contra ≤ 100 MB. `PrivateMemorySize64` é diagnóstico e não tem limite próprio. O `Debug|x64` repete a mesma sequência só como comparativo.
- **Janela:** se a sequência manual passar de 60 s, repetir com `-Seconds` maior e registrar o valor usado; comparações só valem com a mesma janela.
- **Registro:** anotar data/hora, commit, configuração, `amostras`, `min`, `max` e `ultimo`, e o caminho do CSV. A medição datada é a #115.

## Como repetir

Compile `Release|x64` e `Debug|x64` no Visual Studio 2022 (ou pelo MSBuild do `AGENTS.md`). Depois, com o terminal na raiz do repositório:

```powershell
cd "C:/Users/GIGABYTE/OneDrive - SENAC - SP/SENAC/Projetos/2° Semestre/Projeto Integrador/Nanobo"
./x64/Release/Nanobo.exe
```

Com o jogo já aberto, numa segunda janela:

```powershell
cd "C:/Users/GIGABYTE/OneDrive - SENAC - SP/SENAC/Projetos/2° Semestre/Projeto Integrador/Nanobo"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ../Nanobo-testes/medir_memoria.ps1 `
  -Out "C:/Users/GIGABYTE/AppData/Local/Temp/nanobo-memoria-release.csv" -Seconds 60 -EveryMs 250
```

Saída esperada, uma linha por execução:

```text
amostras=228 min=... MB max=... MB ultimo=... MB
csv: C:/Users/GIGABYTE/AppData/Local/Temp/nanobo-memoria-release.csv
```

O `amostras` sai inteiro; 60 s a 250 ms dão 227 ou 228 amostras (não 240) porque o custo do próprio laço come ~6% do intervalo.

O script vive em `../Nanobo-testes/medir_memoria.ps1`, ao lado do harness, fora deste repositório. Ele não abre o jogo sozinho: sem `Nanobo.exe` rodando, imprime `nenhuma amostra: o jogo nao estava aberto` e sai com código 1. O CSV usado fica em `C:/Users/GIGABYTE/AppData/Local/Temp/nanobo-memoria-release.csv` (o `%TEMP%` do usuário) e pode ser trocado pelo `-Out` na mesma chamada.

---

## 11/11/2026 — #115: execução normal

- **Commit medido:** `86a962e` (branch `feat/vermelho-11-11`), com áudio e spritesheet carregados.
- **Builds:** `Release|x64` (referência) e `Debug|x64` (comparativo), ambos com `Rebuild` e 0 avisos.
- **Método:** `medir_memoria.ps1 -EveryMs 250`, quatro janelas no Release — MENU com o jogo já aberto (20 s), GAME (40 s), GAME com ~10 s de amostragem (9,83 s no CSV) depois de dirigir o Nanobô nas quatro direções por ~7 s e a **carga** com o script iniciado antes do jogo (35 s) — e uma janela de 10 s no Debug.

| Build / janela | Amostras | Mín (MB) | Pico (MB) | Última (MB) |
|---|---:|---:|---:|---:|
| Release — MENU com o jogo já aberto, 20 s | 76 | 76,51 | **79,41** | 76,67 |
| Release — carga (script iniciado antes do jogo), 35 s | 125 | 24,85 | 79,12 | 76,50 |
| Release — GAME, 40 s | 151 | 78,05 | 78,27 | 78,27 |
| Release — GAME após dirigir, ~10 s | 38 | 76,34 | 78,15 | 76,34 |
| Debug — MENU com o jogo já aberto, 10 s | 38 | 77,11 | 77,27 | 77,27 |

**Leitura.** O **pico amostrado** do Release é **79,41 MB**, na janela de menu; a carga medida com o script iniciado antes do jogo deu **79,12 MB**, com a privada em **100,03 MB** no mesmo instante. Do pico para o repouso no menu o working set cai para ~76,7 MB. Em partida, o working set fica em **78,0–78,3 MB** e **não sobe** quando o jogador anda: a janela seguinte à movimentação fechou em 76,34 MB, mais baixa que a de repouso. As janelas de GAME não têm scan nem tiro — o HUD da captura estava em `identificados 0/9` e `ameaças 0/6` —, então varredura e combate ficam para a #117. Contra o limite de 100 MB sobram **20,6 MB** no pior instante medido.

**Memória privada.** O pico de 79,41 MB é de **working set**. A privada é maior: na carga medida com o script antes do jogo ela chega a **100,03 MB** (`t=2,42 s`, no mesmo platô do pico de working set), e a primeira amostra da janela de 20 s leu **101,02 MB**; em partida fica em **98,37–98,55 MB**. O critério adotado é o **working set** — o número que o Gerenciador de Tarefas mostra —, e é ele que precisa ficar abaixo de 100 MB. Se a leitura de "RAM" for memória privada, o pico de carga passa do limite e o assunto deixa de ser medição: é otimização (#118/#119).

**Tolerância.** Entre execuções, **±2 MB é ruído**: as janelas medidas vão de 76,34 a 79,41 MB e a de repouso fica em ~76,7. Na #116, só um **Δ acumulado acima de 5 MB** conta como tendência.

**Desvio declarado.** O roteiro pede 60 s de partida com scan e combate manuais; a automação disponível não joga a fase inteira sem foco. Medi 40 s de GAME com os agentes se movendo e mais ~10 s de amostragem depois de dirigir o Nanobô por ~7 s — a diferença entre as duas janelas é menor que 2 MB e é para baixo, então o pico não está na ação do jogador. A varredura manual completa fica para a #117, quando o áudio e as variantes estiverem fechados.

**CSVs:** `t7-release-load.csv`, `t7-release-game.csv`, `t7-release-pos-drive.csv` e `t7-load-real.csv` (a carga de 35 s), gravados em `C:/Users/GIGABYTE/AppData/Local/hermes/cache/scratch/`; a janela de Debug é `%TEMP%/nanobo-medida-teste.csv`.

---

## 11/11/2026 — #116: reinícios

- **Build:** `Debug|x64`. O Release não tem tecla de QA para concluir a fase, e o limite de 100 MB é julgado no Release pela #115; aqui o que importa é a **tendência entre reinícios**, não o valor absoluto.
- **Ciclo completo:** `ESC` (pula o briefing) → `F` (conclui a Fase 1, tecla de QA só do Debug) → `ENTER` (Results → Fase 2) → `F` (conclui a Fase 2) → `ENTER` (Results → Vitória) → `ENTER` (Vitória → briefing de uma partida nova). Cinco ciclos, com **4 s** de amostragem (`-EveryMs 200`) depois de cada um — janela curta de propósito, porque a série já saía plana em cada ciclo (19 amostras, desvio-padrão 0,000 MB) e a repetição de 20 s abaixo confirma que ela não cresce. A captura final mostra o briefing da partida nova, o que prova que o reinício aconteceu de fato.

| Ciclo | Working set depois de 4 s (MB) | Δ para o ciclo anterior |
|---|---:|---:|
| 1 | 77,23 | — |
| 2 | 77,29 | +0,06 |
| 3 | 75,47 | −1,82 |
| 4 | 75,47 | 0,00 |
| 5 | 75,46 | −0,01 |

**Repetição de 20 s.** A janela de 4 s não esconde tendência: depois de um ciclo completo, a mesma captura rodou por **20 s** (`-EveryMs 250`, 19,93 s de CSV, cadência de 0,265 s no CSV, em `t8c-ciclo20.csv`) e saiu `amostras=76`, `min=77,27 MB`, `max=77,42 MB`, `ultimo=77,42 MB`. O regime não muda entre 4 s e 20 s — a série fica plana dentro do ciclo (sd 0,075) e ~1,8 MB acima dos ciclos 3–5, dentro da tolerância de ±2 MB; a diferença entre a primeira e a última amostra é de 0,15 MB, dentro do ruído.

**Leitura.** Não há crescimento persistente: a série sobe 0,06 MB e depois **cai** 1,82 MB, terminando 1,77 MB abaixo do primeiro ciclo. O degrau de −1,82 MB aparece **uma única vez em cinco ciclos**, e os cinco passam por troca de fase: não há como atribuí-lo a um mecanismo, **a causa não foi medida**, e a conclusão não depende dela — o que se procura é crescimento, e não há. **Nenhuma evidência de vazamento por reinício**, então a #118 não tem, por este caminho, o que investigar.

**CSVs:** `t8b-ciclo-1.csv` … `t8b-ciclo-5.csv` e `t8c-ciclo20.csv` (a janela de 20 s), gravados em `C:/Users/GIGABYTE/AppData/Local/hermes/cache/scratch/`.

**Limite do que foi provado:** os cinco reinícios são por **conclusão de fase (Vitória)**. Reinício por Game Over e o retorno ao menu (#195, ainda aberto) não foram exercitados aqui e ficam para a #117/#118.
