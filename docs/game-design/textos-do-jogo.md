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
| FB-10 | contato com invasor (bactéria ou vírus) | `contato com invasor: -10 de integridade` | 39 | #11; #218; #234 (duas barras) |
| FB-15 | célula do paciente perdida (colonizada ou rompida) | `uma célula do paciente foi destruída: -20 vitais` | 47 | #234 |
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

Desenhada **em toda partida**: `MENU → BRIEFING → GAME`, e também depois de `VITÓRIA` e de `GAME OVER`. Desde 26/11 a cena mostra a abertura na caixa de diálogo do diário (ficha do hospital e Anotação 0, 3 páginas cada, rascunho): `ENTER`, `ESPAÇO` ou clique avançam; `ESC` pula tudo para a fase; o fim da Anotação 0 entra na Fase 1. A cada entrada em `BRIEFING` a abertura recomeça da ficha. Os IDs AB-01 a AB-08 substituem NR-01 a NR-14 e os botões e o passo do briefing de 15/11 (BR-02, BR-03, BR-05 e BR-06 saíram). Linhas de até 67 caracteres desde x = 80; texto final depende de aprovação do dono. O paciente se chama Léo (nome do interlúdio, IL-02).

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| BR-04 | abertura, rodapé (y = 24, centrado) | `enter ou clique avança · esc pula` | 33 | #103 |
| AB-01 | cabeçalho da ficha, sem retrato | `Hospital · ficha do paciente` | 28 | #244 |
| AB-02a | ficha, página 1 de 3 | `Paciente: {Léo Martins Ferreira}, 16 anos.` | 40 | #244 |
| AB-02b |  | `Chegou ao hospital com febre alta.` | 34 | #244 |
| AB-03 | página 2 de 3 | `Os exames mostram uma {infecção} que se espalha pelo sangue.` | 58 | #244 |
| AB-04a | página 3 de 3 | `A equipe testa um {nanorrobô} médico, menor que uma célula,` | 57 | #244 |
| AB-04b |  | `dentro do corpo de Léo.` | 23 | #244 |
| AB-05 | cabeçalho da Anotação 0, com retrato | `Nanobô · Anotação 0` | 19 | #244 |
| AB-06 | Anotação 0, página 1 de 3 | `{Sistema ligado}. Minha memória está vazia.` | 41 | #244 |
| AB-07 | página 2 de 3 | `Meu paciente é o {Léo}, de 16 anos, com muita febre.` | 50 | #244 |
| AB-08a | página 3 de 3 | `Vou descobrir o que está {atacando} ele.` | 38 | #244 |
| AB-08b |  | `Anoto tudo aqui, até os meus erros.` | 35 | #244 |
| NR-15 | interlúdio: Resultados entre as fases, subtítulo | `sinais vitais: %s (%d) · a seguir: %s`, com `vírus nas vias aéreas` depois da Fase 1 (`estável` ≥ 70, `em observação` 40–69, `grave` < 40, já com o piso de 60) | até 67 | #244 |
| NR-16 | interlúdio: Resultados depois da Fase 2, subtítulo | o `%s` do NR-15 depois da Fase 2 é `fungos no pulmão` (mesmos estados do NR-15) | até 63 | #227 |

