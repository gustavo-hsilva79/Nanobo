# Nanobô — textos do jogo (M4)

> **Criado em:** 03/11/2026. Fontes das regras: `docs/game-design/ensino-e-aprendizagem.md` (§"Feedback por situação" e §"Regras para qualquer texto"), `docs/game-design/decisoes.md` (armas, coloração e camada 1, 03/11 noite) e as issues #105, #198 e #200.
> Todo texto novo entra aqui com ID, tela, limite de caracteres e de onde vem a regra. Nenhum texto usa PBP, transpeptidase, LPS ou ligações cruzadas.
> **Atualizado em:** 05/11/2026 — varredura completa contra `src/main.c` e `src/gameplay/gameplay.c`: 16 strings que faltavam entraram (CT-48–CT-63) e a tabela voltou a espelhar o código. Texto novo entra aqui no mesmo commit; a conferência está no fim da seção 2.

## 1. Feedback da decisão (HUD da fase, `src/gameplay/gameplay.c`)

| ID | Situação | Texto | Caracteres | Origem |
|---|---|---|---|---|
| FB-01 | primeira dose do antibiótico certo (Gram+ ou Gram-) | `primeira dose: complete o tratamento` | 36 | #215 (FB2-05) |
| FB-01b | prazo da 2ª dose venceu, Gram+ | `tratamento incompleto: resistente à Ruptura` | 43 | #215 (FB2-06) |
| FB-01c | prazo da 2ª dose venceu, Gram- | `tratamento incompleto: resistente à Dissolução` | 46 | #215 (FB2-06) |
| FB-02 | antibiótico do tratamento numa resistente, Gram+ | `resistente à Ruptura: use a Reserva` | 35 | #215 |
| FB-02b | antibiótico do tratamento numa resistente, Gram- | `resistente à Dissolução: use a Reserva` | 38 | #215 |
| FB-03 | acerto que neutraliza, agente identificado | `ameaça neutralizada: +10 pontos` | 31 | já existia (#66) |
| FB-04 | acerto que neutraliza sem identificação | `neutralizada sem identificar: o bônus de 9/9 se perdeu` | 54 | já existia (#67) |
| FB-05 | antibiótico errado depois de ler só a forma (camada 1) | `a forma não define o grupo: veja a coloração` | 44 | #215 (FB2-02) |
| FB-06 | Dissolução em Gram+ | `a Gram+ não tem membrana externa para a Dissolução` | 50 | #215 (FB2-04) |
| FB-07 | Ruptura em Gram- | `a membrana externa barrou a Ruptura` | 35 | #215 (FB2-03) |
| FB-08 | célula do paciente ferida por qualquer ferramenta (-15 vitais) | `você feriu uma célula do paciente: ela tem núcleo, não é bactéria` | 65 | #218 (FB2-01); substitui as três versões por arma |
| FB-11 | Reserva elimina uma resistente | `a Reserva eliminou a resistente: +10 pontos` | 43 | #216 |
| FB-12 | Reserva gasta numa bactéria comum | `Reserva numa bactéria comum: guarde para a resistente` | 53 | #216 (uso racional) |
| FB-13 | disparo da Reserva sem carga | `Reserva esgotada` | 16 | #216 (FB2-07) |
| FB-14 | uma bactéria se dividiu | `uma bactéria se dividiu em duas` | 31 | #217 (FB2-13) |
| FB-10 | contato com invasor (bactéria ou vírus) | `contato com invasor: -8 vitais` | 30 | #11; #218 |

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
| CT-07 | menu, grupo 3 | `1, 2, 3 ou roda: trocar de arma` | 31 | #201 + #216 |
| CT-08 | menu, grupo 3 | `1: Ruptura - age na parede celular` | 34 | decisão 03/11 (noite) |
| CT-08b | menu, grupo 3 | `2: Dissolução - age na camada externa` | 36 | decisão 03/11 (noite) |
| CT-09 | menu, painel do objetivo | `Objetivo da missão` | 18 | #201 |
| CT-10 | menu, painel do objetivo | `neutralizar as %d bactérias e chegar ao destino` | 47 | #201 |
| CT-11 | menu, painel do objetivo | `identificar os %d agentes vale +%d pontos` | 41 | #201 |
| CT-11b | resultados, painel dos objetivos secundários | `bônus de vitais` · `+%d` | 15 | #218; ocupa a linha da CT-11 nos resultados |
| CT-11c | HUD, linha 0 | vitais `%d` (de `0` a `100`), verde ≥ 60, âmbar ≥ 30, vermelho abaixo | até 3 | #218 |
| CT-11d | mundo, acima do agente da dica | `botão direito: escanear` (some no primeiro scan; volta no tiro num invasor sem leitura) | 23 | #219 |
| CT-11e | card de conceito, rótulo e aviso | `conceito` / `enter ou clique para continuar` | 8 / 30 | #222 |
| CT-11f | card de conceito, títulos da Fase 1 | `Coloração de Gram` (CC-01) / `Resistência e Reserva` (CC-02) | 17 / 21 | #222; linhas em §3.1 |
| CT-12 | menu, rodapé | `enter ou clique para começar` | 28 | #201 |
| CT-13 | menu, rodapé | `F11 tela cheia` | 14 | #201 |
| CT-14 | HUD, linha 0 | `ameaças %d/%d` | 11 | #199 |
| CT-15 | HUD, linha 1 | `identificados %d/%d` | 17 | #199 |
| CT-16 | HUD, linha 0 | `pontos %d` | 9 | #199 |
| CT-17 | HUD, linha 0 | `pronto` / `recarga` / `travado` / `defesa` / `esgotada` | 6 / 7 / 7 / 6 / 8 | #199; #215 (trava); #216 (Reserva sem carga) |
| CT-18 | scanner, sem alvo | `Nenhum alvo` | 11 | #199 |
| CT-19 | scanner, sem alvo | `Aproxime-se de um agente para escanear.` | 39 | #199 |
| CT-20 | scanner, qualquer estado | `botão direito sobre o agente` | 28 | #199 |
| CT-21 | scanner, alvo neutralizado | `Alvo neutralizado` / `Chegue perto de outro agente.` | 17 / 28 | #199 |
| CT-22 | scanner, alvo no alcance | `Alvo detectado` | 14 | #199 |
| CT-23 | scanner, alvo no alcance | `Clique com o botão direito para escanear.` | 41 | #199 |
| CT-24 | scanner, alvo no alcance | `o scan revela os dados do agente` | 32 | #199, texto de 04/11 |
| CT-25 | scanner, 1 camada | `Leitura 1 de 2` | 14 | #199 |
| CT-26 | scanner, 1 camada, bactéria | `clique de novo para revelar a coloração` | 39 | #214 |
| CT-27 | scanner, identificado | `Identificação concluída` | 23 | #199 |
| CT-29 | scanner, cabeçalho | `célula` / `bactéria` | 6 / 7 | #199 |
| CT-30 | resultados | `resumo da fase` | 14 | #202 |
| CT-31 | resultados | `pontos` / `ameaças neutralizadas` | 6 / 21 | #202 |
| CT-32 | resultados | `agentes identificados` / `bônus de identificação` | 21 / 22 | #202 |
| CT-33 | resultados | `não obtido` | 10 | #202 |
| CT-34 | resultados | `enter também continua` | 21 | #202 |
| CT-35 | vitória | `Vitória` | 7 | #202 |
| CT-36 | HUD, linha 0 | `Ruptura` / `Dissolução` / `Reserva %d/%d` (`Reserva 2/2`) | 7 / 10 / 11 | decisão 03/11 (noite); #216 |
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
| CT-48 | menu, subtítulo | `DEFENDA O ORGANISMO` | 19 | varredura de 05/11 |
| CT-49 | menu, rodapé do painel de controles | `interprete as informações do scanner antes de atacar` | 52 | varredura de 05/11 |
| CT-50 | HUD, linha 1, Fase 1 | `objetivo: neutralizar as bactérias e chegar ao destino` | 54 | varredura de 05/11 |
| CT-51 | HUD, linha 1, sem ameaças | `objetivo: vá ao destino` | 23 | varredura de 05/11 |
| CT-52 | mundo / marca de destino | `destino` | 7 | varredura de 05/11 |
| CT-53 | pausa, título | `PAUSE` | 5 | varredura de 05/11 |
| CT-54 | pausa, rodapé | `esc para voltar` | 15 | varredura de 05/11 |
| CT-55 | menu, botão | `INICIAR` | 7 | varredura de 05/11 |
| CT-56 | pausa/resultados, botão | `CONTINUAR` | 9 | varredura de 05/11 |
| CT-57 | vitória/game over, botão | `NOVA PARTIDA` | 12 | varredura de 05/11 |
| CT-58 | vitória, subtítulo | `você defendeu o organismo` | 25 | varredura de 05/11 |
| CT-59 | vitória, linha de estatística | `pontuação final` | 15 | varredura de 05/11 |
| CT-60 | vitória, rodapé | `enter também começa de novo` | 27 | varredura de 05/11 |
| CT-61 | game over, título | `GAME OVER` | 9 | varredura de 05/11 |
| CT-62 | game over, subtítulo | `os sinais vitais do paciente zeraram` | 36 | #218 |
| CT-63 | game over, rodapé | `enter também reinicia` | 21 | varredura de 05/11 |

**Conferência de 05/11/2026.** A varredura cobriu os literais passados a `al_draw_text` em `src/main.c` e as mensagens de `set_feedback` em `src/gameplay/gameplay.c`. Os 16 que faltavam estão em CT-48–CT-63. O título `NANOBÔ` é o caso especial da §2.1 (bitmap montado por `make_text_bitmap`) e as linhas de QA sob `#ifdef _DEBUG` não entram na tabela.

### Textos do scanner (`src/main.c` + `src/gameplay/gameplay.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| RT-01 | scanner, camada 1, coco | `forma: coco (esférica)` (sem amostra de cor) | 22 | #214, design 2.0 |
| RT-02 | scanner, camada 1, bacilo | `forma: bacilo (bastão)` (sem amostra de cor) | 22 | #214, design 2.0 |
| RT-03 | scanner, camada 1, célula | `célula do paciente · eucarionte` (sem amostra de cor) | 31 | #214, design 2.0 |
| RT-04 | scanner, camada 2, Gram+ (amostra roxa) | `Gram+ · parede grossa, sem membrana externa · %s` (`... · Streptococcus pyogenes`) | 46 + nome (até 68) | #214, design 2.0 |
| RT-05 | scanner, camada 2, Gram- (amostra rosa) | `Gram- · parede fina sob membrana externa · %s` (`... · Pseudomonas aeruginosa`) | 43 + nome (até 65) | #214, design 2.0 |
| RT-05b | scanner, camada 2, célula | `núcleo guarda o DNA · mitocôndrias · não atacar` | 47 | #214, design 2.0 |
| RT-06 | scanner, linha de estado | ` · alvo sob o cursor` (ciano) / ` · última leitura` (apagado) | 20 / 17 | #199, build de 04/11 |
| RT-07 | scanner, camada 2, revisão bactéria (Gram+ e Gram-) | `procarionte · sem núcleo: DNA solto no nucleoide` | 48 | #214, OA1 |
| RT-08b | scanner, camada 2, revisão célula | `eucarionte: tem núcleo, como as células do corpo` | 48 | #214, OA1 |
| RT-09 | scanner, camada 1, vírus | `vírus · Vírus da fase` | 21 | Fase 2 mínima (04/11) |
| RT-10 | scanner, camada 2, vírus | `Vírus precisa de uma célula hospedeira para se multiplicar` | 58 | Fase 2 mínima (04/11) |
| RT-11 | scanner, camada 2, revisão, vírus | `a célula hospedeira é o que ele usa para se multiplicar` | 55 | Fase 2 mínima (04/11) |

**Limite:** a fonte embutida é de **8 px por caractere** (medido com `al_get_text_width`, não estimado) e a área útil do painel do scanner é de 588 px, a partir de `text_x = 36`. A linha mais longa desenhada na fase é a RT-04 com *Streptococcus pyogenes*: **68 caracteres** (544 px) mais a amostra de 12 px, 556 px no total (medido em 13/11, #214). A mais longa do briefing tem 54 (432 px, dentro dos 548 px do painel) e a dos resultados, 36 (288 px). Nenhuma passa da área do painel em 640×360. A RT-08 (revisão Gram- separada) saiu em 13/11: Gram+ e Gram- compartilham a RT-07, porque a revisão agora fala do procarionte, não do grupo.

## 2.1 Regra de tipografia (medida em 03/11/2026)

**Maiúscula acentuada não desenha.** A fonte embutida devolve glifo errado para `Í`, `Ç`, `Ã`, `Ó` maiúsculos — `FASE 1 CONCLUÍDA` saía `CONCLU f DA`, `MOVIMENTAÇÃO` saía `MOVIMENTAÇ%Ø` — enquanto as **minúsculas acentuadas saem corretas** (`ç`, `ã`, `ó`, `ê`, `í`).

Regras que valem para todo texto novo:

- Títulos e rótulos em **caixa de frase** (`Fase 1 concluída`, `Objetivo da missão`, `Identificação concluída`).
- Caixa alta só em texto **sem acento** (`INICIAR`, `CONTINUAR`, `COMO JOGAR` quando não houver acento).
- Nenhum texto desenhado pode ter maiúscula acentuada. `NANOBÔ` é exceção registrada: é o nome do jogo, desenhado em bitmap ampliado 3×, onde o glifo é legível mas não correto.
- Antes de commitar texto novo, rodar `python -c "print([c for c in 'TEXTO' if ord(c) > 0xFF])"` (codepoint fora de Latin-1 vira `^` na tela).

## 2.2 Relatório da fase (resultados, #210 e #223)

Desde 14/11 (#223), o painel "o que a partida mostrou" fala em linguagem de jogador, sem os códigos internos "OA1:" a "OA4:". São cinco linhas de 14 px, uma por conteúdo, com os números da partida (`MatchProgress`), geradas por `results_report_line` em `src/main.c`. A maior linha tem 41 caracteres (328 px) e cabe nos 344 px do painel. As mensagens OA-01 a OA-04 da #210 saíram do código.

| ID | Linha | Texto | Caracteres | Contador |
|---|---|---|---:|---|
| RR-01 | 1, fase com bactéria | `lidas até a coloração: %d Gram+ e %d Gram-` | 40 | `scans_gram_positive`, `scans_gram_negative` |
| RR-02 | 2, fase com bactéria | `tratamentos completos: %d · resistentes: %d` | 41 | `treatments_completed`, `resistances` |
| RR-03 | 3, fase com bactéria | `nenhum antibiótico errado` / `antibiótico errado: %d (%d pela forma)` | 25 / 38 | `wrong_decisions`, `shape_guesses` |
| RR-04 | 4, fase com bactéria | `Reserva guardada: nenhuma carga gasta` / `Reserva: %d na resistente, %d na comum` | 37 / 36 | `reserve_on_resistant`, `reserve_on_common` |
| RR-05 | 5, fase com bactéria; 3, fase de vírus | `células do paciente feridas: %d` | 30 | `cell_attacks` |
| RR-06 | 1, fase de vírus | `vírus não têm coloração de Gram` | 31 | — |
| RR-07 | 2, fase de vírus | `vírus neutralizados: %d de %d` | 27 | ameaças da fase |

**Leitura dos contadores:** um tratamento completo é uma bactéria eliminada com as duas doses do antibiótico certo. A Reserva não conta como tratamento, e o relatório a separa entre a gasta numa resistente e a gasta numa bactéria comum. `shape_guesses` conta o antibiótico errado dado depois de ler só a forma (FB-05).

**Regra de tipografia:** seguir a seção 2.1. Utilizar hífen ASCII em `Gram-` e evitar maiúsculas acentuadas.

**Fonte:** Issue #210 — Resumo por objetivo na tela de Resultados (RF-07).

## 3. Nanobô 2.0 — rascunhos (13/11/2026)

> **Rascunhos, ainda não estão no código.** Regras em `ensino-e-aprendizagem.md` (§"Nanobô 2.0"). Os cards têm 3 linhas de até 70 caracteres (contagem abaixo, feita com Python). Os textos de HUD e ficha passam a ser medidos com a fonte TTF nova quando ela entrar; até lá, vale a regra da seção 2.1. Os IDs `CC-` são cards de conceito, `FB2-` são feedback do 2.0 e `FE-` são ferramentas.

### 3.1 Cards de conceito

> **No código desde 14/11 (#222):** CC-01 abre na primeira leitura completa de bactéria e CC-02 na primeira bactéria resistente. Cada card aparece uma vez por partida, sobre a fase parada, com as linhas desenhadas a partir de x = 56 num painel de 576 px; a maior (CC-02a, 66 caracteres) ocupa 528 px. CC-03 a CC-06 entram com as Fases 2 e 3.

| ID | Card / quando aparece | Linha | Car. |
|---|---|---|---:|
| CC-01a | Coloração de Gram · 1ª leitura completa de bactéria | No laboratório, um corante roxo tinge a parede das bactérias. | 61 |
| CC-01b | | Gram+: parede grossa de peptidoglicano, que segura o roxo. | 58 |
| CC-01c | | Gram-: parede fina e membrana externa; perde o roxo e fica rosa. | 64 |
| CC-02a | Resistência e Reserva · 1ª bactéria resistente | Uma dose só não basta: as bactérias que sobrevivem se multiplicam. | 66 |
| CC-02b | | Esta ficou resistente: o mesmo antibiótico não age mais nela. | 61 |
| CC-02c | | A Reserva age nela, mas tem poucas cargas: use só se precisar. | 62 |
| CC-03a | Vírus não é célula · 1ª leitura de vírus ou 1º antibiótico em vírus | O vírus não tem parede, ribossomos nem metabolismo próprio. | 59 |
| CC-03b | | Ele só se multiplica dentro de uma célula. Antibiótico não o atinge. | 68 |
| CC-03c | | A Neutralização leva anticorpos, que se prendem ao vírus livre. | 63 |
| CC-04a | Lisogênico e lítico · 1ª leitura de célula infectada | Lisogênico: o vírus fica dentro da célula, sem destruí-la ainda. | 64 |
| CC-04b | | Lítico: ele faz cópias, a célula se rompe e libera novos vírus. | 63 |
| CC-04c | | A Lise destrói a célula infectada, como faz o linfócito T. | 58 |
| CC-05a | Organismo enfraquecido · início da Fase 3 | Fungos oportunistas aproveitam um organismo enfraquecido. | 57 |
| CC-05b | | Os esporos de Aspergillus estão no ar; o corpo saudável os elimina. | 67 |
| CC-05c | | Quanto pior o paciente chega, mais esporos se instalam no pulmão. | 65 |
| CC-06a | Fungo · 1ª leitura de fungo | O fungo tem núcleo, como as nossas células: é um eucarionte. | 60 |
| CC-06b | | Sua parede é de quitina, não de peptidoglicano: antibiótico não age. | 68 |
| CC-06c | | A Perfuração abre poros na membrana dele, que tem ergosterol. | 61 |
| CC-07a | Flagelo · 1ª investida de um bacilo (14/11) | O bacilo tem flagelo, um chicote que gira e o faz nadar. | 56 |
| CC-07b | | Por isso ele vem atrás de você e dá investidas. | 47 |
| CC-07c | | O coco não tem flagelo: é levado pelo sangue e gruda no tecido. | 63 |
| CC-08a | Biofilme · 1º biofilme formado (14/11) | Bactérias grudadas fazem uma camada pegajosa: o biofilme. | 57 |
| CC-08b | | Ele protege as bactérias: dentro dele, o antibiótico age menos. | 63 |
| CC-08c | | No biofilme a bactéria pede mais uma dose, e você anda devagar. | 63 |
| CC-09a | Endotoxina · 1ª nuvem de endotoxina (14/11) | A membrana externa da Gram- tem uma toxina, o LPS. | 50 |
| CC-09b | | Quando a Gram- se rompe, até pelo antibiótico, o LPS se espalha. | 64 |
| CC-09c | | Saia da nuvem: ela fere o Nanobô enquanto dura. | 47 |

### 3.2 Ferramentas (menu "como jogar" e HUD)

| ID | Texto | Fase |
|---|---|---|
| FE-01 | Ruptura · antibiótico de parede: impede a bactéria de montar a parede | 1 |
| FE-02 | Dissolução · antibiótico de membrana: desfaz a membrana externa das Gram- | 1 |
| FE-03 | Reserva · antibiótico de reserva: age na resistente, poucas cargas | 1 |
| FE-04 | Neutralização · anticorpos: prendem o vírus livre | 2 |
| FE-05 | Lise · destrói a célula infectada já lida, como o linfócito T | 2 |
| FE-06 | Perfuração · antifúngico: abre poros na membrana do fungo | 3 |

### 3.3 Feedback do 2.0

| ID | Situação | Texto |
|---|---|---|
| FB2-01 | Tiro na célula do paciente | você feriu uma célula do paciente: ela tem núcleo, não é bactéria |
| FB2-02 | Arma escolhida pela forma, no grupo errado | a forma não define o grupo: veja a coloração |
| FB2-03 | Ruptura numa Gram− | a membrana externa barrou a Ruptura |
| FB2-04 | Dissolução numa Gram+ | a Gram+ não tem membrana externa para a Dissolução |
| FB2-05 | 1ª dose certa | primeira dose: complete o tratamento (no código: FB-01; o "ª" não foi testado na fonte embutida) |
| FB2-06 | Resistência surgiu | tratamento incompleto: resistente à Ruptura (ou à Dissolução) |
| FB2-07 | Reserva sem carga | Reserva esgotada |
| FB2-08 | Antibiótico em vírus | antibiótico não age em vírus |
| FB2-09 | Neutralização num lisogênico | o vírus está dentro da célula: use a Lise |
| FB2-10 | Eclosão | a célula se rompeu: 3 vírus livres (ciclo lítico) |
| FB2-11 | Antibiótico em fungo | a parede do fungo é de quitina: antibiótico não age |
| FB2-12 | Tiro na ponta da hifa | corte a base da colônia |
| FB2-13 | Bactéria se dividiu | uma bactéria se dividiu em duas |
| FB2-14 | Levedura brotou | a levedura brotou: nasceu outra |

**Narrativa** (abertura, interlúdios, final, game over): rascunhos no [`gdd.md`](gdd.md) §"Nanobô 2.0". Eles entram aqui com ID quando forem desenhados.