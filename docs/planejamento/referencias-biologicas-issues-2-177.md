# Referências biológicas para as Issues #2 e #177

**Objetivo:** dar base científica mínima para definir os dados dos agentes e objetivos educacionais observáveis. Este documento separa fatos biológicos de abstrações de gameplay; não altera decisões da dupla.

**Contexto e critérios confirmados no GitHub:** #2 pede nome, tipo e uma informação curta para scanner para vírus como ameaça (1 resistência), bactéria como ameaça (2 resistências) e célula própria não atacada; a dupla revisa o conteúdo. #49 define “resistência” como pontos abstratos de combate, reduzidos em 1 por acerto: vírus tem 1 ponto e bactéria 2; ataque não destrói célula própria. #16 associa raio roxo à Gram+ e raio vermelho à Gram−. #177 pede 1–2 objetivos por conteúdo, com cadeia scanner → decisão → evidência e pendências da Fase 2 explicitamente marcadas.

## Resumo para decisão

- Vírus podem ser tratados como ameaça no jogo, desde que o texto diga que vírus patogênicos infectam células e usam a maquinaria da hospedeira para se replicar. Nem todo vírus infecta humanos nem toda infecção causa doença.
- “Resistência” neste jogo já está definida por #49 como pontos abstratos de combate: vírus começa com 1 e bactéria com 2; cada acerto reduz um ponto. Não é resistência antimicrobiana nem vida útil biológica. Manter essa distinção nos textos e feedbacks.
- Gram+ e Gram− são categorias estruturais úteis para a Fase 1; o raio roxo para Gram+ e vermelho para Gram− é a regra de gameplay de #16. Esses raios e os pontos de resistência não devem ser apresentados como tratamentos médicos nem como valores biológicos universais.
- Célula saudável própria como alvo inválido é uma regra de jogo adequada ao aprendizado pretendido. Biologicamente, o sistema imune normalmente mantém tolerância ao próprio, mas essa tolerância pode falhar em doenças autoimunes; evite formular a regra como “o corpo nunca ataca as próprias células”.

## Fatos e formulações curtas para o scanner

As frases sugeridas são textos didáticos curtos, não descrições clínicas nem identificação de espécies.

