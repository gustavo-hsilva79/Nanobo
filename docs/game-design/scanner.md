# Nanobô — Regras do scanner e da identificação

> **Estado:** regras fechadas pela dupla e consolidadas neste documento, que encerra o fluxo **Scanner → Informação → Interpretação → Decisão → Consequência** previsto na #3.
>
> **Última revisão:** 07/10/2026.
>
> **Tarefas relacionadas:** #59 (seleção do alvo), #60 (pulsos e camadas), #61 (identificação única).

As regras de seleção do alvo já estavam registradas em `docs/game-design/decisoes.md` (#59). Este documento acrescenta as regras de pulsos/camadas (#60) e de identificação única (#61), para que as três tarefas descrevam o mesmo fluxo, sem sobreposição de responsabilidade.

## 1. Fluxo completo

```text
explorar o cenário
        ↓
ativar o scanner          → nenhum alvo válido no alcance encerra aqui, sem efeito
        ↓
selecionar o alvo         → agente válido mais próximo (#59)
        ↓
revelar a próxima camada  → somente a próxima ainda não descoberta (#60)
        ↓
registrar a identificação → quando a última camada do agente é revelada (#61)
        ↓
o jogador interpreta a informação
        ↓
o jogador escolhe a ação / raio
        ↓
consequência
```

O scanner entrega **dados**. A interpretação e a escolha da ação continuam sendo do jogador.

## 2. Ativação

- O scanner funciona em pulsos: cada ativação é independente e produz no máximo um efeito.
- A ativação só existe no estado `GAME`.
- Sem candidato dentro do alcance, a ativação não inicia leitura, não revela camada e não identifica agente.
- O cooldown definitivo permanece **[PENDÊNCIA]**. A estimativa registrada na #3 é de 1–2 s, a ser validada no PoC; nesta revisão não há controle de cooldown implementado.

## 3. Seleção do alvo (#59)

Regra já fechada, repetida aqui para que o fluxo fique em um único lugar:

- São candidatos os agentes ativos e escaneáveis cuja distância ao Nanobô seja menor ou igual ao alcance do scanner.
- A ativação seleciona no máximo um candidato: o de menor distância.
- A comparação pode usar distância ao quadrado, sem alterar a regra geométrica.
- Sem candidatos no alcance, a ativação não inicia uma leitura e não identifica nenhum agente.
- Em empate exato, vence o agente com o menor identificador estável e único no encontro.

## 4. Pulsos e camadas (#60)

- Cada ativação válida revela **somente a próxima camada ainda não descoberta** do alvo selecionado.
- No contexto da PoC existem **2 camadas** por agente, verificáveis.
- Informação já revelada **permanece visível** durante o encontro; não é apagada ao trocar de alvo.
- Depois que todas as camadas do agente foram reveladas, novas ativações sobre ele não revelam informação inexistente e não alteram o estado.
- O conteúdo revelado descreve o agente e **não recomenda** qual ação ou raio o jogador deve escolher.

### Conteúdo das camadas na PoC

O texto da segunda camada é a informação curta de scanner já aprovada em `docs/game-design/conteudo-biologico.md`. A primeira camada identifica a classe do agente.

| Camada | Agente | Conteúdo revelado |
|---:|---|---|
| 1 | Bactéria (Gram+ ou Gram−) | “bactéria detectada” |
| 2 | Bactéria Gram-positiva | “Gram+: parede celular espessa, rica em peptidoglicano.” |
| 2 | Bactéria Gram-negativa | “Gram−: parede fina de peptidoglicano e membrana externa.” |
| 1 | Célula saudável própria | “célula do organismo detectada” |
| 2 | Célula saudável própria | “célula saudável do próprio organismo: preserve-a.” |

Nenhuma camada diz “use o raio roxo” ou “não atire”. A ligação entre a pista e a ação é do jogador, conforme a regra de combate da #16.

**No MVP**, a estrutura deve permitir variar a quantidade e o conteúdo das camadas por agente ou por fase. A configuração final por agente/fase permanece **[PENDÊNCIA]** e não deve ser antecipada antes da revisão das regras da Fase 2 (#83/#87/#88).

## 5. Identificação única por agente (#61)

- O primeiro scan válido que **completar** a identificação de um agente registra essa identificação.
- Completar a identificação significa ter revelado todas as camadas disponíveis daquele agente no encontro.
- Repetições do mesmo agente não criam nova identificação e **não incrementam** o contador de agentes identificados.
- A regra vale igualmente para **ameaças e células saudáveis**.
- Cada agente possui um identificador estável e único no encontro, usado também como critério de desempate na seleção do alvo.
- Esta regra **não atribui pontuação nem bônus**. Ela apenas registra a identificação que as regras de pontuação vão consumir depois (#65–#67).

## 6. Reinicialização

O estado de identificação — identificações registradas, contador e camadas reveladas — pertence ao **encontro/partida**. Ao iniciar uma nova partida, tudo isso é reinicializado. Nenhum dado de identificação é persistido entre execuções.

## 7. Relação com os objetivos de aprendizagem (#177)

| Objetivo | Informação do scanner | Decisão do jogador | Evidência observável |
|---|---|---|---|
| Classificar bactérias pela pista do scanner | camada 2 com a descrição do envelope celular (Gram+ / Gram−) | escolher o raio correspondente | raio escolhido e resultado do ataque |
| Preservar a célula própria saudável | camada 1 e camada 2 indicando célula do próprio organismo | não atacar | ação escolhida e integridade da célula |

## 8. O que o scanner não faz

- Não recomenda ação, raio ou alvo a atacar.
- Não pontua e não dá bônus.
- Não escolhe o alvo para o jogador além da seleção automática do mais próximo.
- Não mantém informação entre encontros.

## 9. [PENDÊNCIA]

- Cooldown definitivo do scanner.
- Quantidade e conteúdo das camadas por agente/fase no MVP.
- Pistas, decisão associada e consequências da Fase 2, que dependem de #83/#87/#88.
- Apresentação final das informações do scanner na interface (#48).
- Espécies bacterianas finais representadas no jogo.

## 10. Implementação atual

| Regra | Onde |
|---|---|
| Seleção do alvo mais próximo | `pick_target()`, chamado por `use_scanner()` com a posição do Nanobô (`player_x`, `player_y`), em `src/main.c` |
| Pulsos e camadas | `use_scanner()` e `draw_scan_info()` em `src/main.c` |
| Texto das camadas | `layer_one_text()` e `layer_two_text()` em `src/main.c` |
| Identificação única | campo `identified` e `identified_count` em `src/main.c` |
| Reinicialização da partida | `reset_match()` em `src/main.c` |

A posição usada na medição de distância é a do Nanobô, alterada pelo teclado desde a #9 (`update_match()` em `src/main.c`). O roteiro de verificação está em `docs/planejamento/qa-poc.md`.

As regras de conteúdo biológico continuam em `docs/game-design/conteudo-biologico.md`.
