# Nanobô — QA do MVP (Fase 1)

> **Escopo:** roteiro ponta a ponta da Fase 1 e do fluxo de telas do MVP.
> **Branch:** `feat/mvp-fluxo-e-hud` · **Commit do código testado:** `79d97b9` · **PR:** [#193](https://github.com/gustavo-hsilva79/Nanobo/pull/193)
> **Data da execução parcial:** 26/10/2026
> **Atualização:** 28/10/2026 — passo 3 (clique em `INICIAR`) executado e corrigido; evidência na §8.
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
