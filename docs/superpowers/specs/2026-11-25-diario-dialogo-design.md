# Diário do Nanobô em caixa de diálogo — design (25/11/2026)

## Intenção

O Nanobô é um jogo para ensinar biologia do ensino médio. Hoje os textos que ensinam são curtos demais (card de título + 3 linhas) e espalhados em três sistemas diferentes (cards de conceito, briefing de 4 cartões, subtítulo de interlúdio). O dono quer que o jogo **explique mais, de forma divertida**, e decidiu (25/11):

- os cards viram o **Diário do Nanobô**: anotações em primeira pessoa que o próprio Nanobô escreve sobre o que descobre (ele é um protótipo recém-ligado, de memória vazia — ninguém explica nada a ele);
- entre as fases, os **interlúdios** contam a história do **Paciente**, Léo Martins Ferreira, 16 anos;
- todo esse texto aparece numa **caixa de diálogo paginada**, que avança com a mesma tecla.

Termos no `docs/game-design/glossario.md` (§Narrativa e ensino): Diário do Nanobô, Anotação, Paciente, Interlúdio. Decisões de conteúdo e narrativa em `docs/game-design/decisoes.md` §"Narrativa e conteúdo (25/11/2026)".

## Metas

1. Uma **caixa de diálogo única** (mesmo desenho, mesmas teclas, mesmo ritmo) usada por: anotações do Diário durante o jogo, abertura (ficha do hospital + Anotação 0) e interlúdios do Léo.
2. Cada entrada tem **várias páginas** de até 3 linhas, com texto que aparece letra a letra.
3. O jogo continua **pausado** enquanto a caixa está aberta, como os cards de hoje.
4. Nenhuma regra de jogo muda além do que a narrativa exige (novas entradas na fila de cards; ordem de exibição).

## Fora do escopo (não-metas)

- Menu para reler o Diário (registrado como próximo passo; fora de 02/12).
- Textos num arquivo externo editável (abordagem C, descartada).
- Troca de fonte: a fonte embutida 8×8 fica; a legibilidade será reavaliada depois.
- Menu "como jogar", HUD e telas de Vitória/Game Over: terão design próprio depois deste.
- Som por letra (blip) e retrato animado.
- Redação final dos textos: é uma etapa separada (inventário de telas com "atual × proposto", aprovado pelo dono). Este design define o mecanismo e os lugares; o conteúdo entra pelas tabelas.

## Anti-metas (falha mesmo que funcione)

- Texto que avança sozinho por tempo.
- Um toque que pula duas páginas.
- O jogo correr (agentes, timers, dano) com a caixa aberta.
- Linha que estoura a caixa, maiúscula acentuada ou hífen tipográfico (regras de `textos-do-jogo.md` §2.1).
- Afirmação biológica sem fonte nos docs (`conteudo-biologico.md`).

## Restrições

- C11, só os 4 `.c` que compilam (`main.c`, `core/game.c`, `entities/entities.c`, `gameplay/gameplay.c`); sem módulo novo (`src/ui/` é pós-entrega). `gameplay.c` não inclui Allegro.
- Tela lógica 640×360, fonte embutida de 8 px por caractere.
- Prazo: implementação em 27–28/11; 29–30/11 ficam para menu e HUD.
- 0 avisos, harness verde, CRLF, todo texto desenhado registrado em `textos-do-jogo.md`.

## Abordagens consideradas

| | Abordagem | Troca |
|---|---|---|
| A | Só estender os cards atuais para várias páginas | Menor risco (~½ dia), mas abertura, interlúdios e cards continuam com visuais e teclas diferentes, e a narrativa não fica com uma cara só |
| **B** | **Uma caixa de diálogo única para cards, abertura e interlúdios, textos em tabelas C** | **Escolhida pelo dono (25/11).** Narrativa coesa e tecla única; mexe na abertura e no fluxo entre fases, então pede mais teste (~1,5 dia) |
| C | B + textos num arquivo lido ao abrir o jogo | Edição sem recompilar, mas exige leitor de arquivo, tratamento de erro e mais um arquivo no instalador — risco sem retorno até 02/12 |

## Como funciona (decidido)

### A caixa

