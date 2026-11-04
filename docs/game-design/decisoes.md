# Nanobô — Decisões de Game Design

Este arquivo registra decisões já tomadas pela dupla. Quando uma decisão importante for fechada, ela deve ser registrada aqui; hipóteses e itens ainda não validados permanecem como pendências.

## Decisões atuais

### Precedência entre fontes (03/11/2026)

Quando documento e código divergem, vale nesta ordem:

1. **Decisão registrada neste arquivo** — é o contrato da dupla.
2. **Corpo da issue**, quando ele foi revisado depois da decisão (o corpo é o contrato operacional que a dupla assinou).
3. **Spec de ensino e demais documentos descritivos** — são a explicação das regras.
4. **Código** — nunca é fonte de verdade; é dívida. Um comportamento no build que contradiz 1–3 não vence por estar rodando.

Corolários: um documento que descreve o que o código ainda não faz precisa dizer **onde** e **quando** aquilo entra (marco), senão vira promessa; e toda decisão nova exige a varredura dos documentos que citam a regra antiga no mesmo commit.

**Descartado por enquanto:** workflow automático de reconciliação documentação × código. A prevenção fica nesta seção, na varredura de cada decisão e no item de QA do fechamento.

### Dívida de implementação (03/11/2026, atualizada em 04/11/2026)

O que está decidido e o código ainda não faz — ou já faz desde o build de 04/11. Atualizar esta tabela quando uma regra entrar no build ou mudar de marco.

