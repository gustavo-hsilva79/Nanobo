# Nanobô — Decisões de Game Design

Este arquivo registra decisões já tomadas pela dupla. Quando uma decisão importante for fechada, ela deve ser registrada aqui; hipóteses e itens ainda não validados permanecem como pendências.

## Decisões atuais

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

### Combate

#### Defesa (16/10/2026)

- A defesa é **imunidade total** a dano de contato durante **2 s**; ativa por tecla, sem custo de energia, com **3 s** de cooldown.
- Contato durante a defesa **empurra a bactéria**: 2× a velocidade dela por 0,5 s, na direção do Nanobô para o agente. Só bactérias; a célula própria é escopo da #14.
- Enquanto a defesa está ativa o disparo é ignorado; um projétil já em voo não é cancelado. Contato fora da defesa continua seguindo a regra da #11.
- Feedback: anel em volta do Nanobô — ciano durante os 2 s, cinza durante a recarga, ausente quando a defesa está pronta.
- Implementação em `src/gameplay/` (#17), com os timers dentro de `update_match` para congelarem na pausa.

#### Controles de combate (16/10/2026)

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

### Artes do Nanobô (v2)

- O sprite do Nanobô continua **32×32**; a ideia de um quadro 48×48 foi descartada — aumentar a resolução não garantia ganho visual.
- O personagem é **novo, inspirado no anterior**: mesma identidade (antenas de ponta ciano, viseira escura com dois olhos ciano, pluma de exaustão embaixo), silhueta e detalhes redesenhados.
- A tira de quadros desenha **as 4 direções desenhadas uma a uma** (cima, baixo, esquerda e direita), nas poses **frente / costas / perfil**. Nada de espelhar um lado no código.
- Grade da tira: **8 quadros por direção** (1–4 parado, 220 ms cada; 5–8 andando, 110 ms cada), 32×32 por quadro, tira final **256×128**, com as linhas na ordem do `player_facing` (`UP`, `DOWN`, `LEFT`, `RIGHT`) — assim o código acha a linha sem tabela de conversão.
- Os arquivos novos são criados **ao lado dos antigos, sem alterá-los** (`nanobo_v2.png`, `nanobo_spritesheet_v2.png` e a fonte `.aseprite`). A troca no jogo é etapa separada.

### Resolução, escala inteira e câmera (22/10/2026)

- A tela lógica é **640×360** (16:9), desenhada num bitmap e ampliada na janela por **escala inteira**, com barras pretas quando a janela não for múltipla. 640×360 é a maior resolução 16:9 que cai exata em 720p (2×), 1080p (3×), 1440p (4×) e 4K (6×).
- A janela é **redimensionável** e abre **maximizada** (`ALLEGRO_MAXIMIZED`); o canvas continua centralizado em escala inteira, então sobra barra preta nos lados quando a janela não for múltipla de 640×360. **F11** alterna fullscreen — em 1440p isso dá 4× exato, sem barras. Escala inteira quando o encaixe der 1× ou mais; janela menor que 640×360 encolhe proporcionalmente.
- A **faixa visível do mundo** é 640×256 (múltipla de 32, o lado do tile do mapa futuro) e o **painel do scanner** ocupa os 104 px do rodapé; o painel é HUD, não faz parte do mundo.
- O **mundo** nasce com **1920×512** (3 telas por 2 faixas) e uma **câmera** presa nas bordas acompanha o Nanobô. O tamanho do mapa passou a ser constante (`WORLD_W`/`WORLD_H`), não o tamanho da tela: é o que permite crescer para paredes e caminhos sem reescrever as regras.
- A Fase 1 passa a ter **9 agentes** (3 Gram+, 3 Gram−, 3 células) para não esvaziar o mapa maior; o **alcance do scanner continua 130 px**.
- O scanner mostra **só a leitura do alvo atual**; `space` de novo relê o alvo. A lista de todos os agentes saiu: ela estourava o painel (12 linhas de 14 px = 168 px num espaço de 110 px).
- O HUD não usa coordenada fixa: cada linha sai de `hud_y()`, que soma `SCAN_PANEL_Y`, `HUD_MARGIN` e `line * HUD_LINE_H` — 5 linhas de 16 px dentro dos 104 px do painel — e os corações começam depois da largura medida do texto de status.
- Princípio de escala inteira para pixel art conforme a documentação do Godot sobre múltiplas resoluções; 1920×1080 como resolução de exibição mais comum.

## Pendências

- Cooldown definitivo do scanner e configuração de camadas por agente/fase no MVP.
- Conteúdo biológico detalhado e validado de cada agente.
- Relação final entre dados do scanner e escolha do laser/ação.
- Balanceamento de vida, dano, ataque, defesa, cooldowns e resistência (a regra de mitigação da defesa está fechada desde 16/10/2026; falta calibrar os números).
- Regras de progressão entre fases.
- Apresentação final das informações do scanner.
- Regras detalhadas da multiplicação limitada da Fase 2.