- Posição: faixa inferior da tela de jogo, de x = 16 a x = 624 (largura 608 px), altura que caiba título + 3 linhas + rodapé; fundo do painel padrão (`draw_panel`) e véu escuro sobre o resto da tela, como o card de hoje.
- **Retrato** à esquerda: um quadro do spritesheet do Nanobô (48×48) nas Anotações. Na ficha do hospital, sem retrato (ou um ícone de prancheta, se já houver um asset; não criar arte nova para isso).
- **Cabeçalho**: quem fala e o quê, ex.: `Nanobô · Anotação 4`, `Hospital · ficha do paciente`, `Enquanto isso, no hospital`.
- **Texto**: até 3 linhas por página, cada linha no máximo o que couber na área de texto (medir: com retrato, cerca de 66 caracteres de 8 px). Entrelinha igual à do card atual (16 px) ou menor se precisar caber.
- **Termo-chave**: um por página, desenhado em cor de destaque (âmbar do UI). A marcação no texto é escolha do construtor (ex.: `{parede}`), desde que não conte na largura e seja verificada pelo `conferir_textos.py`.
- **Rodapé**: contador `2/4` à direita; quando a página está completa, um `▼` piscando (ou o glifo equivalente que a fonte tiver; o `▼` não está no conjunto da fonte embutida — usar um triângulo desenhado com primitiva); na última página, a palavra `fechar` no lugar do `▼`.

### Ritmo e teclas

