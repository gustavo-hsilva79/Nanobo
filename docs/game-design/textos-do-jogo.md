# Nanobô — textos do jogo (M4)

> **Criado em:** 03/11/2026. Fontes das regras: `docs/game-design/ensino-e-aprendizagem.md` (§"Feedback por situação" e §"Regras para qualquer texto"), `docs/game-design/decisoes.md` (armas, coloração e camada 1, 03/11 noite) e as issues #105, #198 e #200.
> Todo texto novo entra aqui com ID, tela, limite de caracteres e de onde vem a regra. Nenhum texto usa PBP, transpeptidase, LPS ou ligações cruzadas.
> **Atualizado em:** 04/11/2026 — todos os textos que o build de 04/11 desenha estão registrados aqui: briefing inteiro, marcador de origem da leitura, frases de revisão da camada 2 e o resumo por contadores. A Fase 2 mínima e o recorde entraram na mesma data (RT-09–RT-11, CT-42–CT-47). A tabela é espelho do código; o bloco **Drift conhecido** saiu porque não há mais divergência entre tabela e tela.

## 1. Feedback da decisão (HUD da fase, `src/gameplay/gameplay.c`)

| ID | Situação | Texto | Caracteres | Origem |
|---|---|---|---|---|
| FB-01 | Ruptura acerta Gram+, primeiro acerto | `Ruptura atingiu a parede: falta 1 acerto` | 40 | spec, tabela de feedback |
| FB-02 | Dissolução acerta Gram-, primeiro acerto | `Dissolução atingiu a camada extra: falta 1 acerto` | 49 | spec, tabela de feedback |
| FB-03 | acerto que neutraliza, agente identificado | `ameaça neutralizada: +10 pontos` | 31 | já existia (#66) |
| FB-04 | acerto que neutraliza sem identificação | `neutralizada sem identificar: o bônus de 9/9 se perdeu` | 54 | já existia (#67) |
| FB-05 | primeiro erro de arma | `que estrutura o scan mostrou? qual arma tem esse alvo?` | 54 | spec, dica escalonada |
| FB-06 | segundo erro, Dissolução em Gram+ | `no jogo, a Gram+ não tem a camada extra atingida pela Dissolução` | 64 | spec, dica escalonada |
| FB-07 | segundo erro, Ruptura em Gram- | `no jogo, a camada extra das Gram- dificulta a ação da Ruptura` | 61 | spec, dica escalonada |
| FB-08 | célula própria ferida pela Ruptura | `no jogo, a Ruptura fere a célula própria: -1 vida` | 49 | spec + D1 |
| FB-09 | célula própria ferida pela Dissolução | `no jogo, a Dissolução fere a célula própria: -1 vida` | 52 | spec + D1 |
| FB-10 | contato com bactéria | `contato com bactéria: -1 de vida` | 32 | já existia (#11) |

**Hífen, não menos tipográfico.** "Gram-" usa hífen ASCII (U+002D). O "−" (U+2212) que a spec traz não existe na fonte embutida (Latin-1): na tela ele saía como `^`, visto no QA de 03/11 e corrigido no commit `5e7cc75`. Ao editar estes textos, não reintroduza U+2212.

## 1.1 Briefing (cena `BRIEFING`, #103, #106)

Desenhado **em toda partida** — `MENU → BRIEFING → GAME`, e também depois de `VITÓRIA` e de `GAME OVER`. São três cartões; `ENTER`, `→` ou clique avançam, e `ESC` pula direto para a fase. O cartão 3 tem **quatro linhas** desde 04/11: a frase aprovada em 03/11 foi quebrada em duas (BR-18 e BR-19) para caber em 640×360. A BR-01 antiga (`nem toda bactéria causa doença: aqui são ameaças do cenário`) não é mais desenhada.

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| BR-02 | briefing, cabeçalho | `Antes de entrar na fase` | 23 | #103, texto aprovado em 04/11 |
| BR-03 | briefing, passo | `cartão %d de %d` (`cartão 1 de 3`) | 13 | #103 |
| BR-04 | briefing, rodapé | `enter ou clique avança · esc pula` | 33 | #103 |
| BR-05 | briefing, botão | `SEGUIR` | 6 | #103 |
| BR-06 | briefing, botão | `PULAR` | 5 | #103 |
| BR-07 | briefing, cartão 1, título | `A coloração de Gram` | 19 | #103 |
| BR-08 | briefing, cartão 1, linha 1 | `A coloração separa as bactérias em dois grupos.` | 47 | #103 |
| BR-09 | briefing, cartão 1, linha 2 | `O resultado é roxo (Gram+) ou vermelho (Gram-).` | 47 | #103 |
| BR-10 | briefing, cartão 1, linha 3 | `A cor indica o grupo, não se a bactéria causa doença.` | 53 | #103 |
| BR-11 | briefing, cartão 2, título | `Duas camadas externas` | 21 | #103 |
| BR-12 | briefing, cartão 2, linha 1 | `Gram+: parede espessa, rica em peptidoglicano.` | 46 | #103 |
| BR-13 | briefing, cartão 2, linha 2 | `Gram-: parede fina e uma membrana externa.` | 42 | #103 |
| BR-14 | briefing, cartão 2, linha 3 | `O peptidoglicano faz parte da parede da bactéria.` | 49 | #103 |
| BR-15 | briefing, cartão 3, título | `As armas e a célula própria` | 27 | #103 |
| BR-16 | briefing, cartão 3, linha 1 | `Ruptura age na parede; Dissolução, na camada extra.` | 51 | #103; decisão 03/11 |
| BR-17 | briefing, cartão 3, linha 2 | `As duas armas ferem a célula própria: mire longe dela.` | 54 | #103; D1 |
| BR-18 | briefing, cartão 3, linha 3 | `Nem toda bactéria causa doença; neste cenário,` | 46 | decisão 03/11, quebrada em 04/11 |
| BR-19 | briefing, cartão 3, linha 4 | `os agentes são tratados como ameaças do jogo.` | 45 | decisão 03/11, quebrada em 04/11 |

## 2. Controles, HUD e dicas (`src/main.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| CT-01 | menu, título do painel | `Como jogar` | 10 | #201 |
| CT-02 | menu, grupo 1 | `Movimentação` | 12 | #201 |
| CT-03 | menu, grupo 1 | `setas ou WASD mover · cursor mirar` | 34 | #201 |
| CT-04 | menu, grupo 2 | `Combate` | 7 | #201 |
| CT-05 | menu, grupo 2 | `botão esquerdo atirar · botão direito escanear` | 46 | #201 |
| CT-06 | menu, grupo 3 | `Equipamentos` | 12 | #201 |
| CT-07 | menu, grupo 3 | `1 e 2 ou roda: trocar de arma` | 29 | #201 + decisão 03/11 |
| CT-08 | menu, grupo 3 | `1: Ruptura - age na parede celular` | 34 | decisão 03/11 (noite) |
| CT-08b | menu, grupo 3 | `2: Dissolução - age na camada externa` | 36 | decisão 03/11 (noite) |
| CT-09 | menu, painel do objetivo | `Objetivo da missão` | 18 | #201 |
| CT-10 | menu, painel do objetivo | `neutralizar as %d bactérias e chegar ao destino` | 47 | #201 |
| CT-11 | menu, painel do objetivo | `identificar os %d agentes vale +%d pontos` | 41 | #201 |
| CT-12 | menu, rodapé | `enter ou clique para começar` | 28 | #201 |
| CT-13 | menu, rodapé | `F11 tela cheia` | 14 | #201 |
| CT-14 | HUD, linha 0 | `ameaças %d/%d` | 11 | #199 |
| CT-15 | HUD, linha 1 | `identificados %d/%d` | 17 | #199 |
| CT-16 | HUD, linha 0 | `pontos %d` | 9 | #199 |
| CT-17 | HUD, linha 0 | `pronto` / `recarga` / `defesa` | 6 / 7 / 6 | #199 |
| CT-18 | scanner, sem alvo | `Nenhum alvo` | 11 | #199 |
| CT-19 | scanner, sem alvo | `Aproxime-se de um agente para escanear.` | 39 | #199 |
| CT-20 | scanner, qualquer estado | `botão direito sobre o agente` | 28 | #199 |
| CT-21 | scanner, alvo neutralizado | `Alvo neutralizado` / `Chegue perto de outro agente.` | 17 / 28 | #199 |
| CT-22 | scanner, alvo no alcance | `Alvo detectado` | 14 | #199 |
| CT-23 | scanner, alvo no alcance | `Clique com o botão direito para escanear.` | 41 | #199 |
| CT-24 | scanner, alvo no alcance | `o scan revela os dados do agente` | 32 | #199, texto de 04/11 |
| CT-25 | scanner, 1 camada | `Leitura 1 de 2` | 14 | #199 |
| CT-26 | scanner, 1 camada | `clique de novo para revelar o grupo` | 35 | #199, texto de 04/11 |
| CT-27 | scanner, identificado | `Identificação concluída` | 23 | #199 |
| CT-29 | scanner, cabeçalho | `célula` / `bactéria` | 6 / 7 | #199 |
| CT-30 | resultados | `resumo da fase` | 14 | #202 |
| CT-31 | resultados | `pontos` / `ameaças neutralizadas` | 6 / 21 | #202 |
| CT-32 | resultados | `agentes identificados` / `bônus de identificação` | 21 / 22 | #202 |
| CT-33 | resultados | `não obtido` | 10 | #202 |
| CT-34 | resultados | `enter também continua` | 21 | #202 |
| CT-35 | vitória | `Vitória` | 7 | #202 |
| CT-36 | HUD, linha 0 | `Ruptura` / `Dissolução` | 7 / 10 | decisão 03/11 (noite) |
| CT-37 | resultados, painel novo | `o que a partida mostrou` | 23 | #202, build de 04/11 |
| CT-38 | resultados, painel novo | `bactérias identificadas: %d Gram+ · %d Gram-` (`bactérias identificadas: 1 Gram+ · 1 Gram-`) | 42 | #174/#202, build de 04/11 |
| CT-39 | resultados, painel novo | `acertos de arma: %d · erros: %d` (`acertos de arma: 1 · erros: 0`) | 29 | #174/#202, build de 04/11 |
| CT-40 | resultados, painel novo | `ataques à célula: %d` (`ataques à célula: 0`) | 19 | #174/#202, build de 04/11 |
| CT-41 | resultados, frase de revisão | `reveja a estrutura lida no scan` / `reveja: as duas armas ferem a célula` / `sem erro de arma nesta partida` | 31 / 36 / 30 | #174/#202, build de 04/11 |
| CT-42 | HUD, linha 1, Fase 2 | `objetivo: neutralizar os vírus e chegar ao destino` | 50 | Fase 2 mínima (04/11) |
| CT-43 | scanner, camada 1, vírus | `clique de novo para concluir a leitura` | 38 | Fase 2 mínima (04/11) |
| CT-44 | scanner, categoria | `vírus` | 5 | Fase 2 mínima (04/11) |
| CT-45 | resultados, título | `Fase %d concluída` | 17 | Fase 2 mínima (04/11) |
| CT-46 | vitória / game over | `tempo total` / `recorde` | 11 / 7 | Recorde mínimo (04/11) |
| CT-47 | menu, rodapé | `recorde %d` | 10 | Recorde mínimo (04/11) |

### Textos do scanner (`src/main.c` + `src/gameplay/gameplay.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| RT-01 | scanner, camada 1, bactéria | `roxo · Staphylococcus aureus` | 28 | decisão 03/11 (noite) |
| RT-02 | scanner, camada 1, bactéria | `vermelho · Escherichia coli` | 27 | decisão 03/11 (noite) |
| RT-03 | scanner, camada 1, célula | `célula do organismo · Célula epitelial` (sem amostra de cor) | 38 | decisão 03/11 (noite) |
| RT-04 | scanner, camada 2, Gram+ | `Gram+ · parede espessa, rica em peptidoglicano` | 46 | #104 |
| RT-05 | scanner, camada 2, Gram- | `Gram- · parede fina sob membrana externa` | 40 | #104 |
| RT-05b | scanner, camada 2, célula | `célula saudável do próprio organismo: preserve-a` | 48 | D1 |
| RT-06 | scanner, linha de estado | ` · alvo sob o cursor` (ciano) / ` · última leitura` (apagado) | 20 / 17 | #199, build de 04/11 |
| RT-07 | scanner, camada 2, revisão Gram+ | `Essa parede mais espessa distingue este grupo.` | 46 | #104 |
| RT-08 | scanner, camada 2, revisão Gram- | `A membrana externa adicional distingue este grupo.` | 50 | #104 |
| RT-08b | scanner, camada 2, revisão célula | `preserve a célula: as duas armas ferem o próprio organismo` | 58 | D1 |
| RT-09 | scanner, camada 1, vírus | `vírus · Vírus da fase` | 21 | Fase 2 mínima (04/11) |
| RT-10 | scanner, camada 2, vírus | `Vírus precisa de uma célula hospedeira para se multiplicar` | 58 | Fase 2 mínima (04/11) |
| RT-11 | scanner, camada 2, revisão, vírus | `a célula hospedeira é o que ele usa para se multiplicar` | 55 | Fase 2 mínima (04/11) |

**Limite:** a fonte embutida é de **8 px por caractere** (medido com `al_get_text_width`, não estimado) e a área útil do painel do scanner é de 588 px. A linha mais longa desenhada na fase tem **58 caracteres** (464 px, `preserve a célula: as duas armas ferem o próprio organismo`); a mais longa do briefing tem 54 (432 px, dentro dos 548 px do painel) e a dos resultados, 36 (288 px). Nenhuma passa da área do painel em 640×360.

## 2.1 Regra de tipografia (medida em 03/11/2026)

**Maiúscula acentuada não desenha.** A fonte embutida devolve glifo errado para `Í`, `Ç`, `Ã`, `Ó` maiúsculos — `FASE 1 CONCLUÍDA` saía `CONCLU f DA`, `MOVIMENTAÇÃO` saía `MOVIMENTAÇ%Ø` — enquanto as **minúsculas acentuadas saem corretas** (`ç`, `ã`, `ó`, `ê`, `í`).

Regras que valem para todo texto novo:

- Títulos e rótulos em **caixa de frase** (`Fase 1 concluída`, `Objetivo da missão`, `Identificação concluída`).
- Caixa alta só em texto **sem acento** (`INICIAR`, `CONTINUAR`, `COMO JOGAR` quando não houver acento).
- Nenhum texto desenhado pode ter maiúscula acentuada. `NANOBÔ` é exceção registrada: é o nome do jogo, desenhado em bitmap ampliado 3×, onde o glifo é legível mas não correto.
- Antes de commitar texto novo, rodar `python -c "print([c for c in 'TEXTO' if ord(c) > 0xFF])"` (codepoint fora de Latin-1 vira `^` na tela).