| Agente | Tipo e regra de jogo (#2/#16/#49) | Fato que pode ser ensinado | Texto curto sugerido para scanner |
|---|---|---|---|
| Vírus | Ameaça da missão; 1 ponto de resistência abstrata; cada acerto reduz um ponto. | Vírus não se reproduzem autonomamente: usam uma célula hospedeira; certos vírus podem danificar ou matar a célula. Vírus têm especificidade de hospedeiro/célula, então não se deve inferir que qualquer vírus infecta qualquer célula. | **“Vírus: precisa de uma célula hospedeira para se multiplicar.”** |
| Bactéria Gram-positiva | Ameaça patogênica do cenário; 2 pontos de resistência abstrata; raio roxo. | Em termos gerais, parede celular espessa rica em peptidoglicano; não possui membrana externa. A coloração de Gram é baseada em diferenças do envelope celular. | **“Gram+: parede celular espessa; grupo identificado pela coloração de Gram.”** |
| Bactéria Gram-negativa | Ameaça patogênica do cenário; 2 pontos de resistência abstrata; raio vermelho. | Em termos gerais, camada fina de peptidoglicano e uma membrana externa. | **“Gram−: parede fina e membrana externa; grupo identificado pela coloração de Gram.”** |
| Célula saudável própria | Célula a preservar, não alvo; não é destruída por ataques. | A tolerância imunológica ao próprio reduz respostas contra tecidos próprios; perda de tolerância está associada à autoimunidade. “Não atacar” é a regra pedagógica/de jogo desta fase. | **“Célula saudável: pertence ao próprio corpo; não é alvo nesta missão.”** |

**Sobre “ameaça”:** bactérias não são todas nocivas. O CDC ressalta que a maioria dos micróbios é inofensiva ou útil e define patógenos como os que causam infecções. Portanto, a classe de alvo no jogo deve ser descrita como **bactéria patogênica** quando essa intenção for relevante, ou contextualizada como ameaça específica da missão. Não ensine que toda bactéria deve ser eliminada.

**Sobre resistência antimicrobiana:** não atribuir “Gram+ = 1” ou “Gram− = 2” como regra biológica. A membrana externa de Gram− pode restringir a entrada de alguns antibióticos, mas mecanismos e perfis variam. O CDC define suscetibilidade/resistência em relação a teste de isolado e fármaco(s). Os valores 1/2 de #49 são pontos abstratos de combate, e o acerto de #16 reduz esse contador conforme a regra do jogo.

## Critérios e objetivos observáveis de #177

O critério da issue é 1–2 objetivos por conteúdo, ligados por **scanner → decisão → evidência**. As propostas abaixo respeitam esse formato; são decisões de aprendizagem/design, não fatos descobertos nas fontes.

### Fase 1 — bactérias e células próprias

1. **Classificar e selecionar o raio:** após ler as pistas de parede/membrana externa no scanner, o jogador distingue Gram+ de Gram− e escolhe o raio roxo (Gram+) ou vermelho (Gram−), conforme #16. Evidência: classe/raio escolhido e resultado do ataque; raio certo reduz um ponto de resistência e o incorreto não, conforme #49.
2. **Preservar a célula própria:** depois de identificá-la no scanner, o jogador decide não atacá-la. Evidência: escolha do jogador e integridade da célula, que não pode ser destruída por ataque conforme #49.

### Fase 2 — vírus e ciclos

1. **Interpretar o ciclo mostrado:** depois de ler a informação do scanner, o jogador identifica se o vírus está no estado lítico ou lisogênico e decide com base nessa informação. Evidência prevista: estado identificado e decisão registrada no encontro. Os pormenores dessa pista/decisão dependem das regras de fase 2 e permanecem **[PENDÊNCIA]**.

**Escopo atual das dependências e pendências (confirmado nas Issues):**

- **#83 — regras do vírus:** os estados lítico/lisogênico; estado lisogênico estático e piscando; timer que leva ao estado lítico; evento de multiplicação limitado a até três novos vírus; trava contra cadeia infinita. Permanecem pendentes: tempo exato do timer, detalhes do comportamento lítico, se/como o limite se aplica entre gerações, informação exibida em cada pulso e consequências exatas.
- **#87 — scanner da Fase 2:** reaproveita seleção de alvo, pulsos e identificação única já existentes; informa o estado atual sem recomendar ação. Dados exatos por camada/pulso dependem da definição de #83 e da revisão da dupla.
- **#88 — resposta à ação:** avalia a ação do jogador contra o estado viral atual e aplica apenas consequências definidas em #83. As ações corretas/incorretas e suas consequências específicas ainda precisam ser definidas.

Esses itens são lacunas de especificação, não fatos biológicos. A evidência final da Fase 2 deve corresponder às regras aprovadas, sem antecipar qual ação é correta enquanto #83/#88 permanecerem pendentes.

O fluxo exigido é: **scanner → decisão → evidência observável**. Para cada objetivo, a equipe ainda precisa especificar um critério de sucesso (por exemplo, taxa de acerto e em quais encontros), pois #177 não fixa limiares.

## Decisões que devem permanecer pendentes para revisão da dupla

1. A dupla ainda deve revisar/aprovar nomes, classificação, informação curta e formulação “ameaça” exigidos por #2; escolher espécies se elas forem exibidas e não sugerir que toda bactéria/vírus cause doença.
2. Confirmar a apresentação dos dois raios #16 (roxo/Gram+, vermelho/Gram−) e do efeito de cada acerto sobre resistência #49 no feedback do jogador.
3. A Fase 2 permanece **[PENDÊNCIA]**: confirmar timer e comportamento lítico, regra do limite de multiplicação entre gerações, conteúdo de cada pulso, ação correta/incorreta e suas consequências (#83, #87, #88). #87 depende de #83/#84/#85; não fixar recomendações do scanner antes da revisão.
4. Especificar evidências de aprendizagem registradas e limiar de sucesso por objetivo, que #177 não fixa.

## Fontes científicas e institucionais

1. [CDC — About Antimicrobial Resistance](https://www.cdc.gov/antimicrobial-resistance/about/index.html): patógenos, maioria dos micróbios inofensivos/úteis; mecanismos de resistência e papel de membrana externa de Gram− na restrição de entrada de alguns antibióticos.
2. [CDC NARMS — Glossary of terms related to antimicrobial resistance](https://www.cdc.gov/narms/glossary/index.html): definição de teste de suscetibilidade, resistência, isolado e conjunto de antibióticos de teste.
3. [NCBI Bookshelf — The Cell: Cell Walls and the Extracellular Matrix](https://www.ncbi.nlm.nih.gov/books/NBK9874/): comparação celular de Gram+ (parede espessa, sem membrana externa) e Gram− (parede fina sob membrana externa).
4. [CDC — Viral Infections, MedlinePlus](https://medlineplus.gov/viralinfections.html): vírus infectam células, usam-nas para multiplicar-se e podem matar, danificar ou alterar células; impacto variável.
5. [OpenStax Microbiology — 6.2 The Viral Life Cycle](https://openstax.org/books/microbiology/pages/6-2-the-viral-life-cycle): dependência dos vírus em relação às células e descrição dos ciclos lítico/lisogênico, incluindo integração de fago temperado no cromossomo hospedeiro.
6. [NCBI Bookshelf — Immunobiology: Self-tolerance and its loss](https://www.ncbi.nlm.nih.gov/books/NBK27174/): tolerância ao próprio e como sua perda pode gerar autoimunidade.

## Issues citadas como fonte das regras do jogo

- [#2 — conteúdo biológico dos agentes](https://github.com/gustavo-hsilva79/Nanobo/issues/2): cadastro mínimo (nome, tipo, informação curta) para vírus, bactéria e célula própria; requer revisão da dupla.
- [#16 — regra dos raios por Gram](https://github.com/gustavo-hsilva79/Nanobo/issues/16): raio roxo para Gram+ e vermelho para Gram−.
- [#49 — colisão do projétil e resistência](https://github.com/gustavo-hsilva79/Nanobo/issues/49): 1 ponto para vírus, 2 para bactéria, um ponto removido por acerto; célula própria não é destruída.
- [#177 — objetivos de aprendizagem e critérios educacionais](https://github.com/gustavo-hsilva79/Nanobo/issues/177): 1–2 objetivos por conteúdo; explicitar scanner → decisão → evidência; marcar pendências da Fase 2.
- [#83 — regras da Fase 2](https://github.com/gustavo-hsilva79/Nanobo/issues/83), [#87 — scanner da Fase 2](https://github.com/gustavo-hsilva79/Nanobo/issues/87) e [#88 — resposta à ação na Fase 2](https://github.com/gustavo-hsilva79/Nanobo/issues/88): escopos e lacunas detalhados acima.
