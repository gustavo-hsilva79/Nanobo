# Nanobô — textos do jogo (M4)

> **Criado em:** 03/11/2026. Fontes das regras: `docs/game-design/ensino-e-aprendizagem.md` (§"Feedback por situação" e §"Regras para qualquer texto"), `docs/game-design/decisoes.md` (armas, coloração e camada 1, 03/11 noite) e as issues #105, #198 e #200.
> Todo texto novo entra aqui com ID, tela, limite de caracteres e de onde vem a regra. Nenhum texto usa PBP, transpeptidase, LPS ou ligações cruzadas.
> **Atualizado em:** 15/11/2026 — conferência por script contra `src/main.c` e `src/gameplay/gameplay.c` (#212): o registro espelha o build. Texto novo entra aqui no mesmo commit; o passo de conferência está no fim da seção 2.

## 1. Feedback da decisão (HUD da fase, `src/gameplay/gameplay.c`)

| ID | Situação | Texto | Caracteres | Origem |
|---|---|---|---|---|
| FB-01 | primeira dose do antibiótico certo (Gram+ ou Gram-) | `primeira dose: complete o tratamento` | 36 | #215 (FB2-05) |
| FB-01b | prazo da 2ª dose venceu, Gram+ | `tratamento incompleto: resistente à Ruptura` | 43 | #215 (FB2-06) |
| FB-01c | prazo da 2ª dose venceu, Gram- | `tratamento incompleto: resistente à Dissolução` | 46 | #215 (FB2-06) |
| FB-02 | antibiótico do tratamento numa resistente, Gram+ | `resistente à Ruptura: use a Reserva` | 35 | #215 |
| FB-02b | antibiótico do tratamento numa resistente, Gram- | `resistente à Dissolução: use a Reserva` | 38 | #215 |
| FB-03 | acerto que elimina, agente identificado | `ameaça eliminada: +10 pontos` | 28 | já existia (#66); "eliminada" no lugar de "neutralizada" no redesenho de 28/11 (verbo único, dono em 26/11) |
| FB-04 | acerto que elimina sem identificação | `eliminada sem identificar: o bônus de 9/9 se perdeu` | 51 | já existia (#67); "eliminada" no redesenho de 28/11 |
| FB-05 | antibiótico errado depois de ler só a forma (camada 1) | `a forma não define o grupo: veja a coloração` | 44 | #215 (FB2-02) |
| FB-06 | Dissolução em Gram+ | `a Gram+ não tem membrana externa para a Dissolução` | 50 | #215 (FB2-04) |
| FB-07 | Ruptura em Gram- | `a membrana externa barrou a Ruptura` | 35 | #215 (FB2-03) |
| FB-08 | célula do Léo ferida por qualquer arma (-15 vitais), Fases 1 e 2 (na 2 não há caminho) | `você feriu uma célula do Léo: ela tem núcleo, não é bactéria` | 60 | #218 (FB2-01); "do Léo" no redesenho de 28/11 |
| FB-08b | célula do Léo ferida na Fase 3 (Perfuração ou Ruptura), escolhida por `phase_has_fungi()` | `você feriu uma célula do Léo: é célula do corpo, não fungo` | 58 | redesenho de 28/11: o fungo também tem núcleo |
| FB-11 | Reserva elimina uma resistente | `a Reserva eliminou a resistente: +10 pontos` | 43 | #216 |
| FB-12 | Reserva gasta numa bactéria comum | `Reserva numa bactéria comum: guarde para a resistente` | 53 | #216 (uso racional) |
| FB-13 | disparo da Reserva sem carga | `Reserva esgotada` | 16 | #216 (FB2-07) |
| FB-14 | uma bactéria se dividiu | `uma bactéria se dividiu em duas` | 31 | #217 (FB2-13) |
| FB-10 | contato com invasor (bactéria ou vírus) | `contato com invasor: -10 de integridade` | 39 | #11; #218; #234 (duas barras) |
| FB-15 | célula do Léo perdida (colonizada ou rompida) | `uma célula do Léo foi destruída: -20 vitais` | 43 | #234; "do Léo" no redesenho de 28/11 |
| FB-16 | toxina do coco acerta o Nanobô | `toxina do coco: -8 de integridade` | 33 | #236 |
| FB-17 | a defesa destrói um projétil inimigo | `a defesa destruiu a toxina` | 26 | #236 |
| FB-18 | primeira dose numa bactéria dentro do biofilme | `o biofilme protege a bactéria: ela pede mais uma dose` | 53 | #237 |
| FB-19 | o Nanobô dentro da nuvem de endotoxina | `endotoxina da Gram-: saia da nuvem` | 34 | #237 |
| FB-20 | Neutralização prende um vírus livre | `anticorpos prenderam o vírus: +10 pontos` | 40 | #88 |
| FB-21 | Neutralização numa célula infectada | `o vírus está dentro da célula: use o Ataque T` | 45 | #88 (FB2-09) |
| FB-22 | Neutralização numa célula saudável (sem custo) | `anticorpos não se prendem à célula do paciente` | 46 | #88 |
| FB-23 | Ataque T numa célula infectada ainda não lida (não dispara) | `leia a célula antes do Ataque T` | 31 | #88 |
| FB-24 | Ataque T sem célula infectada no caminho (não dispara) | `o Ataque T só age em célula infectada` | 37 | #88 |
| FB-25 | projétil do Ataque T acerta um vírus livre | `o Ataque T age na célula infectada, não no vírus livre` | 54 | #88 |
| FB-26 | Ataque T tira a célula infectada | `Ataque T: a célula infectada saiu sem romper: -5 vitais` | 55 | #88 |
| FB-27 | antibiótico num vírus | `antibiótico não age em vírus` | 28 | #88 (FB2-08) |
| FB-28 | vírus livre infecta uma célula | `um vírus entrou numa célula: ela está infectada` | 47 | #85 |
| FB-29 | a célula infectada rompe no fim do eclipse | `a célula se rompeu: 6 vírus livres (ciclo lítico)` | 49 | #86 (FB2-10, com os 6 de 14/11) |
| FB-30 | a defesa destrói um vírus da rajada | `a defesa destruiu o vírus da rajada` | 35 | #86 |
| FB-31 | vírus da rajada acerta o Nanobô | `vírus da rajada: -6 de integridade` | 34 | #86 |
| FB-32 | antibiótico (Ruptura ou Dissolução) num fungo, na hifa ou no esporo pousado; trava o disparo | `a parede do fungo é de quitina: antibiótico não age` | 51 | #230 (FB2-11) |
| FB-33 | 1ª dose de Perfuração na levedura: sobram 2 de resistência | `cápsula enfraquecida: faltam 2 doses` | 36 | #230; contagem certa desde a revisão final de 25/11 (era `mais uma dose`) |
| FB-33b | 2ª dose de Perfuração na levedura: sobra 1 de resistência | `cápsula enfraquecida: falta 1 dose` | 34 | revisão final de 25/11 |
| FB-34 | a levedura brota e nasce outra (só se não houver feedback no quadro) | `a levedura brotou: nasceu outra` | 31 | #228 (FB2-14) |
| FB-35 | o Nanobô toca um segmento da hifa (a defesa não protege) | `hifa: -10 de integridade` | 24 | #229 |
| FB-36 | Perfuração na hifa, não na base: corta o segmento da ponta, sem pontos | `corte a base da colônia` | 23 | #229 (FB2-12) |
| FB-37 | esporo do bolor acerta o Nanobô (some sem pousar) | `esporo do bolor: -8 de integridade` | 34 | #229 |
| FB-38 | a defesa destrói um esporo em voo | `a defesa destruiu o esporo` | 26 | #229 |
| FB-39 | Perfuração elimina um esporo pousado: ameaça a menos, sem pontos (decisão do dono, 25/11) | `esporo destruído` | 16 | #229 |
| FB-40 | Perfuração elimina uma colônia que germinou de esporo: ameaça a menos, sem pontos (decisão do dono, 25/11) | `colônia germinada destruída: sem pontos` | 39 | revisão final de 25/11 |

**Hífen, não menos tipográfico.** "Gram-" usa hífen ASCII (U+002D). O "−" (U+2212) que a spec traz não existe na fonte embutida (Latin-1): na tela ele saía como `^`, visto no QA de 03/11 e corrigido no commit `5e7cc75`. Ao editar estes textos, não reintroduza U+2212.

## 1.1 Abertura da narrativa (cena `BRIEFING`, #244)

Desenhada **em toda partida**: `MENU → BRIEFING → GAME`, e também depois de `VITÓRIA` e de `GAME OVER`. Desde 26/11 a cena mostra a abertura na caixa de diálogo do diário (ficha do hospital e Anotação 0, 3 páginas cada, textos aprovados pelo dono em 26/11): `ENTER`, `ESPAÇO` ou clique avançam; `ESC` pula tudo para a fase; o fim da Anotação 0 entra na Fase 1. A cada entrada em `BRIEFING` a abertura recomeça da ficha. Os IDs AB-01 a AB-08 substituem NR-01 a NR-14 e os botões e o passo do briefing de 15/11 (BR-02, BR-03, BR-05 e BR-06 saíram). Linhas de até 67 caracteres desde x = 80; texto final depende de aprovação do dono. O paciente se chama Léo (nome do interlúdio, IL-02).

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| BR-04 | abertura, rodapé (y = 24, centrado) | `[enter] ou [clique] avança · [esc] pula` | 33 | #103 |
| AB-01 | cabeçalho da ficha, sem retrato | `Hospital · ficha do paciente` | 28 | #244 |
| AB-02a | ficha, página 1 de 3 | `Paciente: {Léo Martins Ferreira}, 16 anos.` | 40 | #244 |
| AB-02b |  | `Chegou ao hospital com febre alta.` | 34 | #244 |
| AB-03 | página 2 de 3 | `Exames: uma {infecção} que se espalha pelo sangue.` | 48 | #244 |
| AB-04a | página 3 de 3 | `Conduta: testar um {nanorrobô} médico, ainda protótipo,` | 53 | #244 |
| AB-04b |  | `dentro do corpo de Léo.` | 23 | #244 |
| AB-05 | cabeçalho da Anotação 0, com retrato | `Nanobô · Anotação 0` | 19 | #244 |
| AB-06a | Anotação 0, página 1 de 3 | `{Sistema ligado}! Minha memória está vazia.` | 41 | #244 |
| AB-06b |  | `Minha missão: aprender a reconhecer as ameaças e eliminá-las.` | 61 | #244 |
| AB-07a | página 2 de 3 | `Meu paciente é o {Léo}, de 16 anos, com febre alta.` | 49 | #244 |
| AB-07b |  | `Estou dentro do sangue dele.` | 28 | #244 |
| AB-08a | página 3 de 3 | `Vou descobrir o que está {atacando} o Léo.` | 40 | #244 |
| AB-08b |  | `Anoto aqui tudo o que aprender, até os meus erros.` | 50 | #244 |
| NR-15 | — | `sinais vitais: %s (%d) · a seguir: %s` | — | #244; saiu em 28/11: substituída pela RS-01 |
| NR-16 | — | o `%s` do NR-15 depois da Fase 2 era `fungos no pulmão` | — | #227; saiu em 28/11: substituída pela RS-01 |
| RS-01 | interlúdio: Resultados entre as fases, subtítulo (y 46) | `vitais do Léo: %s (%d) · a seguir: %s`, com `vírus nas vias aéreas` depois da Fase 1 e `fungos no pulmão` depois da Fase 2 (`estável` ≥ 70, `em observação` 40–69, `grave` < 40, já com o piso de 60) | até 67 | redesenho de 28/11; substitui a NR-15 e a NR-16 |
| RS-02 | Resultados da última fase, subtítulo (y 46) | `vitais do Léo: %s (%d)` (mesmos estados da RS-01, com os vitais reais, sem o piso de 60) | até 34 | redesenho de 28/11; substitui a CT-30; a alta aparece na Vitória |

Os textos do briefing de 04/11 (BR-07 a BR-19, sobre a coloração de Gram e as armas) saíram do jogo em 15/11 e ficam no histórico do git.

## 2. Controles, HUD e dicas (`src/main.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| CT-01 | título da cena como jogar (HW-00, y 16) | `Como jogar` | 10 | #201; o painel saiu do menu em 28/11 (MN-05), fica o título da cena |
| CT-02 | — | `Movimentação` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-03 | — | `setas ou WASD mover · cursor mirar` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-04 | — | `Combate` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-05 | — | `botão esquerdo atirar · botão direito escanear` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-06 | — | `Equipamentos` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-07 | — | `1, 2, 3 ou roda: trocar de arma` | — | #201 + #216; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-08 | — | `1: Ruptura - age na parede celular` | — | decisão 03/11 (noite); saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-08b | — | `2: Dissolução - age na camada externa` | — | decisão 03/11 (noite); saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-09 | — | `Objetivo da missão` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-10 | — | `neutralizar as %d bactérias e chegar ao destino` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-11 | — | `identificar os %d agentes vale +%d pontos` | — | #201; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-11b | — | `bônus de vitais` · `+%d` | — | #218; saiu em 28/11: substituída pela RS-04 |
| RS-04 | resultados, painel dos objetivos secundários | `bônus pelos vitais do Léo` · `+%d` | 25 | redesenho de 28/11; substitui a CT-11b |
| CT-11c | HUD, coluna de vida (x 8 a 92) | duas barras de 10 segmentos sem número nem rótulo: linha 0, ícone do Nanobô + bateria da integridade (ciano); linha 1, coração pulsando + ECG dos vitais do paciente (verde ≥ 60, âmbar ≥ 30, vermelho abaixo) | — | #218; #234; redesenho de 28/11 (HD-01) |
| CT-11d | mundo, acima do agente da dica | `[botão direito]: escanear` (some no primeiro scan; volta no tiro num invasor sem leitura) | 23 | #219 |
| CT-11e | — | `conceito` / `enter ou clique para continuar` | — | #222; saíram em 25/11: a caixa do diário (§3.1) tem cabeçalho, contador `n/N` e triângulo |
| CT-11f | caixa do diário, título no cabeçalho (DG-01, DG-02) | `Coloração de Gram` (CC-01) / `Resistência e Reserva` (CC-02) | 17 / 21 | #222; linhas em §3.1 |
| CT-12 | — | `enter ou clique para começar` | — | #201; saiu em 28/11: substituída pela MN-04 |
| MN-04 | menu, rodapé dos botões | `[setas] escolhem · [enter] confirma` | 31 | redesenho de 28/11 (MN-01); substitui a CT-12 |
| CT-13 | menu, rodapé | `[F11] tela cheia` | 14 | #201 |
| CT-14 | HUD, linha 1, à direita | `ameaças %d/%d · identificados %d/%d` (`ameaças 0/6 · identificados 0/9`) | 31 (até 35) | #199; juntas na mesma linha em 15/11 (#243) |
| CT-15 | — | `identificados %d/%d` | — | entrou na CT-14 em 15/11 (#243) |
| CT-16 | HUD, linha 1, a partir de x 100 | `pontos %d` + `·` + tempo mm:ss (`pontos 9999 · 99:59`) | 9 (até 19 com o tempo) | #199; #243; desceu para a linha 1 em 28/11 (HD-01) |
| CT-17 | HUD, linha 0 | `pronto` / `recarga` / `travado` / `defesa` / `esgotada` | 6 / 7 / 7 / 6 / 8 | #199; #215 (trava); #216 (Reserva sem carga) |
| CT-18 a CT-21 | — | `Nenhum alvo`, `Aproxime-se de um agente para escanear.`, `botão direito sobre o agente`, `Alvo neutralizado`, `Chegue perto de outro agente.` | — | saíram em 15/11 (#243): sem alvo, a ficha do scanner some; a dica do tutorial no mundo ensina o botão direito |
| CT-22 | scanner, alvo no alcance | `Alvo detectado` | 14 | #199 |
| SC-03 | scanner, alvo no alcance, linha 1 | `[botão direito]: escanear` | 23 | redesenho de 28/11; substitui a CT-23 |
| SC-04 | scanner, alvo no alcance, linha 2 | `cada clique revela uma camada` | 29 | redesenho de 28/11; substitui a CT-24 |
| SC-02 | scanner, 1 camada | `Leitura incompleta` | 18 | redesenho de 28/11; substitui a CT-25 (os marcadores mostram o que falta) |
| SC-05 | scanner, 1 camada, bactéria | `[botão direito] de novo: revela a coloração de Gram` | 49 | redesenho de 28/11; substitui a CT-26 |
| CT-27 | scanner, identificado | `Identificação concluída` | 23 | #199 |
| CT-29 | scanner, cabeçalho | `célula` / `bactéria` | 6 / 7 | #199 |
| CT-30 | — | `resumo da fase` | — | #202; saiu em 28/11: substituída pela RS-02 |
| CT-31 | resultados e game over (`pontos`) | `pontos` | 6 | #202; o segundo rótulo (`ameaças neutralizadas`, 21) foi trocado pela RS-03 em 28/11: nos resultados na Task 8 e na vitória e no game over na Task 9 |
| RS-03 | resultados, painel principal; vitória e game over, painel de números | `ameaças eliminadas` | 18 | redesenho de 28/11; substitui a CT-31b nas três telas; casa com a RR-09 e a OB-04 |
| CT-32 | resultados | `agentes identificados` / `bônus de identificação` | 21 / 22 | #202 |
| CT-33 | resultados | `não obtido` | 10 | #202 |
| CT-34 | resultados | `[enter] também continua` | 21 | #202 |
| CT-35 | vitória | `Vitória` | 7 | #202 |
| CT-36 | HUD, linha 0, à direita | `Ruptura · parede` / `Dissolução · membrana` / `Reserva %d/%d · resistente` (`Reserva 2/2 · resistente`) | 16 / 21 / 24 | decisão 03/11 (noite); #216; a arma com o alvo dela em 15/11 (#243) |
| CT-36b | HUD, linha 0, Fase 2 | `Neutralização · vírus livre` / `Ataque T · célula infectada` | 27 / 27 | #88; #243 |
| CT-36c | HUD, linha 0, Fase 3 | `Perfuração · antifúngico` | 24 | #230 |
| CT-37 | resultados, título do painel do relatório | `O que anotei nesta fase` | 23 | #202, build de 04/11; voz do Diário desde 26/11 |
| CT-38 a CT-41 | — | contadores e frase de revisão do painel de 04/11 | — | saíram com o relatório por objetivo (#210, #223); os textos atuais são as RR-01 a RR-10 da §2.2 |
| CT-42 | — | `neutralize os vírus e vá ao destino` | — | Fase 2 mínima (04/11); encurtada em 15/11 (#243); saiu do HUD em 28/11 (HD-01): vira uma variante do aviso de objetivo OB-01 |
| CT-42b | — | `elimine os fungos e vá ao destino` | — | #227; saiu do HUD em 28/11 (HD-01): vira uma variante do aviso de objetivo OB-01 |
| SC-06 | scanner, camada 1, vírus, célula e fungo | `[botão direito] de novo: conclui a leitura` | 40 | redesenho de 28/11; substitui a CT-43 |
| SC-11 | scanner, marcador da camada 1 (linha de estado, à direita) | `forma` | 5 | redesenho de 28/11; quadrado 6×6 apagado `UI_SLOT`, aceso `UI_AMBER` |
| SC-12 | scanner, marcador da camada 2, bactéria | `Gram e espécie` | 14 | redesenho de 28/11; aceso `UI_AMBER`, verde com a identificação |
| SC-13 | scanner, marcador da camada 2, vírus, célula e fungo | `estrutura` | 9 | redesenho de 28/11; aceso `UI_AMBER`, verde com a identificação |
| CT-44 | scanner, categoria (só sem leitura; a variante `célula infectada` saiu da linha 0 do cartão em 27/11) | `vírus` / `célula` | 5 / 6 | Fase 2 mínima (04/11); #87 |
| CT-44b | scanner, categoria, fungo (levedura, bolor e esporo pousado) | `fungo` | 5 | #228 |
| CT-45 | resultados, título | `Fase %d concluída` | 17 | Fase 2 mínima (04/11) |
| CT-46 | vitória / game over | `tempo total` / `recorde` | 11 / 7 | Recorde mínimo (04/11) |
| CT-47 | menu, rodapé | `recorde %d · %s` (`recorde 1234 · Jogador`) / `nenhum recorde ainda` | até 29 / 20 | Top 10 (#34), 15/11 |
| CT-48 | menu, subtítulo | `DEFENDA O ORGANISMO` | 19 | varredura de 05/11 |
| CT-49 | — | `interprete as informações do scanner antes de atacar` | — | varredura de 05/11; saiu do menu em 28/11 (MN-05): o conteúdo foi para a cena como jogar (HW-01 a HW-04) |
| CT-50 | — | `neutralize as bactérias e vá ao destino` | — | varredura de 05/11; encurtada em 15/11 (#243); saiu do HUD em 28/11 (HD-01): vira uma variante do aviso de objetivo OB-01 |
| CT-51 | — | `vá ao destino` | — | varredura de 05/11; #243; saiu do HUD em 28/11 (HD-01): vira uma variante do aviso de objetivo OB-01 |
| CT-52 | mundo / marca de destino | `destino` | 7 | varredura de 05/11 |
| OB-01 | aviso de objetivo, topo do mundo (Fase 1) | `Objetivo: elimine as bactérias e chegue ao {destino}` | 50 | redesenho de 28/11; substitui a CT-50; `destino` em âmbar |
| OB-02 | aviso de objetivo, topo do mundo (Fase 2) | `Objetivo: elimine os vírus e chegue ao {destino}` | 46 | redesenho de 28/11; substitui a CT-42; `destino` em âmbar |
| OB-03 | aviso de objetivo, topo do mundo (Fase 3) | `Objetivo: elimine os fungos e chegue ao {destino}` | 47 | redesenho de 28/11; substitui a CT-42b; `destino` em âmbar |
| OB-04 | aviso de objetivo, ameaças zeradas | `Ameaças eliminadas! Agora vá ao {destino}` | 39 | redesenho de 28/11; substitui a CT-51; `destino` em âmbar |
| CT-53 | — | `PAUSE` | — | varredura de 05/11; saiu em 28/11: substituída pela PA-01 |
| CT-54 | — | `esc para voltar` | — | varredura de 05/11; saiu em 28/11: substituída pela PA-06 |
| PA-01 | pausa, título (y 82) | `PAUSA` | 5 | redesenho de 28/11; substitui a CT-53. Painel de 480 px (x 80 a 560, y 70 a 286), texto de x 96 a 544 (limite 56 caracteres) |
| PA-02a | pausa, linha da fase (Fase 1, `UI_AMBER`, y 104) | `Fase 1 · bactérias no sangue` | 28 | redesenho de 28/11 |
| PA-02b | pausa, linha da fase (Fase 2) | `Fase 2 · vírus nas vias aéreas` | 30 | redesenho de 28/11 |
| PA-02c | pausa, linha da fase (Fase 3) | `Fase 3 · fungos no pulmão` | 25 | redesenho de 28/11 |
| PA-03a | pausa, objetivo da Fase 1, linha 1 (y 124) | `Elimine as bactérias do sangue do Léo e vá ao destino.` | 54 | redesenho de 28/11 |
| PA-03b | pausa, objetivo da Fase 1, linha 2 (y 140) | `Gram+: Ruptura. Gram-: Dissolução. Resistente: Reserva.` | 55 | redesenho de 28/11; mapeamento de arma do glossário (regra do jogo) |
| PA-03c | pausa, objetivo da Fase 1, linha 3 (y 156) | `Leia antes de disparar. Ler todos vale +15 pontos.` | 50 | redesenho de 28/11; o 15 é o `IDENTIFICATION_BONUS` |
| PA-03d | pausa, objetivo da Fase 2, linha 1 | `Elimine os vírus nas vias aéreas do Léo e vá ao destino.` | 56 | redesenho de 28/11 |
| PA-03e | pausa, objetivo da Fase 2, linha 2 | `Vírus livre: Neutralização. Célula infectada: Ataque T.` | 55 | redesenho de 28/11 |
| PA-03f | pausa, objetivo da Fase 2, linha 3 | `Leia a célula antes: o Ataque T custa vitais do Léo.` | 52 | redesenho de 28/11; `LYSE_VITALS` 5 |
| PA-03g | pausa, objetivo da Fase 3, linha 1 | `Elimine os fungos no pulmão do Léo e vá ao destino.` | 51 | redesenho de 28/11 |
| PA-03h | pausa, objetivo da Fase 3, linha 2 | `Antibiótico não tem alvo no fungo: use a Perfuração.` | 52 | redesenho de 28/11 |
| PA-03i | pausa, objetivo da Fase 3, linha 3 | `No bolor, mire na base da colônia, não na hifa.` | 47 | redesenho de 28/11; RT-18 |
| PA-04a | pausa, lembrete de teclas, linha 1 (`UI_DIM`, y 180) | `[WASD] mover · [clique] dispara · [botão direito] lê` | 46 | redesenho de 28/11 |
| PA-04b | pausa, lembrete de teclas, linha 2 (y 196) | `[X] defesa · [1] [2] [3] ou [roda]: arma` | 30 | redesenho de 28/11 |
| PA-05 | pausa, segundo botão (x 328, y 222, ao lado do CONTINUAR em x 152) | `COMO JOGAR` | 10 | redesenho de 28/11; o mesmo literal da MN-01; de volta do como jogar, o foco cai nele |
| PA-06 | pausa, rodapé (y 264) | `[esc] continua` | 12 | redesenho de 28/11; substitui a CT-54 |
| CT-55 | menu, botão | `INICIAR` | 7 | varredura de 05/11 |
| CT-56 | pausa/resultados, botão | `CONTINUAR` | 9 | varredura de 05/11 |
| CT-57 | vitória/game over, botão | `NOVA PARTIDA` | 12 | varredura de 05/11 |
| CT-58 | — | `o paciente recebeu alta: missão cumprida` | — | #244; saiu em 28/11: substituída pela VI-02 |
| VI-02 | vitória, subtítulo centrado (y 38), sobre o quarto de dia (`hospital_vitoria.png` com véu de alfa 150) | `O Léo recebeu alta e vai para casa. Missão cumprida!` | 52 | redesenho de 28/11 (VI-01); substitui a CT-58 |
| VI-03 | vitória, linha do Nanobô em ciano, centrada (y 54) | `Anotei tudo o que aprendi. Obrigado por me guiar!` | 49 | redesenho de 28/11 (VI-01), voz do Nanobô por decisão do dono; os painéis desceram 16 px (y 58 → 74) e o botão de 268 para 276 |
| VI-04 | vitória, painel do nome, sob a caixa (y 240), só com a entrada ativa | `até 10 letras · [backspace] apaga` | 31 | redesenho de 28/11 (VI-01); o nome aceita qualquer caractere do Latin-1 |
| CT-59 | vitória, linha de estatística | `pontuação final` | 15 | varredura de 05/11 |
| CT-60 | vitória, rodapé | `[enter] também começa de novo` | 27 | varredura de 05/11 |
| CT-61 | game over, título | `GAME OVER` | 9 | varredura de 05/11 |
| CT-62 | — | `os sinais vitais caíram demais: a equipe retirou o Nanobô` / `o Nanobô foi danificado: a equipe o retirou do corpo` | — | #218; #234; #244; saiu em 28/11: substituída pelas GO-02a e GO-02b |
| GO-02a | game over, causa (y 52), vitais em 0, sobre o quarto em luz baixa (`hospital_gameover.png` com véu de alfa 100) | `Os vitais do Léo caíram demais: a equipe retirou o Nanobô.` | 58 | redesenho de 28/11 (GO-01); substitui a CT-62a |
| GO-02b | game over, causa (y 52), integridade do Nanobô em 0 | `O Nanobô foi danificado: a equipe o retirou do corpo.` | 53 | redesenho de 28/11 (GO-01); substitui a CT-62b |
| GO-03 | game over, linha fixa (y 68) | `O Léo segue no tratamento convencional do hospital.` | 51 | redesenho de 28/11 (GO-01); não há morte |
| GO-04 | game over, linha do Nanobô em ciano (y 84) | `Anotei os meus erros. Vamos tentar de novo?` | 43 | redesenho de 28/11 (GO-01), voz do Nanobô por decisão do dono; o painel desceu 32 px (y 72 → 104), o botão de 204 para 216 e o rodapé de 248 para 260 |
| CT-63 | game over, rodapé | `[enter] também reinicia` | 21 | varredura de 05/11 |
| CT-64 | menu, botão | `RECORDES` | 8 | #197 |
| MN-01 | menu, botão do meio (entre INICIAR e RECORDES) / pausa, segundo botão | `COMO JOGAR` | 10 | redesenho de 28/11 (linhas do menu: CT-48, CT-55, MN-01, CT-64, MN-04, CT-47, CT-13); abre a cena como jogar. Substitui o antigo MN-03 |
| CT-65 | painel do Top 10, título | `Top 10` | 6 | #197 |
| CT-66 | painel do Top 10, vazio | `nenhum recorde ainda` / `vença uma partida` | 20 / 17 | #197; a segunda linha saiu em 28/11: substituída pela MN-07 |
| MN-07 | painel do Top 10 aberto pelo RECORDES no menu, vazio (segunda linha; na Vitória o painel vazio mostra só a primeira linha) | `dê alta ao Léo para entrar aqui` | 31 | redesenho de 28/11 (MN-06); substitui a CT-66b; liga o recorde à alta do Léo |
| CT-67 | painel do Top 10 no menu, rodapé | `[enter] ou [esc] fecha` | 18 | #197 |
| CT-68 | vitória, entrada de nome | `novo recorde no Top 10!` | 23 | #81 |
| CT-69 | vitória, entrada de nome | `digite seu nome e tecle [enter]` | 29 | #81 |
| CT-70 | vitória, depois do nome | `posição %d no Top 10` | 20 | #81 |
| CT-71 | vitória, fora do Top 10 | `a pontuação não entrou no Top 10` | 32 | #79 |
| CT-72 | vitória, gravação falhou | `não foi possível gravar o recorde` | 33 | #80 |
| CT-73 | nome padrão no ranking | `Jogador` (nome vazio) / `Recorde` (recorde antigo importado) | 7 / 7 | #78, #81 |

**Conferência no QA (#212, desde 15/11).** Na raiz do repositório, rode `python -I ../Nanobo-testes/conferir_textos.py .`. O script cruza todo literal de string de `src/main.c` e `src/gameplay/gameplay.c` com este registro, nos dois sentidos: lista o texto desenhado sem linha aqui e a linha daqui cujo texto saiu do código.
- **Resultado esperado:** `0 literais sem linha no registro, 0 linhas sem texto no codigo`, com código de saída 0.
- **Fica de fora:** o que não é desenhado (mensagens de `_Static_assert`, `printf` de erro, caminhos e o formato do arquivo do ranking) e as linhas de QA sob `#ifdef _DEBUG`. O título `NANOBÔ` é o caso especial da §2.1, um bitmap montado por `make_text_bitmap`.
- **Histórico:** a varredura de 05/11 foi manual e trouxe a CT-48 a CT-63. A de 15/11 tirou a CT-38 a CT-41, que tinham saído do código, e registrou os nomes das bactérias (RT-05c).

### Como jogar (cena `HOWTO`, redesenho de 28/11)

Cena própria, aberta pelo botão COMO JOGAR (MN-01) do menu e da pausa; volta para a cena de origem. Título CT-01 em y 16; painel em y 104 com o título da página em `UI_AMBER` em x 120, o contador à direita (x 616) e até 6 linhas em `UI_TEXT` a partir de x 120, a cada 16 px (limite de 60 caracteres, 480 px). `{termo}` sai em âmbar e `[tecla]` em tampa de tecla (`draw_marked_text`, §2.1). Rodapé centrado em y 336.

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| HW-00a | painel, contador à direita | `%d/%d` (`1/4`) | 3 | redesenho de 28/11 |
| HW-00b | rodapé, páginas 1 a 3 | `[enter], [espaço] ou [clique] avança · [setas] trocam · [esc] volta` | 57 | redesenho de 28/11 |
| HW-00c | rodapé, página 4 | `[enter], [espaço] ou [clique] volta · [setas] trocam · [esc] volta` | 56 | redesenho de 28/11 |
| HW-01 | página 1, título | `Mover e mirar` | 13 | redesenho de 28/11 |
| HW-01a | página 1, linha 1 | `[Setas] ou [WASD]: mover o Nanobô.` | 30 | redesenho de 28/11 |
| HW-01b | página 1, linha 2 | `[Mouse]: mirar. [Botão esquerdo]: disparar a arma.` | 46 | redesenho de 28/11 |
| HW-01c | página 1, linha 3 | `[Botão direito]: escanear o alvo sob o cursor.` | 44 | redesenho de 28/11 |
| HW-01d | página 1, linha 4 | `[X]: defesa por 2 s. Contato não fere; toxinas, vírus e` | 53 | redesenho de 28/11 |
| HW-01e | página 1, linha 5 | `esporos em voo se desfazem. A hifa passa. Recarga: 3 s.` | 55 | redesenho de 28/11 |
| HW-01f | página 1, linha 6 | `[Esc]: pausa. [F11]: tela cheia.` | 28 | redesenho de 28/11 |
| HW-02 | página 2, título | `Ler com o scanner` | 17 | redesenho de 28/11 |
| HW-02a | página 2, linha 1 | `[Botão direito] num agente: eu leio uma camada por vez.` | 53 | redesenho de 28/11 |
| HW-02b | página 2, linha 2 | `Camada 1: a {forma}. Coco, bacilo, levedura, bolor...` | 51 | redesenho de 28/11; `forma` em âmbar |
| HW-02c | página 2, linha 3 | `Camada 2: a {coloração de Gram} e o nome da espécie.` | 50 | redesenho de 28/11; `coloração de Gram` em âmbar |
| HW-02d | página 2, linha 4 | `A forma não basta: existe coco Gram+ e coco Gram-.` | 50 | redesenho de 28/11 |
| HW-02e | página 2, linha 5 | `Com as duas camadas lidas, o agente está identificado.` | 54 | redesenho de 28/11 |
| HW-02f | página 2, linha 6 | `Identificar todos os agentes da fase vale +15 pontos.` | 53 | redesenho de 28/11; o 15 é o `IDENTIFICATION_BONUS` |
| HW-03 | página 3, título | `Armas` | 5 | redesenho de 28/11 |
| HW-03a | página 3, linha 1 | `[1], [2], [3] ou [roda do mouse]: trocar de arma.` | 41 | redesenho de 28/11 |
| HW-03b | página 3, linha 2 | `Fase 1: [1] Ruptura · [2] Dissolução · [3] Reserva (2 cargas)` | 55 | redesenho de 28/11 |
| HW-03c | página 3, linha 3 | `Cada bactéria pede 2 doses, a segunda em até 6 segundos.` | 56 | redesenho de 28/11 |
| HW-03d | página 3, linha 4 | `Fase 2: [1] Neutralização (vírus livre) · [2] Ataque T` | 50 | redesenho de 28/11 |
| HW-03e | página 3, linha 5 | `Fase 3: [1] Perfuração · [2] Ruptura (não age em fungo)` | 51 | redesenho de 28/11 |
| HW-03f | página 3, linha 6 | `Regra do jogo: as armas se inspiram em remédios reais.` | 54 | redesenho de 28/11 |
| HW-04 | página 4, título | `As duas barras` | 14 | redesenho de 28/11 |
| HW-04a | página 4, linha 1 | `Cabeça de robô: a minha {Integridade}. Ela cai quando` | 51 | redesenho de 28/11; `Integridade` em âmbar |
| HW-04b | página 4, linha 2 | `um invasor, uma toxina, a hifa ou a nuvem me acertam.` | 53 | redesenho de 28/11 |
| HW-04c | página 4, linha 3 | `Coração: os {Vitais} do Léo. Caem quando uma célula dele` | 54 | redesenho de 28/11; `Vitais` em âmbar |
| HW-04d | página 4, linha 4 | `é destruída ou atingida por engano, e no Ataque T.` | 50 | redesenho de 28/11 |
| HW-04e | página 4, linha 5 | `Se uma das barras zera, a equipe me retira do corpo.` | 52 | redesenho de 28/11 |
| HW-04f | página 4, linha 6 | `Nova fase: Integridade cheia e Vitais de no mínimo 60.` | 54 | redesenho de 28/11 |

### Textos do scanner (`src/main.c` + `src/gameplay/gameplay.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| RT-01 | scanner, camada 1, coco | `forma: coco (esférica)` (sem amostra de cor) | 22 | #214, design 2.0 |
| RT-02 | scanner, camada 1, bacilo | `forma: bacilo (bastão)` (sem amostra de cor) | 22 | #214, design 2.0 |
| SC-08 | scanner, camada 1, célula | `célula do Léo · eucarionte` (sem amostra de cor) | 26 | #214; redesenho de 28/11; substitui a RT-03 |
| RT-04 | scanner, camada 2, Gram+ (amostra roxa) | `Gram+ · parede grossa, sem membrana externa · %s` (`... · Streptococcus pyogenes`) | 46 + nome (até 68) | #214, design 2.0 |
| RT-05 | scanner, camada 2, Gram- (amostra rosa) | `Gram- · parede fina sob membrana externa · %s` (`... · Pseudomonas aeruginosa`) | 43 + nome (até 65) | #214, design 2.0 |
| RT-05c | scanner, camada 2, o `%s` da RT-04 e da RT-05 (`phases[]` em `src/entities/entities.c`) | Gram+: `Staphylococcus aureus` / `Streptococcus pyogenes` / `Clostridium tetani`; Gram-: `Moraxella catarrhalis` / `Escherichia coli` / `Pseudomonas aeruginosa` | 21 / 22 / 18; 21 / 16 / 22 | #214; registrado na #212 |
| RT-05b | scanner, camada 2, célula | `núcleo guarda o DNA · mitocôndrias · não atacar` | 47 | #214, design 2.0 |
| RT-06 | scanner, linha de estado | ` · alvo sob o cursor` (ciano) / ` · última leitura` (apagado) | 20 / 17 | #199, build de 04/11 |
| RT-07 | scanner, camada 2, revisão bactéria (Gram+ e Gram-) | `procarionte · sem núcleo: DNA solto no nucleoide` | 48 | #214, OA1 |
| RT-08b | scanner, camada 2, revisão célula | `eucarionte: tem núcleo, como as células do corpo` | 48 | #214, OA1 |
| RT-09 | scanner, camada 1, vírus livre | `vírus livre · deriva no muco` | 28 | #87 |
| SC-09 | scanner, camada 2, vírus livre | `acelular: sem parede, ribossomos nem metabolismo próprios` | 57 | #87 (OA6); redesenho de 28/11 (decisão 6 de 25/11); substitui a RT-10 |
| SC-10 | scanner, camada 2, revisão, vírus livre | `só se multiplica dentro de uma célula do Léo` | 44 | #87; redesenho de 28/11; substitui a RT-11 |
| RT-12 | scanner, camada 1, célula infectada | `célula infectada · vírus lisogênico · %d s` (`... · 10 s`) / `célula infectada · eclipse: vai romper` | 42 / 38 | #87 |
| RT-13 | scanner, camada 2, célula infectada | `lisogênico · %d s · o vírus espera dentro da célula` / `eclipse · o vírus fez cópias dentro da célula` | 51 / 45 | #87 (OA7) |
| RT-14 | scanner, camada 2, revisão, célula infectada | `no lítico a célula se rompe e solta novos vírus` | 47 | #87 |
| RT-15 | scanner, camada 1, levedura (sem amostra de cor) | `forma: levedura (arredondada, brota)` | 36 | #228 |
| RT-16 | scanner, camada 2, levedura e esporo pousado (sem amostra de cor) | `eucarionte · parede de quitina · membrana com ergosterol` | 56 | #228; esporo na #229 |
| RT-17 | scanner, camada 1, colônia de bolor (sem amostra de cor) | `forma: bolor (filamentos)` | 25 | #229 |
| RT-18 | scanner, camada 2, colônia de bolor (sem amostra de cor) | `eucarionte · parede de quitina · corte a base da colônia` | 56 | #229 |
| RT-19 | scanner, camada 1, esporo pousado (sem amostra de cor) | `esporo de bolor · vai germinar` | 30 | #229 |

**Limite:** a fonte embutida é de **8 px por caractere** (medido com `al_get_text_width`, não estimado) e a área útil da ficha do scanner é de 588 px, a partir de `text_x = 36`. Desde 15/11 (#243), o HUD tem duas linhas, com estas folgas no pior caso:
- **linha 0:** sobram 20 px entre o tempo e a arma (`Neutralização · vírus livre`, `recarga`, pontos de 3 dígitos);
- **linha 1:** sobram 32 px entre o objetivo da Fase 1 e os contadores com 2 dígitos.
 A linha mais longa desenhada na fase é a RT-04 com *Streptococcus pyogenes*: **68 caracteres** (544 px) mais a amostra de 12 px, 556 px no total (medido em 13/11, #214). A mais longa do briefing tem 54 (432 px, dentro dos 548 px do painel) e a dos resultados, 36 (288 px). Nenhuma passa da área do painel em 640×360. A RT-08 (revisão Gram- separada) saiu em 13/11: Gram+ e Gram- compartilham a RT-07, porque a revisão agora fala do procarionte, não do grupo.

## 2.1 Regra de tipografia (medida em 03/11/2026)

**Maiúscula acentuada não desenha.** A fonte embutida devolve glifo errado para `Í`, `Ç`, `Ã`, `Ó` maiúsculos — `FASE 1 CONCLUÍDA` saía `CONCLU f DA`, `MOVIMENTAÇÃO` saía `MOVIMENTAÇ%Ø` — enquanto as **minúsculas acentuadas saem com o glifo certo** (`ç`, `ã`, `ó`, `ê`, `ô`, `í`).

**`ê` e `ô` ficam apertados na fonte pequena (25/11).** Os textos com `ê` e `ô` (`Lisogênico`, `você`, `colônia`, `Nanobô`, `bônus`) estão **corretos**, decisão do dono de 25/11. O problema é o tamanho da fonte embutida: no glifo de 8 px, o circunflexo encosta na letra, e numa captura reduzida o `ê` lembra `@` e o `ô` lembra `8`. É uma questão de legibilidade da fonte, não do texto nem do código, e fica para o redesenho da interface que vem a seguir.

Regras que valem para todo texto novo:

- Títulos e rótulos em **caixa de frase** (`Fase 1 concluída`, `Objetivo da missão`, `Identificação concluída`).
- Caixa alta só em texto **sem acento** (`INICIAR`, `CONTINUAR`, `COMO JOGAR` quando não houver acento).
- Nenhum texto desenhado pode ter maiúscula acentuada. `NANOBÔ` é exceção registrada: é o nome do jogo, desenhado em bitmap ampliado 3×, onde o glifo é legível mas não correto.
- **Tecla em caixinha (27/11, decisão do dono):** o nome de tecla ou botão vem entre colchetes (`[enter]`, `[botão direito]`) e é desenhado como tampa de tecla: nome em `UI_KEY` numa caixa de 12 px de altura com borda `UI_KEY` e fundo `UI_INK`, 2 px de folga de cada lado. Os colchetes não aparecem nem contam nos caracteres (como as chaves do `{termo}`), mas cada tecla soma **+4 px** à largura da linha (`marked_text_width`). Só nomes de tecla/botão levam colchetes, nunca verbos.
- Antes de commitar texto novo, rodar `python -c "print([c for c in 'TEXTO' if ord(c) > 0xFF])"` (codepoint fora de Latin-1 vira `^` na tela).

## 2.2 Relatório da fase (resultados, #210 e #223)

Desde 26/11 o painel se chama `O que anotei nesta fase` (23 caracteres) e as linhas falam na voz do Nanobô, em primeira pessoa (aprovadas pelo dono em 26/11). Desde 14/11 (#223), o painel fala em linguagem de jogador, sem os códigos internos "OA1:" a "OA4:". São cinco linhas de 14 px, uma por conteúdo, com os números da partida (`MatchProgress`), geradas por `results_report_line` em `src/main.c`. A maior linha tem 42 caracteres (336 px, com contadores de dois dígitos) e cabe nos 344 px do painel (43 caracteres); a da Fase 2, `Ataque T: %d células contidas a tempo`, tem 38 com dois dígitos (304 px). As mensagens OA-01 a OA-04 da #210 saíram do código.

| ID | Linha | Texto | Caracteres | Contador |
|---|---|---|---:|---|
| RR-01 | 1, fase com bactéria | `li %d Gram+ e %d Gram- até a coloração` | 37 | `scans_gram_positive`, `scans_gram_negative` |
| RR-02 | 2, fase com bactéria | `tratei %d até o fim; %d resistiram` | 33 | `treatments_completed`, `resistances` |
| RR-03 | 3, fase com bactéria | `não errei nenhum antibiótico` / `errei %d antibióticos (%d pela forma)` | 28 / 37 | `wrong_decisions`, `shape_guesses` |
| RR-04 | 4, fase com bactéria | `guardei a Reserva: nenhuma carga gasta` / `Reserva: %d na resistente, %d na comum` | 38 / 36 | `reserve_on_resistant`, `reserve_on_common` |
| RR-05 | 5, fase com bactéria | `acertei %d células do Léo por engano` | 35 | `cell_attacks` |
| RR-06 | 1, fase de vírus | `prendi %d vírus livres com anticorpos` | 36 | `viruses_neutralized` (#88) |
| RR-07 | 2, fase de vírus | `Ataque T: contive %d células a tempo` | 35 | `cells_lysed` (#88) |
| RR-08 | 3, fase de vírus | `%d células romperam no ciclo lítico` | 35 | `cells_burst` (#86) |
| RR-09 | 4, fase de vírus | `eliminei %d de %d ameaças` | 24 | ameaças da fase, com os vírus da rajada |
| RR-10 | 5, fase de vírus | `anotei: vírus sem parede nem ribossomos` | 39 | — (OA6) |
| RR-11 | 1, fase de fungos | `eliminei %d leveduras` | 21 | `yeasts_eliminated` (#232) |
| RR-12 | 2, fase de fungos | `eliminei %d colônias (com as germinadas)` | 40 | `molds_eliminated`; a colônia que germinou de esporo conta (#232) |
| RR-13 | 3, fase de fungos | `gastei %d tiros de antibiótico em fungo` | 39 | `antibiotic_on_fungus`: tiro em levedura, colônia, hifa ou esporo (#232) |
| RR-14 | 4, fase de fungos | `esporos pela defesa fraca do Léo: %d` | 36 | `spore_load_current()`, a carga da abertura (#231, #232) |
| RR-15 | 5, fase de fungos | `anotei: fungo tem quitina e ergosterol` | 38 | — (frase fixa) |

**Leitura dos contadores:** um tratamento completo é uma bactéria eliminada com as duas doses do antibiótico certo. A Reserva não conta como tratamento, e o relatório a separa entre a gasta numa resistente e a gasta numa bactéria comum. `shape_guesses` conta o antibiótico errado dado depois de ler só a forma (FB-05).

**Regra de tipografia:** seguir a seção 2.1. Utilizar hífen ASCII em `Gram-` e evitar maiúsculas acentuadas.

**Fonte:** Issue #210 — Resumo por objetivo na tela de Resultados (RF-07).

## 3. Nanobô 2.0 — rascunhos (13/11/2026)

> **Rascunhos, ainda não estão no código.** Regras em `ensino-e-aprendizagem.md` (§"Nanobô 2.0"). Os cards têm 3 linhas de até 70 caracteres (contagem abaixo, feita com Python). Os textos de HUD e ficha seguem a regra da seção 2.1: a fonte embutida foi mantida em 15/11. Os IDs `CC-` são cards de conceito, `FB2-` são feedback do 2.0 e `FE-` são armas.

### 3.1 Cards de conceito

> **Caixa de diálogo desde 25/11 (diário):** cada card é uma entrada do Diário do Nanobô (ou um interlúdio), desenhada na caixa da faixa de baixo (x = 16 a 624, y = 264 a 352) com retrato 48×48, cabeçalho e até 3 linhas por página, de 1 a 4 páginas. As linhas começam em x = 80 e vão até x = 616: **67 caracteres** (536 px), com a mesma largura nas entradas sem retrato. O termo-chave de cada página vem entre chaves (`{termo}`), sai em âmbar e **não conta** nos caracteres; o `conferir_textos.py` tira as chaves antes de comparar. O texto aparece a 40 caracteres/s; o rodapé mostra `n/N`, um triângulo piscando com a página completa e `fechar` na última. Os títulos de antes viraram parte do cabeçalho (DG-01 a DG-09); textos provisórios até a aprovação do dono.
>
> **No código desde 14/11 (#222):** CC-01 abre na primeira leitura completa de bactéria e CC-02 na primeira bactéria resistente. Cada card aparece uma vez por partida, sobre a fase parada, com as linhas desenhadas a partir de x = 56 num painel de 576 px; a maior (CC-02a, 66 caracteres) ocupa 528 px. CC-05 e CC-06 estão no código desde 25/11 (#232): CC-05 abre ao entrar na Fase 3 e CC-06 na 1ª leitura completa de fungo ou no 1º antibiótico num fungo, o que vier primeiro.
>
> **Fase 2 no código desde 14/11 (#87, #88):** CC-03 abre na primeira leitura de um vírus livre, no primeiro vírus preso pelos anticorpos ou no primeiro antibiótico num vírus. CC-04 abre na primeira leitura de uma célula infectada ou na primeira célula que rompe, para quem não leu. Títulos: `Vírus não é célula` (18) e `Lisogênico e lítico` (19).
>
> **Numeração (26/11):** o número da anotação segue a **ordem de descoberta na partida** (a 1ª que abre é a 1, a seguinte a 2, e assim por diante; recomeça em 1 a cada partida; o interlúdio não conta e a abertura é a Anotação 0). Os números nas linhas CC abaixo são só a ordem usual de uma partida; no jogo o número vem de `card_note_number()` e o cabeçalho é o DG-13. CC-01 ganhou a página 2 (jogo simplifica: todas as bactérias juntas no sangue; armas inspiradas em antibióticos reais) e CC-04 a página 3 (lisogênico e lítico foram descritos em bacteriófagos; aqui são comparação), em 26/11.
>
> **Textos aprovados (26/11):** as linhas abaixo são o inventário revisado pelo dono, linha a linha, com as fontes de cada conceito em `conteudo-biologico.md`. Dois ajustes da revisão: a Anotação 0 troca o entusiasmo pela missão (AB-06b) e o CC-07a diz `me atacou` no lugar de `deu um bote`.

| ID | Card / quando aparece | Linha | Car. |
|---|---|---|---:|
| CC-01a | Nanobô · Anotação 1 · Coloração de Gram · 1ª leitura completa de bactéria · página 1 de 4 | Li uma bactéria até o fim! A forma não basta: | 45 |
| CC-01b |  | existe coco Gram+ e coco Gram-. Quem mostra o grupo | 51 |
| CC-01c |  | é a {coloração de Gram}, um corante de laboratório. | 49 |
| CC-01d | página 2 de 4 | O corante roxo fica preso ou sai conforme a parede. | 51 |
| CC-01e |  | Gram+: parede grossa de {peptidoglicano}, o roxo fica. | 52 |
| CC-01f |  | Gram-: parede fina e membrana externa; perde o roxo, fica rosa. | 63 |
| CC-01g | página 3 de 4 | Anotado: no jogo, todas aparecem juntas no sangue para | 54 |
| CC-01h |  | {simplificar}. A Moraxella vive nas vias respiratórias, | 53 |
| CC-01i |  | e o Clostridium tetani entra por ferimentos. | 44 |
| CC-01j | página 4 de 4 | Minhas armas são {regra do jogo}, inspiradas em antibióticos | 58 |
| CC-01k |  | reais, como a penicilina. Na vida real há exceções: | 51 |
| CC-01l |  | muitas cepas de S. aureus resistem à penicilina. | 48 |
| CC-02a | Nanobô · Anotação 2 · Resistência e Reserva · 1ª bactéria resistente · página 1 de 4 | Errei... anotado! Dei uma dose e demorei para dar a outra. | 58 |
| CC-02b |  | Esta bactéria {resistiu}: o mesmo antibiótico não age | 51 |
| CC-02c |  | mais nela nem nas filhas dela. | 30 |
| CC-02d | página 2 de 4 | Regra do jogo: duas doses, a segunda em até 6 segundos. | 55 |
| CC-02e |  | Na vida real, um tratamento mal feito faz {seleção}: | 50 |
| CC-02f |  | morrem as sensíveis e sobram as que já resistiam. | 49 |
| CC-02g | página 3 de 4 | As sobreviventes se multiplicam e passam a resistência | 54 |
| CC-02h |  | para as filhas. Com antibiótico de verdade, a regra é: | 54 |
| CC-02i |  | {siga a prescrição até o fim}. | 28 |
| CC-02j | página 4 de 4 | A {Reserva} age na resistente, mas tem 2 cargas por fase. | 55 |
| CC-02k |  | Até ela pode deixar de funcionar se for usada à toa. | 52 |
| CC-02l |  | Vou guardar para quando precisar! | 33 |
| CC-03a | Nanobô · Anotação 6 · Vírus não é célula · 1ª leitura de vírus ou 1º antibiótico em vírus · página 1 de 4 | Um {vírus}! Ele não é célula: não tem parede, | 43 |
| CC-03b |  | nem metabolismo, nem ribossomos próprios. | 41 |
| CC-03c | página 2 de 4 | Ele só se multiplica {dentro de uma célula}, | 42 |
| CC-03d |  | usando os ribossomos dela. | 26 |
| CC-03e | página 3 de 4 | {Antibiótico} não age nele: não trata gripe nem resfriado. | 56 |
| CC-03f |  | Se gastei um tiro de antibiótico aqui, errei... anotado! | 56 |
| CC-03g | página 4 de 4 | A Neutralização leva {anticorpos}: eles se prendem ao vírus | 57 |
| CC-03h |  | livre e o impedem de entrar nas células. | 40 |
| CC-03i |  | No corpo, quem faz anticorpos são os linfócitos B. | 50 |
| CC-04a | Nanobô · Anotação 7 · Lisogênico e lítico · 1ª leitura de célula infectada · página 1 de 4 | Esta célula do Léo tem um vírus escondido dentro! | 49 |
| CC-04b |  | {Lisogênico}: o vírus fica na célula sem destruí-la ainda. | 56 |
| CC-04c | página 2 de 4 | {Lítico}: o vírus faz cópias, a célula se rompe | 45 |
| CC-04d |  | e libera vírus novos, que infectam outras células. | 50 |
| CC-04e | página 3 de 4 | O {Ataque T} destrói a célula infectada antes que ela | 51 |
| CC-04f |  | se rompa, como faz o linfócito T no corpo. | 42 |
| CC-04g |  | No jogo, ele custa um pouco dos vitais do Léo. | 46 |
| CC-04h | página 4 de 4 | Anotado: lisogênico e lítico foram descritos em vírus de | 56 |
| CC-04i |  | bactérias, os {bacteriófagos}. Aqui servem de comparação. | 55 |
| CC-04j |  | Em nós, o mais parecido é o vírus da catapora, nos nervos. | 58 |
| CC-05a | Nanobô · Anotação 8 · Organismo enfraquecido · início da Fase 3, depois do IL-03 · página 1 de 3 | Estes {esporos} são de Aspergillus, um bolor comum no ar. | 55 |
| CC-05b |  | A maioria das pessoas respira esporos e não adoece. | 51 |
| CC-05c | página 2 de 3 | Fungos {oportunistas} se instalam com mais facilidade | 51 |
| CC-05d |  | em quem está com a defesa enfraquecida. | 39 |
| CC-05e |  | O risco vem da defesa fraca, não de respirar mais esporos. | 58 |
| CC-05f | página 3 de 3 | O Léo está com a {defesa} fraca. Regra do jogo: aqui, | 51 |
| CC-05g |  | defesa mais fraca aparece como mais esporos no chão. | 52 |
| CC-06a | Nanobô · Anotação 9 · Fungo · 1ª leitura de fungo · página 1 de 4 | Um fungo! Ele tem núcleo, como as células do Léo: | 49 |
| CC-06b |  | é um {eucarionte}, bem diferente de uma bactéria. | 47 |
| CC-06c | página 2 de 4 | A parede dele é de {quitina}, não de peptidoglicano. | 50 |
| CC-06d |  | Antibiótico de bactéria não tem alvo nele. | 42 |
| CC-06e |  | Se gastei antibiótico aqui, errei... anotado! | 45 |
| CC-06f | página 3 de 4 | A membrana dele tem {ergosterol}; a nossa tem colesterol. | 55 |
| CC-06g |  | Essa diferença é o alvo de muitos antifúngicos. | 47 |
| CC-06h | página 4 de 4 | A {Perfuração} se inspira na anfotericina B: ela se prende | 56 |
| CC-06i |  | ao ergosterol e abre poros na membrana do fungo. | 48 |
| CC-06j |  | Esse remédio de verdade também pode fazer mal ao paciente. | 58 |
| CC-07a | Nanobô · Anotação 3 · Flagelo · 1ª investida de um bacilo (14/11; no código desde a #235) · página 1 de 3 | Ei! Esse bacilo veio atrás de mim e me atacou! | 46 |
| CC-07b |  | Ele tem {flagelo}, um chicote que gira e o faz nadar. | 51 |
| CC-07c | página 2 de 3 | O coco não nada sozinho: ele {gruda} no tecido | 44 |
| CC-07d |  | e fica ali, soltando toxinas que ferem as células. | 50 |
| CC-07e | página 3 de 3 | Anotado: na natureza há {exceções}. Aqui, todo bacilo | 51 |
| CC-07f |  | do jogo nada e todo coco gruda. | 31 |
| CC-08a | Nanobô · Anotação 4 · Biofilme · 1º contato do Nanobô com um biofilme (14/11; no código desde a #237) · página 1 de 3 | Fiquei preso numa gosma! Bactérias grudadas fazem | 49 |
| CC-08b |  | uma camada pegajosa: o {biofilme}. | 32 |
| CC-08c | página 2 de 3 | Dentro dele, o antibiótico {age menos}: entra devagar, | 52 |
| CC-08d |  | e as bactérias de dentro funcionam mais devagar. | 48 |
| CC-08e | página 3 de 3 | Regra do jogo: no biofilme a bactéria pede uma {dose} a mais, | 59 |
| CC-08f |  | e eu ando devagar. | 18 |
| CC-09a | Nanobô · Anotação 5 · Endotoxina · 1º contato do Nanobô com uma nuvem (14/11; no código desde a #237) · página 1 de 3 | Ai, esta nuvem me fere! Ela veio de uma Gram- eliminada. | 56 |
| CC-09b |  | A membrana externa da Gram- guarda uma {toxina}. | 46 |
| CC-09c | página 2 de 3 | Quando a Gram- morre ou se divide, essa toxina se solta | 55 |
| CC-09d |  | e causa {inflamação} e febre no corpo. | 36 |
| CC-09e | página 3 de 3 | Regra do jogo: a nuvem me fere enquanto dura. | 45 |
| CC-09f |  | Anotado: {sair da nuvem} bem rápido! | 34 |
| IL-02a | Enquanto isso, no hospital · início da Fase 2, antes de qualquer anotação · sem retrato · página 1 de 2 | A infecção no sangue de Léo cedeu. Mas, na enfermaria, | 54 |
| IL-02b |  | ele pegou um {vírus respiratório}. | 32 |
| IL-02c | página 2 de 2 | O Nanobô segue para as {vias aéreas} dele, onde o vírus | 53 |
| IL-02d |  | infecta as células que forram os brônquios. | 43 |
| IL-03a | Enquanto isso, no hospital · início da Fase 3, antes do CC-05 · sem retrato · página 1 de 3 | O vírus foi contido, mas Léo segue com as {defesas} fracas. | 57 |
| IL-03b | página 2 de 3 | {Esporos de bolor}, que estão no ar, | 34 |
| IL-03c |  | chegam ao pulmão dele. | 22 |
| IL-03d | página 3 de 3 | O Nanobô desce até o {pulmão} para agir antes que | 47 |
| IL-03e |  | o fungo se instale. | 19 |

#### Caixa de diálogo: cabeçalho e rodapé (25/11)

| ID | Onde | Texto | Car. | Nota |
|---|---|---|---:|---|
| DG-01 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Coloração de Gram` | 17 | |
| DG-02 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Resistência e Reserva` | 21 | |
| DG-03 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Flagelo` | 7 | |
| DG-04 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Biofilme` | 8 | |
| DG-05 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Endotoxina` | 10 | |
| DG-06 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Vírus não é célula` | 18 | |
| DG-07 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Lisogênico e lítico` | 19 | |
| DG-08 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Organismo enfraquecido` | 22 | |
| DG-09 | tema da anotação, depois de `Nanobô · Anotação N · ` (DG-13) | `Fungo` | 5 | |
| DG-10 | cabeçalho dos interlúdios IL-02 e IL-03 | `Enquanto isso, no hospital` | 26 | sem retrato |
| DG-11 | rodapé, à direita (x = 616) | `%d/%d` (`1/2`) | 3 | página atual e total |
| DG-12 | rodapé, última página completa, à esquerda do contador | `fechar` | 6 | em `UI_AMBER`, no lugar do triângulo |
| DG-13 | cabeçalho da anotação, x = 80, em `UI_DIM`; `%d` é a ordem em que a anotação abre na partida (1, 2, ...) e `%s` o tema DG-01 a DG-09 | `Nanobô · Anotação %d · %s` | 25 | o interlúdio não leva número (DG-10); a abertura é a Anotação 0 |

### 3.2 Armas (menu "como jogar" e HUD)

| ID | Texto | Fase |
|---|---|---|
| FE-01 | Ruptura · antibiótico de parede: impede a bactéria de montar a parede | 1 |
| FE-02 | Dissolução · antibiótico de membrana: desfaz a membrana externa das Gram- | 1 |
| FE-03 | Reserva · antibiótico de reserva: age na resistente, poucas cargas | 1 |
| FE-04 | Neutralização · anticorpos: prendem o vírus livre | 2 |
| FE-05 | Ataque T · destrói a célula infectada já lida, como o linfócito T | 2 |
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
| FB2-09 | Neutralização num lisogênico | o vírus está dentro da célula: use o Ataque T |
| FB2-10 | Eclosão | a célula se rompeu: 3 vírus livres (ciclo lítico) |
| FB2-11 | Antibiótico em fungo | a parede do fungo é de quitina: antibiótico não age |
| FB2-12 | Tiro na ponta da hifa | corte a base da colônia |
| FB2-13 | Bactéria se dividiu | uma bactéria se dividiu em duas |
| FB2-14 | Levedura brotou | a levedura brotou: nasceu outra |

**Narrativa** (abertura, interlúdios, final, game over): rascunhos no [`gdd.md`](gdd.md) §"Nanobô 2.0". Eles entram aqui com ID quando forem desenhados.