| Regra decidida | Onde está registrada | No build de 04/11 | Marco |
|---|---|---|---|
| Camada 1 = coloração (roxo/vermelho) + nome do agente | D2; `scanner.md` §4 | **feito** — amostra de cor + `<cor> · <nome>` | M4 (#104) |
| Camada 2 = grupo + estrutura + porquê | D2; `scanner.md` §4 | **feito** — grupo, estrutura e a frase de revisão | M4 (#104) |
| Briefing de 3 cartões | `decisoes.md`; RF-01 | **feito** — em toda partida, pulável com ESC | M4 (#103, #106) |
| Armas com nome (Ruptura/Dissolução) | `decisoes.md`; `textos-do-jogo.md` | **feito** — menu, HUD e as 7 mensagens de feedback | M4 |
| Armas com cor neutra; ícone e projétil por forma | RF-04 | **feito** — ícones neutros próprios (parede/anéis) e projétil por forma | M4 |
| Disco cinza até a camada 1; sprite + anel depois | RF-03; `decisoes.md` | **feito** — disco cinza, sprite e anel roxo/vermelho depois da leitura | M4 |
| Variação de sprites por espécie | `decisoes.md` (03/11, noite) | um sprite por espécie | M5 |
| Pulso de 0,75 s com imobilização e cancelamento | RF-02; Spec "Dificuldade" | scan instantâneo | M5 |
| Trava de 2 s + perseguição após arma errada | RF-06 | inexistente | M5 |
| Identificação condiciona a conclusão | D2.1 + D2.2 | conclui com 0 identificações | M5 |
| Resumo por objetivo na tela de Resultados | RF-07 (#174) | **parcial** — contadores no painel; o texto por OA fica no M5 | M5 |
| Fase 2 mínima (3 vírus + 3 células, 1 resistência, qualquer arma) | `decisoes.md` (04/11) | **feito** | M4 (#76) |
| Ciclo lisogênico/lítico e multiplicação limitada | #83–#86 | inexistente | M5 |
| Recorde persistente mínimo (melhor pontuação) | `decisoes.md` (04/11) | **feito** — menu e Vitória | M4 (Top 10 e nome no M5) |
| Bônus +15 por run perfeito (0 erro de arma + 0 tiro na célula) | `decisoes.md` (03/11, noite) | paga 9/9 | M5 |
| SFX e animação | requisito final | inexistentes | M5 |

**Oito linhas de M4 fecharam no build de 04/11:** camada 1, camada 2, briefing, nomes das armas, disco cinza/anel, ícones neutros, Fase 2 mínima e recorde. Uma ficou parcial: o resumo mostra os contadores, não o texto por objetivo.

**RF-07 completo segue pendente.** O painel `o que a partida mostrou` traz `bactérias identificadas: %d Gram+ · %d Gram-`, `acertos de arma: %d · erros: %d`, `ataques à célula: %d` e uma frase de revisão — é leitura por contador, não o resumo por objetivo (um texto por OA). A tela de Resultados ainda não diz o que cada objetivo mostrou na partida. Fica para o M5 por decisão de 04/11.

**O bônus é por fase:** +15 com 9/9 na Fase 1 e 6/6 na Fase 2. O run perfeito (0 erro de arma + 0 tiro na célula) só entra com D2.1/D2.2 no M5.

### Estrutura do MVP

O MVP permanece centrado em **uma fase**, garantindo uma partida completa e jogável antes da expansão do conteúdo.

### Segunda fase

A segunda fase é objetivo do **pós-MVP** e integra a entrega final planejada.

### Terceira fase

Uma terceira fase somente será adicionada se houver tempo e estabilidade suficientes.

### Aprendizagem pelo gameplay

A proposta é que o jogador descubra informações, interprete dados do scanner e use essa interpretação para tomar decisões, em vez de reduzir o conteúdo a um quiz.

### Scanner

- O scanner considera o alvo válido mais próximo.
- A leitura ocorre em pulsos/camadas.
- Cada agente possui identificação única.
- As informações obtidas devem apoiar a identificação e a decisão do jogador.
- A relação entre a informação obtida e a escolha do laser/ação deve ser validada durante os testes.

#### Seleção do alvo — regra para implementação

- São candidatos os agentes ativos e escaneáveis cuja distância ao Nanobô seja menor ou igual ao alcance do scanner.
- A ativação seleciona no máximo um candidato: aquele com a menor distância ao Nanobô.
- A comparação pode usar distância ao quadrado, sem alterar a regra geométrica.
- Sem candidatos no alcance, a ativação não inicia uma leitura e não identifica nenhum agente.
- Em empate exato, vence o agente com o menor identificador estável e único no encontro. A geração desses identificadores será definida junto ao modelo de entidades.
- A regra seleciona o alvo; revelação de camadas e registro de identificação continuam sendo responsabilidades separadas.

#### Pulsos, camadas e identificação — regra para implementação

- Cada ativação válida revela somente a próxima camada ainda não descoberta do alvo selecionado.
- No PoC existem 2 camadas por agente, verificáveis; o conteúdo das camadas está em `docs/game-design/scanner.md`.
- Informação já revelada permanece visível durante o encontro.
  > **Atualização 22/10/2026:** o painel passou a mostrar só a leitura do alvo atual; as camadas já reveladas continuam guardadas em cada agente e voltam a aparecer quando ele é lido de novo.
- A identificação de um agente é registrada quando a última camada disponível dele é revelada, e não se repete dentro do mesmo encontro.
- O scanner não recomenda ação e não atribui pontuação nem bônus.

### Interface

#### Interação de mouse

- A interação de mouse da interface é o clique sobre o botão `INICIAR` na tela de `MENU`, que inicia a partida.
- A mesma transição continua disponível por `ENTER`; o clique não substitui nenhum comando de gameplay.
- Nos demais estados, nenhum clique tem ação definida nesta etapa.
- Detalhes e roteiro de verificação em `docs/planejamento/interacao-mouse.md`.

#### HUD de vida (17/10/2026)

- A vida aparece como **3 corações de 16×16** (`assets/sprites/icone_coracao.png`); desde 22/10/2026 a fileira fica na primeira linha do painel (`hud_y(0)`, hoje y=266) logo depois da largura medida do texto de status, com 2 px de folga entre corações (o passo vem de `al_get_bitmap_width`).
- Coração perdido **não some: escurece** (tint 25%), para o jogador enxergar o máximo e o quanto já perdeu.
- O coração é carmim, como o catálogo de arte já especificava, e não usa o vermelho do laser Gram−.
- O texto `vida: %d` sai do HUD; a leitura passa a ser por contagem de corações.
- A regra da #11 não muda: 3 vidas, 1 de dano por contato, 1 s de invulnerabilidade, sem cura.
- Pontuação e objetivos continuam sem desenho definido — são o resto da #19.

#### Tema visual da interface (28/10/2026)

- A interface usa a **mesma paleta dos sprites**: `#121A24` (tinta), `#1A2634` (painel), `#F2F5F7` (texto), `#A8B8C8` (texto apagado), `#FFD23F` (destaque), `#BE6EF0` (Gram+), `#F05A5A` (Gram−), `#7DF9FF` (ciano), `#6EDC78` (verde) e `#FFE894` (feedback). Não há cor avulsa na UI.
- Três níveis de texto: principal, apagado e destaque. Cor com significado fica reservada a conteúdo (espécie, raio, vida, scanner), nunca a decoração.
- Painel, botão, pips de ameaça e barra de recarga são desenhados com primitivas do Allegro; moldura de 1 px sempre com deslocamento **+0,5 px**, senão a linha cai um pixel acima do texto.
- O botão tem três estados: normal, foco do teclado (moldura de destaque) e cursor em cima (moldura de destaque + preenchimento escuro). A ativação continua no **pressionar** do mouse (down), como registrado em `interacao-mouse.md`.
- Desenho e clique leem a **mesma lista de botões** (`screen_buttons`), para a área desenhada e a área clicável nunca divergirem.
- A grade do HUD continua com 5 linhas de 16 px; mudou o conteúdo das zonas: vitais e objetivo nas linhas 0-1, sub-painel do scanner nas linhas 2-4.
- A HUD mostra **pips** de ameaça (bloco cheio = neutralizada) no lugar de `restam: 6/6`, e o tempo em **mm:ss**.
- A interface é desenhada no canvas lógico de 640×360 e ampliada por escala inteira; nenhuma função de desenho lê escala ou deslocamento da janela. A janela tem **mínimo de 640×360**, então a escala nunca fica quebrada.
- O cursor é guardado em pixel da **janela** e convertido no desenho; `resize` e `F11` releem a posição, para o hover não ficar preso no lugar antigo.
- Cor translúcida em `al_map_rgba` **não mistura**: o blend padrão espera cor pré-multiplicada, então alfa sobre o canvas satura. Fundos e molduras da UI usam cor opaca (`UI_HOVER`, `UI_LINE`, `UI_FRAME`, `UI_SLOT`); só o escurecimento da pausa usa alfa, com RGB zero.
- A fonte continua sendo a embutida do Allegro (8×8). O título do menu é o mesmo glifo ampliado 3× a partir de um bitmap montado no carregamento; a fonte definitiva segue pendente.

### Combate

#### Defesa (16/10/2026)

- A defesa é **imunidade total** a dano de contato durante **2 s**; ativa por tecla, sem custo de energia, com **3 s** de cooldown.
- Contato durante a defesa **empurra a bactéria**: 2× a velocidade dela por 0,5 s, na direção do Nanobô para o agente. Só bactérias; a célula própria é escopo da #14.
- Enquanto a defesa está ativa o disparo é ignorado; um projétil já em voo não é cancelado. Contato fora da defesa continua seguindo a regra da #11.
- Feedback: anel em volta do Nanobô — ciano durante os 2 s, cinza durante a recarga, ausente quando a defesa está pronta.
- Implementação em `src/gameplay/` (#17), com os timers dentro de `update_match` para congelarem na pausa.

#### Controles de combate (16/10/2026)

> **Substituída em 03/11/2026** para ataque e scanner: botão esquerdo atira para o cursor e botão direito escaneia o alvo sob o cursor; `ESPAÇO` e `J` saem. `X`, `1`/`2`, `ESC` e `ENTER` continuam valendo.

- **`X` = defesa, `J` = ataque, `1` = raio roxo (Gram+), `2` = raio vermelho (Gram−), `ESPAÇO` = scanner.** O ataque usa a direção do `facing`.
- A escolha do raio é direta (tecla 1 ou 2), não por ciclo: são duas opções e o jogador precisa saber qual está selecionada — o HUD mostra `raio: roxo (Gram+)` ou `raio: vermelho (Gram-)`.
- `SHIFT` foi descartada: tocar SHIFT cinco vezes dispara o aviso de Teclas de Aderência do Windows — ruim para uma habilidade de toque repetido.
- Referências: Hades 2 (`Space` = dash), Enter the Gungeon (botão direito do mouse = esquiva), Hollow Knight (`X` = ataque, `C` = dash).
- **Validado por Gustavo em 17/10/2026** (as quatro regras de defesa e a exclusão com o ataque). A revisão cruzada com o Eric fica registrada como pendência para a entrega do marco, sem bloquear a implementação.

#### Ataque, projétil e resistência (17/10/2026)

- O laser é escolhido por tecla (`1` roxo/Gram+, `2` vermelho/Gram−) e o disparo sai na direção do `facing`, com um projétil por aperto e cooldown de **1 s** (#63, #64).
- O projétil nasce encostado no Nanobô, anda a **320 px/s**, some ao sair da área jogável e ocupa uma das **4** vagas (`MAX_PROJECTILES`).
- Acerto com o laser certo tira **1** de resistência: bactéria tem **2**, célula tem **0**; ao zerar, o agente é neutralizado e sai da cena (#49).
- Laser errado gasta o projétil e não altera a resistência; a célula saudável nunca é destruída por ataque.
- A defesa ativa bloqueia o disparo; um projétil em voo não é cancelado.

#### Célula própria (18/10/2026)

> **Atualização 03/11/2026:** a regra de custo de vida vale para as duas armas (D1); não existe arma segura contra a célula. O restante continua como registrado.

- A célula saudável anda na metade da velocidade dos agentes (`CELL_SPEED 15`) e mantém direção aleatória; ela não persegue o Nanobô.
- Atirar na célula não remove a célula nem altera resistência: tira **1** de vida do Nanobô, respeitando o 1 s de invulnerabilidade da #11.
- A célula continua identificável pelo scanner e não concede pontos; ela não é um alvo de neutralização.
- A defesa não protege do dano de um projétil já em voo contra a célula, porque o disparo é bloqueado enquanto a defesa está ativa; o projétil em voo não é cancelado.

#### Consequência de não atacar (25/10/2026)

- Ignorar uma ameaça não tem penalidade de tempo: o custo é o que o jogo já aplica; os 2,5 s são só o tempo que a mensagem fica visível.
- O contato com a bactéria tira **1** de vida, respeitando a invulnerabilidade de 1 s (#11).
- A ameaça ignorada deixa de valer os **+10** da neutralização (#66).
- A Fase 1 não conclui com ameaça ativa: o destino só libera a vitória com as **6** neutralizadas (#26).
- O feedback educacional (#178) comunica o custo no momento em que ele é aplicado, e só quando é aplicado; não existe barra de integridade do organismo nem segunda condição de derrota no MVP.

**Descartado por enquanto:** cronômetro de infecção, aceleração das bactérias e perda de pontos por tempo. Os três punem o tempo de leitura do scanner, que é o núcleo da aprendizagem, e criam um segundo estado de falha para o MVP. Se o playtest do M5 mostrar que falta pressão, a opção mais barata de ler é a **aceleração** das bactérias, nunca a perda de pontos.

### Fase 1

A Fase 1 trabalha **bactérias e células saudáveis**, incluindo a diferenciação entre bactérias Gram-positivas e Gram-negativas.

### Fase 2

A Fase 2 trabalhará **vírus**, com conteúdo relacionado aos ciclos lítico e lisogênico e uma mecânica de multiplicação limitada.

### Limite de vírus

Foi definido um limite de referência de **3 vírus** para uma mecânica de desafio. As regras exatas ainda precisam ser detalhadas e validadas.

### Pontuação e progressão

O jogo terá pontuação, bônus por identificação e progressão. O hi-score deverá ser persistente na versão final.

#### Identificação e neutralização (25/10/2026)

- Neutralizar uma ameaça antes de escaneá-la não a identifica: ela sai da cena e não volta ao alcance do scanner.
- Com isso, o bônus de identificação completa (#67) fica impossível naquela fase; a fase conclui normalmente, porque identificação não é requisito de vitória (#26).
- Não existe caminho alternativo para identificar um agente neutralizado.
- O HUD mostra as ameaças restantes e a mensagem avisa no momento em que o bônus se perde.

### Artes do Nanobô (v2)

- O sprite do Nanobô continua **32×32**; a ideia de um quadro 48×48 foi descartada — aumentar a resolução não garantia ganho visual.
- O personagem é **novo, inspirado no anterior**: mesma identidade (antenas de ponta ciano, viseira escura com dois olhos ciano, pluma de exaustão embaixo), silhueta e detalhes redesenhados.
- A tira de quadros desenha **as 4 direções desenhadas uma a uma** (cima, baixo, esquerda e direita), nas poses **frente / costas / perfil**. Nada de espelhar um lado no código.
- Grade da tira: **8 quadros por direção** (1–4 parado, 220 ms cada; 5–8 andando, 110 ms cada), 32×32 por quadro, tira final **256×128**, com as linhas na ordem do `player_facing` (`UP`, `DOWN`, `LEFT`, `RIGHT`) — assim o código acha a linha sem tabela de conversão.
- Os arquivos novos são criados **ao lado dos antigos, sem alterá-los** (`nanobo_v2.png`, `nanobo_spritesheet_v2.png` e a fonte `.aseprite`). A troca no jogo é etapa separada.

### Resolução, escala inteira e câmera (22/10/2026)

- A tela lógica é **640×360** (16:9), desenhada num bitmap e ampliada na janela por **escala inteira**, com barras pretas quando a janela não for múltipla. 640×360 é a maior resolução 16:9 que cai exata em 720p (2×), 1080p (3×), 1440p (4×) e 4K (6×).
- A janela é **redimensionável** e abre **maximizada** (`ALLEGRO_MAXIMIZED`); o canvas continua centralizado em escala inteira, então sobra barra preta nos lados quando a janela não for múltipla de 640×360. **F11** alterna fullscreen — em 1440p isso dá 4× exato, sem barras — e, ao sair, a janela volta **maximizada** (não para o tamanho pedido na criação). Escala inteira quando o encaixe der 1× ou mais; janela menor que 640×360 encolhe proporcionalmente.
- A **faixa visível do mundo** é 640×256 (múltipla de 32, o lado do tile do mapa futuro) e o **painel do scanner** ocupa os 104 px do rodapé; o painel é HUD, não faz parte do mundo.
- O **mundo** nasce com **1920×512** (3 telas por 2 faixas) e uma **câmera** presa nas bordas acompanha o Nanobô. O tamanho do mapa passou a ser constante (`WORLD_W`/`WORLD_H`), não o tamanho da tela: é o que permite crescer para paredes e caminhos sem reescrever as regras.
- A Fase 1 passa a ter **9 agentes** (3 Gram+, 3 Gram−, 3 células) para não esvaziar o mapa maior; o **alcance do scanner continua 130 px**.
- O scanner mostra **só a leitura do alvo atual**; `space` de novo relê o alvo. A lista de todos os agentes saiu: ela estourava o painel (12 linhas de 14 px = 168 px num espaço de 110 px).
- O HUD não usa coordenada fixa: cada linha sai de `hud_y()`, que soma `SCAN_PANEL_Y`, `HUD_MARGIN` e `line * HUD_LINE_H` — 5 linhas de 16 px dentro dos 104 px do painel — e os corações começam depois da largura medida do texto de status.
- Princípio de escala inteira para pixel art conforme a documentação do Godot sobre múltiplas resoluções; 1920×1080 como resolução de exibição mais comum.

### Fecho da fase e telas do MVP (26/10/2026)

- A conclusão da Fase 1 (6 ameaças neutralizadas + Nanobô no destino) leva a `RESULTS` automaticamente, uma única vez, sem tecla de teste (#70).
- `RESULTS` apresenta os números reais da partida: pontos, ameaças neutralizadas/total, agentes identificados/total e se o bônus de +15 foi pago (#57, #171–#176).
- Com uma fase no MVP, `RESULTS` continua para `VICTORY`; `VICTORY` e `GAME OVER` iniciam nova partida por `ENTER`, com reset completo de vida, pontuação, identificação, agentes, projéteis, cooldowns e timers (#52, #72, #74, #75).
- A preferência de laser (`1`/`2`) **persiste** entre partidas: é escolha do jogador, não estado da partida. O reset não a altera.
- `F`, `G` e `H` são teclas de QA e existem **somente no build Debug**; o Release não permite saltar estados (#76).

### Pontuação, bônus e composição da Fase 1 (registro de 25/10/2026)

- A Fase 1 tem **9 agentes**: 3 bactérias Gram+, 3 Gram− e 3 células saudáveis.
- São **6 ameaças** (as bactérias); as células não contam para a conclusão (#26).
- Cada ameaça neutralizada vale **+10 pontos**, uma única vez (#66).
- O bônus de **+15** é pago apenas na conclusão com `identificados = 9/9`, uma única vez (#67).
- `9/9` é condição de bônus, **não** de vitória: a fase conclui com menos identificações (#26, #67).
- Os valores são os já implementados e verificados; a calibração de dificuldade continua em aberto.

### Pausa por perda de foco (26/10/2026)

- Quando a janela perde o foco durante `GAME`, a partida entra em `PAUSE`; o retorno é por `ESC`, como na pausa normal.
- A regra existe para que alt-tab, notificação ou troca de janela não causem dano ou Game Over sem o jogador estar vendo o jogo.

### Texto do scanner e aprendizagem (26/10/2026)

> **Substituída em 03/11/2026:** o pulso 2 passa a nomear o grupo e a paleta deixa de ligar cor da bactéria à cor do módulo (ver “Ensino integrado da Fase 1”).

- A camada 2 descreve o envelope (parede espessa sem membrana externa / parede fina sob membrana externa) e **não nomeia Gram+ nem Gram−**.
- A regra que liga a pista à classificação e ao laser é ensinada uma vez na tela inicial: parede espessa = Gram+ = raio roxo; parede fina com membrana externa = Gram− = raio vermelho.
- O feedback posterior à ação pode nomear a classificação; o painel do scanner, não.
- A troca da paleta das bactérias (cor deixa de coincidir com o laser) fica para o M5, condicionada a playtest.

### Triagem de menus e HUD (28/10/2026)

- **Feedback em uma cor.** `set_feedback` recebe só o texto e todas as mensagens saem em `UI_WARM`; a consequência está na frase ("ameaça neutralizada: +10 pontos" × "parede Gram+ espessa: o raio vermelho não penetra"), não na cor. Um `feedback_kind` atravessando `gameplay.c` não paga o custo. Reabrir se o playtest do M5 mostrar jogador confundindo ganho com perda.
- **Botão com três estados.** Normal, foco e hover; não existe estado "pressionado" porque a ativação acontece no *down* e a tela troca no mesmo evento, então esse quadro nunca seria desenhado. Revisitar só se aparecer botão que não navega (um toggle de configuração).
- **`main.c` sem extração por enquanto.** O arquivo concentra loop, eventos, kit de UI, desenho das seis telas e entrada; extrair `ui.c` mexeria no `<ClCompile>`, no `AGENTS.md` e no guia de módulos por ganho organizacional, não funcional. Gatilho objetivo para reavaliar: `main.c` acima de 2.000 linhas, uma terceira tela nova, conflito de merge recorrente no arquivo ou build perceptivelmente lento.
- **Tile de fundo adiado na triagem.** O mundo era preto e a arte antiga (`fundo_microscopico.png`, 640×480) não cobria 1920×512 nem tiling. A decisão foi superada pela dupla: o commit `ca4845a` trouxe `mapa_fase1.png` (1920×512) e `load_resources()` passou a carregá-lo atrás de destino, agentes e Nanobô.
- **Fonte embutida mantida; o que faltava era a acentuação.** Os textos agora são escritos em português correto em UTF-8 e o build usa `/utf-8` (ver `docs/planejamento/guia-refatoracao-modulos.md`); trocar a fonte por TTF ou bitmap continua fora do MVP.

### Ensino integrado da Fase 1 (03/11/2026)

- **Briefing.** Antes da Fase 1, três cartões puláveis: resultado da coloração; comparação das camadas externas; armas como modelo do jogo e preservação da célula própria. ENTER, → ou clique avançam; ESC pula. Não há tutorial guiado depois, e o Diário de Campo fica fora do M4.
- **D1 — célula própria.** No jogo, **as duas armas ferem a célula própria** (−1 vida). É consequência de gameplay; não afirma nada sobre efeitos adversos de medicamentos reais. Substitui a proposta de 02/11 em que só a Dissolução feria.
- **D2 — scanner.** O pulso 1 mostra o resultado da coloração (**roxo** ou **vermelho**) e o **nome do agente** (`Staphylococcus aureus`); o pulso 2 mostra o **nome do grupo** (Gram+ / Gram−), a pista estrutural e o porquê, sem indicar a arma. O nome do agente é identificação, não classificação: não diz o grupo nem a arma. Substitui a decisão de 26/10 (camada 2 sem nome) e a leitura de 22/10 (camada 1 só com o nome).
- **Armas Ruptura e Dissolução.** A **Ruptura** age sobre a parede e neutraliza Gram+; a **Dissolução** age sobre a camada externa e neutraliza Gram−. São nomes de efeito, não de medicamentos; a matriz é regra do jogo, não previsão de tratamento real. As duas armas usam **cor neutra**, e ícone e projétil se distinguem pela **forma**, não pela cor. Substituem os rótulos A/B e a paleta roxo/vermelho da #16. No código, `Laser` e `LASER_*` continuam: renomear identificador não muda nada para o jogador.
- **Coloração: roxo e vermelho.** O resultado é **roxo** (Gram+) e **vermelho** (Gram−). "rosa" fica como sinônimo nos documentos de ensino, nunca na tela: o destaque do Gram− é `#F05A5A`, e escrever "rosa" ao lado de um vermelho recriaria a divergência que esta rodada fecha.
- **Arte neutra e anel de coloração.** Até a camada 1, as duas bactérias são um **disco cinza** desenhado por primitiva, sem forma nem cor que entreguem o grupo. Depois da leitura, o agente recebe o **sprite da espécie** (com a variante sorteada) e um **anel colorido permanente** (roxo ou vermelho). A célula própria mantém sprite próprio, maior, desde o início.
- **Variação de sprites (03/11, noite).** Cada espécie pode ter **mais de um sprite**, sorteado por agente no nascimento, para variar o cenário. Regras: o **mesmo número de variantes por grupo** (senão a variante vira pista), 32×32, a mesma hitbox (`AGENT_RADIUS`), e a variante só aparece **depois** da camada 1 — antes dela todos são o mesmo disco cinza. A quantidade fica para o M5; a regra fica decidida.
- **Espécies do cenário (03/11, noite).** Os seis binômios são **agentes do cenário**, não uma alegação de que a espécie cause doença. *Bacillus subtilis* sai (não é patogênico) e entra *Streptococcus pyogenes* (Gram+); *Escherichia coli* fica como exemplo canônico de Gram−. O briefing carrega a linha "os agentes desta missão são ameaças do cenário; nem toda bactéria causa doença".
- **Camada 1 mantém o nome do agente.** Decisão de 03/11 (noite): o nome científico é a identidade do agente e continua na camada 1, junto da coloração. O jogo pode nomear espécies; o que ele não faz é inferir patogenicidade, tratamento ou classificação clínica a partir do nome.
- **Mouse no jogo.** Botão esquerdo atira na direção do cursor; botão direito escaneia o agente sob o cursor (tolerância de 24 px, alcance de 130 px); roda do mouse e `1`/`2` trocam a arma — o clique no ícone do HUD foi **descartado** em 03/11 (noite). `ESPAÇO` e `J` deixam de ser ataque e scanner; o teclado fica com mover, defender, arma, pausa e navegação.
- **Feedback.** Cada decisão tem uma mensagem curta (até 2 linhas) que explica a pista ou a regra do jogo; o primeiro erro traz uma pergunta e o segundo explica o mecanismo, sem termos moleculares e sem recomendar tratamento.
- **Resumo e contadores.** A tela de Resultados usa apenas contadores em memória (scans por grupo, decisões certas e erradas, ataques a células próprias), zerados em `reset_match()`; sem CSV nem telemetria no M4.
- **Bônus de identificação no M5.** Com a identificação obrigatória (D2.1), o +15 deixa de pagar 9/9 — paga **run perfeito**: `wrong_decisions == 0` **e** `cell_attacks == 0`. O resumo diz por que o bônus não foi pago. A regra do build até o M5 continua 9/9.
- **Arquitetura.** Só os quatro arquivos compilados: briefing, HUD e resumo em `main.c`; regras, contadores e feedback em `gameplay.c`. Nenhum módulo `src/ui/`.
- **D2.1 — a identificação é condição de conclusão (03/11/2026).** Vencer sem escanear **não é aceitável**: identificar os agentes deixa de ser apenas bônus e passa a ser exigência para concluir a fase. Substitui, por decisão da dupla, a regra de #26/#67, que concluíam com 0 identificações. A implementação depende de **D2.2** (em Pendências) e ainda não está no código. **Entrega:** o M4 de 04/11 mantém a regra atual (#26, conclui com 0 identificações); D2.1 e D2.2 ficam para o M5.
- **O menu não entrega a matriz arma→grupo.** Com o pulso 2 nomeando o grupo (D2), o menu pode dizer a função (`1: Ruptura - age na parede celular`), mas não pode afirmar `Ruptura (Gram+) · Dissolução (Gram-)`: isso entregaria antes do primeiro scan a informação que o scanner existe para produzir. A função da arma continua no menu; a ligação com o grupo sai.
- **Fora do M4.** Diário de Campo, zonas guiadas, registro de eventos e calibração de trava/perseguição só voltam ao backlog depois do build e do QA manual. A arte neutra, o anel de coloração e os nomes das armas estão decididos; os assets entram conforme o escopo do marco (ver Pendências). Fase 2/vírus continua pós-MVP.

### Fase 2 mínima e recorde (04/11/2026)

O M4 continua centrado na Fase 1; a **Fase 2 mínima** existe para fechar o fluxo entre fases (#71/#76) e não substitui a Fase 2 do M5.

- **Composição:** 3 vírus e 3 células saudáveis, no mesmo mapa e destino da Fase 1; a tabela de fases vive em `src/entities/entities.c`.
- **Vírus:** 1 ponto de resistência; **qualquer arma** o neutraliza. O ciclo lisogênico/lítico, a multiplicação de até 3 e as consequências de combate continuam no M5 (#83–#91).
- **Scanner do vírus:** camada 1 = `vírus · Vírus da fase`; camada 2 = “Vírus precisa de uma célula hospedeira para se multiplicar” + “a célula hospedeira é o que ele usa para se multiplicar”. Sem amostra de cor e sem anel: não há coloração de Gram.
- **Contato e defesa:** o vírus fere por contato e é empurrado pela defesa, como a bactéria.
- **Vida entre fases:** cada fase começa com 3 vidas.
- **Bônus por fase:** +15 quando todos os agentes **daquela fase** foram identificados (9/9 na Fase 1, 6/6 na Fase 2); a pontuação da run atravessa as fases.
- **Resumo por fase:** os contadores zeram a cada fase; a linha de Gram só aparece na Fase 1.
- **Resultados → próxima fase:** `CONTINUAR` carrega a Fase 2 e preserva a pontuação; na última fase leva a `VICTORY`.
- **Recorde mínimo:** a melhor pontuação da run fica em `nanobo_hiscore.txt`, ao lado do executável, e aparece no menu e na Vitória. Sem nome (#81) e sem Top 10 (#77–#82), que ficam no M5; arquivo ausente ou inválido vale zero.
- **RF-07 completo (texto por objetivo) fica no M5:** o M4 entrega o resumo por contadores (#174, #202); a tela de 640×360 não comporta quatro blocos por OA sem refazer o layout.
- **Ícones das armas:** `icone_ruptura.png` (parede de tijolos) e `icone_dissolucao.png` (anéis concêntricos), 16×16, cor neutra, forma própria; o projétil continua quadrado/círculo. Os ícones roxo/vermelho saem do `load_resources()` e ficam no disco como histórico.

## Pendências

- Duração do pulso do scanner (0,75 s é valor inicial) e configuração de camadas por agente/fase no MVP.
- Conteúdo biológico detalhado e validado de cada agente.
- Relação final entre dados do scanner e escolha da arma — o pulso 2 nomeia o grupo; validar no playtest se isso deixa a decisão trivial.
- **D2.2 — destino de quem atira antes de escanear (bloqueia D2.1; alvo: M5).** `pick_target()` ignora agente inativo, então neutralizar sem identificar remove aquele agente do scanner para sempre; com a identificação obrigatória, isso **travaria a partida** no contador `x/9`. Opções: **(a)** o projétil acerta mas não reduz a resistência até o agente ser identificado, com aviso na tela *(recomendada)*; **(b)** o scanner passa a aceitar agente neutralizado, como uma lâmina corada depois do fato; **(c)** neutralizar sem scan encerra a partida, o que transforma erro de leitura em fim de jogo.
- Balanceamento de vida, dano, ataque, defesa, cooldowns e resistência (a regra de mitigação da defesa está fechada desde 16/10/2026; falta calibrar os números).
- Regras de progressão entre fases.
- Variação de sprites das bactérias (regra decidida em 03/11; quantidade no M5).
- Regras detalhadas da multiplicação limitada da Fase 2.
