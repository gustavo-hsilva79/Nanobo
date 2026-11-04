# Nanobô — Glossário

Vocabulário do jogo. Serve para que texto, documentação e código chamem cada coisa pelo mesmo nome.

## Mundo

**Nanobô**:
O personagem controlado pelo jogador.
_Evitar_: robô, nanorrobô (fora do nome do jogo)

**Jogador**:
A pessoa que joga; quem controla o Nanobô.
_Evitar_: usuário

**Agente**:
Cada ser do cenário que o Nanobô encontra, escaneia e decide sobre. São bactérias e células saudáveis.
_Evitar_: inimigo, NPC, micróbio (a célula saudável não é um micróbio hostil)

**Espécie**:
A categoria do agente no jogo: Gram-positiva, Gram-negativa ou célula saudável própria.
_Evitar_: tipo, variante, nome científico

**Nome do agente**:
O nome exibido na camada 1 do scanner (ex.: `Staphylococcus aureus`). Identifica o agente do cenário; não informa o grupo Gram, não indica doença nem tratamento.
_Evitar_: espécie (para o grupo do jogo)

**Fase**:
A unidade de conteúdo do jogo; a Fase 1 tem 3 bactérias Gram+, 3 Gram− e 3 células saudáveis.
_Evitar_: encontro, nível

**Partida**:
Uma execução do jogo do começo ao fim, com seus próprios tempo e pontuação.
_Evitar_: encontro, sessão

**Ponto válido**:
Posição do cenário onde um agente pode nascer. As regras estão em `docs/game-design/gdd.md`.
_Evitar_: área de spawn, slot

**Hitbox**:
A área invisível de contato de um corpo do cenário, usada para decidir se dois corpos se encostam.
_Evitar_: raio de colisão

## Scanner

**Coloração (resultado de Gram)**:
O resultado do exame no scanner: **roxo** (Gram+) ou **vermelho** (Gram−). "Rosa" é sinônimo de vermelho nos documentos de ensino, nunca na tela.
_Evitar_: cor da bactéria, rosa (na tela)

**Pulso**:
Um acionamento do scanner que revela exatamente a próxima camada não descoberta do alvo.
_Evitar_: scan, leitura (a leitura é o resultado)

**Camada**:
Cada nível de informação de um agente revelado pelo scanner; no PoC são 2.
_Evitar_: nível de detalhe

**Alvo**:
O agente escolhido em um pulso: o candidato mais próximo dentro do alcance do scanner.
_Evitar_: agente válido, alvo atual

**Identificação**:
O registro, uma única vez por partida, de que um agente teve todas as camadas reveladas.
_Evitar_: descoberta, conclusão

## Combate

**Arma**:
O recurso que o jogador escolhe conforme a pista do scanner. São duas: **Ruptura**, que age na parede e neutraliza Gram+, e **Dissolução**, que age na camada externa e neutraliza Gram−. A escolha é regra do jogo, não recomendação de tratamento.
_Evitar_: laser, raio, tiro, módulo A/B

**Projétil**:
O corpo que viaja depois do disparo. Existe um por arma, e os dois se distinguem pela forma, não pela cor.
_Evitar_: tiro, laser

**Resistência**:
Quantos acertos válidos um agente suporta antes de ser neutralizado: 2 para bactéria, 0 para célula.
_Evitar_: vida do agente, HP

**Defesa**:
A janela de 2 s em que o Nanobô não leva dano de contato e empurra bactérias; depois dela, 3 s de recarga.
_Evitar_: escudo, bloqueio

**Invulnerabilidade**:
O 1 s após levar dano em que um novo contato não tira vida.
_Evitar_: i-frames, imunidade (a imunidade é da defesa)

## Arte e animação

**Pluma**:
A exaustão ciano que sai por baixo do Nanobô; curta quando parado, longa quando andando.
_Evitar_: cauda, chama, rastro

**Tira de quadros**:
O arquivo de imagem com todos os quadros de uma animação em sequência.
_Evitar_: spritesheet, atlas, folha

**Quadro**:
Um recorte da tira, ou seja, um instante da animação.
_Evitar_: frame

**Pose**:
A orientação desenhada do Nanobô — frente, costas ou perfil.
_Evitar_: direção (essa palavra é do movimento)
