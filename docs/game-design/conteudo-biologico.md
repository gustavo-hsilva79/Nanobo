# Nanobô — Conteúdo biológico e objetivos de aprendizagem

> **Estado:** Aprovado pela dupla (Gustavo & Eric). Conteúdo factual e textos curtos de scanner revisados e validados, atendendo ao critério de aceite da Issue #2.

> **Estado em 18/10/2026:** os dados da Fase 1 desta tabela estão implementados no jogo (resistência 2 para bactéria, célula não destrutível, laser por espécie, célula lenta e ataque à célula custando 1 vida). O conteúdo da Fase 2 continua apenas planejado.
>
> **Revisão de 03/11/2026:** alinhado à Spec de ensino ([`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md)) e ao escopo do M4 — nível dos termos, textos do scanner e simplificações do jogo.
>
> **Revisão de 03/11/2026 (noite):** a camada 1 passa a trazer a coloração **roxo/vermelho** junto do **nome do agente**; as armas passam a se chamar **Ruptura** e **Dissolução**; "rosa" fica como sinônimo do resultado vermelho. Com o nome na tela, o jogo passa a nomear espécies — o texto abaixo separa o que é identificação do que é classificação clínica.

Este documento limita o conteúdo ao que apoia a decisão do jogador. Separa fatos biológicos de regras abstratas do jogo e registra lacunas como **[PENDÊNCIA]**.

## Fase 1 — bactérias e células próprias

| Agente | Tipo no jogo | Resistência do jogo | Informação curta para o scanner |
|---|---|---:|---|
| Bactéria Gram-positiva do cenário | Ameaça no cenário da missão | 2 pontos de resistência | pulso 1: “roxo · Staphylococcus aureus” (nome do agente); pulso 2: “Gram+: parede celular mais espessa, rica em peptidoglicano.” |
| Bactéria Gram-negativa do cenário | Ameaça no cenário da missão | 2 pontos de resistência | pulso 1: “vermelho · Escherichia coli”; pulso 2: “Gram−: parede mais fina e uma camada externa extra.” |
| Célula saudável própria | Célula a preservar; não é alvo | Não se aplica; ataques não a destroem | pulso 1: “célula do organismo detectada · Célula epitelial”, sem coloração · pulso 2: “Célula saudável do próprio organismo: preserve-a.” |

Os textos do pulso 1 e do pulso 2 descrevem diferenças gerais do envelope celular: Gram− tem camada relativamente fina de peptidoglicano sob membrana externa; Gram+ tipicamente tem parede espessa e não possui essa membrana externa ([OpenStax Biology 2e](https://openstax.org/books/biology-2e/pages/22-2-structure-of-prokaryotes-bacteria-and-archaea)). Essas pistas apoiam a escolha da arma correspondente, conforme a regra do jogo registrada na #105.

### Nível do conteúdo no M4

| Conceito | Nível | O que o jogador vê |
|---|---|---|
| Resultado da coloração de Gram | Essencial | roxo ou vermelho (rosa), com o limite de que a cor não diz se a bactéria causa doença nem indica tratamento |
| Gram-positiva | Essencial | parede celular mais espessa, rica em peptidoglicano |
| Gram-negativa | Essencial | parede mais fina e uma camada externa extra (membrana externa explicada como camada com lipídios e açúcares) |
| Peptidoglicano | Essencial, introdutório | material de açúcares e peptídeos que faz parte da parede bacteriana |
| Armas Ruptura e Dissolução | Essencial, como regra do jogo | Ruptura mira a parede; Dissolução age na camada extra das Gram−; não equivale a escolher antibiótico real |
| Arma e célula própria | Essencial, como regra do jogo | no jogo, qualquer arma pode ferir a célula (−1 vida); isso não descreve efeitos de medicamentos reais |
| Nome do agente | Identificação, não classificação | o nome científico exibido identifica o agente do cenário; não diz o grupo Gram, não indica doença e não recomenda tratamento |
| Etapas da coloração | Para ir além | cristal violeta, iodo, álcool e safranina, nessa ordem; não é critério de vitória |
| Ligações cruzadas, transpeptidase, PBP | Para ir além | detalhes moleculares, apenas em material opcional e com fonte técnica |
| LPS, polimixina, efeitos clínicos | Para ir além | exemplos técnicos; não aparecem no feedback curto |
| Exceções da coloração e espécies | Fora do MVP | o jogo não representa exceções; nomeia os agentes do cenário na camada 1, sem inferir patogenicidade |

A classificação de nível é uma **estimativa pedagógica**: a BNCC do Ensino Médio organiza competências amplas e não lista peptidoglicano, LPS ou PBP como conteúdos obrigatórios. A presença da parede, do peptidoglicano e da distinção Gram+ / Gram− em materiais de preparação para o ENEM sustenta a inclusão como conteúdo escolar, não como prescrição curricular oficial.

### Simplificações do jogo

- A matriz Ruptura/Dissolução é uma regra de balanceamento e aprendizagem, não a eficácia de uma classe real de antibióticos.
- Ruptura e Dissolução são nomes de efeito, não de medicamentos, e a escolha da arma não recomenda tratamento.
- No jogo, as duas armas ferem a célula própria (D1); isso não diz que medicamentos reais sejam seguros nem perigosos.
- O nome do agente é identificação do cenário: não implica que a espécie cause doença, nem que toda bactéria do jogo seja patogênica.
- Os 2 pontos de “resistência” são pontos de combate, não resistência antimicrobiana.
- A coloração tem exceções e não identifica espécie, causa de doença nem tratamento.
- Nem toda bactéria causa doença; as ameaças são agentes do cenário.
- Dose, tempo de ação, diagnóstico e efeitos adversos não são simulados.

**Resistência** é um atributo abstrato de combate do jogo: cada acerto correto reduz um ponto; a bactéria requer dois acertos corretos para ser neutralizada, conforme #49. Não significa resistência a antibióticos nem permite inferir suscetibilidade médica a partir da coloração de Gram ([CDC — antimicrobial resistance](https://www.cdc.gov/antimicrobial-resistance/about/index.html); [CDC NARMS — glossary](https://www.cdc.gov/narms/glossary/index.html)). A célula própria não é destruída por ataques, conforme #49.

As bactérias do cenário são ameaças **da missão**, e o material não afirma que todas as bactérias causam doença. **Resolvido em 03/11 (noite):** com o nome na camada 1, a lista passou a ser conteúdo visível e foi revisada — *Bacillus subtilis* (não patogênico) saiu e entrou *Streptococcus pyogenes* (Gram+); *Escherichia coli* ficou como exemplo canônico de Gram−. Os seis binômios são **agentes do cenário**, e o briefing carrega a linha "os agentes desta missão são ameaças do cenário; nem toda bactéria causa doença". A tolerância imunológica ao próprio reduz respostas contra tecidos próprios, mas pode falhar em doenças autoimunes ([NCBI Bookshelf — Immunobiology](https://www.ncbi.nlm.nih.gov/books/NBK27174/)); portanto, “não atacar” é explicitamente uma regra da missão.

## Fase 2 — vírus

| Agente | Tipo no jogo | Resistência do jogo | Informação curta para o scanner |
|---|---|---:|---|
| Vírus da fase | Ameaça no contexto da missão | 1 ponto de resistência | “Vírus precisa de uma célula hospedeira para se multiplicar.” |

O texto descreve uma propriedade geral: vírus dependem de células hospedeiras para replicar-se ([NCBI Bookshelf — Introduction to Virology](https://www.ncbi.nlm.nih.gov/books/NBK8098/); [MedlinePlus — Viral Infections](https://medlineplus.gov/viralinfections.html)). “Ameaça” e a resistência de um ponto são classificações/valores de gameplay, não afirmações de que todo vírus infecta qualquer célula ou causa doença.

Os estados lisogênico e lítico, a transição temporizada e a multiplicação limitada estão descritos na #83; o modelo didático de ciclos virais está resumido pela [OpenStax Microbiology](https://openstax.org/books/microbiology/pages/6-2-the-viral-life-cycle). O scanner precisa informar o estado atual sem recomendar uma ação; os detalhes de cada pulso/camada e as consequências exatas da decisão permanecem **[PENDÊNCIA]** da dupla/PoC (#83, #87 e #88). O objetivo abaixo não antecipa esses detalhes.

## Objetivos de aprendizagem observáveis

### Fase 1

| OA | O jogador será capaz de… | Evidência no jogo |
|---|---|---|
| OA1 | interpretar o que o resultado roxo ou vermelho (rosa) indica | lê o resultado e identifica o grupo, sem inferir doença ou tratamento a partir do nome |
| OA2 | comparar as camadas externas e localizar o peptidoglicano | explica a diferença em palavras próprias e reconhece onde fica o peptidoglicano |
| OA3 | relacionar a estrutura observada ao alvo da arma | escolhe a arma correspondente e explica o alvo do jogo |
| OA4 | reconhecer e preservar a célula própria | evita atacar a célula e não conclui que a regra do jogo isenta medicamentos reais de efeitos adversos |

A evidência de aprendizagem é separada da pontuação geral: os contadores em memória da partida (#200) alimentam o resumo por objetivo (#174), e o pré/pós-teste P1–P4 pertence ao playtest do M5 (#134–#137).

### Fase 2

1. **Interpretar o ciclo viral mostrado no jogo.** O jogador distingue os estados lisogênico e lítico com base nas pistas disponibilizadas e relaciona o estado à transição/multiplicação observável. Evidência prevista: estado identificado e resposta do jogador registrada durante o encontro. **[PENDÊNCIA]** A pista exata do scanner, a decisão associada e as consequências a observar dependem da revisão das regras de #83/#87/#88.

Para cada fase, a evidência de aprendizagem deve ser separada de pontuação geral. **[PENDÊNCIA]** Definir limiar de sucesso e quais dados da partida serão registrados antes de considerar os objetivos avaliados.

## Revisão necessária antes do fechamento

- [x] Os dois integrantes revisaram e aprovaram os textos factuais e de scanner.
- [x] O nível do conteúdo do M4 foi separado entre essencial, “para ir além” e fora do MVP (03/11/2026).
- [x] A equipe revisou a lista de espécies exibida na camada 1 e o enquadramento como "agentes do cenário" (03/11, noite).
- [ ] A equipe confirmou os pulsos do scanner, decisões observáveis e evidências da Fase 2, sem conflitar com #83.
- [ ] A equipe definiu o limiar de sucesso da avaliação (o protocolo P1–P4 está proposto para o M5).

## Referências

- [OpenStax Biology 2e — Structure of Prokaryotes](https://openstax.org/books/biology-2e/pages/22-2-structure-of-prokaryotes-bacteria-and-archaea): estrutura de bactérias e diferenças gerais entre envelopes Gram+ e Gram−.
- [PrePara ENEM — Bactérias Gram-positivas e negativas](https://www.preparaenem.com/biologia/bacterias-gram-positivas-e-negativas.htm) e [Manual do Enem — Bactérias](https://querobolsa.com.br/enem/biologia/bacteria): referências de preparação para o ENEM sobre cores e diferenças gerais entre os grupos, incluindo parede e peptidoglicano.
- [Ministério da Saúde — Técnica de coloração de Gram](https://bvsms.saude.gov.br/bvs/publicacoes/0107tecnica_gram.pdf): referência factual do procedimento; não é usada para atribuir nível escolar.
- [OpenStax Microbiology, 14.3 — Mechanisms of Antibacterial Drugs](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs): PBP, transpeptidase, ligações cruzadas, LPS e polimixinas; usado só no aprofundamento opcional.
- [BNCC do Ensino Médio — MEC](https://portal.mec.gov.br/docman/abril-2018-pdf/85121-bncc-ensino-medio/file): competências amplas de Ciências da Natureza; não enumera peptidoglicano, LPS ou PBP como conteúdos obrigatórios.
- [NCBI Bookshelf — Introduction to Virology](https://www.ncbi.nlm.nih.gov/books/NBK8098/): vírus dependem da célula hospedeira para multiplicação.
- [CDC — About Antimicrobial Resistance](https://www.cdc.gov/antimicrobial-resistance/about/index.html) e [CDC NARMS — Glossary](https://www.cdc.gov/narms/glossary/index.html): resistência antimicrobiana é distinta do atributo de combate do jogo e é avaliada em relação a medicamentos/microrganismos.
- [MedlinePlus — Viral Infections](https://medlineplus.gov/viralinfections.html): vírus usam células para se multiplicar; efeitos variam.
- [OpenStax Microbiology — The Viral Life Cycle](https://openstax.org/books/microbiology/pages/6-2-the-viral-life-cycle): referências didáticas sobre ciclos lítico e lisogênico.
- [NCBI Bookshelf — Immunobiology: Self-tolerance and its loss](https://www.ncbi.nlm.nih.gov/books/NBK27174/): tolerância imunológica ao próprio e autoimunidade. A frase “não atacar” acima é uma regra do jogo, não a alegação biológica de que a tolerância nunca falha.
