# Nanobô — Decisões de Game Design

Este arquivo registra decisões já tomadas pela dupla. Quando uma decisão importante for fechada, ela deve ser registrada aqui; hipóteses e itens ainda não validados permanecem como pendências.

## Decisões atuais

### Nanobô 2.0 (13/11/2026)

> Decisões do dono em 13/11, tomadas depois da auditoria (`docs/auditoria/auditoria-2026-11-13.md`) e em duas rodadas de arguição. Valem acima das decisões mais antigas deste arquivo onde conflitarem. A visão vigente está no [`gdd.md`](gdd.md) (§"Nanobô 2.0").

**Revisão de 14/11: dificuldade pela IA dos invasores e arte detalhada**

> Brainstorming do dono em 14/11, depois de jogar a Etapa 1. A especificação completa está no [`gdd.md`](gdd.md) (§"Dificuldade: a IA dos invasores" e §"Arte e interface").

- **Sai a drenagem por tempo** (`THREAT_DRAIN` de 0,68 calibrado na #224): o dono não quer a dificuldade vindo do tempo.
- **Duas barras:**
  - integridade do Nanobô, cheia em cada fase;
  - vitais do paciente, que caem quando uma célula dele é destruída e passam de fase com piso de 60.
  - Perde quem zerar qualquer uma.
- **Bactérias pela forma:**
  - o bacilo (flagelo) persegue o Nanobô e dá investidas;
  - o coco (sem flagelo) coloniza as células do paciente e solta toxina no Nanobô que chega perto.
  - O bacilo ameaça o robô e o coco ameaça o paciente.
- **Perigos:**
  - biofilme (lentidão e 3 doses);
  - nuvem de endotoxina da Gram- eliminada;
  - correnteza;
  - na Fase 3, hifas e esporos.
- **Vírus:** derivam no muco; a célula infectada estoura em rajada radial.
- **Fungos:** bolor fixo com hifa e esporos; levedura que anda, com cápsula e brotamento.
- **Defesa:** destrói projéteis inimigos.
- **Arte:**
  - a amostra de 14/11 foi recusada;
  - o Nanobô vai a 48×48, com o detalhe da referência e propulsão magnética;
  - coco e silhueta são refeitos.
- **Spritesheet do Nanobô em 48×48 (15/11, #240):**
  - frente, costas e perfil desenhados no estilo da amostra 2; a direita é o espelho da esquerda;
  - a propulsão é o anel que pulsa no parado e brilha ao andar;
  - `PLAYER_RADIUS` passou de 16 para 18, e o alcance do destino foi de 40 para 42 px;
  - com o robô maior, o bot leitor da Fase 1 vence 60% (era 64%), ainda dentro da meta.
- **Amostra 2 aprovada (14/11, #225):** o dono aprovou a segunda amostra no mesmo dia. Ela vira o estilo da arte final:
  - **Nanobô 48×48, de frente:** cabeça larga com visor escuro e dois olhos ciano, antenas com ponta ciano redonda (a da direita mais alta), corpo em ovo com placas, núcleo dourado no peito e braços curtos e redondos;
  - **propulsão magnética:** uma faixa ciano na base do corpo emite o campo, e um anel ciano flutua embaixo, com faíscas. Não há hélice nem peça que gira;
  - **coco 32×32:** três esferas cinza neutro, com especular, luz de retorno e sombra de contato entre elas. O tingimento do Gram multiplica por cima;
  - **silhueta 32×32:** disco de holograma com linhas de varredura e um "?" no centro, sem forma nem cor que entregue o grupo;
  - **célula do paciente 64×64:** membrana dupla, citoplasma em cúpula com grânulos, núcleo com cromatina e nucléolo, três mitocôndrias com cristas e o retículo em volta do núcleo;
  - **estilo:** contorno escuro de 1 px, luz no alto à esquerda e paleta fora do vermelho do mapa;
  - os arquivos ficam em `../Nanobo-testes/arte-amostra/` (`amostra2_*`), gerados pelo `amostra2.lua`. As outras três direções e os 8 quadros do Nanobô entram na Etapa 3.
- **Escala dos sprites:** vírus 16, bactéria 32, levedura 40, Nanobô 48 e célula do paciente 64, com colisão por espécie. *(Em 25/11 o nome "escala biológica" foi trocado por "fora de escala"; ver §"Narrativa e conteúdo (25/11/2026)", decisão 8.)*
- **Ordem:** a Fase 1 primeiro (15–18/11), a nova amostra em 17–19/11, e as Fases 2 e 3 já nascem com a IA delas.
- **Calls de implementação (#235):**
  - **5 s de calma** no começo de cada fase (`AI_GRACE_TIME`): ninguém entra em alerta antes disso. Sem isso, um bacilo nascido a 198 px do início investia aos 2 s, antes do tutorial;
  - **células do paciente fixas:** o endotélio forra o vaso. Na colisão, quem recua é o outro agente ou o Nanobô, e a célula de 64 px vira obstáculo;
  - **o coco do tutorial fica segurado** (`AI_HOLD`) até o primeiro scan e depois segue para uma célula como os outros;
  - **até a arte nova**, os sprites atuais de 32 px são esticados para 64 (célula) e 16 (vírus).
- **Calls de implementação (#237):**
  - **biofilme com raio 72**, e não 40: com 40, a mancha ficaria quase toda escondida debaixo da célula de 64 px e dos cocos grudados nela;
  - **os cards de biofilme e de endotoxina abrem no primeiro contato do Nanobô** com a mancha ou a nuvem, e não quando ela nasce. O card pausa a partida, e o conceito importa quando o jogador sente o efeito;
  - **duas faixas de correnteza** na Fase 1 (y de 140 a 190 e de 330 a 380), longe do meio do vaso, onde o Nanobô nasce. Elas empurram para a direita, rumo ao destino, e não levam bacilos (que nadam), cocos colonizando nem a bactéria do tutorial segurada;
  - **tecla de QA K** (só no Debug): cria um biofilme e uma nuvem perto do Nanobô.
- **Calls de implementação da Fase 2 (#85–#89, 14/11):** detalhe em `fase2-virus.md` (§"Implementação de 14/11"):
  - **lisogênico de 10 s, com os três da fase escalonados em 10, 14 e 18 s**, e eclipse de 2 s. Pela regra de 11/11 os três começavam com 12 s cheios e romperiam juntos antes de o jogador cruzar o mapa; o escalonamento e os 10 s saíram dos bots;
  - **célula livre vizinha da infectada (140 px)**, como no epitélio. Sem isso, quem não lê o scan sobrevivia às três rupturas e vencia 63% das vezes começando com 77 de vitais;
  - **a Neutralização não fere a célula do paciente**: anticorpo não ataca célula, então ensinar o contrário seria errado. O custo de errar o tiro é a recarga;
  - **a Lise não dispara** sem uma célula infectada lida no caminho, como o jogo pede ("numa célula saudável ela não dispara"). Basta uma camada lida, porque a 1ª já mostra a infecção;
  - **o vírus da rajada que acerta o Nanobô cai livre**, porque continua sendo vírus. Só a defesa o destrói;
  - **o vírus de dentro não se lê sozinho**: ler a infectada até o fim o identifica, e a infectada que sai de cena sem leitura sai da meta do bônus com o vírus dela;
  - **mapa próprio da Fase 2 já em 14/11**, a pedido do dono, adiantado da Etapa 3: o interior de um brônquio (`mapa_fase2.png`), com as correntes de muco nas faixas da correnteza.

**Escopo e equipe**
- Entram cinco frentes: game design, ensino, narrativa, arte e interface com textos. **Nada cede no prazo**; a ordem de corte do `kanban.md` é a rede de segurança.
- O Gustavo assume os requisitos obrigatórios que estavam com o Eric: hi-score Top 10 (#77–#82, #197), build Release (#42, #154–#158) e instalador (#43, #159–#164). O Eric fica com QA, documentação, textos (#212) e volta ao menu (#195).
- **Sem arquivos novos**, a não ser que seja extremamente necessário: o design vive nos documentos que já existem, e a interface nova é redesenhada dentro do `main.c` (sem `src/ui/`).
- Merge direto na `master`, sem PR. Resumo diário ao dono.
- Não há teste com aluno antes de 02/12; a calibração usa bots no harness e as partidas do dono.
- Sem modo de demonstração.

**Ensino**
- Os conteúdos são de ensino médio: bactérias a fundo, célula humana × bactéria, sistema imune (só anticorpos), vírus e ciclos, e **fungos** (Fase 3). São 8 objetivos como mecânicas (`ensino-e-aprendizagem.md`, §"Nanobô 2.0").
- Cada conteúdo tem definição, feedback do erro com o porquê biológico e retomada nos resultados. Há 6 cards de conceito. O scanner continua sem recomendar a arma.

**Mecânicas**
- **O Nanobô é o único defensor.** Macrófago e linfócito T saem.
- **Camadas do scan:** a 1ª dá a forma, e a 2ª dá a coloração e a estrutura. Isso substitui "camada 1 com coloração e nome" (03/11, noite). A marcação acontece sozinha ao concluir o scan.
- **Resistência por tratamento incompleto:** 2 doses; 1 dose e mais de 6 s sem a segunda geram resistência ao antibiótico usado. O antibiótico errado não faz efeito e não gera resistência. A regra antiga, "erro de arma gera resistência a tudo", ensinaria multirresistência e foi descartada.
- **Reserva:** antibiótico de reserva com 2 cargas por fase; elimina a resistente.
- **Substituições de regras antigas:** a perseguição por arma errada (RF-06, 04/11) é substituída pela resistência. Os 3 corações dão lugar à barra de vitais de 0 a 100, que passa entre as fases.
- **Divisão binária (#217):**
  - cada bactéria viva se divide em 12 s ± 3 s (era 22 ± 4; mudou na calibração da #238), com teto de 16 vivas;
  - a filha nasce ao lado da mãe numa vaga nova do array (`MAX_AGENTS` 24), então uma fase tem no máximo 15 nascimentos;
  - a filha herda a resistência e as camadas lidas, mas não a dose;
  - o bônus de identificação e o "identificados x/9" contam só os 9 agentes iniciais;
  - as ameaças do HUD e da run somam as filhas.
- **Sinais vitais (#218):**
  - o contato custa 8 e o tiro na célula do paciente custa 15, os dois com a invulnerabilidade de 1 s;
  - cada invasor vivo drena 0,68/s (bactérias na Fase 1, vírus na Fase 2); era 0,12 e subiu na calibração da #224;
  - só a run nova enche a barra: entre as fases os vitais continuam, com piso de 60;
  - a conclusão paga vitais × 0,5, mostrado nos resultados como "bônus de vitais";
  - no HUD, a barra fica verde a partir de 60, âmbar a partir de 30 e vermelha abaixo disso.
- **Resposta visual (#221):**
  - um ponto de chamada por evento (`SfxEvent`), em três níveis:
    - leve: scan, disparo, defesa;
    - médio: dose e antibiótico sem efeito;
    - forte: dano, eliminação, fase concluída;
  - o tremor é de 0, 1 ou 3 px, decai em 0,25 s e só desloca o desenho do mundo;
  - efeitos por evento:
    - flash vermelho de 0,2 s no dano;
    - o Nanobô pisca na invulnerabilidade;
    - anel de impacto no acerto (claro na dose, apagado sem efeito);
    - anel do scan saindo do Nanobô;
    - partículas, "+10" e hit-stop de 50 ms na eliminação;
  - a camada não chama `rand()`, para não mudar a sequência sorteada do gameplay;
  - o hit-stop só segura o `update_match`: a entrada continua chegando.
- **Trava do antibiótico errado (call de 13/11, #215):** a perseguição saiu, mas a trava de 2 s do disparo ficou. Sem ela, chutar a arma custaria só 1 s de recarga, e a simulação de 01/11 mostrou que o chute vence quando o erro sai barato. A bactéria não muda; só o disparo trava. A resistência perde 5 pontos, sem deixar a pontuação negativa. Os bots da #224 conferem se a trava basta.
- **Fase 2:** vias aéreas; vírus livres na eclosão, que infectam células livres; **Lise** para a célula infectada (custa poucos vitais); teto de 12 vírus (`fase2-virus.md`).
- **Fase 3:** pulmão; levedura (*Cryptococcus*, brotamento) e bolor (*Aspergillus*, hifas e esporos); a parede de quitina impede o antibiótico; **Perfuração** é o antifúngico. A carga de esporos depende do estado do paciente ao fim da Fase 2 e das cargas de Reserva gastas, e o jogo não afirma que o antibiótico causa aspergilose. A Perfuração **não** custa vitais (o dono não escolheu o efeito colateral).
- **Nomes das armas:** Ruptura e Dissolução continuam; entram **Reserva**, **Neutralização**, **Lise** e **Perfuração**. Cada fase mostra só as suas armas (até 3).
- A identificação deixa de ser condição de vitória.

**Arte e interface**
- Pixel art à mão, desenhada por Claude. A amostra 2 (Nanobô, coco, silhueta e célula do paciente) foi aprovada em 14/11 (#225).
- O mapa da Fase 1 é desenhado escurecido e dessaturado (o PNG não muda). As Fases 2 e 3 ganham mapas próprios desenhados por Claude, que o dono pode melhorar com IA de imagem.
- **Sprites das bactérias** neutros por forma, com 2 variantes cada (#211 reescopada). O Gram vem do tingimento depois da 2ª camada.
- **Tiros:** um sprite de 12×12 com 2 quadros por arma, em cor neutra (a forma mostra o mecanismo). As animações de impacto são 3 por resultado (dose certa, sem efeito, eliminado). O recuo do Nanobô é feito em código.
- HUD mais baixo, com a ficha do scanner só quando há alvo; ←/→ entre botões lado a lado.
- **Fonte embutida do Allegro mantida (15/11, decisão do dono).** A Pixelify Sans (OFL) foi testada no jogo de 8 a 20 px. Só fica nítida em 20 px, e de 12 a 16 px os glifos saem irregulares. A troca saiu da #243, e a regra de não usar maiúscula acentuada (§2.1 de `textos-do-jogo.md`) continua valendo.

### Precedência entre fontes (03/11/2026)

Quando documento e código divergem, vale nesta ordem:

1. **Decisão registrada neste arquivo** — é o contrato da dupla.
2. **Corpo da issue**, quando ele foi revisado depois da decisão (o corpo é o contrato operacional que a dupla assinou).
3. **Spec de ensino e demais documentos descritivos** — são a explicação das regras.
4. **Código** — nunca é fonte de verdade; é dívida. Um comportamento no build que contradiz 1–3 não vence por estar rodando.

Corolários: um documento que descreve o que o código ainda não faz precisa dizer **onde** e **quando** aquilo entra (marco), senão vira promessa; e toda decisão nova exige a varredura dos documentos que citam a regra antiga no mesmo commit.

**Descartado por enquanto:** workflow automático de reconciliação documentação × código. A prevenção fica nesta seção, na varredura de cada decisão e no item de QA do fechamento.

### Dívida de implementação (03/11/2026, atualizada em 04/11/2026)

O que está decidido e o código ainda não faz — ou já faz desde o build de 04/11. Atualizar esta tabela quando uma regra entrar no build ou mudar de marco.

| Regra decidida | Onde está registrada | No build de 04/11 | Marco |
|---|---|---|---|
| Camada 1 = coloração (roxo/vermelho) + nome do agente | D2; `scanner.md` §4 | **feito** — amostra de cor + `<cor> · <nome>` | M4 (#104) |
| Camada 2 = grupo + estrutura + porquê | D2; `scanner.md` §4 | **feito** — grupo, estrutura e a frase de revisão | M4 (#104) |
| Briefing de 3 cartões | `decisoes.md`; RF-01 | **feito** — em toda partida, pulável com ESC | M4 (#103, #106) |
| Armas com nome (Ruptura/Dissolução) | `decisoes.md`; `textos-do-jogo.md` | **feito** — menu, HUD e as 7 mensagens de feedback | M4 |
| Armas com cor neutra; ícone e projétil por forma | RF-04 | **feito** — ícones neutros próprios (parede/anéis) e projétil por forma | M4 |
| Disco cinza até a camada 1; sprite + anel depois | RF-03; `decisoes.md` | **feito** — disco cinza, sprite e anel roxo/vermelho depois da leitura | M4 |
| Variação de sprites por espécie | `decisoes.md` (03/11, noite) | um sprite por espécie — #211 | M5 |
| Pulso de 0,75 s com imobilização e cancelamento | RF-02; Spec "Dificuldade" | scan instantâneo — #209 | M5 |
| Trava de 2 s + perseguição após arma errada | RF-06 | **feito** — trava de 2 s e perseguição a 60 px/s, em qualquer distância | M5 (antecipado no build de 04/11) |
| Identificação condiciona a conclusão | D2.1 + D2.2 | ausente — #206 (trava) e #207 (9/9; 6/6 na Fase 2) no M5 | M5 |
| Resumo por objetivo na tela de Resultados | RF-07 (#174) | **parcial** — contadores no painel; o texto por OA fica no M5 (#210) | M5 |
| Fase 2 mínima (3 vírus + 3 células, 1 resistência, qualquer arma) | `decisoes.md` (04/11) | **feito** | M4 (#76) |
| Ciclo lisogênico/lítico e multiplicação limitada | #83; `fase2-virus.md` | inexistente — regras fechadas em 11/11/2026 (#83), implementação em #84–#88 | M5 |
| Recorde persistente mínimo (melhor pontuação) | `decisoes.md` (04/11) | **substituído pelo Top 10 em 15/11** — só a Vitória grava, com nome; validado ponta a ponta (#34/#82) | M5 |
| Bônus +15 por run perfeito (0 erro de arma + 0 tiro na célula) | `decisoes.md` (03/11, noite) | paga 9/9 — #208 troca para run perfeito | M5 |
| SFX e animação | requisito final | inexistentes | M5 |

**Oito linhas de M4 fecharam no build de 04/11:** camada 1, camada 2, briefing, nomes das armas, disco cinza/anel, ícones neutros, Fase 2 mínima e recorde. Uma ficou parcial: o resumo mostra os contadores, não o texto por objetivo.

**RF-07 completo segue pendente.** O painel `o que a partida mostrou` traz `bactérias identificadas: %d Gram+ · %d Gram-`, `acertos de arma: %d · erros: %d`, `ataques à célula: %d` e uma frase de revisão — é leitura por contador, não o resumo por objetivo (um texto por OA). A tela de Resultados ainda não diz o que cada objetivo mostrou na partida. Fica para o M5 por decisão de 04/11.

**O bônus é por fase:** +15 com 9/9 na Fase 1 e 6/6 na Fase 2. O run perfeito (0 erro de arma + 0 tiro na célula) entra com D2.1/D2.2 no M5 (#208).

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

#### Tema visual da interface (28/10/2026)

- A interface usa a **mesma paleta dos sprites**: `#121A24` (tinta), `#1A2634` (painel), `#F2F5F7` (texto), `#A8B8C8` (texto apagado), `#FFD23F` (destaque), `#BE6EF0` (Gram+), `#F05A5A` (Gram−), `#7DF9FF` (ciano), `#6EDC78` (verde) e `#FFE894` (feedback). Não há cor avulsa na UI. *(Substituída pela paleta clínica em 27/11: §"Interface (26/11/2026)", item 10.)*
- Três níveis de texto: principal, apagado e destaque. Cor com significado fica reservada a conteúdo (espécie, raio, vida, scanner), nunca a decoração.
- Painel, botão, pips de ameaça e barra de recarga são desenhados com primitivas do Allegro; moldura de 1 px sempre com deslocamento **+0,5 px**, senão a linha cai um pixel acima do texto.
- O botão tem três estados: normal, foco do teclado (moldura de destaque) e cursor em cima (moldura de destaque + preenchimento escuro). A ativação continua no **pressionar** do mouse (down), como registrado em `interacao-mouse.md`.
- Desenho e clique leem a **mesma lista de botões** (`screen_buttons`), para a área desenhada e a área clicável nunca divergirem.
- A grade do HUD continua com 5 linhas de 16 px; mudou o conteúdo das zonas: vitais e objetivo nas linhas 0-1, sub-painel do scanner nas linhas 2-4.
- A HUD mostra **pips** de ameaça (bloco cheio = neutralizada) no lugar de `restam: 6/6`, e o tempo em **mm:ss**.
- A interface é desenhada no canvas lógico de 640×360 e ampliada por escala inteira; nenhuma função de desenho lê escala ou deslocamento da janela. A janela tem **mínimo de 640×360**, então a escala nunca fica quebrada.
- O cursor é guardado em pixel da **janela** e convertido no desenho; `resize` e `F11` releem a posição, para o hover não ficar preso no lugar antigo.
- Cor translúcida em `al_map_rgba` **não mistura**: o blend padrão espera cor pré-multiplicada, então alfa sobre o canvas satura. Fundos e molduras da UI usam cor opaca (`UI_HOVER`, `UI_LINE`, `UI_FRAME`, `UI_SLOT`); só o escurecimento da pausa usa alfa, com RGB zero.
- A fonte continua sendo a embutida do Allegro (8×8). O título do menu é o mesmo glifo ampliado 3× a partir de um bitmap montado no carregamento; a fonte definitiva segue pendente.

### Combate

#### Defesa (16/10/2026)

- A defesa é **imunidade total** a dano de contato durante **2 s**; ativa por tecla, sem custo de energia, com **3 s** de cooldown.
- Contato durante a defesa **empurra a bactéria**: 2× a velocidade dela por 0,5 s, na direção do Nanobô para o agente. Só bactérias; a célula própria é escopo da #14.
- Enquanto a defesa está ativa o disparo é ignorado; um projétil já em voo não é cancelado. Contato fora da defesa continua seguindo a regra da #11.
- Feedback: anel em volta do Nanobô — ciano durante os 2 s, cinza durante a recarga, ausente quando a defesa está pronta.
- Implementação em `src/gameplay/` (#17), com os timers dentro de `update_match` para congelarem na pausa.

#### Controles de combate (16/10/2026)

> **Substituída em 03/11/2026** para ataque e scanner: botão esquerdo atira para o cursor e botão direito escaneia o alvo sob o cursor; `ESPAÇO` e `J` saem. `X`, `1`/`2`, `ESC` e `ENTER` continuam valendo.

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

#### Trava e perseguição (04/11/2026)

- Arma errada em uma bactéria **trava o disparo por 2 s** e liga o **alerta** só naquele agente: ele persegue o Nanobô a **60 px/s** até ser neutralizado.
- A perseguição **não tem raio**: fugir não apaga o alerta. O raio de 150 px da proposta de 02/11 foi descartado.
- O empurrão da defesa continua valendo durante a perseguição; o fim do impulso não sorteia direção nova para quem está alertado.
- Trava e alerta zeram em `reset_match`/`start_phase` e no `reset_agent`.
- `DEFENSE_COOLDOWN` continua **3 s**; a calibração com pessoas fica no playtest do M5.
- O pulso de scan de 0,75 s (RF-02) e a identificação obrigatória (D2.1/D2.2) **continuam no M5**: esta rodada muda uma alavanca por vez.

#### Célula própria (18/10/2026)

> **Atualização 03/11/2026:** a regra de custo de vida vale para as duas armas (D1); não existe arma segura contra a célula. O restante continua como registrado.

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

> **Nota de 11/11/2026:** na Fase 2, a eclosão do vírus rompe a célula hospedeira e cobra 1 de vida do jogador. Isso **introduz uma segunda condição de derrota** na Fase 2, por decisão do dono em 11/11/2026, e **substitui a frase acima** no que vale para a Fase 2; a Fase 1 continua sem segunda condição de derrota. Regras em `fase2-virus.md`.

**Descartado por enquanto:** cronômetro de infecção, aceleração das bactérias e perda de pontos por tempo. Os três punem o tempo de leitura do scanner, que é o núcleo da aprendizagem, e criam um segundo estado de falha para o MVP. Se o playtest do M5 mostrar que falta pressão, a opção mais barata de ler é a **aceleração** das bactérias, nunca a perda de pontos.

### Fase 1

A Fase 1 trabalha **bactérias e células saudáveis**, incluindo a diferenciação entre bactérias Gram-positivas e Gram-negativas.

### Fase 2

A Fase 2 trabalhará **vírus**, com conteúdo relacionado aos ciclos lítico e lisogênico e uma mecânica de multiplicação limitada.

### Limite de vírus

Foi definido um limite de referência de **3 vírus** para uma mecânica de desafio. As regras exatas ainda precisam ser detalhadas e validadas.

> **Nota de 11/11/2026:** a regra fechada em #83 **não tem teto** — os 3 vírus são a composição inicial do encontro e a população cresce por geração (3 → 12 → 39), como registra `fase2-virus.md`. O "3" aqui é tamanho de encontro, não limite de vírus vivos.

### Pontuação e progressão

O jogo terá pontuação, bônus por identificação e progressão. O hi-score deverá ser persistente na versão final.

#### Identificação e neutralização (25/10/2026)

- Neutralizar uma ameaça antes de escaneá-la não a identifica: ela sai da cena e não volta ao alcance do scanner.
- Com isso, o bônus de identificação completa (#67) fica impossível naquela fase; a fase conclui normalmente, porque identificação não é requisito de vitória (#26).
- Não existe caminho alternativo para identificar um agente neutralizado.
- O HUD mostra as ameaças restantes e a mensagem avisa no momento em que o bônus se perde.

### Artes do Nanobô (v2)

- O sprite do Nanobô continua **32×32**; a ideia de um quadro 48×48 foi descartada — aumentar a resolução não garantia ganho visual.
- O personagem é **novo, inspirado no anterior**: mesma identidade (antenas de ponta ciano, viseira escura com dois olhos ciano, pluma de exaustão embaixo), silhueta e detalhes redesenhados.
- A tira de quadros desenha **as 4 direções desenhadas uma a uma** (cima, baixo, esquerda e direita), nas poses **frente / costas / perfil**. Nada de espelhar um lado no código.
- Grade da tira: **8 quadros por direção** (1–4 parado, 220 ms cada; 5–8 andando, 110 ms cada), 32×32 por quadro, tira final **256×128**, com as linhas na ordem do `player_facing` (`UP`, `DOWN`, `LEFT`, `RIGHT`) — assim o código acha a linha sem tabela de conversão.
- Os arquivos novos são criados **ao lado dos antigos, sem alterá-los** (`nanobo_v2.png`, `nanobo_spritesheet_v2.png` e a fonte `.aseprite`). A troca no jogo é etapa separada.
- **Entregue em 11/11/2026:** a folha saiu com **256×128**, linhas na ordem `UP`/`DOWN`/`LEFT`/`RIGHT` e 8 quadros por linha. Foi gerada por script (`Nanobo-testes/gerar_spritesheet.py`, fora do repositório) e a troca não esperou arquivos `_v2`: o `nanobo_spritesheet.png` do jogo passou a ser a folha nova, e a arte antiga ficou no histórico do git. A linha da direita é o **espelho** da esquerda, feito na geração da arte — o código continua sem espelhar nada.

### Resolução, escala inteira e câmera (22/10/2026)

- A tela lógica é **640×360** (16:9), desenhada num bitmap e ampliada na janela por **escala inteira**, com barras pretas quando a janela não for múltipla. 640×360 é a maior resolução 16:9 que cai exata em 720p (2×), 1080p (3×), 1440p (4×) e 4K (6×).
- A janela é **redimensionável** e abre **maximizada** (`ALLEGRO_MAXIMIZED`); o canvas continua centralizado em escala inteira, então sobra barra preta nos lados quando a janela não for múltipla de 640×360. **F11** alterna fullscreen — em 1440p isso dá 4× exato, sem barras — e, ao sair, a janela volta **maximizada** (não para o tamanho pedido na criação). Escala inteira quando o encaixe der 1× ou mais; janela menor que 640×360 encolhe proporcionalmente.
- A **faixa visível do mundo** é 640×320 desde 15/11 (era 640×256) e o **HUD** ocupa os 40 px do rodapé, em duas linhas (#243). A **ficha do scanner** só aparece com alvo e flutua dentro da faixa do mundo:
  - fica embaixo, ou em cima quando o alvo está na metade de baixo, para nunca cobrir o agente lido;
  - o aviso de feedback e a marca do destino desviam dela;
  - "há alvo" quer dizer agente sob o cursor dentro do alcance, ou a última leitura ainda ativa e ao alcance.
- O **mundo** nasce com **1920×512** (3 telas por 2 faixas) e uma **câmera** presa nas bordas acompanha o Nanobô. O tamanho do mapa passou a ser constante (`WORLD_W`/`WORLD_H`), não o tamanho da tela: é o que permite crescer para paredes e caminhos sem reescrever as regras.
- A Fase 1 passa a ter **9 agentes** (3 Gram+, 3 Gram−, 3 células) para não esvaziar o mapa maior; o **alcance do scanner continua 130 px**.
- O scanner mostra **só a leitura do alvo atual**; `space` de novo relê o alvo. A lista de todos os agentes saiu: ela estourava o painel (12 linhas de 14 px = 168 px num espaço de 110 px).
- O HUD não usa coordenada fixa: cada linha sai de `hud_y()`, que soma `SCAN_PANEL_Y`, `HUD_MARGIN` e `line * HUD_LINE_H`. São 2 linhas a 16 px uma da outra, com 8 px de margem, nos 40 px do painel (#243).
  - **Linha 0:** as duas barras, pontos e tempo à esquerda; a arma com o alvo, o ícone, a recarga e o estado à direita, medidos da borda.
  - **Linha 1:** o objetivo à esquerda; as ameaças e os identificados à direita.
  - A regra "o mundo tem um número inteiro de faixas" (`WORLD_H % PLAY_H`) virou "a faixa cabe no mundo", porque a câmera já para em `[0, WORLD_H - PLAY_H]`.
- Princípio de escala inteira para pixel art conforme a documentação do Godot sobre múltiplas resoluções; 1920×1080 como resolução de exibição mais comum.

### Fecho da fase e telas do MVP (26/10/2026)

- A conclusão da Fase 1 (6 ameaças neutralizadas + Nanobô no destino) leva a `RESULTS` automaticamente, uma única vez, sem tecla de teste (#70).
- `RESULTS` apresenta os números reais da partida: pontos, ameaças neutralizadas/total, agentes identificados/total e se o bônus de +15 foi pago (#57, #171–#176).
- Com uma fase no MVP, `RESULTS` continua para `VICTORY`; `VICTORY` e `GAME OVER` iniciam nova partida por `ENTER`, com reset completo de vida, pontuação, identificação, agentes, projéteis, cooldowns e timers (#52, #72, #74, #75).
- A preferência de laser (`1`/`2`) **persiste** entre partidas: é escolha do jogador, não estado da partida. O reset não a altera.
- `F`, `G` e `H` são teclas de QA e existem **somente no build Debug**; o Release não permite saltar estados (#76).

### Pontuação, bônus e composição da Fase 1 (registro de 25/10/2026)

- A Fase 1 tem **9 agentes**: 3 bactérias Gram+, 3 Gram− e 3 células saudáveis.
- São **6 ameaças** (as bactérias); as células não contam para a conclusão (#26).
- Cada ameaça neutralizada vale **+10 pontos**, uma única vez (#66).
- O bônus de **+15** é pago apenas na conclusão com `identificados = 9/9`, uma única vez (#67).
- `9/9` é condição de bônus, **não** de vitória: a fase conclui com menos identificações (#26, #67).
- Os valores são os já implementados e verificados; a calibração de dificuldade continua em aberto.

### Pausa por perda de foco (26/10/2026)

- Quando a janela perde o foco durante `GAME`, a partida entra em `PAUSE`; o retorno é por `ESC`, como na pausa normal.
- A regra existe para que alt-tab, notificação ou troca de janela não causem dano ou Game Over sem o jogador estar vendo o jogo.

### Texto do scanner e aprendizagem (26/10/2026)

> **Substituída em 03/11/2026:** o pulso 2 passa a nomear o grupo e a paleta deixa de ligar cor da bactéria à cor do módulo (ver “Ensino integrado da Fase 1”).

- A camada 2 descreve o envelope (parede espessa sem membrana externa / parede fina sob membrana externa) e **não nomeia Gram+ nem Gram−**.
- A regra que liga a pista à classificação e ao laser é ensinada uma vez na tela inicial: parede espessa = Gram+ = raio roxo; parede fina com membrana externa = Gram− = raio vermelho.
- O feedback posterior à ação pode nomear a classificação; o painel do scanner, não.
- A troca da paleta das bactérias (cor deixa de coincidir com o laser) fica para o M5, condicionada a playtest.

### Triagem de menus e HUD (28/10/2026)

- **Feedback em uma cor.** `set_feedback` recebe só o texto e todas as mensagens saem em `UI_WARM`; a consequência está na frase ("ameaça neutralizada: +10 pontos" × "parede Gram+ espessa: o raio vermelho não penetra"), não na cor. Um `feedback_kind` atravessando `gameplay.c` não paga o custo. Reabrir se o playtest do M5 mostrar jogador confundindo ganho com perda.
- **Botão com três estados.** Normal, foco e hover; não existe estado "pressionado" porque a ativação acontece no *down* e a tela troca no mesmo evento, então esse quadro nunca seria desenhado. Revisitar só se aparecer botão que não navega (um toggle de configuração).
- **`main.c` sem extração por enquanto.** O arquivo concentra loop, eventos, kit de UI, desenho das seis telas e entrada; extrair `ui.c` mexeria no `<ClCompile>`, no `AGENTS.md` e no guia de módulos por ganho organizacional, não funcional. Gatilho objetivo para reavaliar: `main.c` acima de 2.000 linhas, uma terceira tela nova, conflito de merge recorrente no arquivo ou build perceptivelmente lento.
- **Tile de fundo adiado na triagem.** O mundo era preto e a arte antiga (`fundo_microscopico.png`, 640×480) não cobria 1920×512 nem tiling. A decisão foi superada pela dupla: o commit `ca4845a` trouxe `mapa_fase1.png` (1920×512) e `load_resources()` passou a carregá-lo atrás de destino, agentes e Nanobô.
- **Fonte embutida mantida; o que faltava era a acentuação.** Os textos agora são escritos em português correto em UTF-8 e o build usa `/utf-8` (ver `docs/planejamento/guia-refatoracao-modulos.md`); trocar a fonte por TTF ou bitmap continua fora do MVP.

### Ensino integrado da Fase 1 (03/11/2026)

- **Briefing.** Antes da Fase 1, três cartões puláveis: resultado da coloração; comparação das camadas externas; armas como modelo do jogo e preservação da célula própria. ENTER, → ou clique avançam; ESC pula. Não há tutorial guiado depois, e o Diário de Campo fica fora do M4.
- **D1 — célula própria.** No jogo, **as duas armas ferem a célula própria** (−1 vida). É consequência de gameplay; não afirma nada sobre efeitos adversos de medicamentos reais. Substitui a proposta de 02/11 em que só a Dissolução feria.
- **D2 — scanner.** O pulso 1 mostra o resultado da coloração (**roxo** ou **vermelho**) e o **nome do agente** (`Staphylococcus aureus`); o pulso 2 mostra o **nome do grupo** (Gram+ / Gram−), a pista estrutural e o porquê, sem indicar a arma. O nome do agente é identificação, não classificação: não diz o grupo nem a arma. Substitui a decisão de 26/10 (camada 2 sem nome) e a leitura de 22/10 (camada 1 só com o nome).
- **Armas Ruptura e Dissolução.** A **Ruptura** age sobre a parede e neutraliza Gram+; a **Dissolução** age sobre a camada externa e neutraliza Gram−. São nomes de efeito, não de medicamentos; a matriz é regra do jogo, não previsão de tratamento real. As duas armas usam **cor neutra**, e ícone e projétil se distinguem pela **forma**, não pela cor. Substituem os rótulos A/B e a paleta roxo/vermelho da #16. No código, `Laser` e `LASER_*` continuam: renomear identificador não muda nada para o jogador.
- **Coloração: roxo e vermelho.** O resultado é **roxo** (Gram+) e **vermelho** (Gram−). "rosa" fica como sinônimo nos documentos de ensino, nunca na tela: o destaque do Gram− é `#F05A5A`, e escrever "rosa" ao lado de um vermelho recriaria a divergência que esta rodada fecha.
- **Arte neutra e anel de coloração.** Até a camada 1, as duas bactérias são um **disco cinza** desenhado por primitiva, sem forma nem cor que entreguem o grupo. Depois da leitura, o agente recebe o **sprite da espécie** (com a variante sorteada) e um **anel colorido permanente** (roxo ou vermelho). A célula própria mantém sprite próprio, maior, desde o início.
- **Variação de sprites (03/11, noite).** Cada espécie pode ter **mais de um sprite**, sorteado por agente no nascimento, para variar o cenário. Regras: o **mesmo número de variantes por grupo** (senão a variante vira pista), 32×32, a mesma hitbox (`AGENT_RADIUS`), e a variante só aparece **depois** da camada 1 — antes dela todos são o mesmo disco cinza. A quantidade fica para o M5; a regra fica decidida.
- **Espécies do cenário (03/11, noite).** Os seis binômios são **agentes do cenário**, não uma alegação de que a espécie cause doença. *Bacillus subtilis* sai (não é patogênico) e entra *Streptococcus pyogenes* (Gram+); *Escherichia coli* fica como exemplo canônico de Gram−. O briefing carrega a linha "os agentes desta missão são ameaças do cenário; nem toda bactéria causa doença".
- **Camada 1 mantém o nome do agente.** Decisão de 03/11 (noite): o nome científico é a identidade do agente e continua na camada 1, junto da coloração. O jogo pode nomear espécies; o que ele não faz é inferir patogenicidade, tratamento ou classificação clínica a partir do nome.
- **Mouse no jogo.** Botão esquerdo atira na direção do cursor; botão direito escaneia o agente sob o cursor (tolerância de 24 px, alcance de 130 px); roda do mouse e `1`/`2` trocam a arma — o clique no ícone do HUD foi **descartado** em 03/11 (noite). `ESPAÇO` e `J` deixam de ser ataque e scanner; o teclado fica com mover, defender, arma, pausa e navegação.
- **Feedback.** Cada decisão tem uma mensagem curta (até 2 linhas) que explica a pista ou a regra do jogo; o primeiro erro traz uma pergunta e o segundo explica o mecanismo, sem termos moleculares e sem recomendar tratamento.
- **Resumo e contadores.** A tela de Resultados usa apenas contadores em memória (scans por grupo, decisões certas e erradas, ataques a células próprias), zerados em `reset_match()`; sem CSV nem telemetria no M4.
- **Bônus de identificação no M5.** Com a identificação obrigatória (D2.1), o +15 deixa de pagar 9/9 — paga **run perfeito**: `wrong_decisions == 0` **e** `cell_attacks == 0`. O resumo diz por que o bônus não foi pago. A regra do build até o M5 continua 9/9.
- **Arquitetura.** Só os quatro arquivos compilados: briefing, HUD e resumo em `main.c`; regras, contadores e feedback em `gameplay.c`. Nenhum módulo `src/ui/`.
- **D2.1 — a identificação é condição de conclusão (03/11/2026).** Vencer sem escanear **não é aceitável**: identificar os agentes deixa de ser apenas bônus e passa a ser exigência para concluir a fase. Substitui, por decisão da dupla, a regra de #26/#67, que concluíam com 0 identificações. A implementação depende de **D2.2** (em Pendências) e ainda não está no código. **Entrega:** o M4 de 04/11 mantém a regra atual (#26, conclui com 0 identificações); D2.1 e D2.2 ficam para o M5.
- **O menu não entrega a matriz arma→grupo.** Com o pulso 2 nomeando o grupo (D2), o menu pode dizer a função (`1: Ruptura - age na parede celular`), mas não pode afirmar `Ruptura (Gram+) · Dissolução (Gram-)`: isso entregaria antes do primeiro scan a informação que o scanner existe para produzir. A função da arma continua no menu; a ligação com o grupo sai.
- **Fora do M4.** Diário de Campo, zonas guiadas, registro de eventos e calibração de trava/perseguição só voltam ao backlog depois do build e do QA manual. A arte neutra, o anel de coloração e os nomes das armas estão decididos; os assets entram conforme o escopo do marco (ver Pendências). A Fase 2 completa (ciclo viral) continua pós-MVP; a mínima entrou no M4.

### Fase 2 mínima e recorde (04/11/2026)

O M4 continua centrado na Fase 1; a **Fase 2 mínima** existe para fechar o fluxo entre fases (#71/#76) e não substitui a Fase 2 do M5.

- **Composição:** 3 vírus e 3 células saudáveis, no mesmo mapa e destino da Fase 1; a tabela de fases vive em `src/entities/entities.c`.
- **Vírus:** 1 ponto de resistência; **qualquer arma** o neutraliza. O ciclo lisogênico/lítico, a multiplicação de até 3 e as consequências de combate continuam no M5 (#83–#91).
- **Scanner do vírus:** camada 1 = `vírus · Vírus da fase`; camada 2 = “Vírus precisa de uma célula hospedeira para se multiplicar” + “a célula hospedeira é o que ele usa para se multiplicar”. Sem amostra de cor e sem anel: não há coloração de Gram.
- **Contato e defesa:** o vírus fere por contato e é empurrado pela defesa, como a bactéria.
- **Vida entre fases:** cada fase começa com 3 vidas.
- **Bônus por fase:** +15 quando todos os agentes **daquela fase** foram identificados (9/9 na Fase 1, 6/6 na Fase 2); a pontuação da run atravessa as fases.
- **Resumo por fase:** os contadores zeram a cada fase; a linha de Gram só aparece na Fase 1.
- **Resultados → próxima fase:** `CONTINUAR` carrega a Fase 2 e preserva a pontuação; na última fase leva a `VICTORY`. *(25/11: com a Fase 3, a última fase passou a ser a 3; a regra é a mesma, `has_next_phase()`.)*
- **Recorde mínimo:** a melhor pontuação da run fica em `nanobo_hiscore.txt`, ao lado do executável, e aparece no menu e na Vitória. Sem nome (#81) e sem Top 10 (#77–#82), que ficam no M5; arquivo ausente ou inválido vale zero.
- **RF-07 completo (texto por objetivo) fica no M5:** o M4 entrega o resumo por contadores (#174, #202); a tela de 640×360 não comporta quatro blocos por OA sem refazer o layout.
- **Ícones das armas:** `icone_ruptura.png` (parede de tijolos) e `icone_dissolucao.png` (anéis concêntricos), 16×16, cor neutra, forma própria; o projétil continua quadrado/círculo. Os ícones roxo/vermelho saem do `load_resources()` e ficam no disco como histórico.

### D2.2 e elegibilidade do recorde (05/11/2026)

- **D2.2 — opção (a), decidida em 05/11/2026.** Quem atira antes de escanear não reduz a resistência do agente: o projétil acerta, gasta o disparo e a tela avisa, mas o agente continua ativo e escaneável. Nenhum agente pode ser neutralizado sem identificação. Implementação em #206.
- **D2.1 segue decidida (03/11).** A fase só conclui com todos os agentes identificados: 9/9 na Fase 1 e 6/6 na Fase 2. Entra junto com D2.2 (#207).
- **Recorde só na Vitória.** A melhor pontuação é a de uma execução vencida. Game Over não grava recorde; abandonar pelo menu também não. `finish_run()` deixa de ser chamado no Game Over (`src/main.c:2108`) — pendente no build de 04/11, que ainda grava. Vale para o recorde mínimo e para o Top 10 (#34, #82).
- **Sem corte de escopo em 05/11.** A Fase 2 completa (#55/#83–#88/#91) e o Top 10 (#34/#77–#82/#197) continuam no M5. O marco fica com 85 issues e 228 SP brutos; 195 SP sem as oito agrupadoras.

### Top 10 (15/11/2026, #77–#82 e #197)

Substitui o recorde mínimo de 04/11. As regras de #34 continuam: 10 posições, nome de até 10 caracteres, empate com o registro mais antigo em cima e gravação só na Vitória.
- **Local do arquivo (#77):** `nanobo_ranking.txt` na pasta do jogo, junto de `assets/` e do recorde antigo, por decisão do dono em 15/11. O caminho é relativo, como o dos assets. Antes ficava em `%APPDATA%\Nanobo\ranking.txt`.
  - **Consequência para o instalador (#43):** em `Arquivos de Programas` o Windows não deixa um usuário comum gravar. Por isso o instalador precisa pôr o jogo numa pasta gravável, como a instalação por usuário em `%LOCALAPPDATA%\Programs\Nanobo`; senão a Vitória avisa que não gravou.
- **Formato (#77):** texto. A 1ª linha é `NANOBO-RANKING 1`, e cada uma das seguintes é `pontos nome`, com o nome em UTF-8.
- **Arquivo fora do esperado (#77, #78):**
  - ausente: ranking vazio, e o recorde antigo (`nanobo_hiscore.txt`) entra como o registro "Recorde", para a troca não apagar o que já foi conquistado;
  - vazio ou sem o cabeçalho: ranking vazio;
  - linha inválida (sem número, número negativo ou sem nome): ignorada;
  - a leitura ordena de novo e, no empate, mantém a ordem do arquivo.
- **Inserção (#79):** com o ranking cheio, só entra quem passa o último; empatar com ele não basta.
- **Gravação (#80):** o jogo grava num `.tmp` e só então troca pelo arquivo antigo, para não deixar um ranking pela metade. Se falhar, o ranking fica na memória e a Vitória avisa.
  - **Troca sem janela de perda (16/11, auditoria):** o antigo vira `nanobo_ranking.txt.bak`, o `.tmp` assume o nome e o `.bak` é apagado. Se a troca falha (arquivo preso pelo sistema ou pelo OneDrive), o antigo volta para o lugar. Se o jogo cai entre os dois passos, a leitura usa o `.bak`. Antes, o arquivo era apagado antes do `rename`, e uma falha ali perdia o Top 10 inteiro.
  - **Harness:** 4 casos novos (334/334); o código antigo falha nos 4.
- **Nome (#81):**
  - é pedido na Vitória só quando a pontuação entra no Top 10, e ENTER ou ESC confirmam;
  - vazio vira "Jogador";
  - aceita só os caracteres que a fonte desenha (Latin-1), e a maiúscula acentuada vira minúscula, porque a fonte erra o glifo dela (§2.1 de `textos-do-jogo.md`).
- **Exibição (#197):** um painel, sem cena nova. Na Vitória, o registro novo fica em destaque; no menu, o painel abre pelo botão RECORDES e fecha com ENTER, ESC ou clique.
- **Caminho com acento:** com o caminho relativo, o `fopen` funciona mesmo dentro de uma pasta acentuada (o repositório fica em `2° Semestre`). A limitação do caminho absoluto em `%APPDATA%` saiu junto com ele.
- **Validação ponta a ponta (#82, 15/11):** feita em duas frentes.
  - **Harness** (`../Nanobo-testes/`, 17 casos do Top 10, 328/328): ranking vazio, parcial e cheio; corte com o ranking cheio, inclusive o empate com o último; empate com o mais antigo em cima; nome com corte em 10 caracteres, vazio e com acento; gravar e reler; arquivo sem cabeçalho, vazio ou com linhas inválidas; importação do recorde antigo.
  - **Jogo** (Debug, `qa_jogo.ps1`, teclas F e G):
    - Vitória com o nome "pedra" e 210 pontos: posição 2, em destaque. Depois de fechar e reabrir, o painel RECORDES do menu mostra o mesmo ranking.
    - Vitória com o nome vazio: entra "Jogador" na posição 3.
    - NOVA PARTIDA depois da Vitória e depois do Game Over: a fase começa com 0 pontos e 00:00, e o Top 10 continua no arquivo.
    - Game Over com 0 pontos: o arquivo não ganha linha nova.
  - **Defeito achado e corrigido:** o rodapé do painel RECORDES no menu ficava em cima dos botões.

### Fase 3 de fungos (25/11/2026)

Decisões do dono em 24–25/11/2026 para a #226 (#227–#233). As regras implementadas estão em [`fase3-fungos.md`](fase3-fungos.md).

- **D1 — onde trabalhar:** branch `feat/fase3`; o merge na `master` só acontece com o sim do dono.
- **D2 — armas (#227 × #230):** **Perfuração** na tecla 1 e **Ruptura** na tecla 2; a tecla 3 fica sem arma e a roda alterna as duas.
- **D3 — arte:** sprites e mapa gerados por script Lua no Aseprite, no estilo aprovado em 14/11. O dono aprovou os sprites e o mapa em 25/11.
- **D4 — carga de esporos (#231):** extras = (1 se os vitais ao fim da Fase 2 < 80) + (1 se < 60) + cargas de Reserva gastas na partida, no máximo 4.
- **D5 — esporo:** voa 1,5 s, pousa e vira ameaça no chão (1 tiro de Perfuração mata), germina em colônia em 6 s; com 4 colônias vivas, espera a vaga. Os esporos da carga inicial seguem a mesma regra.
- **D6 — hifa:** cresce rumo à célula do paciente mais próxima, fixada quando a colônia nasce; machuca o Nanobô (−10) e não fere a célula.
- **D7 — silhueta:** fungos aparecem como silhueta até a 1ª camada; a hifa é desenhada desde o início, porque é parede que machuca.
- **D8 — arma inicial:** a Fase 3 começa com a Ruptura na mão.
- **Esporo sem pontos e sem dano de contato (25/11, depois da revisão da Task 4):** o esporo, em voo ou pousado, não dá pontos; o pousado não causa dano de contato. O que fere é o esporo em voo que acerta o Nanobô (−8).
- **Decisões técnicas (aceitas na revisão):**
  - **R1:** partida e destino continuam os globais; a "partida e destino próprios" da #227 fica como dívida.
  - **R2:** sem correnteza.
  - **R3:** fungos não atacam células; elas só perdem vitais por tiro do Nanobô (−15).
  - **R4:** a dose de Perfuração na levedura não expira e não cria resistência.
  - **R5:** antibiótico em fungo é arma errada (sem efeito, trava de 2 s, FB2-11).
  - **R6:** sem bots de calibração para a Fase 3.
- **Perfuração e vitais:** "não custa vitais" vale para o efeito colateral da arma, que o dono não escolheu; Perfuração em célula do paciente continua custando 15 pelo fogo amigo.
- **Hifa:** age como parede (empurra o Nanobô e tira 10 no máximo uma vez por segundo) e a defesa não protege dela.
- **Estado:** harness de regras 470/0, Debug e Release com 0 avisos, pico de memória de ~78,25 MB nas três fases.

### Narrativa e conteúdo (25/11/2026)

Decisões do dono em 25/11/2026, depois da verificação do conteúdo de biologia com fontes (relatório de pesquisa e notas em `.hermes/`; o resultado está em [`conteudo-biologico.md`](conteudo-biologico.md) §"Verificação com fontes" e em [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md)). O veredito da pesquisa foi que nenhuma afirmação estava errada e que os problemas eram de enquadramento. **Só documentação**: nenhum código nem texto do jogo (`textos-do-jogo.md`) mudou nesta rodada; os textos novos serão redigidos depois, para aprovação.

1. **Narrativa: Diário do Nanobô e o Paciente Léo.**
   - Os cards de conceito viram o **Diário do Nanobô**: **Anotações** em primeira pessoa, escritas pelo Nanobô (um protótipo recém-ligado, de memória vazia) sobre o que descobre. A caixa de diálogo é paginada: máquina de escrever de cerca de 40 caracteres por segundo; a mesma tecla completa a página e depois avança; ▼ piscando; "fechar" na última página; contador "2/4"; 0,2 s de debounce; jogo pausado.
   - Os **Interlúdios** contam a história do **Paciente, Léo Martins Ferreira, 16 anos**: na Fase 1 a infecção no sangue melhora; na enfermaria ele pega um vírus respiratório (Fase 2); com a defesa enfraquecida, esporos de mofo chegam ao pulmão (Fase 3). **Vitória = alta. Game Over = a equipe retira o Nanobô e Léo segue o tratamento convencional; não há morte.**
   - Abertura = ficha do hospital + "Anotação 0". O relatório de fim de fase vira uma página do diário, **"O que anotei nesta fase"**. Um **menu para reler o diário fica fora do escopo de 02/12**.
   - Tom: frases curtas, um termo-chave colorido por página, nada que os documentos não sustentem.
   - *Por quê:* o 2.0 saiu da auditoria como "raso no ensino" e "sem narrativa"; o diário em primeira pessoa dá voz ao ensino sem aula expositiva, e um paciente com nome e idade dá ligação emocional às três fases e à perda. O Game Over sem morte mantém o tom de jogo educativo para o ensino médio. O nome completo substitui a regra anterior de "o paciente não tem nome nem gênero no texto" ([`gdd.md`](gdd.md) §"Narrativa").
2. **Espécies: trocar as que contrariam a regra das armas.**
   - ***Neisseria meningitidis* sai, *Moraxella catarrhalis* entra** (coco Gram−, diplococo); ***Bacillus cereus* sai, *Clostridium tetani* entra** (bacilo Gram+). *S. aureus* fica, com a nota de que muitas cepas resistem à penicilina.
   - Uma linha do diário diz que as armas são **regra do jogo, inspirada em antibióticos reais**, e que **no jogo todas as espécies aparecem juntas no sangue, por simplicidade**.
   - *Por quê:* o meningococo é Gram− mas é tratado com penicilina e não responde à polimixina (inverte os dois lados); *B. cereus* é Gram+ com β-lactamase potente e resiste à "arma de Gram+". *M. catarrhalis* resiste à penicilina e, em geral, é sensível à colistina; *C. tetani* é sensível à penicilina G (Guia de Vigilância do Ministério da Saúde) e não responde à polimixina por ser Gram+. Ressalva: nenhuma das duas é típica de corrente sanguínea, e a fonte da polimixina para *Moraxella* é fraca. Detalhe e fontes em `conteudo-biologico.md`.
3. **A arma "Lise" será renomeada** (proposta: **"Ataque T"**, que evoca o linfócito T).
   - *Por quê:* "lise" é, no ensino médio, o rompimento da célula pelo vírus no ciclo lítico, e a arma que "destrói a célula infectada" com esse nome confunde as duas coisas. A renomeação **no código e nos textos acontece depois**; até lá, os documentos (inclusive `fase2-virus.md` e `gdd.md`) ainda dizem "Lise".
4. **A analogia dos ciclos lisogênico e lítico entra no jogo.** No primeiro encontro com uma célula infectada, o diário diz que esses ciclos foram descritos em vírus de bactérias (bacteriófagos) e que aqui são uma analogia.
   - *Por quê:* a nota só existia nos documentos internos, e sem ela a descrição é enganosa para vírus humanos (em que o análogo real é a latência, como no herpes; não há fonte que atribua isso ao vírus da gripe).
5. **Texto da resistência.** "Siga a prescrição até o fim"; a resistência é explicada por **seleção** (as sobreviventes passam a resistência às filhas); a mecânica de 2 doses e 6 s é apresentada **como regra do jogo**.
   - *Por quê:* "uma dose e mais de 6 s sem a segunda tornam a bactéria resistente" soa lamarckista; a posição brasileira (Agência Brasil, Anvisa, Fiocruz) é "siga a prescrição", e o jogo evita a controvérsia de que parar cedo cause resistência (BMJ 2017).
6. **Correções de ponto aplicadas nos documentos.**
   - **CC-05c:** o risco vem da defesa fraca, não da quantidade de esporos;
   - **RT-10:** "sem metabolismo nem ribossomos próprios";
   - **CC-01a** (o corante roxo fica preso ou sai conforme a parede) e **CC-02b** ("esta resistiu... nem nas filhas");
   - a **Perfuração** cita a anfotericina B como inspiração;
   - evitar "só" nos textos de fungos oportunistas (*C. gattii* também afeta pessoas saudáveis).
   - Nos documentos já estão as descrições e regras; as redações finais dos textos ficam **pendentes de redação**.
   - *Por quê:* a verificação com fontes mostrou que são enquadramentos inexatos, e não erros de fato.
7. **Termos acima do ensino médio ficam fora do texto do jogador** (LPS, β-lactamase, transpeptidase); **"LPS" vira "toxina da membrana externa"** no jogo. Os documentos mantêm o termo técnico, com fonte.
   - *Por quê:* BNCC organiza competências amplas e não pede esses termos; o público é o ensino médio.
8. **Sprites "fora de escala".** Só a ordem **vírus < bactéria < célula** é mantida; deixa de existir o rótulo "escala biológica".
   - *Por quê:* a razão real de tamanhos é muito maior que 16:64 px, e os tempos do jogo (12 s, 6 s, 10 s) também não são reais; "escala biológica" prometia o que o jogo não entrega.

**Pendente no código** (fora desta rodada): a troca de espécies nos dados e textos, a renomeação da Lise, a caixa de diálogo paginada, as telas do diário e dos interlúdios e a revisão de `textos-do-jogo.md`.

### Interface (26/11/2026)

Decisões do dono em 26/11/2026 para o redesenho das telas fora do Diário (#245) e a arte nova da interface (#246). Plano: design, inventário de textos e arte em 28/11; implementação em 29/11; 30/11 de folga.

1. **Arte em issue própria (#246).** Barras de vida, mira e fundos; a #245 fica com textos e layout. A arte é gerada por script Lua no Aseprite (`../Nanobo-testes/gerar_interface.lua`) com a paleta atual, e cada imagem é aprovada pelo dono antes de entrar em `assets/`.
2. **Fundos.** Quatro temas de 640×360: menu (vaso sanguíneo com o Nanobô e hemácias à deriva, o Nanobô flutuando), "como jogar", e o quarto do hospital em três luzes: abertura (fria, monitor acelerado), Vitória (dia, leito arrumado: alta) e Game Over (baixa, monitor estável). O Léo não tem rosto: é uma silhueta sob o lençol. Resultados e Pausa usam o mapa da fase atual escurecido.
3. **Vida.** Integridade do Nanobô: ícone da cabeça do robô e barra de bateria em 10 segmentos. Vitais do Léo: coração que pulsa mais rápido com vitais baixos e barra com linha de ECG em 10 segmentos. Cor por faixa (verde, âmbar, vermelho), com os limites atuais.
4. **Mira.** Sprite 16×16 com três quadros (pronto, recarga, travado), tingido com a cor da arma; só dentro do jogo. Fora dele fica o cursor do Windows.
5. **HUD continua com 40 px.** As barras viram uma coluna à esquerda (Nanobô na linha 0, Léo na linha 1); pontos e tempo vão para a linha 1; o objetivo da fase sai do HUD e vira um **aviso de objetivo** de 4 s no topo do mundo (início da fase e mudança de meta) e um texto completo na Pausa. `PLAY_H` não muda.
6. **"Como jogar" vira cena própria** (`GAME_SCENE_HOWTO`), em 3–4 páginas com ícones (mover e mirar; scanner; armas; Integridade e Vitais). Abre do menu e da Pausa e volta para a cena de origem; aberta da Pausa, a partida continua parada. O menu fica com INICIAR, COMO JOGAR e RECORDES.
7. **Ficha do scanner** ganha retrato 24×24 do agente lido (silhueta antes da camada 1) e três marcadores de camada (forma, Gram, espécie), sem aumentar a altura.
8. **Execução por subagentes**, como no Diário, com inventário "atual × proposto" aprovado pelo dono antes do código.
9. **"Arma" em todo texto** (revisão do inventário, 26/11): o recurso que o jogador escolhe se chama **arma** em todo texto do jogo e nos docs vivos, como o glossário já definia; "ferramenta" sai. Os controles do "como jogar" ficam no formato direto "tecla: ação", sem primeira pessoa; as páginas que ensinam (scanner, barras) seguem na voz do Nanobô. Os nomes no código (`select_tool`, `Laser`) não mudam.
10. **Teclas em caixinha e paleta clínica** (decisão do dono, 27/11). Todo nome de tecla ou botão nos textos (`[enter]`, `[botão direito]`, `[1]`) é desenhado como tampa de tecla: nome em `UI_KEY` numa caixa de 14 px com borda `UI_KEY` e fundo `UI_INK`, 3 px de folga de cada lado (+6 px por tecla na largura; ajustado de 12/2 px depois das capturas, para a borda não encostar nos acentos e cedilhas); os colchetes não aparecem nem contam nos caracteres. A interface troca a paleta pela **clínica em azul-petróleo**: `UI_INK` `#0F1922` (fundo), `UI_PANEL` `#16242F`, `UI_FRAME` `#3A5566`, `UI_TEXT` `#EEF3F6`, `UI_DIM` `#9DB2C2`, `UI_AMBER` `#FFC94A` (termos de biologia, foco de botão, objetivo), `UI_KEY` e `UI_CYAN` `#6FE7F2` (teclas; integridade, alvo sob o cursor, dica de escanear), `UI_RED` `#FF6B6B` (perigo), `UI_GREEN` `#5FD6A0` (bom) e, derivadas na mesma família, `UI_SLOT` `#25394A`, `UI_LINE` `#22343F`, `UI_HOVER` `#3A3218` e `UI_WARM` `#FFE7A8`; os véus das telas passam de preto a tinta (`UI_VEIL`). `UI_PURPLE` e `UI_PINK` ficam: são o roxo e o rosa da coloração de Gram. Contraste: texto sobre painel 14,2:1, texto apagado sobre painel 7,2:1, tecla 12,2:1 e âmbar 11,6:1 sobre a tinta.
    - *Por quê:* no "como jogar" todo o texto tinha a mesma cor e as teclas se perdiam no meio da frase; a caixinha as destaca sem mudar as palavras aprovadas. A paleta alinha a interface ao azul-petróleo do fundo do "como jogar" e do quarto do hospital.
11. **Cor por marca nos textos** (decisão do dono, 27/11). Três categorias, marcadas no próprio texto com `<c:texto>`: o grupo de Gram na cor do corante (`p` `Gram+` em `UI_PURPLE`, `r` `Gram-` em `UI_PINK`), a arma na cor da mira e do tiro (`wR` Ruptura, `wD` Dissolução, `wS` Reserva, `wN` Neutralização, `wT` Ataque T, `wF` Perfuração, por `laser_color()`) e a consequência (`g` ganho em `UI_GREEN`, `x` perda em `UI_RED`, só no trecho do resultado). Por quê: o jogador lê o grupo de Gram, a arma e a consequência pela cor, ligando o texto ao que vê no mundo (a amostra do scanner, a mira, o tiro). As marcas não contam nos caracteres nem na largura e não se aninham; regra e tabela em [`textos-do-jogo.md`](textos-do-jogo.md) §2.1.

- *Por quê:* a auditoria apontou telas que não explicam o jogo; o HUD estava no limite de largura (20 px livres) e o objetivo ali competia com as barras; fundos desenhados dão ao menu e ao hospital o mesmo cuidado visual das fases sem custo relevante de memória (~0,9 MB por fundo, com pico atual de 79 MB e limite de 100 MB).

### Refatoração pós-entrega (06/11/2026)

- **Estrutura-alvo enxuta.** As cinco casas continuam: `main.c` (loop, recursos, entrada e composição das telas), `entities/`, `gameplay/`, `core/` e `scenes/`. A Wave 2 acrescenta só `src/ui/` (menus, HUD, painel do scanner, resultados, botões e textos) e `src/render/` (mapa, agentes, projéteis, destino e mira). `app/`, `input/`, `resources/` e `persistence/` não entram.
- **`gameplay.c` não é dividido.** Scanner, combate, movimento e progressão continuam no mesmo arquivo; a divisão em quatro módulos foi descartada por não resolver defeito nenhum antes da entrega.
- **Sem `GameState`.** As escritas diretas que a UI e os atalhos de QA fazem hoje (`player.laser`, `agents[]`) passam por funções estreitas (`select_laser`, `debug_complete_phase`, `debug_identify_all`) quando a Wave 2 começar; não há struct central de estado.
- **Quando.** A Wave 2 só começa depois da entrega de 02/12/2026 e depois de #195, #197, #199, #210, #211, #212 e #30 fecharem. Antes disso, o trabalho de refatoração é só o baseline e o harness fora do repositório (`../Nanobo-testes/`).
- **Gate.** Cada extração é um commit `refactor(escopo): ...` que move código sem mudar comportamento; o gate é build Debug/Release com 0 avisos, harness 85/85 e as sete capturas de cena.

## Pendências

- Duração do pulso do scanner (0,75 s é valor inicial; implementação #209) e configuração de camadas por agente/fase no MVP.
- Conteúdo biológico detalhado e validado de cada agente.
- Relação final entre dados do scanner e escolha da arma — o pulso 2 nomeia o grupo; validar no playtest se isso deixa a decisão trivial.
- **D2.2 — resolvida em 05/11/2026: opção (a).** O projétil acerta mas não reduz a resistência até o agente ser identificado; a tela avisa e o agente continua escaneável (#206). As opções (b) e (c) foram descartadas.
- Balanceamento de vida, dano, ataque, defesa, cooldowns e resistência (a regra de mitigação da defesa está fechada desde 16/10/2026; falta calibrar os números).
- Regras de progressão entre fases.
- Variação de sprites das bactérias (regra decidida em 03/11; quantidade no M5, #211).
- Regras detalhadas da multiplicação limitada da Fase 2 — resolvidas em 11/11/2026, ver `fase2-virus.md`; falta balancear os números.
