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
O tipo biológico de um agente: Gram-positiva, Gram-negativa ou célula saudável própria.
_Evitar_: tipo, variante

**Fase**:
A unidade de conteúdo do jogo; a Fase 1 tem 3 bactérias e 3 células saudáveis.
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

**Laser**:
A arma que o jogador escolhe conforme a pista do scanner; há um para Gram-positiva (roxo) e um para Gram-negativa (vermelho).
_Evitar_: raio, tiro, projétil (o projétil é o corpo que viaja)

> O HUD ainda escreve `raio: roxo (Gram+)` / `raio: vermelho (Gram-)`. A troca dessa string para `laser` é tarefa de código separada; os documentos usam **laser**.

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
