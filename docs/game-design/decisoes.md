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
- A relação entre a informação obtida e a escolha do raio/ação deve ser validada durante os testes.

#### Seleção do alvo — regra para implementação

- São candidatos os agentes ativos e escaneáveis cuja distância ao Nanobô seja menor ou igual ao alcance do scanner.
- A ativação seleciona no máximo um candidato: aquele com a menor distância ao Nanobô.
- A comparação pode usar distância ao quadrado, sem alterar a regra geométrica.
- Sem candidatos no alcance, a ativação não inicia uma leitura e não identifica nenhum agente.
- Em empate exato, vence o agente com o menor identificador estável e único no encontro. A geração desses identificadores será definida junto ao modelo de entidades.
- A regra seleciona o alvo; revelação de camadas e registro de identificação continuam sendo responsabilidades separadas.

### Fase 1

A Fase 1 trabalha **bactérias e células saudáveis**, incluindo a diferenciação entre bactérias Gram-positivas e Gram-negativas.

### Fase 2

A Fase 2 trabalhará **vírus**, com conteúdo relacionado aos ciclos lítico e lisogênico e uma mecânica de multiplicação limitada.

### Limite de vírus

Foi definido um limite de referência de **3 vírus** para uma mecânica de desafio. As regras exatas ainda precisam ser detalhadas e validadas.

### Pontuação e progressão

O jogo terá pontuação, bônus por identificação e progressão. O hi-score deverá ser persistente na versão final.

## Pendências

- Regras finais de identificação e recomendação.
- Conteúdo biológico detalhado e validado de cada agente.
- Relação final entre dados do scanner e escolha do raio/ação.
- Balanceamento de vida, dano, ataque, defesa, cooldowns e resistência.
- Regras de progressão entre fases.
- Apresentação final das informações do scanner.
- Regras detalhadas da multiplicação limitada da Fase 2.
