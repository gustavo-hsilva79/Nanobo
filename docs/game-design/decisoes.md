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
- A identificação de um agente é registrada quando a última camada disponível dele é revelada, e não se repete dentro do mesmo encontro.
- O scanner não recomenda ação e não atribui pontuação nem bônus.

### Interface

#### Interação de mouse

- A interação de mouse da interface é o clique sobre o botão `INICIAR` na tela de `MENU`, que inicia a partida.
- A mesma transição continua disponível por `ENTER`; o clique não substitui nenhum comando de gameplay.
- Nos demais estados, nenhum clique tem ação definida nesta etapa.
- Detalhes e roteiro de verificação em `docs/planejamento/interacao-mouse.md`.

#### HUD de vida (17/10/2026)

- A vida aparece como **3 corações de 16×16** (`assets/sprites/icone_coracao.png`), alinhados em (10, 74) com 18 px de espaçamento.
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

## Pendências

- Cooldown definitivo do scanner e configuração de camadas por agente/fase no MVP.
- Conteúdo biológico detalhado e validado de cada agente.
- Relação final entre dados do scanner e escolha do laser/ação.
- Balanceamento de vida, dano, ataque, defesa, cooldowns e resistência (a regra de mitigação da defesa está fechada desde 16/10/2026; falta calibrar os números).
- Regras de progressão entre fases.
- Apresentação final das informações do scanner.
- Regras detalhadas da multiplicação limitada da Fase 2.
