# Nanobô — Spec de Ensino e Aprendizagem (MVP)

31 out 2026 · @Gustavo Herick Silva
Revisão pedagógica: 2 nov 2026
Sincronização com a auditoria e com o escopo real do M4: 3 nov 2026

> **Histórico:** o texto de 31/10 é a proposta inicial; a revisão de 02/11 acrescentou os objetivos, o fluxo, os requisitos e a verificação; **esta cópia de 03/11 é a fonte pedagógica do M4** e aplica as correções da auditoria de escopo (#150). O que a auditoria não decidiu está marcado como proposta ou pendência, nunca como regra.

## Correções desta revisão (03/11/2026)

| # | Correção | O que substitui |
|---|---|---|
| 1 | **D1:** as duas armas ferem a célula própria (−1 vida). É regra do jogo, sem alegação sobre medicamentos. | “A não fere a célula própria e B fere” |
| 2 | **D2:** o segundo pulso nomeia o grupo (Gram+ / Gram−) e mostra a pista estrutural. | decisão de 26/10 (camada 2 sem nome) |
| 3 | **Contrato de código:** só os quatro arquivos compilados; briefing, HUD e resumo em `main.c`; regras e contadores em `gameplay.c`. Nada de `src/ui/`. | seção “Impacto no código” de 02/11 |
| 4 | **Validação:** build Debug e Release x64 + QA manual. O harness fora do repositório (83 casos em 30/10) é evidência complementar, não critério de aceite. | “78 casos do harness” como requisito |
| 5 | **Acentuação:** já implementada (`/utf-8` na linha 114 do `.vcxproj`, textos acentuados, #194 fechada). D4 concluída. | “nenhum arquivo de `src/` tem byte fora de ASCII” |
| 6 | **Escopo:** Diário, zonas e CSV/telemetria ficam fora do M4; Fase 2/vírus fora do M4. | “o Desejável entra se sobrar tempo” |
| 7 | **Plano:** substituído pelo plano por issue (`#58`, `#103`–`#107`, `#198`–`#200`, `#174`/`#176`, `#129`, `#150`). | plano T1–T13 com horas |
| 8 | **Português:** concordância de “camadas externas” corrigida em todo o documento (feminino). | — |

A auditoria de 03/11 **não** decidiu os valores de calibração (pulso, trava, perseguição) nem a arte neutra: os dois continuam como proposta e não são critério do M4.


## Resumo e decisões

Esta Spec organiza a aprendizagem biológica do MVP dentro do gameplay. Na Fase 1, o jogador interpreta o resultado da coloração de Gram, compara em linguagem simples as camadas externas bacterianas, escolhe entre duas armas segundo as regras do jogo e aprende a preservar a célula própria. O peptidoglicano é apresentado como parte da parede bacteriana, sem ocupar sozinho a maior parte do ensino ou da avaliação.

Briefing, scanner, feedback e resumo retomam os mesmos quatro objetivos em etapas diferentes (o Diário de Campo, que faria a quinta, é pós-M4). A Spec separa o conteúdo essencial ao MVP dos aprofundamentos técnicos e das simplificações do jogo. Ela mantém as decisões de gameplay aprovadas e organiza o material de apoio e a verificação da aprendizagem.

A adequação ao Ensino Médio é uma estimativa pedagógica, não uma certificação curricular: a BNCC define competências amplas sem prescrever esses termos de microbiologia, e a comparação de nível também usa materiais de preparação para o ENEM. A confiança geral da Spec passa a 0,73 após explicitar essa incerteza.

As decisões de 31/10 sobre arte neutra, controles por mouse e português permanecem; a arte neutra continua sendo decisão de arte, mas não tem issue no M4. D1 foi revista em 03/11 (as duas armas ferem a célula própria) e D2 foi confirmada com o nome do grupo no segundo pulso.

## Validação das ideias

Das 20 ideias das duas IAs e da nossa conversa, adoto 8, ajusto 5, adio 6 para a entrega final e descarto 1; quatro afirmações delas não se sustentam nas fontes e não devem ir para o professor.

| # | Ideia (origem) | Veredito | Por quê e como entra na Spec |
| --- | --- | --- | --- |
| 1 | Arte neutra até escanear; o scan vira a coloração de Gram (DeepSeek) | Adotar | Já aprovada (decisão 1). A bactéria só ganha cor depois da camada 1; a célula própria continua visualmente distinta. |
| 2 | Scanner mostra a estrutura (parede, membrana), não só uma etiqueta (DeepSeek) | Adotar | Camada 1 = resultado da coloração. Camada 2 = estrutura, nome do grupo e o porquê. Muda a decisão de 26/10, que escondia o nome Gram: agora o jogo explica. |
| 3 | Briefing interativo antes de jogar (DeepSeek) | Adotar com ajuste | 3 cartões: resultado da coloração; comparação das estruturas; armas como modelo do jogo e preservação da célula própria. Os detalhes moleculares ficam em “Para ir além”. Pulável com ESC; reabre no Diário (pós-M4). |
| 4 | Feedback explicativo depois de cada decisão (DeepSeek) | Adotar | Troca as mensagens atuais que entregam a resposta pela cor ("o raio vermelho nao penetra") por uma explicação do mecanismo. |
| 5 | Diário de Campo que desbloqueia entradas (DeepSeek) | Adiar (pós-M4) | Aba no pause; reaproveita o texto do briefing. Precedente: o Log Book do Metroid Prime guarda o que o jogador escaneou ([Metroid Wiki](https://metroid.fandom.com/wiki/Scan_Visor)). A auditoria de 03/11 tirou o Diário do M4. |
| 6 | Matriz objetivo → mecânica → evidência → teste (Qwen) | Adotar | É o formato que a issue #177 já pede (scanner → decisão → evidência). Está na seção de objetivos. |
| 7 | Critérios de aceite do MVP e pré/pós-teste (Qwen) | Adotar com ressalva | Com 3–5 jogadores o resultado é formativo, não estatístico. Limiares na seção de verificação. |
| 8 | Controles em português; mouse com mira livre, esquerdo atira, direito escaneia, roda troca a arma (você + as duas IAs) | Adotar | Decisão 2 e o pedido de PT-BR. O teclado continua como alternativa. |
| 9 | Dois módulos inspirados em classes de antibiótico (DeepSeek) | Ajustar | No MVP, A representa um alvo na parede bacteriana e B uma ação sobre a camada externa das Gram−. A matriz é uma regra simplificada do jogo, não uma previsão de tratamento. Exemplos de fármacos e mecanismos ficam em “Para ir além”. |
| 10 | Polimixina como arma sem custo (DeepSeek) | Ajustar | No jogo, **as duas armas** ferem a célula própria segundo a regra D1. O MVP explica esse risco em linguagem simples; LPS, polimixina e efeitos clínicos ficam como aprofundamento e não justificam orientação médica. |
| 11 | Indicador de dano esperado antes do disparo (Qwen) | Ajustar | Mostrar antes entrega a resposta. Trocar por dica em forma de pergunta depois do 1º erro e explicação completa depois do 2º. |
| 12 | Manter laser roxo = Gram+ e vermelho = Gram− (#16/#62) | Ajustar | Com arte neutra, módulo da mesma cor da coloração recria o atalho "cor com cor". As armas ganham outra identidade (nomes Ruptura/Dissolução, ícone e cor neutra). Corrijo minha resposta anterior, que dizia que o #62 não mudava. |
| 13 | Plano de 28 h (DeepSeek) | Ajustar | Serve de esqueleto, não de estimativa: não há histórico de velocidade da dupla. O plano passou a ser o das issues (#58, #103–#107, #198–#200, #174/#176, #129, #150). |
| 14 | Terceira classe, inibidor de ribossomo (Qwen) | Adiar (final) | Age nos dois grupos de bactérias e viraria a arma boa para tudo, a menos que tenha custo próprio. Exige ensinar ribossomo antes de a parede estar firme. |
| 15 | Barra de endotoxina / saúde do tecido (Qwen) | Adiar (final) | Conflita com o decisoes.md de 25/10, que rejeitou barra de integridade e segunda condição de derrota. Reabrir só por decisão da dupla. |
| 16 | Resistência por β-lactamase (Qwen) | Adiar (final) | Cria um terceiro estado ("resistente a A") que só faz sentido com a terceira classe. |
| 17 | Fase 2 de vírus e fases 2–3 (Qwen) | Adiar (final) | Já é pós-MVP em escopo.md. O MVP prova a estrutura de ensino em uma fase. |
| 18 | Varredura do scanner arrastando o mouse (Qwen) | Adiar | A mira livre com botão direito já dá ao mouse um papel real, sem um segundo gesto. |
| 19 | Economia de "amostras" e divisão bacteriana | Adiar | Somam sistemas sem objetivo de aprendizagem ligado. |
| 20 | "Ataques iônicos" como classe de módulo (DeepSeek) | Descartar | Não corresponde a mecanismo de antibiótico; ensinaria algo sem base. |

### Afirmações que não se sustentam

| Afirmação (origem) | O que a fonte mostra |
| --- | --- |
| "Mindustry ensina programação" (DeepSeek) | A página do projeto o descreve como "automation tower defense RTS", em Java ([GitHub](https://github.com/Anuken/Mindustry)). Não alega uso educacional. |
| "Immune Defense melhorou a aprendizagem" (Qwen e PDF) | O estudo controlado é do **Immune Attack** ([Stegman 2014](https://pmc.ncbi.nlm.nih.gov/articles/PMC4489431/)): alunos do ensino médio que jogaram foram melhor em termos e conceitos num teste 24 h depois. Os autores citam limites (teste criado por eles, tutorial pouco intuitivo). A página do [Immune Defense](https://melanieann.itch.io/immune-defense) mostra um RTS em desenvolvimento, sem avaliação de aprendizagem. |
| DungeonRoyale como referência educacional (Qwen) | É um jogo no estilo Bomberman/battle royale em Godot, sem proposta educacional. |
| "Modelo Stairway" como base científica (Qwen e PDF) | A fonte é um blog, não pesquisa revisada por pares. Usar como inspiração, não como evidência. |

Atenção: as duas IAs receberam o prompt com a minha resposta anterior. Quando elas concordam com o plano (arte neutra, scan com custo, mouse), isso mostra consistência, não validação independente. A validação de verdade vem das fontes e do playtest.

## Objetivos de aprendizagem e conteúdo biológico

Ao fim da Fase 1, o jogador deve interpretar o resultado da coloração de Gram, comparar as camadas externas de Gram+ e Gram−, escolher uma arma de acordo com a regra simplificada do jogo e preservar a célula própria. O peptidoglicano aparece como um componente importante da parede bacteriana, dentro desse conjunto de ideias.

### Objetivos e rastreabilidade

| OA | O jogador será capaz de… | Onde o jogo ensina | O que a mecânica exige | Evidência no jogo | Fora do jogo |
| --- | --- | --- | --- | --- | --- |
| OA1 | interpretar o que o resultado roxo ou vermelho (rosa) da coloração de Gram indica | Briefing (cartão 1), scan pulso 1 e resumo | Revelar a primeira camada antes de agir | Lê o resultado e identifica o grupo, sem inferir espécie ou doença | Pré/pós-teste P1 |
| OA2 | comparar as camadas externas: Gram+ tem parede mais espessa; Gram− tem parede mais fina e uma camada externa extra | Briefing (cartão 2) e scan pulso 2 | Usar a pista estrutural para interpretar o grupo | Explica a diferença em palavras próprias; reconhece onde fica o peptidoglicano | Pré/pós-teste P2 |
| OA3 | relacionar a estrutura observada ao alvo da arma dentro das regras do jogo | Briefing (cartão 3), scan, decisão e feedback | Escolher Ruptura ou Dissolução a partir da leitura, sem receber a resposta do scanner | Escolhe a arma correspondente e explica o alvo do jogo | Pré/pós-teste P3 |
| OA4 | reconhecer e preservar uma célula saudável do organismo, distinguindo a regra do jogo de uma afirmação sobre medicamentos reais | Briefing (cartão 3), identificação no scan e feedback | Evitar atacar a célula própria | Preserva a célula e não conclui que a regra do jogo isenta medicamentos reais de efeitos adversos | Pré/pós-teste P4 |

Os quatro objetivos recebem espaço próprio no briefing, na mecânica e na avaliação. Nenhuma pontuação total substitui a leitura do resultado de cada objetivo.

### Conteúdo e textos de referência

A tabela separa o nível essencial ao MVP dos termos que só aparecem como aprofundamento. Os textos do scanner e do HUD têm no máximo duas linhas de até 70 caracteres; explicações maiores ficam no briefing.

| Conceito | Nível | Texto-base para o jogador | Uso didático e fonte |
| --- | --- | --- | --- |
| Resultado da coloração de Gram | Essencial no MVP | “Roxo e vermelho (rosa) indicam grupos bacterianos com estruturas diferentes.”<br>“A cor não diz se a bactéria causa doença nem indica tratamento.” | A cor e a comparação geral aparecem em materiais de preparação para o ENEM ([PrePara ENEM](https://www.preparaenem.com/biologia/bacterias-gram-positivas-e-negativas.htm), [Manual do Enem: bactérias](https://querobolsa.com.br/enem/biologia/bacteria)). |
| Gram-positiva | Essencial no MVP | “Tem parede celular mais espessa e costuma ficar roxa na coloração.” | Apresentar como característica geral do grupo; não inferir tratamento. |
| Gram-negativa | Essencial no MVP | “Tem parede mais fina e uma camada extra por fora; costuma ficar vermelha (rosa).” | Na primeira ocorrência, explicar “membrana externa” como camada extra com lipídios e açúcares. |
| Peptidoglicano | Essencial no MVP, em nível introdutório | “Material de açúcares e peptídeos que faz parte da parede bacteriana.” | Explicar uma vez como parte da parede; não transformar sua composição molecular no objetivo principal. Materiais de preparação para o ENEM incluem parede e peptidoglicano. |
| Membrana externa | Essencial no MVP, com explicação simples | “Camada externa adicional presente nas bactérias Gram-negativas.” | É uma pista estrutural do jogo; o scanner não precisa mostrar LPS. |
| Armas Ruptura e Dissolução | Essencial no MVP como modelo de jogo | “No jogo, a Ruptura mira a parede; a Dissolução age na camada extra das Gram−.” | Regra fictícia para ligar leitura e decisão. Não equivale à escolha de antibiótico real. |
| Arma e célula própria | Essencial no MVP como regra do jogo | “No jogo, qualquer arma pode ferir a célula do organismo.”<br>“Atacar célula custa 1 de vida; isso não descreve efeitos de medicamentos reais.” | A consequência de D1 é regra do jogo; não usar “toxicidade seletiva” com o jogador nem sugerir que a regra valha para tratamento real. |
| Etapas da coloração de Gram | Para ir além | “Cristal violeta, iodo, álcool e safranina, nessa ordem.” | O MVP ensina o significado geral das cores; memorizar a sequência não é critério de vitória nem de avaliação. |
| Ligações cruzadas, transpeptidase e PBP | Para ir além | “Detalhes moleculares de como alguns fármacos interferem na parede.” | A descrição aparece apenas em material opcional (Diário, pós-M4) e com fonte técnica ([OpenStax Microbiology, 14.3](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs)). |
| LPS, polimixina e efeitos clínicos | Para ir além | “Exemplos técnicos; não necessários para jogar.” | Não usar esses termos no feedback curto nem como explicação obrigatória da ação da Dissolução. OpenStax é referência de aprofundamento, não de nível curricular. |
| Exceções à coloração e espécies bacterianas | Fora do conteúdo obrigatório do MVP | “Há exceções e diferenças entre espécies que o jogo não representa.” | Sem nomes de espécie ou classificação clínica na fase. O jogo não ensina que todas as bactérias causam doença. |

**Como estimamos o nível:** a BNCC do Ensino Médio organiza competências amplas de Ciências da Natureza e não enumera peptidoglicano, LPS ou PBP como conteúdos obrigatórios ([BNCC do Ensino Médio](https://portal.mec.gov.br/docman/abril-2018-pdf/85121-bncc-ensino-medio/file)). A presença da parede bacteriana, do peptidoglicano e da distinção Gram+ / Gram− em materiais de preparação para o ENEM sustenta sua inclusão como conteúdo de nível escolar; essa comparação é uma estimativa, não uma afirmação oficial da BNCC. A sequência detalhada de corantes e os mecanismos moleculares ficam fora da aprendizagem obrigatória do MVP.

### Simplificações assumidas

Estas simplificações precisam aparecer na interface como “Na vida real” — no M4, no briefing e no feedback —, sem transformar a partida em aula de farmacologia:

- A matriz Ruptura/Dissolução é uma regra de balanceamento e aprendizagem do jogo. Não representa a eficácia de uma classe real de antibióticos.
- No jogo, a Ruptura tem como alvo a parede bacteriana e a Dissolução atinge a camada externa das Gram−. Ruptura e Dissolução não são nomes de medicamentos, e a escolha da arma não recomenda tratamento.
- D1 (revista em 03/11): no jogo, **as duas armas podem ferir a célula própria** (−1 vida). Isso é uma consequência do jogo; não significa que medicamentos reais sejam seguros nem que sejam perigosos, e não descreve efeitos adversos reais.
- Os 2 acertos de “resistência” são pontos de combate, não resistência antimicrobiana.
- A coloração de Gram tem exceções e não identifica espécie, causa de doença ou tratamento adequado.
- Nem toda bactéria faz mal; os alvos são ameaças específicas do cenário.
- Dose, tempo de ação, diagnóstico e efeitos adversos de medicamentos reais não são simulados. O jogo não orienta tratamento.

### Regras para qualquer texto do jogo

- Explicar a pista e o conceito em linguagem simples; o scanner não recomenda arma nem ação.
- Não usar PBP, transpeptidase, ligações cruzadas ou LPS no feedback do HUD.
- Até 70 caracteres por linha e até 2 linhas no scanner/HUD. O que não couber vai para o briefing.
- Não usar nomes de espécies nem inferir patogenicidade pela cor, formato ou grupo.
- Cada texto novo entra em “textos-do-jogo.md” com ID, tela, limite de caracteres e fonte.

## Como o jogo ensina

O jogador aprende na ordem briefing → scan → decisão → feedback → resumo (o Diário entra depois do M4), e cada passo reaproveita o mesmo conteúdo: a mecânica é o conteúdo, e o texto só explica o porquê.

### Uma partida, passo a passo

1. **Menu.** Botão INICIAR e a caixa “Como jogar” mostram os controles em português, sem antecipar a resposta biológica.
2. **Briefing, 3 cartões (cerca de 60 s).** (1) O que a coloração de Gram revela e o limite da cor. (2) Comparação simples entre as camadas externas Gram+ e Gram−, com peptidoglicano explicado como parte da parede e a camada externa apresentada como uma característica adicional. (3) Armas como modelo do jogo e a regra de preservar a célula própria. O cartão 3 também enquadra os agentes: são ameaças da missão, e nem toda bactéria causa doença. ENTER, → ou clique avançam; ESC pula. No M4 o conteúdo não reabre depois: o Diário de Campo ficou fora do marco.
3. **Fase única.** Os 9 agentes entram juntos (3 Gram+, 3 Gram−, 3 células) e as duas armas ficam disponíveis desde o início. *O cartão rápido na primeira leitura de uma Gram− é proposta de 02/11, sem issue no M4: o briefing é o único texto obrigatório.*
4. **Scan = coloração.** Dois pulsos com o Nanobô parado e sem poder atirar. Pulso 1 mostra a coloração (roxo ou vermelho) e o nome do agente; pulso 2 mostra o nome do grupo (Gram+ ou Gram−), a pista estrutural e o porquê, sem indicar a arma. Dano de contato cancela o pulso. *A duração de 0,75 s e a imobilização são valores iniciais propostos em 02/11; a auditoria de 03/11 não os decidiu (ver “Dificuldade e balanceamento”).*
5. **Decisão.** O jogador escolhe a arma (1/2 ou roda do mouse) e mira com o mouse.
6. **Consequência.** Acerto avança a resistência e explica em linguagem simples qual alvo do jogo foi atingido. Arma errada trava os disparos e a bactéria passa a perseguir. A mensagem começa com uma pergunta e, em erros repetidos, explica a pista sem termos moleculares. *Trava de 2 s e perseguição a 60 px/s em qualquer distância: decisão de 04/11; a calibração com pessoas fica no playtest do M5.*
7. **Célula própria.** No jogo, as duas armas ferem a célula (−1 vida, D1): não existe arma “segura”, então o jogador precisa mirar longe das células ao escolher o alvo.
8. **Resumo.** A tela de Resultados mostra, por objetivo, o que foi descoberto, onde houve dificuldade e qual conteúdo rever, a partir dos contadores em memória (#200, #174). Um total geral não esconde um objetivo que ainda não foi aprendido. O bônus aparece com o critério explícito: 9/9 hoje; no M5, run perfeito (0 erro de arma e 0 tiro na célula).


### Feedback por situação

Até 2 linhas de 70 caracteres. As mensagens explicam a pista ou a regra do jogo, sem ensinar mecanismo molecular nem recomendar tratamento real.

| Situação | Mensagem | Efeito |
| --- | --- | --- |
| Ruptura acerta Gram+ (1º acerto) | Ruptura atingiu a parede: falta 1 acerto. | Resistência −1 |
| Ruptura neutraliza Gram+ | Ameaça neutralizada: +10 pontos. | +10; identificação registrada |
| Dissolução acerta Gram− (1º acerto) | Dissolução atingiu a camada extra: falta 1 acerto. | Resistência −1 |
| Dissolução neutraliza Gram− | Ameaça neutralizada: +10 pontos. | +10; identificação registrada |
| Arma errada, 1ª vez | Que estrutura o scan mostrou? Qual arma tem esse alvo? | Trava 2 s; bactéria persegue |
| Ruptura em Gram−, 2ª vez em diante | No jogo, a camada extra das Gram- dificulta a ação da Ruptura. | Trava 2 s; bactéria persegue |
| Dissolução em Gram+, 2ª vez em diante | No jogo, a Gram+ não tem a camada extra atingida pela Dissolução. | Trava 2 s; bactéria persegue |
| Ruptura em célula (decisão D1) | No jogo, a Ruptura fere a célula própria: -1 vida. | −1 vida |
| Dissolução em célula (decisão D1) | No jogo, a Dissolução fere a célula própria: -1 vida. | −1 vida |
| Contato com bactéria | Contato com bactéria: -1 de vida. | −1 vida (já existe) |

### Por que esta ordem

| Princípio | Como aparece no Nanobô | Fonte |
| --- | --- | --- |
| Conteúdo dentro da mecânica (integração intrínseca) | Scan = coloração; escolher a arma = ligar estrutura a efeito | [Habgood e Ainsworth 2011](https://shura.shu.ac.uk/3556/): com o jogo *Zombie Division* (crianças de 7 a 11 anos), a versão com o conteúdo na mecânica rendeu mais aprendizagem sob tempo fixo e cerca de 7 vezes mais tempo de jogo voluntário |
| Instrução complementar ao jogo | Briefing e resumo (Diário pós-M4) | [Wouters et al. 2013](https://karlkapp.com/?p=4333): jogos sérios superam a instrução convencional em aprendizagem (d = 0,29) e retenção (d = 0,36), mas não motivam mais; o efeito é maior com instrução complementar, várias sessões e jogo em grupo |
| Prova em jogo parecido | Meta de aprendizagem por termos e conceitos | [Stegman 2014](https://pmc.ncbi.nlm.nih.gov/articles/PMC4489431/): jogadores do Immune Attack foram melhor em termos e conceitos que o grupo de controle, em teste 24 h depois; teste feito pelos próprios autores |
| Explicar na hora da decisão † | Feedback com mecanismo e dica escalonada | Princípio geral de feedback explicativo † |
| Pré-treino e segmentação † | 3 cartões curtos antes; cartão da Zona 2 na hora de uso | Princípios de aprendizagem multimídia (Mayer) † |

† De memória, não conferido nesta rodada. Os três primeiros foram abertos e lidos; o resumo de Habgood e Ainsworth veio da página do repositório da Universidade de Nottingham.

## Requisitos funcionais e decisões

O M4 tem os requisitos RF-01 a RF-07 e RF-10 a RF-13 (o Núcleo da revisão de 02/11). A auditoria de 03/11 manteve esse conjunto e tirou o Desejável do M4. D1, D2 e D4 estão resolvidas; D3 e D5 ficaram fora do marco; D6 depende do professor.

### Decisões abertas

| ID | Decisão | Situação em 03/11 |
| --- | --- | --- |
| D1 | Armas e célula própria | **Resolvida:** as duas armas ferem a célula própria (−1 vida), como simplificação do jogo e sem alegação sobre medicamentos. |
| D2 | Segundo pulso do scanner nomeia o grupo | **Resolvida:** o pulso 2 mostra o nome do grupo e a pista estrutural; substitui a decisão de 26/10. |
| D3 | Zonas guiadas (Zona 1 só Gram+, Zona 2 com Gram−) | **Fora do M4:** a fase do M4 é única; as zonas voltam ao backlog depois do build e do QA manual. |
| D4 | Acentuação | **Resolvida e implementada:** `/utf-8` no `.vcxproj` (linha 114) e textos em UTF-8 acentuado (commit `75030ea`, #194 fechada). |
| D5 | Diário de Campo | **Fora do M4:** entra no backlog depois do build e do QA manual aprovados. |
| D6 | Perguntar ao professor se “Core gameplay loop” e “MVP” são entregas separadas e quanto do ensino precisa estar jogável em 04/11 | **Pendente com a dupla** (aula de 04/11). |

### Requisitos

Prioridade: **N** = Núcleo do M4 (04/11); **N¹** = decisão mantida, sem issue no M4; **F** = pós-M4 / entrega final (02/12).

| ID | Pri. | Requisito | Critério de aceite |
| --- | --- | --- | --- |
| RF-01 | N | Briefing de 3 cartões antes da 1ª partida: resultado da coloração; comparação simples das camadas externas; armas como modelo, preservação da célula própria e o enquadramento dos agentes como ameaças do cenário. ENTER, → ou clique avançam; ESC pula. | Texto cabe em 640×360 sem corte (medido com `al_get_text_width`). QA manual: os três cartões avançam e pulam. |
| RF-02 | N | Scan em 2 pulsos (valor inicial 0,75 s), com o Nanobô parado e sem tiro. Pulso 1 mostra a coloração e o nome do agente; pulso 2, o nome do grupo, a estrutura e o porquê. | Durante o pulso o Nanobô não anda nem atira. Dano de contato cancela o pulso. Após 2 pulsos `identified` vira true (regra atual). Conferido no QA manual (#107, #129). |
| RF-03 | N¹ | Arte neutra: Gram+ e Gram− usam o mesmo sprite até o pulso 1; depois ganham um anel (roxo ou vermelho) que permanece. A célula própria tem sprite próprio e maior. | Captura com 6 bactérias não permite separar os grupos (duas pessoas conferem). |
| RF-04 | N | Armas Ruptura e Dissolução com a matriz de efeitos: Ruptura neutraliza Gram+; Dissolução neutraliza Gram−; arma errada não causa dano e trava; célula: as duas armas custam 1 vida (D1). Identidade própria: nome, ícone e cor neutra; ícone e projétil distintos pela forma. | Matriz arma × espécie conferida no QA manual. Cor do projétil neutra e diferente da cor do anel. |
| RF-05 | N | Feedback explicativo conforme a tabela da seção "Como o jogo ensina", com dica escalonada (pergunta no 1º erro, mecanismo no 2º). | Toda linha da tabela tem texto e efeito testados. Nenhuma mensagem usa cor para dizer a arma certa. |
| RF-06 | N¹ | Trava de disparo após arma errada e perseguição da bactéria (2 s; 60 px/s; em qualquer distância). **No build desde 04/11**, sem issue. | Metas da simulação (bot leitor ≥ 95%, humano 55–75%, chute ≤ 10%, parado ≤ 5%) — a verificação com pessoas fica no playtest do M5. |
| RF-07 | N | Resumo na tela de Resultados (fecha #174): evidências separadas por OA, erros por tipo e uma frase de revisão para cada lacuna. | Cenários sem erro, Ruptura em Gram− e arma em célula geram resumos corretos; um acerto em um OA não mascara dificuldade em outro. |
| RF-08 | F | Pós-M4 — Diário de Campo na pausa (TAB abre). Entradas desbloqueadas por evento; bloqueadas aparecem como “???”; cada entrada identifica o que é conteúdo essencial, simplificação do jogo ou “Para ir além”. Reabre o briefing. | Abre por teclado e mouse; a partida fica congelada; todas as entradas têm fonte registrada. |
| RF-09 | F | Pós-M4 — Zonas guiadas (D3) e cartão da Zona 2. | Na Zona 1 não existe Gram−; ao cruzar x = 960 o cartão aparece uma única vez. |
| RF-10 | N¹ | Marca de identificação sobre o agente: anel âmbar com 1 pulso, verde com 2, mais o contador do HUD (item 3 do professor). | Depois de escanear todos e matar as bactérias, as células restantes mostram quem foi identificado. |
| RF-11 | N | Controles em português em todas as telas: "Como jogar", pausa e dica fixa "esc: controles" no HUD, vindos de uma única tabela. | Nenhum texto exibido contém "space". Menu e pausa mostram a mesma tabela. |
| RF-12 | N | Mouse no jogo: mira livre; botão esquerdo atira para o cursor; botão direito escaneia o agente sob o cursor (até 24 px de tolerância e dentro de 130 px do Nanobô); roda e 1/2 trocam a arma — o clique no ícone do HUD foi descartado em 03/11 (noite). O ataque e o scanner deixam de ter tecla. | Partida completa só com mouse e WASD. Clique no HUD não dispara. O cursor do sistema vira mira dentro do mundo. |
| RF-13 | N (feito) | Texto em português com acento: `/utf-8` no `Nanobo.vcxproj` e arquivos salvos em UTF-8. Sem `src/ui/`: os textos ficam em `main.c` e `gameplay.c`. | Nenhum "?" nem caractere estranho no Release — verificado em 30/10 (`qa-mvp.md` §9). |
| RF-14 | F | Pós-M4 — Registro de eventos para o playtest (CSV local): tempo até o 1º pulso, releituras, erros por tipo, fonte de entrada. | O arquivo é gerado ao fim da partida. |
| RF-15 | F | Fase 2 (vírus) com a mesma estrutura de ensino; 3ª classe; β-lactamase; barra de endotoxina se a dupla reabrir o `decisoes.md` de 25/10. | Fora do MVP. |
| RF-16 | N | Identificação como condição de conclusão (D2.1, 03/11/2026): a fase só conclui quando todos os agentes foram identificados. A regra atual (#26/#67) aceita concluir com 0 identificações. | Só vale depois de resolvida D2.2 (`decisoes.md`): precisa existir sempre um caminho para concluir, inclusive para quem atirou antes de escanear. QA manual: partida com scan em todos conclui; sem scan não conclui. **Entrega:** o M4 mantém a regra #26; D2.1 e D2.2 entram no M5. |

¹ Decisão de design mantida, mas sem issue no M4: não entra no critério de aceite de 04/11.

**RF-16** entrou em 03/11/2026 por decisão da dupla (D2.1), depois da auditoria de clareza; no código continua valendo a regra de #26/#67 até D2.2 ser resolvida.

### Fora do M4 (03/11/2026)

- Diário de Campo (RF-08), zonas guiadas (RF-09) e registro de eventos (RF-14) só voltam ao backlog depois do build e do QA manual aprovados.
- Fase 2 (vírus), 3ª classe, β-lactamase e barra de endotoxina continuam fora do MVP (RF-15).
- Arte neutra (RF-03) segue como decisão de design, sem issue aberta; a trava e a perseguição (RF-06) entraram no build em 04/11 e falta a calibração com pessoas.
- Varredura por arrasto e economia de amostras continuam adiadas.

### Validação do M4

Uso de RAM ≤ 100 MB no Release; 0 avisos no build Debug e Release x64 (nível 4, `/std:c11`); QA manual do fluxo educativo (#107, #129) aprovado. O harness de regras fora do repositório (83 casos em 30/10) continua como evidência complementar, sem casos novos obrigatórios.

## Controles, mouse e idioma

Todo texto e toda tecla ficam em português, e o mouse passa a ser a forma de mirar, atirar e escanear; o teclado fica para mover, defender, trocar de arma e navegar.

### Tabela de controles

Uma única tabela alimenta o menu, a pausa e o README.

| Ação | Teclado | Mouse |
| --- | --- | --- |
| Mover | Setas ou WASD | — |
| Mirar |  | Mover o cursor (mira livre) |
| Atirar | — | Botão esquerdo |
| Escanear | — | Botão direito sobre o alvo |
| Trocar de arma | 1 (Ruptura) e 2 (Dissolução) | Roda do mouse |
| Defender | X | — |
| Diário de Campo | TAB (pós-M4) | Botão DIÁRIO na pausa (pós-M4) |
| Pausar | ESC | — |
| Tela cheia | F11 | — |
| Confirmar, avançar | ENTER | Clique no botão |
| Pular o briefing | ESC | Botão PULAR |

As teclas F, G e H continuam sendo de QA e só existem no build Debug.

### Regras do mouse no jogo

1. **Coordenadas.** Janela → tela lógica (`to_logical_x` e `to_logical_y`, que já existem) → mundo (lógica + `camera`). O cursor já é lido em `mouse_x` e `mouse_y` a cada `ALLEGRO_EVENT_MOUSE_AXES`.
2. **Mira.** Direção = cursor no mundo − posição do Nanobô, normalizada. Com o cursor a menos de 8 px do Nanobô vale `facing`. O projétil já guarda `dx` e `dy` em float, então só `shoot()` muda: nasce `shoot_toward(wx, wy)`.
3. **Botões.** Em `ALLEGRO_EVENT_MOUSE_BUTTON_DOWN`, `event.mouse.button == 1` chama `shoot_toward` e `== 2` chama o scan no alvo sob o cursor. Clique com y ≥ `PLAY_H` (HUD) nunca dispara nem troca de arma.
4. **Roda.** O `dz` de `ALLEGRO_EVENT_MOUSE_AXES` alterna Ruptura e Dissolução.
5. **Cursor.** `al_hide_mouse_cursor` ao entrar em GAME e `al_show_mouse_cursor` ao sair (menu, pausa, resultados). A mira é um desenho próprio no ponto do cursor.
6. **Botões de interface.** Continuam ativando no clique para baixo (decisão de 28/10 em `interacao-mouse.md`).
7. **Foco e tela cheia.** `refresh_mouse()` e `update_screen_fit()` já cobrem redimensionar e F11; a mira precisa usar a mesma conversão, para não errar o alvo em janela maximizada.

### Idioma e acentuação

- **A fonte aguenta.** A documentação do Allegro lista para a fonte embutida as faixas ASCII, Latin-1 (0x00A1 a 0x00FF), Extended-A e o euro ([al\_create\_builtin\_font](https://manpages.ubuntu.com/manpages/jammy/man3/al_create_builtin_font.3alleg5.html)). A página não fala em acentos; como as letras acentuadas do português estão em Latin-1, devem existir (inferência, confirmada pelo teste D4).
- **O compilador é o risco.** Sem `/utf-8`, o MSVC lê arquivo sem BOM na página de código do usuário e grava as strings estreitas nela ([Microsoft Learn](https://learn.microsoft.com/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=msvc-170), [execution\_character\_set](https://learn.microsoft.com/cpp/preprocessor/execution-character-set?view=msvc-170)). O Allegro espera UTF-8. Por isso “arquivo salvo com BOM e sem `/utf-8`” tende a gravar “ã” como 1 byte e quebrar o texto na tela (inferência a partir da documentação; o teste D4 confirma). O aviso C4819 sugere “UTF-8 com assinatura”: só aceite com `/utf-8` ligado.
- **A correção aplicada:** `/utf-8` em `AdditionalOptions` do grupo x64; equivale a `/source-charset:utf-8 /execution-charset:utf-8`.
- **Situação verificada em 03/11/2026:** o `/utf-8` está ligado no grupo x64 (`Nanobo.vcxproj`, linha 114) e os textos do jogo já estão acentuados em UTF-8 (commit `75030ea`, #194 fechada). O teste de D4 foi feito e passou nas seis telas do QA (`qa-mvp.md` §9).
- **Onde os textos moram.** Nos próprios arquivos compilados: `main.c` para telas e HUD, `gameplay.c` para scanner e feedback. Não existe `src/ui/`.
- **Vocabulário.** “Escanear” (não “ler”), “arma” (não “laser”, “raio” nem “módulo A/B”). No código, os nomes `Laser` e `LASER_*` ficam: renomear gera conflito no `main.c` sem ganho para o jogador.
- **Teste D4 (feito em 30/10).** Desenhar “Ação, coração, é, ü, ç” no menu, em Debug e Release. Passou: as seis telas do QA saem sem mojibake e sem “?”.

## Dificuldade e balanceamento

> **Escopo:** esta seção é uma proposta de calibração da revisão de 02/11. A trava e a perseguição entraram no build de 04/11 por decisão da dupla (`decisoes.md` §"Trava e perseguição"); o pulso do scan continua fora.
>
> **Leia com a tabela de dívida:** os números abaixo vêm de uma cópia patchada do `gameplay.c`. Só o pulso de 0,75 s **não está no build**; o estado de cada regra está em `decisoes.md` §"Dívida de implementação".

A dificuldade passa a vir do raciocínio e não do reflexo: o scan custa tempo parado, a arma errada trava o fogo e põe a bactéria em perseguição. Recalibrei essas regras com a mira livre (rodada de 01/11). Com perseguição de 150 px a 60 px/s, trava de 2 s e pulso de 0,75 s, o “humano” simulado vence de 61% a 72% das partidas, quem chuta a arma vence de 1% a 3% e o jogador ausente nunca vence. A meta que não fechou é a duração: o leitor termina a fase em 31 s.

### Como a simulação foi feita

Simulação headless sobre uma cópia patchada do `gameplay.c` real (mira livre, pulso do scan que imobiliza e cancela com dano, as duas armas — então A/B —, trava, perseguição e D1), com 500 partidas por bot e configuração e limite de 300 s. Os bots são modelos simples, não pessoas:

- **Leitor:** escaneia cada bactéria, mira perfeito com antecipação e usa a arma certa.
- **Humano simulado:** erro de mira gaussiano de 5°, 0,4 s de reação por alvo, 0,25 s entre ler e atirar, 5% de arma trocada e 85% de chance de checar se há célula na linha.
- **Chuta a arma:** não escaneia e troca de arma depois de errar.
- **Aperta tudo:** arma aleatória, sem defesa, atira em qualquer coisa.
- **Ausente:** fica parado.

### Resultado: porcentagem de vitórias

| Configuração | Leitor | Humano | Chuta a arma | Aperta tudo | Ausente | Dano do humano (de 3) |
| --- | --- | --- | --- | --- | --- | --- |
| A. perseguição 45 px/s, trava 2 s, pulso 0,75 s | 100% | 94% | 2% | 0% | 0% | 0,49 |
| **B. perseguição 60 px/s, trava 2 s, pulso 0,75 s** | 100% | 72% | 2% | 0% | 0% | 0,90 |
| C. perseguição 60 px/s, trava 3 s, pulso 0,75 s | 100% | 69% | 2% | 0% | 0% | 1,00 |
| D. perseguição 60 px/s, trava 3 s, pulso 1,0 s | 100% | 68% | 2% | 0% | 0% | 1,08 |
| E. perseguição 75 px/s, trava 3 s, pulso 0,75 s | 100% | 67% | 0% | 0% | 0% | 1,15 |
| F. igual a C, com a D1 desligada | 100% | 69% | 2% | 0% | 0% | 1,03 |
| G. sem perseguição, trava 2 s, pulso 0,75 s | 100% | 100% | 97% | 0% | 0% | 0,07 |

O humano simulado muda pouco quando o perfil muda (vitórias do humano nas configurações A e B):

| Erro de mira e reação | Config. A | Config. B |
| --- | --- | --- |
| 3° e 0,25 s | 94% | 69% |
| 5° e 0,4 s | 94% | 72% |
| 8° e 0,6 s | 89% | 65% |
| 12° e 0,8 s | 83% | 61% |

### O que o resultado mostra

- **A perseguição é o que obriga a escanear.** Sem ela (linha G), quem chuta vence de 93% a 97%, porque basta trocar de arma depois do erro; a trava de 2 s sozinha não ensina nada. Com a perseguição depois do arma errada, o chute cai para 0% a 3%.
- **O degrau de 82% para 6% da simulação antiga não se repete.** Aquela rodada usava mira em 4 direções e perseguição sempre ligada. Com perseguição só depois do erro, subir de 45 para 60 px/s leva o humano simulado de 94% para 72%, sem degrau.
- **Trava de 3 s e pulso de 1,0 s mudam pouco** (humano em 69% e 68%, contra 72%). O pulso de 1,0 s só estica a fase em 3 a 4 s. Uma trava de 1 s, em rodada preliminar de 100 partidas, deixou quem chuta passar de 3% para 26%, então 2 s é o mínimo.
- **A D1 quase não mexe na dificuldade** para quem escaneia (0,03 contra 0,07 acertos em célula por partida). Ela é uma decisão de ensino, não de balanceamento.
- **O leitor vence 100% em todas as configurações.** A meta antiga de 70% a 90% para ele não era alcançável sem tornar o jogo impossível para o humano simulado, então ela foi trocada por “pelo menos 95%”, que mostra que o jogo é justo e aprendível.
- **A fase dura pouco.** O leitor termina em 31 s e o humano simulado em 35 a 47 s, bem abaixo da meta de 60 a 150 s. Constantes não resolvem isso; só mais bactérias ou as zonas (D3) alongam a fase.

### Constantes iniciais

| Constante | Valor inicial | Observação |
| --- | --- | --- |
| `SCAN_PULSE_TIME` | 0,75 s por pulso, 1,5 s no total | Faixa a testar: 0,5 a 1,0 s. |
| `FIRE_LOCK_TIME` | 2,0 s (no build desde 04/11) | Não descer de 2 s; 3 s não muda o resultado. |
| Alcance da perseguição | qualquer distância (04/11) | Fugir não apaga o alerta; os 150 px da proposta foram descartados. |
| `CHASE_SPEED` | 60 px/s (no build desde 04/11) | A mais sensível: 45 deixa o humano simulado em 94%, 60 em 72%; 45 é o recuo se o playtest mostrar frustração. |
| Gatilho da perseguição | só depois de uma arma errada | A bactéria fica em alerta até ser neutralizada. Sem esse gatilho, o chute vence quase sempre. |
| Recarga do disparo | 1,0 s (a de hoje) | Não muda. |

Todas ficam juntas em `gameplay.h`, para ajustar em minutos durante o playtest.

### Metas de aceite

Bots de calibração: leitor com mira perfeita vence pelo menos 95%; humano simulado vence de 55% a 75%; bot de chute e bot que atira no escuro vencem no máximo 10%; jogador ausente, no máximo 5%. Com pessoas: pelo menos 3 de 5 vencem em até 3 tentativas, e ninguém vence sem escanear ao menos 3 bactérias. A duração média de 60 a 150 s para o leitor continua em aberto, pelo motivo acima.

Se a meta não fechar, as alavancas vêm da mais barata para a mais cara: velocidade de perseguição (uma constante, como já previa o `decisoes.md` de 25/10), tempo do pulso e, por fim, a quantidade de bactérias por zona, que é a única que alonga a fase. Uma barra de integridade continua rejeitada.

### Limites desta simulação

- Os bots não são pessoas. A mira é instantânea (sem o tempo de apontar com o mouse), então pessoas reais tendem a vencer menos que o humano simulado; por isso 60 px/s é o teto razoável, e 45 px/s é o ponto de partida seguro se o playtest mostrar frustração.
- O perfil do humano simulado (5°, 0,4 s, 5% de arma trocada) é uma suposição minha, sem medição com jogadores.
- A perseguição é em linha reta, sem obstáculos, e o alerta nunca expira.
- Vale para o mundo de hoje, com 9 agentes e uma única fase.

## Sincronização documental (03/11/2026)

Esta seção substitui o mapa de 02/11: ela registra o que esta sincronização já alterou e o que falta, para o próximo passo ser decidido pela leitura e não pela memória de quem escreveu.

| Arquivo | Estado em 03/11 | O que falta |
|---|---|---|
| `docs/game-design/ensino-e-aprendizagem.md` | criado nesta sincronização | — |
| `decisoes.md` | atualizado: D1, D2, briefing, módulos, mouse e as decisões substituídas de 18/10 e 26/10 | — |
| `scanner.md` | atualizado: pulsos 1 e 2, nome do grupo, scan por cursor, marca de identificação | conferir contra a implementação da #104 |
| `gdd.md` | atualizado: ciclo de aprendizagem, briefing, módulos, controles e resumo | — |
| `conteudo-biologico.md` | atualizado: nível dos termos, textos do M4 e simplificações | validação da dupla (#150) |
| `kanban.md` | atualizado com o retrato de 03/11 e o M4 por issue | reconferir antes de citar |
| `qa-mvp.md` | nova seção datada com os passos do fluxo educativo | executar depois da implementação (#107, #129) |
| `textos-do-jogo.md` | criado em 03/11; strings das armas e da camada 1 atualizadas na mesma data | — |
| `glossario.md` | atualizado em 03/11 (noite): verbetes Arma, Coloração e Nome do agente | — |
| `escopo.md` | **não atualizado** | mover Diário, zonas e telemetria para pós-M4 |
| `interacao-mouse.md` | **não atualizado** | regras do cursor em `GAME` |
| `fluxo-estados.md` | **não atualizado** | cena `BRIEFING` e transições |
| `milestones.md` | **não atualizado** | critério do M4 e o que sobra para o M5 |
| `README.md` | **não atualizado** | só depois do código entrar no `main` |

Para achar o vocabulário antigo:

```bash
grep -rniE "space|espaço|laser|raio (roxo|vermelho)|J atacar" README.md docs assets/catalogo-recursos.md
```

Quem escreve não valida: cada PR de documentação é revisto pela outra pessoa, como no código.

## Impacto no código (contrato de 03/11)

Só os quatro arquivos de `<ClCompile>` compilam: `src/main.c`, `src/core/game.c`, `src/entities/entities.c` e `src/gameplay/gameplay.c` (`Nanobo.vcxproj`, linhas 118–121). **Não nascem módulos `src/ui/`**: a interface nova continua no `main.c` e as regras no `gameplay.c`.

| Arquivo | O que muda |
|---|---|
| `src/scenes/scenes.h` e `src/core/game.c` | `GAME_SCENE_BRIEFING` entra no enum e nas transições permitidas: `MENU → BRIEFING → GAME`, com `BRIEFING → GAME` por ESC ou clique. |
| `src/main.c` (1.609 linhas hoje) | Eventos: botão 1 atira para o cursor, botão 2 escaneia o alvo sob o cursor, `dz` da roda troca a arma e `1`/`2` idem; `SPACE` e `J` saem do ataque e do scanner (#198). Desenho: cena de briefing, ícones das armas no lugar do raio, caixa de feedback de 2 linhas, resumo em `draw_results` (hoje na linha 1.082) e quebra de linha por `al_get_text_width`. |
| `src/gameplay/gameplay.h` | Constantes de calibração em um bloco só (pulso, trava, perseguição — proposta), protótipos `shoot_toward(float x, float y)` e `scan_at(float x, float y)`, leitura dos contadores e do resumo. |
| `src/gameplay/gameplay.c` | `use_scanner()` (hoje na linha 327) vira pulso; `shoot()` (391) passa a chamar `shoot_toward`; `hit_projectiles()` (229) aplica a matriz arma × espécie e o feedback por ID; `update_match()` (816) move a perseguição e imobiliza no pulso; `reset_match()` (732) zera os contadores; `layer_one_text()`/`layer_two_text()` (58 e 70) devolvem a coloração e o nome do grupo. |
| `src/entities/entities.h` | `Projectile` já guarda `dx` e `dy`; `Agent` não precisa de campo novo. `Laser` e `LASER_*` ficam. |
| `Nanobo.vcxproj` | Nada a fazer além de manter os quatro `<ClCompile>`: `/utf-8` já está na linha 114. |

- `shoot_toward` e `scan_at` recebem coordenadas do mundo e não leem o mouse: é o que permite conferir a regra sem Allegro.
- `al_init_primitives_addon()` não é chamado no `main()`; não há reprodução no Windows e a auditoria de 03/11 não abriu issue para isso.

## Plano do M4 por issue (03/11/2026)

| Issue | Entrega | Dono | Depende de |
|---|---|---|---|
| #103 | Briefing de três cards, pulável | Eric | — |
| #106 | Fluxo Menu → Briefing → Fase 1 | Eric | #103 |
| #104 | Scanner em dois pulsos: nome e estrutura | Gustavo | — |
| #105 | Módulos, célula própria e feedback educativo | Gustavo | #104 |
| #200 | Progresso de aprendizagem em memória | Gustavo | #104, #105 |
| #198 | Controles por cursor e seleção de módulo | Gustavo | #104, #105, #106 |
| #199 | HUD e sinais visuais didáticos | Eric | #104, #105 |
| #174 | Resumo por objetivo (em #57) | Eric | #200 |
| #176 | QA dos resultados (em #57) | Gustavo | #174 |
| #107 | QA manual do fluxo educativo | Gustavo | todas as filhas de #58 |
| #129 | QA cruzado do scanner, identificação e controles | Eric | #104, #198 |
| #150 | Sincronizar documentação | Eric | — |

Ordem em 03/11 (noite): **briefing (#103/#106)** → **camada 1 (#104)** → **nomes das armas + cor neutra no menu, HUD e feedback** → **disco cinza + anel** → ícones novos (M5). Corte de baixo para cima se o tempo não fechar; briefing e camada 1 não se cortam. Mudanças concorrentes no `main.c` entram em série, e o glifo `MOVIMENTAÇÃO` é corrigido no mesmo bloco do menu.

## Verificação, playtest e perguntas do professor

O M4 se prova com build Debug e Release x64 e QA manual; o playtest com pré e pós-teste pertence ao M5 (#134–#137).

### 1. Build e QA manual

- `MSBuild Nanobo.vcxproj -p:Configuration=Debug -p:Platform=x64 -t:Rebuild` e o mesmo em Release: **0 avisos, 0 erros** (nível 4, `/std:c11`).
- QA manual de #107 e #129 conforme `qa-mvp.md` §10: pular briefing, os dois resultados de Gram, troca de arma, acerto e erro, dano à célula com as duas armas, transição para Resultados, resumo por objetivo e reinício limpo.
- O harness de regras fora do repositório (83 casos em 30/10) continua como evidência complementar; o M4 não exige casos novos.

### 2. Playtest com pré e pós-teste (M5)

3 a 5 pessoas, de preferência em duplas conversando. O resultado é formativo, não estatístico. Aplicar as mesmas quatro perguntas antes e depois da partida, em ordem diferente, com jogo sem ajuda entre elas. Quem puder, repete o pós-teste no dia seguinte; esse resultado é registrado separadamente.

| ID | Pergunta | Resposta esperada | OA |
| --- | --- | --- | --- |
| P1 | O que os resultados roxo e vermelho (rosa) da coloração de Gram indicam? | Grupos bacterianos com camadas externas diferentes; a cor não diz se a bactéria causa doença nem indica tratamento, e o nome do agente identifica sem classificar. | OA1 |
| P2 | Compare as camadas externas Gram+ e Gram− e diga onde entra o peptidoglicano. | Gram+ costuma ter parede mais espessa; Gram− tem parede mais fina e camada externa extra. O peptidoglicano é parte da parede bacteriana. | OA2 |
| P3 | Diante da estrutura lida no scan, qual arma corresponde à regra do jogo e qual é seu alvo? | A representa um alvo na parede; B, a camada externa extra das Gram−. É uma regra do jogo, não uma recomendação de medicamento. | OA3 |
| P4 | Como agir perto da célula própria? A regra do jogo prova que medicamentos reais são seguros? | Preservar a célula. Não: a regra é simplificada e não diz que medicamentos reais sejam isentos de efeitos adversos. | OA4 |

Cada pergunta vale 1 ponto e corresponde a um objetivo; registrar resultados por OA para que a soma não esconda uma lacuna. Sinal formativo provisório: pelo menos 80% dos participantes respondem corretamente a cada objetivo no pós-teste. Se algum objetivo ficar abaixo, registrar a lacuna e revisar o ensino antes de afirmar que todos os conteúdos foram aprendidos. Com 3–5 participantes, esse sinal não demonstra eficácia estatística. Os dados de gameplay (escaneamentos, erros e vitória) são analisados à parte e não substituem o pré/pós-teste.

Cada jogador vira uma linha em qa-aprendizagem.md: resposta por OA antes e depois, releituras, erros por tipo, vitória, mouse ou teclado e 3 falas literais, sem nome.

### 3. Perguntas prováveis do professor

- **O que o jogador aprende além de peptidoglicano?** A interpretar as cores da coloração, comparar as camadas externas, decidir com base na leitura do scanner e preservar a célula própria.
- **Isso não é só escolher entre Ruptura e Dissolução?** A escolha é o último elo da sequência: interpretar o resultado, ler a estrutura, escolher segundo a regra do jogo e observar a consequência. A arma é um modelo didático, não uma prescrição.
- **Como vocês medem cada conteúdo?** No M4, com os contadores em memória e o resumo por objetivo; no M5, com uma pergunta de pré/pós-teste para cada objetivo (P1–P4).
- **Por que as armas podem ferir a célula própria?** Essa é a regra de risco das armas no jogo; não é uma explicação literal de um medicamento real.
- **O que ficou de fora?** Detalhes de PBP/transpeptidase, ligações cruzadas, LPS, polimixina e efeitos clínicos; a ordem completa dos corantes; nomes de espécies; e a Fase 2.
- **Para que serve o mouse?** Mirar, atirar, escanear e trocar de arma.

## Confiança, riscos e ressalvas

A confiança geral é 0,73: o conteúdo factual central tem apoio em fontes de biologia e preparação para o ENEM, mas a classificação de nível não é uma determinação oficial da BNCC. A confiança no desenho de aprendizagem é 0,75; a calibração de dificuldade permanece 0,65 até o playtest, e o prazo permanece 0,55.

| Subproblema | Peso | Confiança | Por quê |
| --- | --- | --- | --- |
| Validação das ideias das IAs | 15% | 0,85 | Fatos conferidos em fontes abertas; o julgamento de escopo é meu. |
| Conteúdo biológico e adequação ao Ensino Médio | 15% | 0,60 | A parede, o peptidoglicano e a distinção Gram aparecem em material de preparação para o ENEM; a BNCC não prescreve esses termos como conteúdos isolados. |
| Design de ensino (fluxo, feedback, armas) | 25% | 0,75 | Coerente com Habgood e Wouters, mas a evidência é de outros jogos; o efeito real só aparece no playtest. |
| Requisitos e mapa da documentação | 15% | 0,85 | Leitura de quase todos os documentos; 4 não foram lidos. |
| Impacto no código | 10% | 0,80 | Código relevante lido; só uma cópia patchada do gameplay.c foi compilada e simulada. |
| Dificuldade e calibração | 10% | 0,65 | Simulação com mira livre de 01/11 (500 partidas por bot), mas com bots e não com jogadores; a duração da fase não fecha a meta. |
| Plano e prazo | 10% | 0,55 | A aula é em 04/11 e o fluxo educativo ainda não está no código. |

A média ponderada é 0,73, abaixo de 0,8 (estimativa de 02/11, não recalculada após a auditoria de 03/11). A redução em relação à versão anterior reflete a incerteza curricular explicitada, não uma mudança no plano de gameplay. A duração e o equilíbrio continuam dependendo de playtest com pessoas.

### Riscos

| Risco | Impacto | Mitigação |
| --- | --- | --- |
| A escolha entre Ruptura e Dissolução é vista como “escolher entre opções”, que o curso não espera | Alto | Cadeia interpretar, ler, deduzir, escolher; custo de tempo, trava e perseguição; consequência na célula própria (D1); levar a justificativa ao professor (D6). |
| A cor da arma vira atalho “cor com cor” | Alto | RF-04: cor neutra, ícone e projétil por forma; conferir em captura de tela. |
| O briefing é pulado | Médio | Curto e com as dicas na hora do uso; o Diário que o reabriria ficou fora do M4. |
| Acentos quebram no Release | Baixo | Resolvido: `/utf-8` e teste D4 passaram (#194); a regra de salvar sempre em UTF-8 fica registrada. |
| Texto não cabe na tela de 640×360 | Médio | Limite de 70 caracteres por linha e draw_text_wrapped. |
| Conflitos no main.c | Médio | Interface no próprio `main.c`, mudanças em série e revisão cruzada; uma issue por vez no arquivo. |
| A mira livre elimina o desafio de reflexo | Alto | Calibrar as constantes; metas dos bots. |
| A simplificação biológica vira uma afirmação sobre medicamentos reais | Médio | Marcar a matriz Ruptura/Dissolução como regra do jogo, manter os aprofundamentos fora do núcleo e revisar os quatro OAs separadamente. |
| Playtest pequeno demais | Baixo | Tratar como sinal formativo, não como estatística. |

### Ressalvas

- A dificuldade foi recalibrada com mira livre, só com bots: a mira instantânea e o perfil do humano simulado são suposições, e a duração da fase (31 s para o leitor) não fecha a meta de 60 a 150 s.
- Em 03/11 nada do fluxo educativo está no código: o scanner ainda revela camadas por tecla, o raio ainda é roxo/vermelho por espécie e não existe cena de briefing. Esta revisão é documental e não altera código.
- As horas do plano são estimativas.
- A adequação ao Ensino Médio é estimada a partir de materiais de preparação para o ENEM. A BNCC é referência de competências gerais da área e não uma lista específica desses termos; não foi consultado um livro do PNLD nem currículo estadual.
- A sequência detalhada da coloração e os mecanismos de antibióticos não são requisitos do MVP. OpenStax é usado para aprofundamento técnico, não como evidência de que esses detalhes sejam conteúdo obrigatório do Ensino Médio.
- Cinco documentos do repositório seguem sem revisão nesta rodada (glossario, escopo, interacao-mouse, fluxo-estados, milestones) — ver “Sincronização documental”.
- As duas IAs viram a resposta anterior; a concordância delas não é validação independente.
- D1, D2 e D4 ficaram registradas em `decisoes.md` em 03/11; D3 e D5 estão fora do M4; D6 (perguntas ao professor) continua pendente com a dupla.
- Correção de 03/11 (noite): o mapeamento roxo/vermelho (#62) mudou — a coloração é roxo/vermelho, as armas têm nome e cor neutra, e o HUD não usa mais letra A/B.

## Fontes

### Nível escolar e conteúdo essencial

- [BNCC do Ensino Médio — MEC](https://portal.mec.gov.br/docman/abril-2018-pdf/85121-bncc-ensino-medio/file): referência oficial de competências amplas de Ciências da Natureza; não enumera peptidoglicano, LPS ou PBP como conteúdos obrigatórios.
- [PrePara ENEM — Bactérias Gram-positivas e negativas](https://www.preparaenem.com/biologia/bacterias-gram-positivas-e-negativas.htm): referência de preparação para o ENEM sobre cores e diferenças gerais entre os grupos.
- [Manual do Enem — Bactérias](https://querobolsa.com.br/enem/biologia/bacteria): material de preparação que aborda parede celular, peptidoglicano e classificação de Gram.
- [Ministério da Saúde — Técnica de coloração de Gram](https://bvsms.saude.gov.br/bvs/publicacoes/0107tecnica_gram.pdf): referência factual para o procedimento; não é usada para atribuir nível escolar.

### Aprofundamento técnico, fora do núcleo

- [OpenStax Microbiology, 14.3 — Mechanisms of Antibacterial Drugs](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs): PBP/transpeptidase, ligações cruzadas, LPS, polimixinas e seletividade; usado apenas para conferir ou explicar material opcional.
- [OpenStax Biology 2e — Structure of Prokaryotes](https://openstax.org/books/biology-2e/pages/22-2-structure-of-prokaryotes-and-archaea): referência adicional de estrutura geral de bactérias.

### Outras fontes preservadas

- [Allegro, al_create_builtin_font](https://manpages.ubuntu.com/manpages/jammy/man3/al_create_builtin_font.3alleg5.html): faixas Unicode da fonte embutida.
- Microsoft Learn: [/utf-8](https://learn.microsoft.com/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=msvc-170), [execution_character_set](https://learn.microsoft.com/cpp/preprocessor/execution-character-set?view=msvc-170) e [aviso C4819](https://learn.microsoft.com/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4819?view=msvc-170).
- [Habgood e Ainsworth 2011, Journal of the Learning Sciences 20(2):169–206](https://nottingham-repository.worktribe.com/output/23577510/motivating-children-to-learn-effectively-exploring-the-value-of-intrinsic-integration-in-educational-games): resumo do repositório.
- [Wouters et al. 2013, Journal of Educational Psychology 105:249–265](https://research-portal.uu.nl/en/publications/a-meta-analysis-of-the-cognitive-and-motivational-effects-of-seri/): dados bibliográficos; o abstract foi lido em [Karl Kapp](https://karlkapp.com/?p=4333), fonte secundária.
- [Stegman 2014, Faraday Discussions 169:403–423](https://pmc.ncbi.nlm.nih.gov/articles/PMC4489431/): estudo em sala de aula com o Immune Attack.
- [Immune Defense (itch.io)](https://melanieann.itch.io/immune-defense), [Mindustry (GitHub)](https://github.com/Anuken/Mindustry) e [Scan Visor (Metroid Wiki)](https://metroid.fandom.com/wiki/Scan_Visor): usados para corrigir afirmações das IAs e como precedente de design.

Não conferidos nesta rodada (†): princípios de Mayer, princípio de feedback explicativo e a descrição do DungeonRoyale, lida na rodada anterior. A adequação curricular continua estimada; não foi conferida em livro do PNLD nem currículo estadual.

Documentos do repositório usados: README.md, gdd.md, scanner.md, conteudo-biologico.md, glossario.md, decisoes.md, escopo.md, one-sheet.md, interacao-mouse.md, milestones.md, qa-mvp.md, referencias-biologicas-issues-2-177.md, kanban.md, plano-backlog-restante.md, divisao-issues-dupla.md, fluxo-estados.md, plano-de-execucao.md, catalogo-recursos.md; código: main.c, gameplay.c, gameplay.h, entities.h, Nanobo.vcxproj.
