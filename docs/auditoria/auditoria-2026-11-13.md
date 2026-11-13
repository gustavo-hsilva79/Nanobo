# Auditoria do projeto e do repositório — 13/11/2026

> **Escopo:** repositório local e remoto (`gustavo-hsilva79/Nanobo`), issues e marcos do GitHub, tarefas do Todoist e o build atual. O build foi avaliado jogando de forma automatizada (`../Nanobo-testes/qa_jogo.ps1`, 11 capturas) sob três lentes: sensação de jogo (game feel), interface (UI/UX) e direção de arte 2D.
> **Referência de rumo:** o design 2.0, que no momento desta auditoria estava em `docs/superpowers/specs/2026-11-13-nanobo-2-design.md`. Em 13/11 ele foi migrado para os documentos vivos (`gdd.md`, `ensino-e-aprendizagem.md`, `decisoes.md` e outros, cada um na sua §"Nanobô 2.0"), e o arquivo foi movido para `docs/arquivo/`.
> **Prazo:** M5 interno em 29/11 e entrega final em 02/12 (16 e 19 dias).

## 1. Resumo

| Área | Estado | Risco |
|---|---|---|
| Git local × remoto | 8 commits só na branch local `feat/fase2-lisogenico-12-11`; a #84 já está fechada no GitHub sem o código no remoto | **alto** (fácil de resolver) |
| Marcos e issues | M5: 75 abertas e 16 fechadas, vence em 16 dias; nenhuma issue representa o design 2.0 | **alto** |
| Equipe | o Eric tem 44 issues abertas, entre elas 3 requisitos obrigatórios da disciplina (hi-score Top 10, build Release e instalador); 9 commits desde 01/11, quase todos de texto | **alto** |
| Todoist | 9 tarefas do Gustavo atrasadas ou vencendo hoje | médio |
| Game feel | só som e texto respondem às ações; não há resposta visual de dano, acerto, neutralização nem invulnerabilidade | médio |
| Interface | funcional e legível, mas toda em texto sobre preto; o HUD ocupa 29% da tela | médio (já coberto pelo design 2.0) |
| Arte | o mapa compete com os agentes; tem leucócitos e hemácias decorativos no tamanho de agentes jogáveis | **alto** para a legibilidade |

## 2. Repositório

### 2.1 Local × remoto

- `master` local está igual a `origin/master` (`bc1566b`, commit do Eric em 11/11).
- `feat/fase2-lisogenico-12-11` tem **8 commits que não estão no remoto**: o fix da OA2, a #84 com a #89 parcial, o piscar do vírus, o doc do eclipse, a #211 (arte, código e catálogo) e a medição parcial da #118.
- **Inconsistência:** a #84 foi fechada no GitHub e no Todoist em 13/11, mas o código só existe localmente. Quem olhar o GitHub vê a issue fechada e não acha a implementação.
- O documento de design 2.0 (`docs/superpowers/`) não está versionado.
- **Correção:** enviar a branch e fazer o merge na `master`, como foi feito com a `feat/vermelho-11-11`.

### 2.2 Branches

- **Remoto:** 9 branches, todas já mescladas na `master`: `docs/revisao-geral`, `feat/destino-conclusao-bonus`, `feat/feedback-decisoes`, `feat/mvp-fluxo-e-hud`, `feat/pontuacao-ameaca-neutralizada`, `fix/aviso-bonus-identificacao`, `issue-13-bacteria` e `poc/movimento-teclado` (a mais antiga é de 07/10). Podem ser apagadas sem perda.
- **Local:** `agent/auditoria-t01-t12`, `docs/revisao-geral`, `feat/mvp-fluxo-e-hud` e `issue-13-bacteria`, também mescladas.
- Não há PRs abertos. O fluxo atual é merge direto na `master`. Para a apresentação ("simula um ambiente corporativo", Aula 1), um PR por etapa do design 2.0 deixaria rastro de revisão.

### 2.3 Histórico e identidade

