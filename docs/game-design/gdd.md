# Nanobô — GDD

> **Nanobô 2.0 (13/11/2026).** A seção abaixo é a visão vigente do jogo, decidida pelo dono em 13/11 depois da auditoria (`docs/auditoria/auditoria-2026-11-13.md`). As seções seguintes descrevem o MVP do M4 e valem só onde não conflitarem com ela. O detalhe pedagógico está em [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md) (§"Nanobô 2.0"), o biológico em [`conteudo-biologico.md`](conteudo-biologico.md) e as decisões em [`decisoes.md`](decisoes.md) (§"Nanobô 2.0").

## Nanobô 2.0 — visão vigente (13/11/2026)

### Por que mudou

Ao jogar o build de 12/11, o dono avaliou que o jogo estava:

- **fácil:** nada pressionava o jogador;
- **pouco intuitivo:** as bactérias tinham a cor da parede do vaso;
- **raso no ensino:** quase só "Gram+ roxo, Gram− vermelho";
- **sem narrativa;**
- **sem coerência na arte.**

O 2.0 responde aos critérios da disciplina (Plano de Ensino e Aulas 1 e 2):

- tema do ensino médio;
- aprendizado vindo das mecânicas, e não de quiz;
- "um jogo é uma série de escolhas interessantes";
- estética que comunica a mecânica.

### Narrativa

Premissa curta, contada em cards de até 3 linhas.

- **Abertura (4 cards):**
  1. Um paciente de 16 anos chega ao hospital com febre alta e uma infecção que se espalha pelo sangue.
  2. A equipe testa o **Nanobô**, um nanorrobô médico menor que uma célula.
  3. Ele ajuda o corpo sem ferir as células do paciente, e vai atrás só dos invasores (a maioria das bactérias do corpo é inofensiva).
  4. A missão é manter os sinais vitais estáveis.
- **Interlúdios** entre as fases mostram os sinais vitais ("estável" ≥ 70, "em observação" 40–69, "grave" < 40) e anunciam o próximo invasor.
- **Final:** o paciente recebe alta, com o relatório do que o Nanobô viu.
- **Game over:** os sinais vitais caem demais e a equipe retira o Nanobô.
- O paciente não tem nome nem gênero no texto.

### O Nanobô é o único defensor

Não há aliados: o macrófago e o linfócito T planejados foram retirados em 13/11. O que o corpo faria aparece nas ferramentas do Nanobô e na explicação dos cards.

### Fases

| Fase | Local | Invasor | Ferramentas | Vitória |
|---|---|---|---|---|
| 1 | corrente sanguínea (mapa atual) | 6 bactérias (cocos e bacilos, Gram+ e Gram−) + 3 células do paciente | **Ruptura** (antibiótico de parede), **Dissolução** (antibiótico de membrana), **Reserva** (antibiótico de reserva, 2 cargas) | zero bactérias e Nanobô no destino |
| 2 | vias aéreas (brônquio, mapa novo) | vírus respiratório: 3 vírus lisogênicos dentro de células + 2 células livres | **Neutralização** (anticorpos, vírus livre), **Lise** (destrói célula infectada já lida, papel do linfócito T) | zero vírus e Nanobô no destino |
| 3 | pulmão (alvéolos, mapa novo) | fungos: 4 leveduras (*Cryptococcus*) + 2 colônias de bolor (*Aspergillus*) + 3 células, mais esporos conforme o estado do paciente | **Perfuração** (antifúngico, abre poros na membrana com ergosterol) | zero fungos e Nanobô no destino |

Cada fase tem mapa, ponto de partida e destino próprios. O mapa da Fase 1 continua o atual, mas é desenhado escurecido e dessaturado com `al_draw_tinted_bitmap` para os agentes se destacarem; o PNG não muda. Os mapas das Fases 2 e 3 (1920×512) são desenhados por Claude e podem ser melhorados pelo dono com IA de geração de imagem.

### Regras centrais

- **Scanner em duas camadas:**
  - a 1ª dá a **forma** (coco ou bacilo; levedura ou filamento);
  - a 2ª dá a **coloração e a estrutura**.
  - Antes da leitura, o agente é uma silhueta azul-acinzentada neutra. O sprite da forma é o mesmo para Gram+ e Gram−, e a cor do Gram vem do tingimento depois da 2ª camada.
