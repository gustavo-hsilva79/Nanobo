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

> **Reescrita em 25/11/2026** (decisão do dono, [`decisoes.md`](decisoes.md) §"Narrativa e conteúdo (25/11/2026)"). O que está no build desde 15/11 (#244: abertura em 4 cards, interlúdio como subtítulo dos Resultados, Vitória e Game Over) continua até a implementação do diário, que fica para depois; esta seção é o alvo.

A história é contada pelo **Diário do Nanobô**: o Nanobô é um protótipo **recém-ligado, de memória vazia**, e escreve em **primeira pessoa** as **Anotações** sobre o que descobre. Os termos estão no [`glossario.md`](glossario.md) (Diário do Nanobô, Anotação, Paciente, Interlúdio).

- **Abertura:** a **ficha do hospital** do Paciente e a **"Anotação 0"** do Nanobô, no lugar dos 4 cards de hoje.
- **O Paciente:** **Léo Martins Ferreira, 16 anos**, internado com febre alta e uma infecção que se espalha pelo sangue. A equipe testa o Nanobô, um nanorrobô médico menor que uma célula, que ajuda o corpo sem ferir as células do paciente e vai atrás só dos invasores (a maioria das bactérias do corpo é inofensiva).
- **O arco, em três Interlúdios:**
  1. **Fase 1:** a infecção no sangue melhora;
  2. **Fase 2:** na enfermaria, Léo pega um **vírus respiratório**;
  3. **Fase 3:** com a defesa enfraquecida, **esporos de mofo** chegam ao pulmão. O risco vem da defesa fraca, não de mais esporos.
  Os Interlúdios seguem mostrando os sinais vitais ("estável" ≥ 70, "em observação" 40–69, "grave" < 40) e anunciam o próximo invasor.
- **Final:** **Vitória = alta** de Léo, com a página do diário **"O que anotei nesta fase"** no lugar do relatório de fim de fase.
- **Game Over:** a equipe **retira o Nanobô** e Léo **segue o tratamento convencional**. **Ninguém morre.**
- **As Anotações** são as antigas "cards de conceito" (IDs CC-01 a CC-09 mantidos): caixa de diálogo **paginada** (máquina de escrever de cerca de 40 caracteres por segundo; a mesma tecla completa e depois avança; ▼ piscando; "fechar" na última página; contador "2/4"; 0,2 s de debounce), **jogo pausado**, frases curtas e um termo-chave colorido por página. O conteúdo vem só do que os documentos sustentam ([`conteudo-biologico.md`](conteudo-biologico.md)). Entre elas, uma diz que as armas são regra do jogo, inspirada em antibióticos reais, e que no jogo todas as espécies aparecem juntas no sangue, por simplicidade.
- **Fora do escopo de 02/12:** o menu para reler o diário.
- **Pendente:** os textos novos ainda serão redigidos para aprovação do dono; `textos-do-jogo.md` não mudou.

### O Nanobô é o único defensor

Não há aliados: o macrófago e o linfócito T planejados foram retirados em 13/11. O que o corpo faria aparece nas armas do Nanobô e na explicação dos cards.

### Fases

| Fase | Local | Invasor | Armas | Vitória |
|---|---|---|---|---|
| 1 | corrente sanguínea (mapa atual) | 6 bactérias (cocos e bacilos, Gram+ e Gram−) + 3 células do paciente | **Ruptura** (antibiótico de parede), **Dissolução** (antibiótico de membrana), **Reserva** (antibiótico de reserva, 2 cargas) | zero bactérias e Nanobô no destino |
| 2 | vias aéreas (brônquio, mapa novo) | vírus respiratório: 3 vírus lisogênicos dentro de células + 2 células livres | **Neutralização** (anticorpos, vírus livre), **Lise** (destrói célula infectada já lida, papel do linfócito T; **nome a trocar por "Ataque T", renomeação pendente no código**) | zero vírus e Nanobô no destino |
| 3 | pulmão (alvéolos, mapa novo) | fungos: 4 leveduras (*Cryptococcus*) + 2 colônias de bolor (*Aspergillus*) + 3 células, mais esporos conforme o estado do paciente | **Perfuração** (antifúngico inspirado na anfotericina B, abre poros na membrana com ergosterol) | zero fungos e Nanobô no destino |

Cada fase tem mapa próprio; no build, as Fases 2 e 3 usam a partida e o destino globais (`DESTINATION_X/Y`), e a Fase 3 não tem pontos próprios (dívida em `fase3-fungos.md`). O mapa da Fase 1 continua o atual, mas é desenhado escurecido e dessaturado com `al_draw_tinted_bitmap` para os agentes se destacarem; o PNG não muda. Os mapas das Fases 2 e 3 (1920×512) são desenhados por Claude e podem ser melhorados pelo dono com IA de geração de imagem.

### Regras centrais

- **Scanner em duas camadas:**
  - a 1ª dá a **forma** (coco ou bacilo; levedura ou filamento);
  - a 2ª dá a **coloração e a estrutura**.
  - Antes da leitura, o agente é uma silhueta azul-acinzentada neutra. O sprite da forma é o mesmo para Gram+ e Gram−, e a cor do Gram vem do tingimento depois da 2ª camada.
- **Tratamento:**
  - cada bactéria precisa de **2 doses** do antibiótico certo;
  - uma dose e mais de 6 s sem a segunda deixam a bactéria **resistente àquele antibiótico**, e daí só a **Reserva** a elimina;
  - o antibiótico errado não faz efeito, e não cria resistência; ele trava o disparo por 2 s, o custo do chute (#215);
  - saída sem Reserva: se só sobram resistentes e as cargas acabaram, chegar ao destino faz a equipe isolá-las (-15 vitais cada, sem pontos) e a fase conclui, a menos que os vitais zerem.
- **Divisão e crescimento:**
  - as bactérias se dividem a cada cerca de 12 s (12 ± 3 s, calibrado na #238), com teto de 16 vivas e no máximo 15 divisões por fase;
  - as leveduras brotam a cada cerca de 20 s, com teto de 12;
  - a hifa do bolor cresce 1 segmento a cada cerca de 5 s, até 5 segmentos. Acertar a **base** mata a colônia, e acertar a ponta só corta um segmento. Com a hifa no tamanho máximo, a colônia solta esporos e nasce outra, até 4 colônias.
- **Vírus (Fase 2):**
  - o lisogênico fica dentro da célula, piscando, por 10 s (os três da fase escalonados em 10, 14 e 18 s; calibrado na #89), e o eclipse dura 2 s;
  - depois do eclipse, a célula se rompe e solta 6 vírus numa **rajada radial**; eles ferem o Nanobô e depois derivam no muco como vírus **livres**, que infectam células livres ao tocá-las (14/11);
  - a **Lise** elimina a célula infectada já lida: custa poucos vitais, mas impede a eclosão;
  - o antibiótico não age em vírus;
  - teto de 12 vírus.
- **Ligação com a Fase 3:** a carga inicial de esporos sobe quanto menores forem os vitais ao fim da Fase 2 e quanto mais cargas de Reserva tiverem sido gastas. Fungos oportunistas se instalam com mais facilidade quando a defesa está fraca (o risco vem da defesa fraca, não da quantidade de esporos).
- **Duas barras (14/11):**
  - **Integridade do Nanobô:** de 0 a 100, cheia no início de cada fase. Cai com contato (10), toxina (8), esporo (8), vírus da rajada (6), hifa (10) e nuvem de endotoxina (10/s).
  - **Vitais do paciente:** de 0 a 100. Caem com célula do paciente destruída (−20), tiro do Nanobô numa célula (−15) e Lise (−5). Passam de uma fase para a outra, e o interlúdio recupera até no mínimo 60.
  - Derrota quando qualquer uma zera; a tela de Game Over diz qual.
  - Não há drenagem por tempo: a pressão vem dos invasores (§"Dificuldade").
- **Pontuação:**
  - +10 por invasor eliminado (filhas da divisão, brotos, esporos e colônias germinadas não pontuam; vírus da rajada sim);
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
| Bacilo (*E. coli*, *P. aeruginosa*, *C. tetani*) | tem flagelo e nada | persegue a 70 px/s; investida de 0,4 s a 150 px/s a cada 2,5 s |
| Coco (*S. aureus*, *S. pyogenes*, *M. catarrhalis*) | sem flagelo; coloniza tecido e produz toxina | deriva a 20 px/s até a célula do paciente mais próxima e coloniza (5/s numa célula de 100); a menos de 180 px do Nanobô, solta toxina a cada 3 s (90 px/s) |
| Biofilme | matriz que protege as bactérias | coco colonizando por mais de 6 s cria uma mancha de raio 40: o Nanobô anda a 50% e a bactéria dentro pede 3 doses |
| Endotoxina | LPS da membrana externa da Gram-, liberado quando ela se rompe | a Gram- eliminada deixa uma nuvem de raio 36 por 2,5 s (10/s) |
| Correnteza | fluxo do sangue; muco empurrado pelos cílios | faixas que empurram o Nanobô, os cocos e as toxinas (40 px/s) |
| Vírus (Fase 2) | não tem motor; só se multiplica dentro de células | deriva no muco, infecta a célula livre que toca; a célula infectada estoura em rajada radial de 6 vírus (110 px/s) |
| Bolor (*Aspergillus*, Fase 3) | hifas invadem o tecido; esporos espalham o fungo | colônia fixa; hifa como parede que machuca; 3 esporos em leque a cada 5 s, e o esporo que pousa vira colônia em 6 s (teto de 4) |
| Levedura (*Cryptococcus*, Fase 3) | cápsula protege; brotamento | avança a 35 px/s; cápsula pede 3 doses; brota a cada 20 s (teto de 12) |

- **Defesa (X):** além de empurrar, destrói os projéteis inimigos que tocam o escudo (toxina, esporo, vírus da rajada). Não protege da nuvem, do biofilme nem da hifa.
- **Calibração:** pelos bots do harness (leitor 55–75%, chute até 10%), ajustando a perseguição, a toxina e a colonização, não o tempo.
- **Escala dos sprites:** **ilustrativos, fora de escala** (decisão de 25/11); só a ordem vírus < bactéria < levedura < célula do paciente é mantida, como na tabela de §"Arte e interface".

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
- **Fonte:** a embutida do Allegro (8 px por caractere), mantida em 15/11. A TTF pixel foi testada e descartada (`decisoes.md`).
- **HUD (no build desde 15/11, #243):**
  - 40 px em duas linhas, no lugar dos 104; o mundo passou de 256 para 320 px de altura;
  - barra de vitais;
  - arma ativa com o alvo ("Ruptura · parede");
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

Os seis binômios do cenário são **identificação do agente**, não alegação de patogenicidade: *Staphylococcus aureus*, *Streptococcus pyogenes*, *Clostridium tetani* (Gram+), *Moraxella catarrhalis*, *Escherichia coli* e *Pseudomonas aeruginosa* (Gram−). (Lista atualizada em 25/11/2026; a de M4 trazia S. pneumoniae e Salmonella, e o 2.0 de 13/11 trazia N. meningitidis e B. cereus; ver `conteudo-biologico.md`.) Cada espécie poderá ter mais de um sprite no M5 (#211), sorteado por agente; a variante só aparece depois da camada 1 (ver `decisoes.md`).

O jogador deve usar as informações do scanner para interpretar o agente encontrado e escolher a ação correspondente.

### Fase 1 — de onde os agentes podem nascer

Os agentes nascem em pontos fixos do mundo, sorteados a cada partida sem repetir ponto (implementados em `src/gameplay/gameplay.c`, na tabela `valid_points`, com **12** coordenadas conferidas por `_Static_assert`). Um ponto válido respeita três regras:

1. fica dentro do mundo de **1920×512** (3 telas de 640 px de largura), com margem das bordas;
2. fica dentro da altura do mundo e não mais da faixa do painel: a câmera mostra 320 px por vez (256 até 15/11), então o painel do scanner deixou de limitar o nascimento — a tabela usa `y = 96`/`140` na metade de cima e `y = 380`/`420` na de baixo;
3. fica a pelo menos **100 px** do ponto de nascimento do Nanobô `(960, 256)` — em 12/10/2026 um ponto a 22 px fazia um agente nascer já encostado (e, com a vida da #11, isso seria dano de graça no primeiro frame).

### Fase 2 — vírus

A Fase 2 do 2.0 entrou no build em 14/11 (#85–#89).
- **Composição:** 3 vírus lisogênicos dentro de células do epitélio e 2 células livres, cada uma vizinha de uma infectada.
- **Ciclo:** o vírus fica lisogênico (10, 14 e 18 s), entra no eclipse (2 s) e rompe a célula numa rajada de 6 vírus livres, que derivam no muco e infectam a célula livre que tocam.
- **Armas:** a Neutralização prende o vírus livre; a Lise tira a célula infectada já lida (−5 vitais) antes de ela romper.
- **Mapa:** o interior de um brônquio (`mapa_fase2.png`), com paredes de epitélio ciliado, as duas correntes de muco e a saída no destino.
- **Regras completas:** `fase2-virus.md` (§"Implementação de 14/11").

### Fase 3 — fungos

A Fase 3 do 2.0 entrou no build em 25/11 (#226, #227–#233), na branch `feat/fase3`.
- **Composição:** 4 leveduras, 2 colônias de bolor e 3 células alveolares, mais esporos no chão conforme o estado do paciente ao fim da Fase 2 (de 0 a 4).
- **Levedura:** cápsula de 3 doses, avança a 35 px/s em alerta e brota a cada 20 ± 3 s, até 12 vivas.
- **Bolor:** colônia fixa; a hifa cresce 1 segmento a cada 5 ± 1 s, até 5, rumo à célula mais próxima, e é parede que tira 10 de integridade; com a hifa cheia e a colônia em alerta saem 3 esporos em leque a cada 5 s. O esporo pousa, vira ameaça e germina em colônia em 6 s (teto de 4 colônias).
- **Armas:** Perfuração (tecla 1) e Ruptura (tecla 2, a arma inicial); o antibiótico não age em fungo e trava o disparo por 2 s. O esporo não dá pontos.
- **Mapa:** o pulmão (`mapa_fase3.png`), com alvéolos e capilares.
- **Regras completas:** `fase3-fungos.md`.

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

O HUD ocupa o rodapé de 40 px em duas linhas (#243, 15/11):
- **Linha 0:** a integridade do Nanobô e os vitais do paciente, pontos, tempo e a arma ativa com o alvo (`Ruptura · parede`), o ícone, a recarga e o estado (`pronto`/`recarga`/`travado`/`defesa`/`esgotada`).
- **Linha 1:** o objetivo da fase, as ameaças x/total e os identificados x/total.

A ficha do scanner só aparece quando há alvo e flutua na faixa do mundo, longe do agente lido. Os controles ficam no menu e na pausa, para não competir com a leitura do jogo. A marca do destino aparece na borda da faixa visível quando ele está fora da câmera (#19).

## Estados e fluxo

O fluxo está implementado em `src/core/game.c` com sete estados — `MENU`, `BRIEFING`, `GAME`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER` — e `BRIEFING` fica entre o menu e a fase, em toda partida (#106). As transições permitidas e a regra de que só `GAME` atualiza a lógica estão em [Fluxo de estados](../planejamento/fluxo-estados.md).

`GAME OVER` é alcançado por vida zerada; `RESULTS` é alcançado automaticamente quando a fase conclui (Fase 1: 6 bactérias neutralizadas; Fase 2: os vírus; Fase 3: os fungos, esporos inclusive; sempre com o Nanobô no destino); `VICTORY` vem de `RESULTS`; `VICTORY` e `GAME OVER` iniciam nova partida com reset completo. `F`, `G` e `H` são teclas de QA e existem somente no build Debug (#70, #76).

### Resultados e vitória

A tela de Resultados apresenta os números reais da partida: pontos, ameaças neutralizadas/total, agentes identificados/total e se o bônus de +15 foi pago. O terceiro painel, `O que anotei nesta fase`, traz cinco linhas na voz do Nanobô, uma por conteúdo da fase, com os números da partida (registro em `textos-do-jogo.md` §2.2) — tudo a partir de contadores em memória, sem CSV nem telemetria. É leitura por contador, não o resumo por objetivo (#174/RF-07): o texto por OA continua pendente. A tela de Vitória mostra a pontuação final e inicia nova partida. Com três fases, CONTINUAR nos Resultados carrega a fase seguinte (a Fase 2 depois da 1, a Fase 3 depois da 2) e preserva a pontuação; na última fase leva a VICTORY, que mostra o total da run, o tempo total e o recorde. O recorde é o Top 10 de `nanobo_ranking.txt`, na pasta do jogo (caminho relativo, como os assets; decisão do dono de 15/11): só a Vitória grava, depois que o jogador digita o nome na tela de Vitória, e o Game Over não grava nada. O `nanobo_hiscore.txt` antigo só é importado, se existir (#34, #77–#82, #197).

## Áudio e arte

O projeto prevê:

- spritesheet e animação;
- música;
- efeitos sonoros;
- cenários e demais recursos visuais.

Os assets são organizados em `assets/`, separados por tipo.

## Escopo de desenvolvimento

O MVP foi centrado na Fase 1 para garantir uma partida completa e jogável antes da expansão do conteúdo. A Fase 2 mínima entrou em 04/11 para fechar o fluxo entre fases; a completa entrou em 14/11. A Fase 3 de fungos entrou em 25/11 e fecha a partida em três fases.

## Pendências de design

- Conteúdo biológico detalhado e validado de cada agente.
- Duração do pulso do scanner e intervalo entre leituras.
- Calibração dos números já implementados de vida, dano, ataque, defesa e resistência.
- Regras de progressão entre fases.
- Demais assets do M5: variação de sprites das bactérias, spritesheet e áudio.
- Regras detalhadas da multiplicação limitada da Fase 2.

Fora do M4: Diário de Campo, zonas guiadas e registro de eventos/telemetria; a calibração de trava/perseguição com pessoas fica no playtest do M5. Ver [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md).