- **A mesma pessoa aparece com quatro identidades:** `gustavoh-silva`, `gustavo-hsilva79`, `GUSTAVO HERICK SILVA` e `gustavo.hsilva79`.
- **O Eric aparece com três:** `ERIC BRITO SAMPAIO DE MELO`, `EricBrito-bcc` e `Eric B`.
- O GitHub atribui as contribuições pelo e-mail. Se algum e-mail não estiver ligado à conta, os commits aparecem como de autor desconhecido, e isso pesa na avaliação de "divisão igual". Vale um `.mailmap` e conferir o `git config user.email` das duas máquinas.
- Os commits do Eric não seguem Conventional Commits (por exemplo, "adicionado frase nova na tela de resultado"), enquanto o `AGENTS.md` pede `tipo(escopo): descrição`.

## 3. Marcos e issues

### 3.1 Números

| Marco | Vence | Abertas | Fechadas |
|---|---|---:|---:|
| M5 — Polimento, Estabilização e RC | 29/11 | 75 | 16 |
| M6 — Entrega Final e Apresentação | 02/12 | 4 | 0 |

- Abertas por responsável: **Eric 44** (42 no M5 e 2 no M6) e **Gustavo 35** (33 no M5 e 2 no M6). Todas têm responsável e marco.
- Fechadas no M5: 14 do Gustavo e 2 do Eric (#71 e #210).

### 3.2 Requisitos obrigatórios da disciplina (Aula 1) sem andamento

| Requisito | Issues | Responsável | Estado |
|---|---|---|---|
| Hi-score persistente | #34, #77–#82, #197 | Eric (#34 é do Gustavo, agrupadora) | só existe o recorde único (`nanobo_hiscore.txt`); Top 10 e nome não começaram |
| Instalador para Windows | #43, #159–#164 | Eric | não começou |
| Build Release para distribuição | #42, #154–#158 | Eric | o Release compila; empacotamento e validação fora do ambiente não começaram |
| Até 100 MB de RAM | #36, #117–#119 | Gustavo | medido (77–79 MB), falta fechar o Game Over e a Fase 2 |

Esses são os itens que **reprovam** a entrega se faltarem, independentemente do design 2.0.

### 3.3 Issues que o design 2.0 torna obsoletas ou muda

| Issue | Situação diante do design 2.0 |
|---|---|
| #206 D2.2 (travar neutralização sem identificação) | substituída pela resistência e pela marcação |
| #207 D2.1 (identificação como condição de vitória) | descartada (§12 item 11 do design) |
| #208 Bônus de run perfeito | substituída pelo bônus de vitais |
| #209 Pulso de scan de 0,75 s | não prevista no design 2.0 |
| #185 Sinalizar risco de contato | absorvida pelo novo feedback de dano (§5 desta auditoria) |
| #211 Variação de sprites | reescopada: 8 variantes por categoria, arte nova |
| #85–#88 | continuam, mas com a regra de eclosão de 13/11 (vírus nascem livres e infectam) |
| #91 Validar a Fase 2 | continua, com a Fase 2 nova |

### 3.4 Duplicidades

- #44 e #47 (M6, Gustavo) repetem #165–#170 (M5, Gustavo).
- #43 e #46 (M6, Eric) repetem #159–#164 (M5, Eric).
- O `kanban.md` §M6 já registra isso e deixa a decisão para a dupla.

### 3.5 Lacuna

O trabalho do design 2.0 (narrativa, mecânicas novas da Fase 1, aliados, interface nova, arte nova) **não tem nenhuma issue**. Pelo fluxo do projeto, a etapa 1 precisa delas antes de começar.

### 3.6 Todoist

Estão atrasadas ou vencem hoje: #85, #86, #89, #36, #117, #118, #134, #185 e #211. As datas foram planejadas antes do design 2.0 e precisam ser remarcadas segundo o cronograma da §11 do design.

## 4. Build atual — o que as capturas mostram

| Captura | Observação |
|---|---|
| Menu | parede de texto em 2 painéis sobre preto; título em bitmap; nenhuma imagem do jogo, do Nanobô ou de agente |
| Briefing | 3 cards só de texto, sobre preto; os botões SEGUIR e PULAR ficam lado a lado, mas o foco anda com ↑/↓ e ←/→ não move o foco |
| Início da Fase 1 | **nenhum agente na tela**: os 9 nascem fora da câmera; o primeiro quadro mostra só o Nanobô, o mapa e o "destino" |
| Andando | o primeiro agente que aparece é o disco cinza, e ele lê como um elemento de interface, não como organismo |
| Fase 2 | mesmo mapa e mesmo ponto de partida da Fase 1; o vírus laranja nasce colado na borda inferior e quase some no vermelho |
| Resultados | 3 painéis de texto; o resumo OA usa jargão ("OA1 Gram:") |
| Vitória e Game Over | o mesmo modelo de painel; o Game Over diz "suas 3 vidas acabaram", sem narrativa |

## 5. Game feel

**Existe hoje:**
- aceleração e frenagem do Nanobô (#179);
- 6 efeitos sonoros e música em loop;
- a mensagem de feedback no rodapé do mundo;
- o anel da defesa;
- o vírus lisogênico piscando.

**Falta, por ordem de impacto:**

1. **Invulnerabilidade invisível.** Depois de levar dano, o Nanobô tem 1 s de invulnerabilidade (`INVULNERABILITY_TIME`) sem nenhuma pista visual. O jogador não sabe por que o segundo contato não doeu. É o caso clássico de piscar o sprite em 8–10 Hz durante o timer.
2. **Dano sem impacto.** Perder vida toca um som e escreve um texto. Faltam um flash vermelho curto na borda da tela, tremor de câmera pequeno e um empurrão.
3. **Neutralização sem recompensa.** O agente some no mesmo quadro. Faltam uma pequena explosão de partículas na cor do Gram, um "+10" subindo e um hit-stop curto (cerca de 60 ms).
4. **Acerto parcial invisível.** A bactéria leva 2 acertos, mas o primeiro não muda nada nela. Faltam um flash branco de 1 quadro no sprite e um recuo pequeno.
5. **Scan sem gesto.** O scan é instantâneo e silencioso no mundo. Um anel que se expande a partir do Nanobô até o alvo dá peso à ação central do jogo.

**Como fazer em Allegro sem tocar a simulação:**
- **Tremor:** um deslocamento aplicado só na hora de desenhar (`view_x`/`view_y`), alimentado por um valor de "trauma" que decai; nunca na posição do jogador.
- **Hit-stop:** pular `update_match` por N quadros, com o render continuando.
- **Flash:** `al_draw_tinted_bitmap` com o sprite saturado em branco.
- **Partículas:** um array estático pequeno, como já é feito com os projéteis.
- **Três níveis de intensidade:** pequeno (scan, tiro), médio (acerto, dano) e grande (neutralizar resistente, eclosão, fim de fase). Um único ponto de chamada por evento usa o `SfxEvent` que já existe.

## 6. Interface (UI/UX)

**Bom hoje:**
- canvas lógico de 640×360 com escala inteira e barras pretas, o que resolve resolução e proporção sem âncoras;
- foco por teclado e mouse coexistindo;
- uma lista única de botões para desenho e clique (`screen_buttons`);
- a largura do HUD medida com `al_get_text_width`.

**Problemas:**
1. **O HUD ocupa 104 de 360 px (29%)**, e o painel do scanner passa a maior parte da partida mostrando "Nenhum alvo". O design 2.0 reduz isso: o scanner aparece quando há alvo, e a área livre vai para o mundo.
2. **A navegação não segue a geometria:** botões lado a lado (briefing) são navegados com ↑/↓, e ←/→ não fazem nada.
3. **Não há volta ao menu** pela pausa, pelo game over nem pela vitória (#195, do Eric).
4. **Todas as telas usam o mesmo modelo** (título, régua, painel, botão) sobre preto. Não há hierarquia visual nem imagem.
5. **Fonte 8 px ampliada 4×:** legível, mas sem identidade. Maiúsculas acentuadas são proibidas pela limitação da fonte; a fonte TTF do design 2.0 resolve.
6. **O resumo de resultados usa códigos internos** ("OA1 Gram:", "OA2 Estrutura:"), que são termos de documento e não de jogador.

## 7. Direção de arte

O mapa fica (decisão do dono). Os testes de legibilidade sobre as capturas:

- **Escala de cinza:** o mapa está inteiro em valores médio e médio-escuro, com textura densa (hemácias, fibras). O vírus laranja e o disco cinza caem no mesmo valor médio do fundo: é o caso de mid-on-mid. Só o Nanobô (branco e ciano, o valor mais alto) separa.
- **Teste do olho semicerrado:** as hemácias e as duas células brancas pintadas no mapa formam blocos tão fortes quanto os agentes. O olho vai primeiro para a célula branca decorativa à esquerda, que **não é jogável**.
- **Falsa interação:** o mapa tem leucócitos e hemácias pintados **no mesmo tamanho e com mais detalhe** que os agentes de 32 px. O jogador não sabe o que é cenário e o que é alvo. É a falha mais grave de legibilidade do jogo.
- **Hierarquia:** a regra "UI > efeitos > personagem > fundo" está invertida, porque o fundo é a coisa mais detalhada da tela.

**Recomendações, sem refazer o mapa:**
1. **Baixar o contraste e a saturação do mapa na hora de desenhar** (`al_draw_tinted_bitmap` com um tom escuro, cerca de 70% de valor), sem editar o PNG. O mapa continua o mesmo, só recua.
2. **Agentes com contorno escuro e uma borda clara** ou um halo suave por baixo, como no design 2.0 §8.
3. **Paleta dos agentes fora do vermelho:** ciano e azul (aliados e Nanobô), roxo e magenta (Gram+), rosa claro e salmão com contorno (Gram−), verde-água (células do paciente), âmbar (resistência). O laranja do vírus atual fica perto demais do vermelho; o vírus da arte nova deve ir para o verde-limão ou o amarelo-esverdeado.
4. **Sombra de contato** (uma elipse escura de 30% sob cada agente), para tirar a sensação de "colado no fundo".
5. **Teste do olho semicerrado em cada captura do QA**, a partir de agora, como critério de aceite da arte.

## 8. Recomendações priorizadas

1. **Hoje:** enviar a branch e fazer o merge da `feat/fase2-lisogenico-12-11`; versionar o design 2.0 e esta auditoria.
2. **Hoje:** decidir o destino das issues obsoletas (§3.3) e das duplicadas (§3.4); criar as issues do design 2.0, etapa 1.
3. **Esta semana:** confirmar com o Eric a capacidade dele para Top 10, Release e instalador (§3.2). São requisitos de aprovação e não podem ficar para 01/12.
4. **Etapa 1 do design 2.0:** incluir o recuo do mapa (§7, recomendação 1) e os quatro primeiros itens de game feel (§5). São baratos e mudam a percepção do jogo antes da arte nova.
5. **Remarcar o Todoist** pelo cronograma do design 2.0.
6. Apagar as 9 branches remotas mescladas e criar um `.mailmap`.

## 9. Decisões do dono (13/11)

- **Branch:** enviar e fazer o merge de `feat/fase2-lisogenico-12-11` na `master`. Os sprites `_b` ficam até a arte nova.
- **Requisitos obrigatórios:** o Gustavo assume o Top 10 (#77–#82, #197), o build Release (#42, #154–#158) e o instalador (#43, #159–#164). O Eric fica com QA, documentação, textos e a volta ao menu (#195). A divisão de story points em `divisao-issues-dupla.md` precisa ser recalculada.
- **Mapa:** autorizado escurecer e dessaturar na hora de desenhar; o PNG não muda.
- **Issues obsoletas e duplicadas (§3.3, §3.4):** nada é fechado por enquanto.
