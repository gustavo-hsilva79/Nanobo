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
O recurso que o jogador escolhe conforme a pista do scanner. Cada fase tem as suas: na Fase 1, **Ruptura** (parede, Gram+), **Dissolução** (membrana externa, Gram−) e **Reserva** (bactéria resistente); na Fase 2, **Neutralização** (vírus livre) e **Ataque T** (célula infectada); na Fase 3, **Perfuração** (fungo) e **Ruptura**. A escolha é regra do jogo, inspirada em remédios e defesas reais, não recomendação de tratamento.
_Evitar_: ferramenta, laser, raio, tiro, módulo A/B

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

## Narrativa e ensino

**Diário do Nanobô**:
O conjunto de anotações que o próprio Nanobô escreve, em primeira pessoa, sobre o que descobre ao escanear e agir. É a forma que o card de conceito assume no jogo.
_Evitar_: card de conceito (para o que o jogador vê), tutorial, aula

**Anotação**:
Uma entrada do Diário do Nanobô, aberta por um evento do jogo e lida em páginas que avançam pela mesma tecla.
_Evitar_: card, popup, mensagem

**Paciente**:
A pessoa de 16 anos dentro de quem o Nanobô trabalha; a história dela liga as três fases.
_Evitar_: hospedeiro, alvo

**Interlúdio**:
O trecho entre duas fases que mostra o estado do Paciente e o que acontece com ele no hospital.
_Evitar_: transição, loading

## Interface

**Integridade**:
A vida do Nanobô dentro do corpo. Cai quando ele sofre dano e volta cheia a cada fase; se acaba, a equipe retira o Nanobô.
_Evitar_: vida do jogador, HP, energia

**Vitais**:
O estado do Paciente durante a missão. Caem quando uma célula dele é perdida ou atingida por engano; se acabam, a equipe retira o Nanobô e o Paciente segue o tratamento convencional.
_Evitar_: vida do paciente, saúde

**Mira**:
O ponto para onde o Nanobô dispara a arma escolhida; mostra se a arma está pronta, recarregando ou travada.
_Evitar_: cursor, retícula

**Como jogar**:
A tela que explica, em páginas, como mover, ler com o scanner, usar as armas e acompanhar Integridade e Vitais. Abre do menu e da pausa.
_Evitar_: tutorial, ajuda, manual

**Aviso de objetivo**:
A frase curta que aparece por alguns segundos quando a fase começa ou quando a meta dela muda.
_Evitar_: missão, quest, objetivo do HUD