- Texto aparece a **~40 caracteres/s**.
- **Enter, Espaço ou clique esquerdo**: se a página ainda está aparecendo, completa a página; se está completa, vai para a próxima; na última página, fecha.
- **Trava de 0,2 s** após o início de cada página: toques nesse intervalo são ignorados. Repetição automática de tecla segurada não avança.
- **Esc**: fecha a entrada inteira de uma vez (pular), como hoje o Esc fecha o card. Na abertura, Esc pula toda a abertura, como hoje.
- Setas/WASD continuam registradas enquanto a caixa está aberta (o movimento não se perde ao fechar), como no card atual (#222).
- O jogo fica pausado: o mesmo mecanismo de hoje (`pending_card()` diferente de `CARD_NONE` faz `update_match` sair cedo). O efeito letra a letra roda no laço de desenho do `main.c`, não em `update_match`.

### Onde a caixa aparece

| Lugar | Hoje | Com o Diário |
|---|---|---|
| Anotações durante o jogo | 9 cards de conceito (CC-01..09), 1 página cada | as mesmas 9 entradas, mesmos gatilhos, com várias páginas cada |
| Abertura (cena `BRIEFING`) | 4 cartões de narrador (NR-01..14), botões SEGUIR/PULAR | 2 entradas em sequência: **ficha do hospital** (Léo, 16 anos, febre alta, infecção no sangue) e **Anotação 0** (Nanobô ligando, memória vazia, missão); fim da última → entra na Fase 1 |
| Interlúdio antes da Fase 2 | subtítulo na tela de Resultados | entrada "Enquanto isso, no hospital" que abre **no início da Fase 2**, antes de qualquer anotação: a infecção no sangue cedeu, mas Léo pegou um vírus respiratório na enfermaria |
| Interlúdio antes da Fase 3 | subtítulo na tela de Resultados | entrada no **início da Fase 3**, antes da anotação do organismo enfraquecido (CC-05): com a defesa fraca, esporos de bolor que estão no ar chegam ao pulmão |
| Resultados de fim de fase | painel "o que a partida mostrou" | **continua o painel** (não vira caixa), com o título e as linhas reescritos na voz do Diário: "O que anotei nesta fase" — mudança só de texto |
| Vitória / Game Over | telas próprias | **fora deste design** (texto do Léo entra na etapa de menu/telas): Vitória = alta do Léo; Game Over = a equipe retira o Nanobô e Léo segue no tratamento convencional, sem morte |

O subtítulo atual do interlúdio na tela de Resultados (NR-15/NR-16) pode ficar como resumo curto dos vitais ou sair — decisão do construtor, desde que não repita o texto do interlúdio.

### Ordem e fila

- As entradas durante o jogo continuam na fila de cards do `gameplay.c` (uma vez por partida, limpas só em `reset_match`).
- Os dois interlúdios entram na mesma fila, enfileirados ao iniciar as Fases 2 e 3, e **precisam aparecer antes** de qualquer outra entrada pendente naquele momento (hoje a fila mostra o menor índice do enum; o construtor ajusta para que interlúdio venha primeiro).
- Reiniciar a partida (Game Over → abertura) mostra a abertura e os interlúdios de novo.

### Conteúdo que muda junto (decidido pelo dono)

Estas mudanças de conteúdo dependem do código e entram no mesmo trabalho, com fontes já registradas nos docs:

- Espécies: *Neisseria meningitidis* → ***Moraxella catarrhalis*** (coco Gram−); *Bacillus cereus* → ***Clostridium tetani*** (bacilo Gram+). Nomes, textos do scanner e dados em `entities.c`; ver `conteudo-biologico.md`.
- Ferramenta **"Lise" → "Ataque T"** (nome, HUD, feedback, registro de textos), porque o nome antigo se confundia com a lise da célula no ciclo lítico.
- Uma anotação explica que, no jogo, todas as bactérias aparecem juntas no sangue para simplificar; outra, que lisogênico/lítico foram descritos em vírus de bactérias e servem de comparação.

### Textos

- Escritos em tabelas C no `main.c` (abordagem B), registrados em `textos-do-jogo.md` com ID e contagem.
- Tom: Nanobô curioso e entusiasmado, que também anota os próprios erros; frases curtas; um termo-chave em cor por página; nada que `conteudo-biologico.md` não sustente.
- A redação final passa por aprovação do dono num inventário "atual × proposto" antes de ir para as tabelas. A implementação pode começar com os textos atuais divididos em páginas e trocar o conteúdo quando ele for aprovado.

### Respostas à revisão do construtor (25/11)

- **Onde vive a caixa.** O desenho e as regras de avanço são um só, no `main.c`. A *fonte* do texto muda: na cena `BRIEFING` a caixa mostra a abertura (ficha + Anotação 0) com um estado próprio do `main.c`, fora da fila do `gameplay.c`; na cena `GAME` ela mostra a entrada pendente da fila de cards. `game.c` e `fluxo-estados.md` não mudam. Na abertura não há jogo por trás (fundo escuro, como hoje), e o Esc pula a abertura inteira; no jogo, o Esc fecha a entrada atual.
- **Rearmar a abertura.** Ao entrar em `BRIEFING` (vindo do menu ou do Game Over) o `main.c` zera o estado da abertura, como já faz com `briefing_card`.
- **Prioridade dos interlúdios.** Os dois interlúdios recebem os menores índices do enum de cards (o valor do enum não é gravado em lugar nenhum), então saem antes de qualquer outra entrada pendente, inclusive CC-05.
- **Páginas por entrada.** Variável, de 1 a **4** páginas, cada uma com até 3 linhas. As duas anotações novas não ganham ID: "todas juntas no sangue, para simplificar" vira uma página da anotação de Gram (CC-01) e "lisogênico/lítico foram descritos em vírus de bactérias" vira uma página da anotação do ciclo (CC-04). Os 9 IDs ficam; entram só os 2 interlúdios.
- **Retrato e largura.** Retrato = linha de frente (`Facing` 1), quadro ocioso 0. A área de texto tem a **mesma largura em todas as entradas** (começa depois da coluna do retrato mesmo quando não há retrato), para o ritmo e o quebra-linha serem iguais.
- **Verificador de textos.** O `conferir_textos.py` existe em `../Nanobo-testes/` (fora do repositório); o plano inclui ensiná-lo a ignorar a marcação do termo-chave ao comparar e ao contar caracteres.
- **Movimento.** Setas/WASD continuam registradas também nos interlúdios (são cards no início da fase). Não há salto de tempo ao fechar: `update_match` recebe o passo fixo do timer e não acumula tempo enquanto a entrada está pendente.

## Verificação

- Harness: as regras de fila (interlúdio enfileirado no início das Fases 2 e 3, prioridade sobre CC-05, uma vez por partida, limpo no `reset_match`) e a pausa do jogo com entrada pendente.
- Capturas pelo `qa_jogo.ps1`: abertura (ficha e Anotação 0), uma anotação de várias páginas no meio da revelação e completa, último página com `fechar`, interlúdio da Fase 2 e da Fase 3.
- Teste manual do dono: um toque nunca pula duas páginas; Esc pula; o movimento não se perde.

## Deixado para o construtor

- Desenho exato da caixa (alturas, margens, entrelinha), desde que caibam 3 linhas e o rodapé.
- Estrutura de dados das entradas e páginas, e onde fica o estado da revelação (no `main.c`).
- Sintaxe da marcação do termo-chave.
- Se a ficha do hospital usa ícone ou fica sem retrato.
- O que fazer com o subtítulo NR-15/NR-16.