Os textos do briefing de 04/11 (BR-07 a BR-19, sobre a coloração de Gram e as armas) saíram do jogo em 15/11 e ficam no histórico do git.

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
| CT-11c | HUD, linha 0 | duas barras de 36 px sem número: escudo + integridade do Nanobô (ciano) e coração + vitais do paciente (verde ≥ 60, âmbar ≥ 30, vermelho abaixo) | — | #218; #234 |
| CT-11d | mundo, acima do agente da dica | `botão direito: escanear` (some no primeiro scan; volta no tiro num invasor sem leitura) | 23 | #219 |
| CT-11e | — | `conceito` / `enter ou clique para continuar` | — | #222; saíram em 25/11: a caixa do diário (§3.1) tem cabeçalho, contador `n/N` e triângulo |
| CT-11f | caixa do diário, título no cabeçalho (DG-01, DG-02) | `Coloração de Gram` (CC-01) / `Resistência e Reserva` (CC-02) | 17 / 21 | #222; linhas em §3.1 |
| CT-12 | menu, rodapé | `enter ou clique para começar` | 28 | #201 |
| CT-13 | menu, rodapé | `F11 tela cheia` | 14 | #201 |
| CT-14 | HUD, linha 1, à direita | `ameaças %d/%d · identificados %d/%d` (`ameaças 0/6 · identificados 0/9`) | 31 (até 35) | #199; juntas na mesma linha em 15/11 (#243) |
| CT-15 | — | `identificados %d/%d` | — | entrou na CT-14 em 15/11 (#243) |
| CT-16 | HUD, linha 0, depois das barras | `pontos %d` | 9 | #199; #243 |
| CT-17 | HUD, linha 0 | `pronto` / `recarga` / `travado` / `defesa` / `esgotada` | 6 / 7 / 7 / 6 / 8 | #199; #215 (trava); #216 (Reserva sem carga) |
| CT-18 a CT-21 | — | `Nenhum alvo`, `Aproxime-se de um agente para escanear.`, `botão direito sobre o agente`, `Alvo neutralizado`, `Chegue perto de outro agente.` | — | saíram em 15/11 (#243): sem alvo, a ficha do scanner some; a dica do tutorial no mundo ensina o botão direito |
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
| CT-36 | HUD, linha 0, à direita | `Ruptura · parede` / `Dissolução · membrana` / `Reserva %d/%d · resistente` (`Reserva 2/2 · resistente`) | 16 / 21 / 24 | decisão 03/11 (noite); #216; a ferramenta com o alvo dela em 15/11 (#243) |
| CT-36b | HUD, linha 0, Fase 2 | `Neutralização · vírus livre` / `Ataque T · célula infectada` | 27 / 27 | #88; #243 |
| CT-36c | HUD, linha 0, Fase 3 | `Perfuração · antifúngico` | 24 | #230 |
| CT-37 | resultados, painel novo | `o que a partida mostrou` | 23 | #202, build de 04/11 |
| CT-38 a CT-41 | — | contadores e frase de revisão do painel de 04/11 | — | saíram com o relatório por objetivo (#210, #223); os textos atuais são as RR-01 a RR-10 da §2.2 |
| CT-42 | HUD, linha 1, Fase 2 | `neutralize os vírus e vá ao destino` | 35 | Fase 2 mínima (04/11); encurtada em 15/11 (#243) |
| CT-42b | HUD, linha 1, Fase 3 | `elimine os fungos e vá ao destino` | 33 | #227 |
| CT-43 | scanner, camada 1, vírus | `clique de novo para concluir a leitura` | 38 | Fase 2 mínima (04/11) |
| CT-44 | scanner, categoria | `vírus` / `célula infectada` (depois da leitura) | 5 / 16 | Fase 2 mínima (04/11); #87 |
| CT-44b | scanner, categoria, fungo (levedura, bolor e esporo pousado) | `fungo` | 5 | #228 |
| CT-45 | resultados, título | `Fase %d concluída` | 17 | Fase 2 mínima (04/11) |
| CT-46 | vitória / game over | `tempo total` / `recorde` | 11 / 7 | Recorde mínimo (04/11) |
| CT-47 | menu, rodapé | `recorde %d · %s` (`recorde 1234 · Jogador`) / `nenhum recorde ainda` | até 29 / 20 | Top 10 (#34), 15/11 |
| CT-48 | menu, subtítulo | `DEFENDA O ORGANISMO` | 19 | varredura de 05/11 |
| CT-49 | menu, rodapé do painel de controles | `interprete as informações do scanner antes de atacar` | 52 | varredura de 05/11 |
| CT-50 | HUD, linha 1, Fase 1 | `neutralize as bactérias e vá ao destino` | 39 | varredura de 05/11; encurtada em 15/11 (#243) |
| CT-51 | HUD, linha 1, sem ameaças | `vá ao destino` | 13 | varredura de 05/11; #243 |
| CT-52 | mundo / marca de destino | `destino` | 7 | varredura de 05/11 |
| CT-53 | pausa, título | `PAUSE` | 5 | varredura de 05/11 |
| CT-54 | pausa, rodapé | `esc para voltar` | 15 | varredura de 05/11 |
| CT-55 | menu, botão | `INICIAR` | 7 | varredura de 05/11 |
| CT-56 | pausa/resultados, botão | `CONTINUAR` | 9 | varredura de 05/11 |
| CT-57 | vitória/game over, botão | `NOVA PARTIDA` | 12 | varredura de 05/11 |
| CT-58 | vitória, subtítulo | `o paciente recebeu alta: missão cumprida` | 40 | #244 (final da narrativa) |
| CT-59 | vitória, linha de estatística | `pontuação final` | 15 | varredura de 05/11 |
| CT-60 | vitória, rodapé | `enter também começa de novo` | 27 | varredura de 05/11 |
| CT-61 | game over, título | `GAME OVER` | 9 | varredura de 05/11 |
| CT-62 | game over, subtítulo | `os sinais vitais caíram demais: a equipe retirou o Nanobô` (vitais em 0) / `o Nanobô foi danificado: a equipe o retirou do corpo` (integridade em 0) | 57 / 52 | #218; #234; #244 |
| CT-63 | game over, rodapé | `enter também reinicia` | 21 | varredura de 05/11 |
| CT-64 | menu, botão | `RECORDES` | 8 | #197 |
| CT-65 | painel do Top 10, título | `Top 10` | 6 | #197 |
| CT-66 | painel do Top 10, vazio | `nenhum recorde ainda` / `vença uma partida` | 20 / 17 | #197 |
| CT-67 | painel do Top 10 no menu, rodapé | `enter ou esc fecha` | 18 | #197 |
| CT-68 | vitória, entrada de nome | `novo recorde no Top 10!` | 23 | #81 |
| CT-69 | vitória, entrada de nome | `digite seu nome e tecle enter` | 29 | #81 |
| CT-70 | vitória, depois do nome | `posição %d no Top 10` | 20 | #81 |
| CT-71 | vitória, fora do Top 10 | `a pontuação não entrou no Top 10` | 32 | #79 |
| CT-72 | vitória, gravação falhou | `não foi possível gravar o recorde` | 33 | #80 |
| CT-73 | nome padrão no ranking | `Jogador` (nome vazio) / `Recorde` (recorde antigo importado) | 7 / 7 | #78, #81 |

**Conferência no QA (#212, desde 15/11).** Na raiz do repositório, rode `python -I ../Nanobo-testes/conferir_textos.py .`. O script cruza todo literal de string de `src/main.c` e `src/gameplay/gameplay.c` com este registro, nos dois sentidos: lista o texto desenhado sem linha aqui e a linha daqui cujo texto saiu do código.
- **Resultado esperado:** `0 literais sem linha no registro, 0 linhas sem texto no codigo`, com código de saída 0.
- **Fica de fora:** o que não é desenhado (mensagens de `_Static_assert`, `printf` de erro, caminhos e o formato do arquivo do ranking) e as linhas de QA sob `#ifdef _DEBUG`. O título `NANOBÔ` é o caso especial da §2.1, um bitmap montado por `make_text_bitmap`.
- **Histórico:** a varredura de 05/11 foi manual e trouxe a CT-48 a CT-63. A de 15/11 tirou a CT-38 a CT-41, que tinham saído do código, e registrou os nomes das bactérias (RT-05c).

### Textos do scanner (`src/main.c` + `src/gameplay/gameplay.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| RT-01 | scanner, camada 1, coco | `forma: coco (esférica)` (sem amostra de cor) | 22 | #214, design 2.0 |
| RT-02 | scanner, camada 1, bacilo | `forma: bacilo (bastão)` (sem amostra de cor) | 22 | #214, design 2.0 |
| RT-03 | scanner, camada 1, célula | `célula do paciente · eucarionte` (sem amostra de cor) | 31 | #214, design 2.0 |
| RT-04 | scanner, camada 2, Gram+ (amostra roxa) | `Gram+ · parede grossa, sem membrana externa · %s` (`... · Streptococcus pyogenes`) | 46 + nome (até 68) | #214, design 2.0 |
| RT-05 | scanner, camada 2, Gram- (amostra rosa) | `Gram- · parede fina sob membrana externa · %s` (`... · Pseudomonas aeruginosa`) | 43 + nome (até 65) | #214, design 2.0 |
| RT-05c | scanner, camada 2, o `%s` da RT-04 e da RT-05 (`phases[]` em `src/entities/entities.c`) | Gram+: `Staphylococcus aureus` / `Streptococcus pyogenes` / `Clostridium tetani`; Gram-: `Moraxella catarrhalis` / `Escherichia coli` / `Pseudomonas aeruginosa` | 21 / 22 / 18; 21 / 16 / 22 | #214; registrado na #212 |
| RT-05b | scanner, camada 2, célula | `núcleo guarda o DNA · mitocôndrias · não atacar` | 47 | #214, design 2.0 |
| RT-06 | scanner, linha de estado | ` · alvo sob o cursor` (ciano) / ` · última leitura` (apagado) | 20 / 17 | #199, build de 04/11 |
| RT-07 | scanner, camada 2, revisão bactéria (Gram+ e Gram-) | `procarionte · sem núcleo: DNA solto no nucleoide` | 48 | #214, OA1 |
| RT-08b | scanner, camada 2, revisão célula | `eucarionte: tem núcleo, como as células do corpo` | 48 | #214, OA1 |
| RT-09 | scanner, camada 1, vírus livre | `vírus livre · deriva no muco` | 28 | #87 |
| RT-10 | scanner, camada 2, vírus livre | `acelular: sem parede, ribossomos nem metabolismo` | 48 | #87 (OA6) |
| RT-11 | scanner, camada 2, revisão, vírus livre | `só se multiplica dentro de uma célula do paciente` | 49 | #87 |
| RT-12 | scanner, camada 1, célula infectada | `célula infectada · vírus lisogênico · %d s` (`... · 10 s`) / `célula infectada · eclipse: vai romper` | 42 / 38 | #87 |
| RT-13 | scanner, camada 2, célula infectada | `lisogênico · %d s · o vírus espera dentro da célula` / `eclipse · o vírus fez cópias dentro da célula` | 51 / 45 | #87 (OA7) |
| RT-14 | scanner, camada 2, revisão, célula infectada | `no lítico a célula se rompe e solta novos vírus` | 47 | #87 |
| RT-15 | scanner, camada 1, levedura (sem amostra de cor) | `forma: levedura (arredondada, brota)` | 36 | #228 |
| RT-16 | scanner, camada 2, levedura e esporo pousado (sem amostra de cor) | `eucarionte · parede de quitina · membrana com ergosterol` | 56 | #228; esporo na #229 |
| RT-17 | scanner, camada 1, colônia de bolor (sem amostra de cor) | `forma: bolor (filamentos)` | 25 | #229 |
| RT-18 | scanner, camada 2, colônia de bolor (sem amostra de cor) | `eucarionte · parede de quitina · corte a base da colônia` | 56 | #229 |
| RT-19 | scanner, camada 1, esporo pousado (sem amostra de cor) | `esporo de bolor · vai germinar` | 30 | #229 |

**Limite:** a fonte embutida é de **8 px por caractere** (medido com `al_get_text_width`, não estimado) e a área útil da ficha do scanner é de 588 px, a partir de `text_x = 36`. Desde 15/11 (#243), o HUD tem duas linhas, com estas folgas no pior caso:
- **linha 0:** sobram 20 px entre o tempo e a ferramenta (`Neutralização · vírus livre`, `recarga`, pontos de 3 dígitos);
- **linha 1:** sobram 32 px entre o objetivo da Fase 1 e os contadores com 2 dígitos.
 A linha mais longa desenhada na fase é a RT-04 com *Streptococcus pyogenes*: **68 caracteres** (544 px) mais a amostra de 12 px, 556 px no total (medido em 13/11, #214). A mais longa do briefing tem 54 (432 px, dentro dos 548 px do painel) e a dos resultados, 36 (288 px). Nenhuma passa da área do painel em 640×360. A RT-08 (revisão Gram- separada) saiu em 13/11: Gram+ e Gram- compartilham a RT-07, porque a revisão agora fala do procarionte, não do grupo.

## 2.1 Regra de tipografia (medida em 03/11/2026)

**Maiúscula acentuada não desenha.** A fonte embutida devolve glifo errado para `Í`, `Ç`, `Ã`, `Ó` maiúsculos — `FASE 1 CONCLUÍDA` saía `CONCLU f DA`, `MOVIMENTAÇÃO` saía `MOVIMENTAÇ%Ø` — enquanto as **minúsculas acentuadas saem com o glifo certo** (`ç`, `ã`, `ó`, `ê`, `ô`, `í`).

**`ê` e `ô` ficam apertados na fonte pequena (25/11).** Os textos com `ê` e `ô` (`Lisogênico`, `você`, `colônia`, `Nanobô`, `bônus`) estão **corretos**, decisão do dono de 25/11. O problema é o tamanho da fonte embutida: no glifo de 8 px, o circunflexo encosta na letra, e numa captura reduzida o `ê` lembra `@` e o `ô` lembra `8`. É uma questão de legibilidade da fonte, não do texto nem do código, e fica para o redesenho da interface que vem a seguir.

Regras que valem para todo texto novo:

- Títulos e rótulos em **caixa de frase** (`Fase 1 concluída`, `Objetivo da missão`, `Identificação concluída`).
- Caixa alta só em texto **sem acento** (`INICIAR`, `CONTINUAR`, `COMO JOGAR` quando não houver acento).
- Nenhum texto desenhado pode ter maiúscula acentuada. `NANOBÔ` é exceção registrada: é o nome do jogo, desenhado em bitmap ampliado 3×, onde o glifo é legível mas não correto.
- Antes de commitar texto novo, rodar `python -c "print([c for c in 'TEXTO' if ord(c) > 0xFF])"` (codepoint fora de Latin-1 vira `^` na tela).

## 2.2 Relatório da fase (resultados, #210 e #223)

Desde 14/11 (#223), o painel "o que a partida mostrou" fala em linguagem de jogador, sem os códigos internos "OA1:" a "OA4:". São cinco linhas de 14 px, uma por conteúdo, com os números da partida (`MatchProgress`), geradas por `results_report_line` em `src/main.c`. A maior linha tem 42 caracteres (336 px, com contadores de dois dígitos) e cabe nos 344 px do painel (43 caracteres); a da Fase 2, `Ataque T: %d células contidas a tempo`, tem 38 com dois dígitos (304 px). As mensagens OA-01 a OA-04 da #210 saíram do código.

| ID | Linha | Texto | Caracteres | Contador |
|---|---|---|---:|---|
| RR-01 | 1, fase com bactéria | `lidas até a coloração: %d Gram+ e %d Gram-` | 40 | `scans_gram_positive`, `scans_gram_negative` |
| RR-02 | 2, fase com bactéria | `tratamentos completos: %d · resistentes: %d` | 41 | `treatments_completed`, `resistances` |
| RR-03 | 3, fase com bactéria | `nenhum antibiótico errado` / `antibiótico errado: %d (%d pela forma)` | 25 / 38 | `wrong_decisions`, `shape_guesses` |
| RR-04 | 4, fase com bactéria | `Reserva guardada: nenhuma carga gasta` / `Reserva: %d na resistente, %d na comum` | 37 / 36 | `reserve_on_resistant`, `reserve_on_common` |
| RR-05 | 5, fase com bactéria | `células do paciente feridas: %d` | 30 | `cell_attacks` |
| RR-06 | 1, fase de vírus | `anticorpos: %d vírus livres presos` | 34 | `viruses_neutralized` (#88) |
| RR-07 | 2, fase de vírus | `Ataque T: %d células contidas a tempo` | 37 | `cells_lysed` (#88) |
| RR-08 | 3, fase de vírus | `ciclo lítico: %d células romperam` | 32 | `cells_burst` (#86) |
| RR-09 | 4, fase de vírus | `ameaças eliminadas: %d de %d` | 28 | ameaças da fase, com os vírus da rajada |
| RR-10 | 5, fase de vírus | `vírus não têm parede nem ribossomos` | 35 | — (OA6) |
| RR-11 | 1, fase de fungos | `leveduras eliminadas: %d` | 24 | `yeasts_eliminated` (#232) |
| RR-12 | 2, fase de fungos | `colônias eliminadas: %d (com germinadas)` | 40 | `molds_eliminated`; a colônia que germinou de esporo conta (#232) |
| RR-13 | 3, fase de fungos | `antibiótico em fungo: %d tiros` | 30 | `antibiotic_on_fungus`: tiro em levedura, colônia, hifa ou esporo (#232) |
| RR-14 | 4, fase de fungos | `esporos pelo estado do paciente: %d` | 35 | `spore_load_current()`, a carga da abertura (#231, #232) |
| RR-15 | 5, fase de fungos | `fungo: parede de quitina, alvo: ergosterol` | 42 | — (frase fixa; 42 caracteres = 336 px, a largura útil do painel; a versão com "alvo é o ergosterol" tinha 45 e passava de 360 px) |

**Leitura dos contadores:** um tratamento completo é uma bactéria eliminada com as duas doses do antibiótico certo. A Reserva não conta como tratamento, e o relatório a separa entre a gasta numa resistente e a gasta numa bactéria comum. `shape_guesses` conta o antibiótico errado dado depois de ler só a forma (FB-05).

**Regra de tipografia:** seguir a seção 2.1. Utilizar hífen ASCII em `Gram-` e evitar maiúsculas acentuadas.

**Fonte:** Issue #210 — Resumo por objetivo na tela de Resultados (RF-07).

## 3. Nanobô 2.0 — rascunhos (13/11/2026)

> **Rascunhos, ainda não estão no código.** Regras em `ensino-e-aprendizagem.md` (§"Nanobô 2.0"). Os cards têm 3 linhas de até 70 caracteres (contagem abaixo, feita com Python). Os textos de HUD e ficha seguem a regra da seção 2.1: a fonte embutida foi mantida em 15/11. Os IDs `CC-` são cards de conceito, `FB2-` são feedback do 2.0 e `FE-` são ferramentas.

### 3.1 Cards de conceito

> **Caixa de diálogo desde 25/11 (diário):** cada card é uma entrada do Diário do Nanobô (ou um interlúdio), desenhada na caixa da faixa de baixo (x = 16 a 624, y = 264 a 352) com retrato 48×48, cabeçalho e até 3 linhas por página, de 1 a 4 páginas. As linhas começam em x = 80 e vão até x = 616: **67 caracteres** (536 px), com a mesma largura nas entradas sem retrato. O termo-chave de cada página vem entre chaves (`{termo}`), sai em âmbar e **não conta** nos caracteres; o `conferir_textos.py` tira as chaves antes de comparar. O texto aparece a 40 caracteres/s; o rodapé mostra `n/N`, um triângulo piscando com a página completa e `fechar` na última. Os títulos de antes viraram parte do cabeçalho (DG-01 a DG-09); textos provisórios até a aprovação do dono.
>
> **No código desde 14/11 (#222):** CC-01 abre na primeira leitura completa de bactéria e CC-02 na primeira bactéria resistente. Cada card aparece uma vez por partida, sobre a fase parada, com as linhas desenhadas a partir de x = 56 num painel de 576 px; a maior (CC-02a, 66 caracteres) ocupa 528 px. CC-05 e CC-06 estão no código desde 25/11 (#232): CC-05 abre ao entrar na Fase 3 e CC-06 na 1ª leitura completa de fungo ou no 1º antibiótico num fungo, o que vier primeiro.
>
> **Fase 2 no código desde 14/11 (#87, #88):** CC-03 abre na primeira leitura de um vírus livre, no primeiro vírus preso pelos anticorpos ou no primeiro antibiótico num vírus. CC-04 abre na primeira leitura de uma célula infectada ou na primeira célula que rompe, para quem não leu. Títulos: `Vírus não é célula` (18) e `Lisogênico e lítico` (19).
>
> **Numeração (26/11):** o número da anotação segue a **ordem de descoberta na partida** (a 1ª que abre é a 1, a seguinte a 2, e assim por diante; recomeça em 1 a cada partida; o interlúdio não conta e a abertura é a Anotação 0). Os números nas linhas CC abaixo são só a ordem usual de uma partida; no jogo o número vem de `card_note_number()` e o cabeçalho é o DG-13. CC-01 ganhou a página 2 (jogo simplifica: todas as bactérias juntas no sangue; armas inspiradas em antibióticos reais) e CC-04 a página 3 (lisogênico e lítico foram descritos em bacteriófagos; aqui são comparação), em 26/11.

| ID | Card / quando aparece | Linha | Car. |
|---|---|---|---:|
| CC-01a | Nanobô · Anotação 1 · Coloração de Gram · 1ª leitura completa de bactéria · página 1 de 2 | No laboratório, um corante roxo tinge a parede das bactérias. | 61 |
| CC-01b |  | Gram+: parede grossa de {peptidoglicano}, que segura o roxo. | 58 |
| CC-01c |  | Gram-: parede fina e membrana externa; perde o roxo e fica rosa. | 64 |
| CC-01d | página 2 de 2 | No jogo, todas as bactérias aparecem juntas no sangue, | 54 |
| CC-01e |  | só para simplificar. As armas são regra do jogo, inspirada | 58 |
| CC-01f |  | em {antibióticos} reais. | 22 |
| CC-02a | Nanobô · Anotação 2 · Resistência e Reserva · 1ª bactéria resistente · página 1 de 1 | Uma dose só não basta: as bactérias que sobrevivem se multiplicam. | 66 |
| CC-02b |  | Esta ficou {resistente}: o mesmo antibiótico não age mais nela. | 61 |
| CC-02c |  | A Reserva age nela, mas tem poucas cargas: use só se precisar. | 62 |
| CC-03a | Nanobô · Anotação 6 · Vírus não é célula · 1ª leitura de vírus ou 1º antibiótico em vírus · página 1 de 2 | O vírus não tem parede, ribossomos nem metabolismo próprio. | 59 |
| CC-03b |  | Ele só se multiplica {dentro de uma célula}. | 42 |
| CC-03c |  | Antibiótico não o atinge. | 25 |
| CC-03d | página 2 de 2 | A Neutralização leva {anticorpos}, que se prendem ao vírus livre. | 63 |
| CC-04a | Nanobô · Anotação 7 · Lisogênico e lítico · 1ª leitura de célula infectada · página 1 de 3 | {Lisogênico}: o vírus fica dentro da célula, sem destruí-la ainda. | 64 |
| CC-04b | página 2 de 3 | {Lítico}: ele faz cópias, a célula se rompe e libera novos vírus. | 63 |
| CC-04c |  | O Ataque T destrói a célula infectada, como faz o linfócito T. | 62 |
| CC-04d | página 3 de 3 | Lisogênico e lítico foram descritos em vírus de bactérias, | 58 |
| CC-04e |  | os {bacteriófagos}. Aqui servem de comparação. | 44 |
| CC-05a | Nanobô · Anotação 8 · Organismo enfraquecido · início da Fase 3, depois do IL-03 · página 1 de 1 | Fungos {oportunistas} aproveitam um organismo enfraquecido. | 57 |
| CC-05b |  | Os esporos de Aspergillus estão no ar; o corpo saudável os elimina. | 67 |
| CC-05c |  | Quanto pior o paciente chega, mais esporos se instalam no pulmão. | 65 |
| CC-06a | Nanobô · Anotação 9 · Fungo · 1ª leitura de fungo · página 1 de 2 | O fungo tem núcleo, como as nossas células: é um {eucarionte}. | 60 |
| CC-06b |  | Sua parede é de quitina, não de peptidoglicano: | 47 |
| CC-06c |  | antibiótico não age. | 20 |
| CC-06d | página 2 de 2 | A Perfuração abre poros na membrana dele, que tem {ergosterol}. | 61 |
| CC-07a | Nanobô · Anotação 3 · Flagelo · 1ª investida de um bacilo (14/11; no código desde a #235) · página 1 de 1 | O bacilo tem {flagelo}, um chicote que gira e o faz nadar. | 56 |
| CC-07b |  | Por isso ele vem atrás de você e dá investidas. | 47 |
| CC-07c |  | O coco não tem flagelo: é levado pelo sangue e gruda no tecido. | 63 |
| CC-08a | Nanobô · Anotação 4 · Biofilme · 1º contato do Nanobô com um biofilme (14/11; no código desde a #237) · página 1 de 1 | Bactérias grudadas fazem uma camada pegajosa: o {biofilme}. | 57 |
| CC-08b |  | Ele protege as bactérias: dentro dele, o antibiótico age menos. | 63 |
| CC-08c |  | No biofilme a bactéria pede mais uma dose, e você anda devagar. | 63 |
| CC-09a | Nanobô · Anotação 5 · Endotoxina · 1º contato do Nanobô com uma nuvem (14/11; no código desde a #237) · página 1 de 1 | A membrana externa da Gram- tem uma toxina, o {LPS}. | 50 |
| CC-09b |  | Quando a Gram- se rompe, até pelo antibiótico, o LPS se espalha. | 64 |
| CC-09c |  | Saia da nuvem: ela fere o Nanobô enquanto dura. | 47 |
| IL-02a | Enquanto isso, no hospital · início da Fase 2, antes de qualquer anotação · sem retrato · página 1 de 2 | A infecção no sangue de Léo deu trégua, mas na enfermaria | 57 |
| IL-02b |  | ele pegou um {vírus respiratório}. | 32 |
| IL-02c | página 2 de 2 | O Nanobô segue para as {vias aéreas}. | 35 |
| IL-03a | Enquanto isso, no hospital · início da Fase 3, antes do CC-05 · sem retrato · página 1 de 2 | Com as defesas fracas, {esporos de bolor} que estão no ar | 55 |
| IL-03b |  | chegam ao pulmão de Léo. | 24 |
| IL-03c | página 2 de 2 | O Nanobô precisa agir antes que o {fungo} se instale. | 51 |

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

### 3.2 Ferramentas (menu "como jogar" e HUD)

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