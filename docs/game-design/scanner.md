# Nanobô — Regras do scanner e da identificação

> **Estado:** regras do M4 fechadas e consolidadas neste documento. O build de 04/11 mostra as duas camadas como este documento descreve: camada 1 com coloração e nome, camada 2 com grupo, estrutura e a frase de revisão; o painel também marca de onde vem a leitura (#104, #198, #199).
>
> **Última revisão:** 04/11/2026 (sincronização com o build de 04/11: textos dos pulsos, frase de revisão e marcador de origem da leitura).
>
> **Tarefas relacionadas:** #59 (seleção do alvo), #60 (pulsos e camadas), #61 (identificação única), #104 (pulsos com nome e estrutura), #198 (scan sob o cursor).

As regras de seleção do alvo já estavam registradas em `docs/game-design/decisoes.md` (#59). Este documento acrescenta as regras de pulsos/camadas (#60) e de identificação única (#61), para que as três tarefas descrevam o mesmo fluxo, sem sobreposição de responsabilidade.

## 1. Fluxo completo

```text
explorar o cenário
        ↓
botão direito sobre o alvo → nenhum alvo válido sob o cursor encerra aqui, sem efeito
        ↓
selecionar o alvo          → agente válido sob o cursor, dentro do alcance (#59, #198)
        ↓
pulso 1                    → coloração (roxo ou vermelho) e nome do agente
        ↓
pulso 2                    → nome do grupo, pista estrutural e o porquê (#104)
        ↓
registrar a identificação  → quando a última camada do agente é revelada (#61)
        ↓
o jogador interpreta a informação
        ↓
o jogador escolhe a arma e atira
        ↓
consequência
```

O scanner entrega **dados**. A interpretação e a escolha da ação continuam sendo do jogador.

## 2. Ativação

- A ativação é o **botão direito do mouse** sobre o alvo; o teclado não tem tecla de scanner no M4 (#198).
- O scanner funciona em pulsos: cada ativação válida dispara os dois pulsos, em sequência, e produz no máximo uma identificação.
- A leitura tem **2 pulsos**; a duração de 0,75 s por pulso é valor inicial (proposta de 02/11, ainda não calibrada).
- Durante os pulsos o Nanobô fica **parado e sem atirar**; dano de contato cancela o pulso.
- A ativação só existe no estado `GAME`.
- Sem candidato sob o cursor e dentro do alcance, a ativação não inicia leitura, não revela camada e não identifica agente.
- O intervalo entre leituras continua **[PENDÊNCIA]**: o pulso tem tempo, mas não há recarga própria implementada.

## 3. Seleção do alvo (#59)

Regra já fechada, repetida aqui para que o fluxo fique em um único lugar:

- São candidatos os agentes ativos e escaneáveis sob o cursor (tolerância de **24 px**) e dentro do alcance de **130 px** do Nanobô (#198).
- A ativação seleciona no máximo um candidato: o mais próximo do cursor.
- Sem candidato sob o cursor, a ativação não inicia uma leitura e não identifica nenhum agente.
- Em empate exato, vence o agente com o menor identificador estável e único no encontro.

## 4. Pulsos e camadas (#60)

- Cada ativação válida percorre as **2 camadas** do alvo selecionado, em sequência: pulso 1 (coloração) e pulso 2 (grupo e estrutura).
- Existem **2 camadas** por agente, verificáveis.
- Informação já revelada **permanece visível** durante o encontro; não é apagada ao trocar de alvo.
  > **Atualização 22/10/2026:** o painel passou a mostrar só a leitura do alvo atual; as camadas já reveladas continuam guardadas em cada agente e voltam a aparecer quando ele é lido de novo.
- Depois que as duas camadas do agente foram reveladas, novas ativações sobre ele não revelam informação inexistente e não alteram o estado.
- O conteúdo revelado descreve o agente e **não recomenda** qual arma o jogador deve usar.

### Conteúdo das camadas no M4

O texto da segunda camada é a informação curta de scanner aprovada em `docs/game-design/conteudo-biologico.md`. A primeira camada é o resultado da coloração **e o nome do agente**; a segunda nomeia o grupo e explica a estrutura (D2, 03/11). A coloração aparece de duas formas que se reforçam: uma **amostra de cor** antes do nome, no painel, e um **anel colorido permanente** no agente, no mundo.

| Camada | Agente | Conteúdo revelado |
|---:|---|---|
| 1 | Bactéria (Gram+ ou Gram−) | coloração e nome do agente: “roxo · Staphylococcus aureus” (Gram+) ou “vermelho · Escherichia coli” (Gram−) |
| 2 | Bactéria Gram-positiva | “Gram+ · parede espessa, rica em peptidoglicano” e a frase de revisão “Essa parede mais espessa distingue este grupo.” |
| 2 | Bactéria Gram-negativa | “Gram- · parede fina sob membrana externa” e “A membrana externa adicional distingue este grupo.” |
| 1 | Célula saudável própria | “célula do organismo · Célula epitelial”, sem amostra de coloração |
| 2 | Célula saudável própria | “célula saudável do próprio organismo: preserve-a” e “preserve a célula: as duas armas ferem o próprio organismo” |

O “Gram-” usa hífen ASCII (U+002D): a fonte embutida não tem o menos tipográfico (U+2212), que sai como `^` na tela.

Na linha de estado, o painel mostra **de onde vem a leitura**: ` · alvo sob o cursor`, em ciano, quando existe um agente escaneável sob o cursor; ` · última leitura`, apagado, quando o painel está exibindo o último alvo lido. Com agente sob o cursor, o painel segue esse agente mesmo antes do primeiro clique — é o que permite conferir a coloração e a estrutura sem trocar de alvo.

Nenhuma camada diz “use a Ruptura” ou “não atire”. A ligação entre a pista e a ação é do jogador.

A camada 2 **nomeia a classificação** desde 03/11: sem o nome, o jogo não ensinava o termo que o professor cobrou. O scanner continua sem recomendar arma; a decisão é do jogador.

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
| OA1 — interpretar a coloração | camada 1 com roxo ou vermelho e o nome do agente | reconhecer o grupo sem inferir doença ou tratamento a partir do nome | leitura do resultado e identificação do grupo |
| OA2 — comparar as camadas externas | camada 2 com o nome do grupo, a parede e a camada externa | explicar a diferença em palavras próprias | descrição do jogador e reconhecimento do peptidoglicano na parede |
| OA3 — relacionar estrutura e arma | camada 2 com a pista estrutural, sem indicar a arma | escolher Ruptura ou Dissolução segundo a regra do jogo | arma escolhida e resultado do ataque |
| OA4 — preservar a célula própria | camadas 1 e 2 indicando célula do próprio organismo | não atacar a célula | ação escolhida e integridade da célula |

## 8. O que o scanner não faz

- Não recomenda ação, arma ou alvo a atacar.
- Não pontua e não dá bônus.
- Não escolhe o alvo para o jogador além da seleção automática do mais próximo.
- Não mantém informação entre encontros.

## 9. [PENDÊNCIA]

- Duração do pulso (0,75 s é valor inicial) e intervalo entre leituras.
- Quantidade e conteúdo das camadas por agente/fase nas fases seguintes (a Fase 1 está decidida: 2 camadas, com os textos acima).
- Pistas, decisão associada e consequências da Fase 2, que dependem de #83/#87/#88.
- Apresentação das camadas (amostra de cor no painel e anel no agente): implementada em 04/11 (#199).
- Espécies bacterianas finais representadas no jogo.

## 10. Implementação atual

| Regra | Onde |
|---|---|
| Seleção do alvo sob o cursor (24 px) e dentro do alcance (130 px) | `pick_target_at()`, chamado por `scan_at()` e pelo desenho do painel, em `src/gameplay/gameplay.c` |
| Pulsos e camadas | `reveal_layer()`, por `scan_at()`; desenho em `draw_scan_panel()`, `src/main.c` |
| Texto das camadas | `layer_one_text()`, `layer_two_text()` e `layer_two_reason()`, em `src/gameplay/gameplay.c` |
| Amostra de cor e anel | `draw_scan_swatch()` em `src/main.c` no painel; anel desenhado no agente depois da camada 1 |
| Marcador de origem da leitura | `draw_scan_panel()` em `src/main.c`: ` · alvo sob o cursor` / ` · última leitura` |
| Identificação única | campos `identified` e `identified_count` em `src/entities/entities.c` |
| Reinicialização da partida | `reset_match()` em `src/gameplay/gameplay.c` |
| Agentes fora do scanner | agente com `active == false` não é candidato nem é desenhado |

A tabela descreve o código de 04/11: a ativação é o **botão direito** sobre o agente sob o cursor (#198), o pulso 1 mostra a coloração e o nome e o pulso 2 mostra o grupo, a estrutura e a frase de revisão (#104). `use_scanner()`, a leitura pelo alvo mais próximo a partir do Nanobô, continua em `gameplay.c`, mas o `main.c` não a chama: o caminho do jogo é `scan_at()`. A posição usada na medição de distância continua sendo a do Nanobô (`Nanobo player`, `src/entities/entities.c`), alterada pelo teclado em `update_match()`. O encontro da Fase 1 tem **9 agentes** (3 Gram+, 3 Gram−, 3 células), embora a regra de camadas continue com 2 por agente. O roteiro de verificação está em `docs/planejamento/qa-mvp.md` §10.

As regras de conteúdo biológico continuam em `docs/game-design/conteudo-biologico.md`.
