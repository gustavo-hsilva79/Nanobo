# Nanobô — QA do MVP (Fase 1)

> **Escopo:** roteiro ponta a ponta da Fase 1 e do fluxo de telas do MVP.
> **Branch:** `feat/mvp-fluxo-e-hud` · **Commit do código testado:** `79d97b9` · **PR:** [#193](https://github.com/gustavo-hsilva79/Nanobo/pull/193)
> **Data da execução parcial:** 26/10/2026
> **Atualização:** 28/10/2026 — passo 3 (clique em `INICIAR`) executado e corrigido; evidência na §8.
> **Atualização:** 30/10/2026 — textos da interface e do feedback acentuados (#194) e evidências datadas do polimento de menus e HUD (#196); ver §9.
> **Atualização:** 03/11/2026 — roteiro do fluxo educativo (briefing, dois pulsos, módulos A/B, controles por cursor e resumo por objetivo) incorporado como §10; **nenhum passo executado**.
> **Ambiente:** Windows 11, monitor 2560×1440, janela maximizada, executável a partir da raiz do repositório; Debug|x64 e Release|x64.

## 1. Como reproduzir

1. Fechar qualquer `Nanobo.exe` aberto (o linker não regrava o executável em uso).
2. Compilar a configuração desejada no Visual Studio ou por MSBuild.
3. Executar `x64\Debug\Nanobo.exe` ou `x64\Release\Nanobo.exe` **a partir da raiz do repositório**, para `assets/` resolver.
4. Seguir os passos da §4. Para os passos que dependem de teclado físico, use o teclado de verdade — a injeção de tecla segurada não é confiável neste ambiente.

### Teclas de QA (somente no build Debug)

| Tecla | Efeito | Uso |
|---|---|---|
| `F` | neutra as 6 ameaças pelos dois acertos válidos e leva o Nanobô ao destino | testar a transição automática para Resultados sem jogar a fase inteira |
| `G` | força Game Over | conferir a tela de derrota |
| `H` | identifica os 9 agentes, neutra as 6 ameaças e leva ao destino | conferir o caminho do bônus +15 |

Essas teclas **não existem no Release** (`#ifdef _DEBUG`). O Release foi testado com `F` e `H`: nenhuma transição acontece.

## 2. Build

| Configuração | Comando | Resultado | Log |
|---|---|---|---|
| Debug\|x64 | `MSBuild Nanobo.vcxproj -p:Configuration=Debug -p:Platform=x64 -t:Rebuild` | 0 avisos, 0 erros | `docs/auditoria/evidencias/build-debug.log` |
| Release\|x64 | idem, `-p:Configuration=Release` | 0 avisos, 0 erros | `docs/auditoria/evidencias/build-release.log` |

## 3. Regras puras (harness fora do repositório)

- Cópia fresca de `src/gameplay/gameplay.c` e `src/entities/entities.c` desta branch.
- Compilação com `/std:c11`, sem Allegro.
- **78 casos, 78 passaram, 0 falharam.**
- Inclui os 68 casos anteriores e 10 novos: `threats_total`, bônus pago/não pago, reset do bônus, `attack_ready` antes/depois do disparo e durante a defesa.

## 4. Roteiro e resultado

Legenda: **[OK]** executado e observado · **[PEND]** depende de teclado físico ou de pessoa · **[N/E]** não executado nesta sessão.

**Como ler os passos 2, 11 e 16:** a HUD de 26/10 mostrava `restam: 6/6` em cinco linhas de texto. Desde `4ae9135` a linha do HUD é `identificados x/9`, `ameaças` com marcas, `pontos`, tempo e o raio com a barra de recarga; a tela atual está na §9. Os resultados observados na data original ficam como estão.

| # | Passo | Esperado | Observado | Status | Evidência |
|---|---|---|---|---|---|
| 1 | Abrir o jogo | Menu com título, botão e instrução do objetivo | Menu com objetivo, controles e regra Gram/laser | **[OK]** | `qa-mvp-01-menu.png` |
| 2 | Iniciar por `ENTER` | Partida com 3 corações, 0/9, restam 6/6 | 0/9, 6/6, 0 pontos, 3 corações | **[OK]** | `qa-mvp-02-jogo.png` |
| 3 | Iniciar por clique em `INICIAR` | Mesma partida | Clique dentro do botão inicia a partida; clique fora do retângulo não inicia (§8) | **[OK]** | `qa-mvp-10-menu-clique.png`, `qa-mvp-11-clique-iniciar.png` |
| 4 | Mover com setas/WASD e parar ao soltar | Nanobô anda e para; não sai do mundo | — | **[PEND]** | — |
| 5 | Aproximar de uma bactéria e apertar `SPACE` 1× | Camada 1; contador 0/9 | — | **[PEND]** | — |
| 6 | `SPACE` 2ª vez | Camada 2 descreve o envelope; contador 1/9 | — | **[PEND]** | — |
| 7 | `SPACE` 3ª/4ª vez no mesmo alvo | Nada muda; contador 1/9 | — | **[PEND]** | — |
| 8 | Atirar com o raio errado | Nenhum dano; mensagem explica a parede/membrana | — | **[PEND]** | — |
| 9 | Atirar 2× com o raio certo | +10; agente sai da cena | — | **[PEND]** | — |
| 10 | Atacar uma célula saudável | −1 vida, célula intacta, mensagem | — | **[PEND]** | — |
| 11 | Neutralizar as 6 ameaças | `restam: 0/6`; objetivo muda para o destino | — (QA `F`/`H` neutraliza sem passar pelo combate) | **[PEND]** | — |
| 12 | Chegar ao destino | Entra em Resultados sozinho, uma vez | Resultados abriu sozinho com `F` e com `H` | **[OK]** | `qa-mvp-03-resultados.png`, `qa-mvp-04-resultados-bonus.png` |
| 13 | Conferir Resultados sem bônus | 60 pontos, 6/6, 0/9, bônus não obtido | 60, 6/6, 0/9, não obtido | **[OK]** | `qa-mvp-03-resultados.png` |
| 14 | Conferir Resultados com bônus | 75 pontos, 6/6, 9/9, +15 | 75, 6/6, 9/9, +15 | **[OK]** | `qa-mvp-04-resultados-bonus.png` |
| 15 | `ENTER` em Resultados | Vitória com a pontuação final | Vitória com 75 | **[OK]** | `qa-mvp-05-vitoria.png` |
| 16 | `ENTER` em Vitória | Nova partida com vida, score, contadores e tempo zerados | 0/9, 6/6, 0 pontos, 3 corações, tempo 0,6 | **[OK]** | `qa-mvp-06-nova-partida.png` |
| 17 | Alt-Tab por 10 s durante a partida | Partida pausada; sem dano | Entrou em `PAUSE` com a mensagem `esc para voltar` | **[OK]** | `qa-mvp-08-pausa-foco.png` |
| 18 | Deixar a vida chegar a 0 | Game Over; `ENTER` reinicia | Partida AFK terminou em `GAME OVER` com `enter = nova partida` | **[OK]** | `qa-mvp-09-game-over.png` |
| 19 | Release + `F` e `H` | Nenhuma transição | Permaneceu em GAME | **[OK]** | `qa-mvp-07-release-sem-teclas.png` |
| 20 | Fechar e reabrir o jogo | Nova execução começa no menu, sem estado anterior | Abriu no menu em todas as execuções desta sessão | **[OK]** | `qa-mvp-01-menu.png` |

## 5. O que este roteiro ainda não prova

- Movimento, leitura do scanner e combate com teclado físico (passos 4–11). A regra está coberta pelo harness, mas a experiência real não foi observada nesta sessão.
- Comportamento em máquina limpa e a partir de um atalho do instalador (não há instalador).
- Aprendizagem e diversão: exigem playtest com pessoas; protocolo em `docs/planejamento/qa-mvp.md` §6.

## 6. Protocolo de playtest (3–5 pessoas, ~15 min)

**Preparação:** build Release, `assets/` ao lado, sem explicação prévia além de “é um jogo educativo; jogue como quiser”.

**Tarefas observadas, sem ajudar:**
1. Iniciar e dizer em voz alta qual é o objetivo.
2. Identificar um alvo usando o scanner.
3. Usar a informação para escolher o raio.
4. Encontrar uma célula saudável e decidir o que fazer.
5. Explicar uma mensagem de erro e tentar de novo.

**Registrar:** tempo até o primeiro scan; se leu a camada 2; erros de laser por espécie; se atacou célula; se achou o destino; tempo total; três falas literais (sem identificar a pessoa).

**Perguntas finais:** “O que você aprendeu?” · “Qual decisão exigiu mais raciocínio?” · “Em que momento perdeu o interesse?” · “O que não entendeu?”

**Limiar para agir:** se ≥2 de 3 jogadores vencem sem escanear, o atalho educativo está confirmado; se ≥2 não entendem o objetivo, a HUD é a prioridade.

## 7. Registro de validação

| Etapa | Quem executou | Data | Resultado |
|---|---|---|---|
| Build Debug/Release | auditoria | 26/10/2026 | 0 avisos, 0 erros |
| Harness de regras | auditoria | 26/10/2026 | 78/78 |
| Fluxo de telas com teclas de QA | auditoria | 26/10/2026 | OK (passos 12–16, 19) |
| Pausa por perda de foco | auditoria | 26/10/2026 | OK (passo 17; Alt+Tab real) |
| Clique em `INICIAR` | auditoria | 28/10/2026 | OK (passo 3; corrigido, §8) |
| Acentuação dos textos (#194) | auditoria | 30/10/2026 | OK (seis telas, §9) |
| Harness de regras após a acentuação | auditoria | 30/10/2026 | 83/83 (§9) |
| Passos com teclado físico | — | — | pendente |
| Playtest com pessoa externa | — | — | pendente |
| Revisão cruzada do PR | — | — | pendente |

## 8. Atualização 28/10/2026 — clique em `INICIAR`

O passo 3 estava pendente porque o clique não iniciava a partida. A causa não era o retângulo do botão: com a janela criada por `ALLEGRO_MAXIMIZED`, o Allegro continuava reportando o tamanho pedido na criação (1920×1080) em vez do cliente real (2560×1411), porque o resize da maximização acontece antes da fila de eventos existir e o `al_acknowledge_resize` do loop nunca recebia o evento. O driver esticava então o backbuffer de 1920×1080 na janela inteira, enquanto `update_screen_fit()` calculava escala 3 com offset (0, 0) e `handle_mouse_click` convertia o clique com um tamanho que não era o da tela — o clique caía fora do retângulo 220..420 × 200..250 do `INICIAR`.

Correção em `src/main.c`, sem mudar coordenada nenhuma:

- `create_window()` termina com `al_acknowledge_resize(display)` + `update_screen_fit()`, então a abertura já sincroniza o tamanho real da janela.
- `toggle_fullscreen()` faz o mesmo par depois da troca de modo e, ao sair do fullscreen, reaplica `ALLEGRO_MAXIMIZED` — a janela volta maximizada em vez de encolher para 1920×1080.

Evidência (Debug|x64, árvore com a correção, ainda sem commit). Na abertura o cliente é 2560×1411, escala 3 com offset (320, 165); os pontos abaixo estão em pixel do cliente:

| Ponto clicado | Pixel lógico | Resultado |
|---|---|---|
| 1280, 840 (centro) | 320, 225 | inicia a partida |
| 985, 770 (canto interno) | 221, 201 | inicia a partida |
| 1280, 950 (abaixo do botão) | 320, 261 | permanece no menu |
| 1590, 840 (fora à direita) | 423, 225 | permanece no menu |

- `qa-mvp-10-menu-clique.png` — menu maximizado antes do clique.
- `qa-mvp-11-clique-iniciar.png` — partida aberta pelo clique.
- F11: fullscreen em 2560×1440 (4× exato, sem barras); na volta, janela maximizada em 2556×1407, escala 3 com offset (318, 163), e o clique no botão continua iniciando a partida. `ENTER` e o fluxo Resultados/Vitória não mudam.

## 9. Atualização 30/10/2026 — textos acentuados e evidências do polimento

**Commit testado:** `75030ea` (`master`) — textos em UTF-8 acentuado, `/utf-8` no `ItemDefinitionGroup` x64 e o harness com as asserções de texto atualizadas.
**Build:** `Debug|x64` e `Release|x64` com `-t:Rebuild`: **0 avisos, 0 erros**.
**Ambiente:** Windows 11, monitor 2560×1440 a 125%, janela maximizada — cliente 2560×1411, escala inteira 3× com offset (320, 165). Executável aberto a partir da raiz do repositório.
**Como as capturas foram feitas:** o jogo foi dirigido por `PostMessage` (`ENTER`, `ESC`, `F`, `G`), sem roubar o foco, e a área de cliente foi copiada em 1:1 com `CopyFromScreen`; os PNGs deste registro estão reduzidos a 50% (1280×706) com `magick`.

### O que foi observado

| # | Passo | Resultado | Evidência |
|---|---|---|---|---|
| 21 | Abrir o jogo | Menu com `NANOBÔ`, `enter ou clique para começar` e `objetivo: neutralizar as 6 bactérias e chegar ao destino` | `qa-mvp-12-menu-acentos.png` |
| 22 | `ENTER` inicia a partida | HUD `identificados 0/9  ameaças`, `pontos 0`, `00:01`, raio roxo e `pronto`; objetivo `neutralizar as bactérias` | `qa-mvp-13-jogo-acentos.png` |
| 23 | `ESC` pausa | `PAUSE`, botão `CONTINUAR` e `esc para voltar` sobre o mundo escurecido | `qa-mvp-14-pausa-acentos.png` |
| 24 | `F` (QA) conclui a fase | `FASE 1 CONCLUÍDA`, `pontos 60`, `ameaças neutralizadas 6/6`, `agentes identificados 0/9`, `bônus de identificação  não obtido`, `enter também continua` | `qa-mvp-15-resultados-acentos.png` |
| 25 | `ENTER` em Resultados | `VITÓRIA`, `você defendeu o organismo`, `pontuação final 60`, `ameaças neutralizadas 6/6`, `tempo de partida`, `enter também começa de novo` | `qa-mvp-16-vitoria-acentos.png` |
| 26 | `ENTER` e depois `G` (QA) | `GAME OVER`, `suas 3 vidas acabaram`, `0/6` e `enter também reinicia` | `qa-mvp-17-game-over-acentos.png` |

- Nenhuma das seis telas exibiu mojibake: `ç`, `ã`, `ê`, `ó` e `ô` saem como um glifo só e o layout não se desloca — a fonte embutida desenha UTF-8 e a largura é contada por caractere.
- O painel do scanner também foi observado com alvo no alcance: `alvo 8`, `bactéria detectada` (texto de `src/gameplay/gameplay.c`) e `space: revelar a próxima camada`, sem mojibake.

### O que não foi testado nesta execução

- **Mensagens de feedback na tela** (`ameaça neutralizada: +10 pontos`, `contato com bactéria: -1 de vida`, raio errado em cada espécie, ataque à célula): exigem mirar e andar com tecla segurada, e neste ambiente a injeção de tecla perde o foco para outra janela. A codificação está provada em dois níveis — o harness compara as sete mensagens byte a byte (83/83) e as sequências UTF-8 acentuadas estão no binário. A captura on-screen fica pendente.
- Teclado físico: os passos 4–11 e 19 continuam `[PEND]`. `F11` já está registrado na §8 (fullscreen 4× sem barras) e não foi reexecutado nesta sessão.
- Escala 4× (fullscreen em 1440p) e janela mínima: não recapturadas nesta sessão.
- Playtest com pessoa externa (§6).

### Harness de regras

**83 casos, 83 passaram, 0 falharam** — mesmo total da medição de 28/10. Nove asserções de texto passaram a comparar a grafia acentuada; nenhum caso foi acrescentado ou removido.


## 10. Roteiro do fluxo educativo (03/11/2026)

**Estado (03/11/2026, manhã):** nenhum passo executado. O fluxo depende das issues #103–#107 e #198–#200, que não estão no código naquele momento — o scanner ainda lê por tecla e não existe cena de `BRIEFING`.

**Atualização (03/11/2026, após o merge do PR #203):** o roteiro passou a ser executável em parte. #198, #105 e #200 estão em `master`, então E9–E12 (módulos, roda, teclas e clique no HUD) e a parte de controles de E15 podem ser rodados. Continuam bloqueados E1–E3 (briefing), E4 no que depende do **nome do grupo** no pulso 2 (D2.1, ainda não implementado), E13 (resumo por objetivo) e E14 na parte do resumo. Nenhum passo foi executado até esta atualização.

**Como executar:** Debug e Release x64, a partir da raiz do repositório, com teclado e mouse físicos. Os passos 4–11 da §4 continuam válidos apenas para o controle antigo; com os controles novos, valem os passos abaixo.

| # | Passo | Esperado | Status |
|---|---|---|---|
| E1 | Abrir o jogo e escolher `INICIAR` | Briefing com o primeiro cartão antes da fase | **[PEND]** |
| E2 | Avançar com ENTER, → ou clique até o terceiro cartão | Três cartões, sem corte em 640×360 | **[PEND]** |
| E3 | Sair do briefing com ESC e iniciar de novo | Vai direto para a fase; 3 corações, 0/9, 6/6, 0 pontos | **[PEND]** |
| E4 | Escanear uma bactéria com o botão direito | Pulso 1 roxo ou rosa; pulso 2 com o nome do grupo e a camada; contador 1/9 | **[PEND]** |
| E5 | Escanear a mesma bactéria de novo | Nada muda; contador continua 1/9 | **[PEND]** |
| E6 | Escanear uma célula própria | Identificação registrada; o texto não recomenda ação | **[PEND]** |
| E7 | Atirar com o módulo errado | Nenhum dano; mensagem em forma de pergunta; disparo travado | **[PEND]** |
| E8 | Repetir o erro no mesmo grupo | Mensagem explica o mecanismo, sem PBP, LPS ou ligações cruzadas | **[PEND]** |
| E9 | Atirar 2× com o módulo certo | +10; agente sai da cena | **[PEND]** |
| E10 | Atirar na célula com A e com B | −1 vida em cada caso; célula intacta e ainda identificável | **[PEND]** |
| E11 | Trocar o módulo pela roda e por `1`/`2` | HUD mostra o módulo ativo | **[PEND]** |
| E12 | Clicar no ícone do módulo no HUD | Troca o módulo e não dispara | **[PEND]** |
| E13 | Concluir a fase | Resultados com resumo por objetivo: scans por grupo, decisões certas e erradas, ataques a células | **[PEND]** |
| E14 | Reiniciar por ENTER | Vida, score, identificação e contadores zerados | **[PEND]** |
| E15 | Repetir o fluxo em Release | Mesmo comportamento, sem as teclas de QA | **[PEND]** |

**Limite deste roteiro:** ele prova que o fluxo funciona, não que ensina. A aprendizagem é medida no pré/pós-teste P1–P4 do M5 (#134–#137), registrado em `qa-aprendizagem.md` (a criar).