- **Tratamento:**
  - cada bactéria precisa de **2 doses** do antibiótico certo;
  - uma dose e mais de 6 s sem a segunda deixam a bactéria **resistente àquele antibiótico**, e daí só a **Reserva** a elimina;
  - o antibiótico errado não faz efeito, e não cria resistência; ele trava o disparo por 2 s, o custo do chute (#215).
- **Divisão e crescimento:**
  - as bactérias se dividem a cada cerca de 12 s (12 ± 3 s, calibrado na #238), com teto de 16;
  - as leveduras brotam a cada cerca de 20 s, com teto de 12;
  - a hifa do bolor cresce 1 segmento a cada cerca de 5 s, até 5 segmentos. Acertar a **base** mata a colônia, e acertar a ponta só corta um segmento. Com a hifa no tamanho máximo, a colônia solta esporos e nasce outra, até 4 colônias.
- **Vírus (Fase 2):**
  - o lisogênico fica dentro da célula, piscando, por 12 s;
  - depois do eclipse, a célula se rompe e solta 6 vírus numa **rajada radial**; eles ferem o Nanobô e depois derivam no muco como vírus **livres**, que infectam células livres ao tocá-las (14/11);
  - a **Lise** elimina a célula infectada já lida: custa poucos vitais, mas impede a eclosão;
  - o antibiótico não age em vírus;
  - teto de 12 vírus.
- **Ligação com a Fase 3:** a carga inicial de esporos sobe quanto menores forem os vitais ao fim da Fase 2 e quanto mais cargas de Reserva tiverem sido gastas. Fungos oportunistas aproveitam um organismo enfraquecido.
- **Duas barras (14/11):**
  - **Integridade do Nanobô:** de 0 a 100, cheia no início de cada fase. Cai com contato (10), toxina (8), esporo (8), vírus da rajada (6), hifa (10) e nuvem de endotoxina (10/s).
  - **Vitais do paciente:** de 0 a 100. Caem com célula do paciente destruída (−20), tiro do Nanobô numa célula (−15) e Lise (−5). Passam de uma fase para a outra, e o interlúdio recupera até no mínimo 60.
  - Derrota quando qualquer uma zera; a tela de Game Over diz qual.
  - Não há drenagem por tempo: a pressão vem dos invasores (§"Dificuldade").
- **Pontuação:**
  - +10 por invasor eliminado;
  - −5 por resistência criada;
  - +15 de bônus de identificação completa (agentes iniciais);
  - bônus de vitais no fim de cada fase (vitais × 0,5).
  - **Top 10** com nome de até 10 caracteres, gravado só em vitória (#34/#77–#82/#197).

### Dificuldade: a IA dos invasores (14/11)

A dificuldade vem do que cada invasor faz, e o comportamento ensina a biologia dele. Cada agente tem três estados:
- **vagar**;
- **alerta:** o Nanobô a 200 px, ou o agente levou tiro;
- **atacar.**

O agente volta a vagar se o Nanobô ficar a mais de 320 px por 3 s. Todo ataque tem 0,4 s de aviso (pisca ou encolhe).

| Invasor | Biologia que o comportamento mostra | Comportamento |
|---|---|---|
| Bacilo (*E. coli*, *P. aeruginosa*, *B. cereus*) | tem flagelo e nada | persegue a 70 px/s; investida de 0,4 s a 150 px/s a cada 2,5 s |
| Coco (*S. aureus*, *S. pyogenes*, *N. meningitidis*) | sem flagelo; coloniza tecido e produz toxina | deriva a 20 px/s até a célula do paciente mais próxima e coloniza (5/s numa célula de 100); a menos de 180 px do Nanobô, solta toxina a cada 3 s (90 px/s) |
| Biofilme | matriz que protege as bactérias | coco colonizando por mais de 6 s cria uma mancha de raio 40: o Nanobô anda a 50% e a bactéria dentro pede 3 doses |
| Endotoxina | LPS da membrana externa da Gram-, liberado quando ela se rompe | a Gram- eliminada deixa uma nuvem de raio 36 por 2,5 s (10/s) |
| Correnteza | fluxo do sangue; muco empurrado pelos cílios | faixas que empurram o Nanobô, os cocos e as toxinas (40 px/s) |
| Vírus (Fase 2) | não tem motor; só se multiplica dentro de células | deriva no muco, infecta a célula livre que toca; a célula infectada estoura em rajada radial de 6 vírus (110 px/s) |
| Bolor (*Aspergillus*, Fase 3) | hifas invadem o tecido; esporos espalham o fungo | colônia fixa; hifa como parede que machuca; 3 esporos em leque a cada 5 s, e o esporo que pousa vira colônia em 6 s (teto de 4) |
| Levedura (*Cryptococcus*, Fase 3) | cápsula protege; brotamento | avança a 35 px/s; cápsula pede 3 doses; brota a cada 20 s (teto de 12) |

- **Defesa (X):** além de empurrar, destrói os projéteis inimigos que tocam o escudo (toxina, esporo, vírus da rajada). Não protege da nuvem, do biofilme nem da hifa.
- **Calibração:** pelos bots do harness (leitor 55–75%, chute até 10%), ajustando a perseguição, a toxina e a colonização, não o tempo.
- **Escala dos sprites:** segue a biologia (vírus < bactéria < levedura < célula do paciente), como na tabela de §"Arte e interface".

### Arte e interface

- Pixel art à mão, com amostra aprovada pelo dono antes do resto. A primeira amostra de 14/11 foi recusada; a segunda, com Nanobô, coco, silhueta e célula do paciente no detalhe da referência do dono (luz de borda, brilho especular, contorno escuro), foi **aprovada no mesmo dia** (#225, descrita em `decisoes.md`).
- **Nanobô:** 48×48, cabeça arredondada com visor escuro e olhos ciano, núcleo dourado, braços curtos, antenas com ponta ciano e **propulsão magnética** (anel ciano que pulsa embaixo, sem hélice nem parte móvel que machuque o corpo).
- **Escala dos sprites (14/11):**

  | Sprite | Tamanho | Raio de colisão |
  |---|---|---|
  | vírus | 16×16 | 7 |
  | bactéria e silhueta | 32×32 | 14 |
  | levedura | 40×40 | 18 |
  | colônia de bolor; segmento de hifa | 48×48; 16×16 | 20; 7 |
  | Nanobô | 48×48 | 18 |
  | célula do paciente | 64×64 | 28 |
  | toxina e esporo | 8×8 | 4 |

  A colisão passa a ser por espécie (`agent_radius`), e não mais `AGENT_RADIUS` para todos.
- Agentes com contorno escuro e cores fora do vermelho.
- Um sprite de tiro por arma (a forma mostra o mecanismo: Ruptura é uma cunha, Dissolução uma gota, Neutralização um anticorpo em Y, Perfuração um anel) e 3 animações de impacto por resultado (dose certa, sem efeito, eliminado).
- **Fonte:** TTF pixel com licença OFL.
- **HUD:**
  - mais baixo que os 104 px atuais;
  - barra de vitais;
  - ferramenta ativa com o alvo ("Ruptura · parede");
  - ficha do scanner só quando há alvo.
- **Telas novas:** narrativa (substitui o briefing), interlúdios, 6 cards de conceito, entrada de nome e Top 10.
- **Navegação:** ←/→ entre botões lado a lado, e volta ao menu (#195).
- A interface é redesenhada **dentro do `main.c`**, sem módulo novo.

### Prazo e corte

O cronograma, a ordem de corte e a divisão da equipe estão em [`../planejamento/kanban.md`](../planejamento/kanban.md) (§"Nanobô 2.0").

## Visão geral

Nanobô é um jogo educacional 2D desenvolvido em C com Allegro 5. O jogador controla um nanorrobô em um ambiente microscópico inspirado no interior do corpo humano.

## High Concept / Mantra

**Explorar, analisar, interpretar e decidir.**

## Loop de gameplay

O gameplay central segue o ciclo:

**Explorar → Escanear → Identificar → Decidir → Agir → Consequência**

O jogador explora o ambiente, utiliza o scanner para analisar organismos, interpreta as informações apresentadas e decide como agir. A consequência da decisão influencia a pontuação, a sobrevivência e a progressão.

## Aprendizagem

A aprendizagem deve acontecer por meio da interpretação das informações e das consequências das decisões, e não somente por perguntas de múltipla escolha.

O conteúdo educacional deve ter função dentro do gameplay: a informação obtida pelo scanner precisa ajudar o jogador a distinguir agentes e escolher uma ação adequada.

A Fase 1 trabalha quatro objetivos, detalhados em [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md):

| OA | O jogador será capaz de… |
|---|---|
| OA1 | interpretar o que o resultado roxo ou vermelho (rosa) da coloração de Gram indica |
| OA2 | comparar as camadas externas: Gram+ tem parede mais espessa; Gram− tem parede mais fina e uma camada externa extra |
| OA3 | relacionar a estrutura observada ao alvo da arma dentro das regras do jogo |
| OA4 | reconhecer e preservar uma célula saudável do organismo, distinguindo a regra do jogo de uma afirmação sobre medicamentos reais |

O caminho do ensino é **briefing → scan → decisão → feedback → resumo**, e cada etapa retoma os mesmos quatro objetivos. A tela de Resultados apresenta os contadores da partida (bactérias identificadas por grupo, acertos e erros de arma, ataques à célula); o resumo por objetivo — um texto por OA — ainda não está no build.

## Protagonista

O jogador controla o **Nanobô**, um nanorrobô responsável por explorar o ambiente microscópico, analisar agentes e agir de acordo com as informações coletadas.

## Briefing

Antes da Fase 1, três cartões puláveis apresentam o necessário para decidir: (1) o que a coloração de Gram revela e o limite da cor; (2) a comparação das camadas externas Gram+ e Gram−, com o peptidoglicano como parte da parede; (3) as armas como modelo do jogo e a regra de preservar a célula própria. ENTER, → ou clique avançam; ESC pula. O briefing aparece **em toda partida**, inclusive depois de `VITÓRIA` e de `GAME OVER`, e o terceiro cartão tem quatro linhas desde 04/11. Não há tutorial guiado depois, e o Diário de Campo — que reabriria o conteúdo no pause — ficou fora do M4 (#103, #106).

## Scanner

O scanner é uma mecânica central de investigação.

- A ativação é o **botão direito** sobre o agente sob o cursor (tolerância de 24 px, alcance de 130 px); o teclado não escaneia no M4.
- A leitura ocorre em **dois pulsos**: o pulso 1 mostra a amostra de cor, a coloração (**roxo** ou **vermelho**) e o **nome do agente**; o pulso 2 mostra o nome do grupo (`Gram+`/`Gram-`), a pista estrutural e a frase que explica por que aquele grupo se distingue.
- O painel marca a origem da leitura: ` · alvo sob o cursor` quando há agente sob o cursor, ` · última leitura` quando mostra o último alvo lido; com agente sob o cursor, o painel segue esse agente.
- **[Proposta para o M5]** durante os pulsos o Nanobô ficaria parado e sem atirar, e dano de contato cancelaria o pulso; no build de 04/11 o scan é instantâneo e não imobiliza o jogador.
- Cada agente possui identificação única, para evitar que a mesma identificação seja contabilizada repetidamente.
- As informações obtidas apoiam a interpretação do agente e a decisão do jogador; o scanner não recomenda arma.

## Agentes e conteúdo

### Fase 1 — bactérias e células saudáveis

A primeira fase é o núcleo do MVP. Ela trabalha a diferenciação entre bactérias e células saudáveis, incluindo a distinção entre bactérias Gram-positivas e Gram-negativas.

Os seis binômios do cenário são **identificação do agente**, não alegação de patogenicidade: *Staphylococcus aureus*, *Streptococcus pneumoniae*, *Streptococcus pyogenes* (Gram+), *Escherichia coli*, *Salmonella enterica* e *Pseudomonas aeruginosa* (Gram−). Cada espécie poderá ter mais de um sprite no M5 (#211), sorteado por agente; a variante só aparece depois da camada 1 (ver `decisoes.md`).

O jogador deve usar as informações do scanner para interpretar o agente encontrado e escolher a ação correspondente.

### Fase 1 — de onde os agentes podem nascer

Os agentes nascem em pontos fixos do mundo, sorteados a cada partida sem repetir ponto (implementados em `src/gameplay/gameplay.c`, na tabela `valid_points`, com **12** coordenadas conferidas por `_Static_assert`). Um ponto válido respeita três regras:

1. fica dentro do mundo de **1920×512** (3 telas de 640 px por 2 faixas de 256 px), com margem das bordas;
2. fica dentro da altura do mundo e não mais da faixa do painel: a câmera mostra 256 px por vez, então o painel do scanner deixou de limitar o nascimento — a tabela usa `y = 96`/`140` na metade de cima e `y = 380`/`420` na de baixo;
3. fica a pelo menos **100 px** do ponto de nascimento do Nanobô `(960, 256)` — em 12/10/2026 um ponto a 22 px fazia um agente nascer já encostado (e, com a vida da #11, isso seria dano de graça no primeiro frame).

### Fase 2 — vírus

A Fase 2 mínima entrou no build de 04/11 para fechar o fluxo entre fases (#71/#76): **3 vírus e 3 células saudáveis**, no mesmo mapa e destino da Fase 1. O vírus tem **1 ponto de resistência** e **qualquer arma o neutraliza**; o contato tira vida e a defesa o empurra, como na bactéria.

O ciclo lisogênico/lítico, a multiplicação de até 3 vírus e as consequências de combate do ciclo continuam no M5 (#83–#91); a Fase 2 completa do GDD é essa, não a mínima.

## Combate e consequências

O combate está implementado no M3.5; as armas **Ruptura** e **Dissolução** entraram no M4 e são o que o build de 04/11 usa, com projétil neutro distinguido pela forma. Os números atuais são ponto de partida, não balanceamento final:

| Regra | Valor atual | Onde |
|---|---|---|
| Vida do Nanobô | 3 corações; 1 de dano por contato com bactéria ou por acertar a célula; 1 s de invulnerabilidade | `entities.h`, `gameplay.c` |
| Ataque | arma na direção do cursor, 1 disparo por segundo, até 4 projéteis na tela | `gameplay.c`, `main.c` |
| Armas | **Ruptura** neutraliza Gram+ (alvo na parede); **Dissolução** neutraliza Gram− (alvo na camada externa); arma errada gasta o projétil, não tira resistência, trava o disparo por 2 s e põe a bactéria atingida atrás do Nanobô | `gameplay.c` |
| Resistência | bactéria 2 acertos, célula 0 (não é destruída) | `entities.h` |
| Defesa | 2 s de imunidade, 3 s de recarga; empurra bactéria a 2× a velocidade por 0,5 s | `gameplay.c` |
| Consequência | resistência zerada neutraliza o agente, que sai da cena | `gameplay.c` |
| Célula atacada | tiro na célula tira 1 de vida do Nanobô, **com qualquer arma** (D1), e não remove a célula; ela continua identificável | `gameplay.c` |

O balanceamento de dano, cooldowns e resistência continua **[PENDÊNCIA]**; a regra fechada está em `docs/game-design/decisoes.md`.

## Pontuação e progressão

A pontuação atual da Fase 1:

| Evento | Pontos | Regra |
|---|---:|---|
| Ameaça neutralizada | **+10** | uma vez por ameaça, só no acerto que zera a resistência (#66) |
| Identificação completa | **+15** | uma vez, na conclusão, se `identificados = 9/9` (#67) |
| Placar máximo da fase | **75** | 6 ameaças + bônus |

Em 03/11 (noite) a dupla decidiu que, quando a identificação virar obrigatória (M5, D2.1), o **+15 passa a exigir run perfeito**: nenhum erro de arma e nenhum tiro na célula (#208). A regra do build até lá continua 9/9.

`9/9` **era** condição apenas de bônus: a fase concluía com menos identificações (#26, #67). Em 03/11/2026 a dupla decidiu o contrário (D2.1, `decisoes.md`): a identificação dos agentes passa a ser **condição de conclusão**. A regra em vigor no código ainda é a de #26/#67. D2.2 foi resolvida em 05/11/2026 (opção a: o projétil não reduz resistência até a identificação) e D2.1 entra com ela (#206/#207). **Entrega decidida em 03/11 (noite):** o M4 mantém #26; D2.1 e D2.2 entram no M5. O hi-score deve ser persistente na versão final.

## Interface e controles

O jogo usa teclado e mouse, com o mouse como forma de mirar, atirar e escanear:

- setas ou `WASD`: mover o Nanobô;
- mover o cursor: mirar; **botão esquerdo**: atirar na direção do cursor;
- **botão direito** sobre o agente: escanear;
- `1`/`2` ou roda do mouse: trocar a arma (Ruptura/Dissolução); o clique no ícone do HUD foi descartado;
- `X`: defesa;
- `ESC`: pausa; `ENTER`: confirmar/iniciar;
- `F11`: tela cheia, escala inteira quando o encaixe der 1x ou mais; janela menor que 640×360 encolhe proporcionalmente;
- clique no botão `INICIAR` do menu.

O ataque e o scanner **não têm tecla** no M4 (#198).

> **Estado em 04/11/2026:** os controles por cursor (#198), as armas **Ruptura**/**Dissolução** (#105), o conteúdo novo do scanner (#104), o briefing (#103/#106) e o resumo por contadores (#174/#202) **estão no build** e são o que o jogo faz. As duas bactérias usam **sprite neutro** até a camada 1 e ganham **anel colorido permanente** depois; "rosa" saiu da tela e fica como sinônimo nos documentos.
>
> **Ainda ausentes do build, no M5:** o pulso de 0,75 s com imobilização (#209) e a identificação como condição de conclusão (D2.1/D2.2, #206/#207). A trava de 2 s com perseguição (RF-06) entrou no build de 04/11. Ver `decisoes.md` e a Spec.

O HUD ocupa o rodapé de 104 px: identificados x/total de agentes da fase, ameaças restantes x/total de ameaças da fase (9 e 6 na Fase 1; 6 e 3 na Fase 2), pontos, tempo, três corações, arma selecionada com estado (`[pronto]`/`[recarga]`/`[travado]`) e a leitura do alvo atual. O objetivo da fase aparece na segunda linha do painel; os controles ficam no menu e na pausa, para não competir com a leitura do jogo. A marca do destino aparece na borda da faixa visível quando ele está fora da câmera (#19).

## Estados e fluxo

O fluxo está implementado em `src/core/game.c` com sete estados — `MENU`, `BRIEFING`, `GAME`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER` — e `BRIEFING` fica entre o menu e a fase, em toda partida (#106). As transições permitidas e a regra de que só `GAME` atualiza a lógica estão em [Fluxo de estados](../planejamento/fluxo-estados.md).

`GAME OVER` é alcançado por vida zerada; `RESULTS` é alcançado automaticamente quando a fase conclui (6 ameaças neutralizadas + Nanobô no destino); `VICTORY` vem de `RESULTS`; `VICTORY` e `GAME OVER` iniciam nova partida com reset completo. `F`, `G` e `H` são teclas de QA e existem somente no build Debug (#70, #76).

### Resultados e vitória

A tela de Resultados apresenta os números reais da partida: pontos, ameaças neutralizadas/total, agentes identificados/total e se o bônus de +15 foi pago. O terceiro painel, `o que a partida mostrou`, traz `bactérias identificadas: %d Gram+ · %d Gram-`, `acertos de arma: %d · erros: %d`, `ataques à célula: %d` e uma frase de revisão — tudo a partir de contadores em memória, sem CSV nem telemetria. É leitura por contador, não o resumo por objetivo (#174/RF-07): o texto por OA continua pendente. A tela de Vitória mostra a pontuação final e inicia nova partida. Com duas fases, CONTINUAR nos Resultados carrega a Fase 2 e preserva a pontuação; na última fase leva a VICTORY, que mostra o total da run, o tempo total e o recorde. O recorde é a melhor pontuação da run, gravada em nanobo_hiscore.txt; a decisão de 05/11 é que só a Vitória grave, e o build de 04/11 ainda grava no Game Over (#34/#82).

## Áudio e arte

O projeto prevê:

- spritesheet e animação;
- música;
- efeitos sonoros;
- cenários e demais recursos visuais.

Os assets são organizados em `assets/`, separados por tipo.

## Escopo de desenvolvimento

O MVP foi centrado na Fase 1 para garantir uma partida completa e jogável antes da expansão do conteúdo. A Fase 2 mínima entrou em 04/11 para fechar o fluxo entre fases; a completa é M5.

Uma terceira fase somente deve ser considerada se houver tempo e estabilidade suficientes.

## Pendências de design

- Conteúdo biológico detalhado e validado de cada agente.
- Duração do pulso do scanner e intervalo entre leituras.
- Calibração dos números já implementados de vida, dano, ataque, defesa e resistência.
- Regras de progressão entre fases.
- Demais assets do M5: variação de sprites das bactérias, spritesheet e áudio.
- Regras detalhadas da multiplicação limitada da Fase 2.

Fora do M4: Diário de Campo, zonas guiadas e registro de eventos/telemetria; a calibração de trava/perseguição com pessoas fica no playtest do M5. Ver [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md).
