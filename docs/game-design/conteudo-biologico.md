# Nanobô — Conteúdo biológico e objetivos de aprendizagem

> **Estado:** Aprovado pela dupla (Gustavo & Eric). Conteúdo factual e textos curtos de scanner revisados e validados, atendendo ao critério de aceite da Issue #2.

> **Estado em 18/10/2026:** os dados da Fase 1 desta tabela estão implementados no jogo (resistência 2 para bactéria, célula não destrutível, laser por espécie, célula lenta e ataque à célula custando 1 vida). O conteúdo da Fase 2 continua apenas planejado.
>
> **Revisão de 03/11/2026:** alinhado à Spec de ensino ([`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md)) e ao escopo do M4 — nível dos termos, textos do scanner e simplificações do jogo.
>
> **Revisão de 03/11/2026 (noite):** a camada 1 passa a trazer a coloração **roxo/vermelho** junto do **nome do agente**; as armas passam a se chamar **Ruptura** e **Dissolução**; "rosa" fica como sinônimo do resultado vermelho. Com o nome na tela, o jogo passa a nomear espécies — o texto abaixo separa o que é identificação do que é classificação clínica.

> **Revisão de 25/11/2026:** conteúdo conferido com fontes (relatório de pesquisa de 25/11, com as notas em `.hermes/research_notes/`) e decisões do dono do mesmo dia, em [`decisoes.md`](decisoes.md) §"Narrativa e conteúdo (25/11/2026)". Mudam as espécies da Fase 1 (*Moraxella catarrhalis* no lugar de *Neisseria meningitidis*; *Clostridium tetani* no lugar de *Bacillus cereus*), o rótulo das armas ("regra do jogo, inspirada em"), o texto da resistência (seleção), a nota de analogia dos ciclos virais e a escala dos sprites ("fora de escala"). As afirmações verificadas estão em §"Verificação com fontes (25/11/2026)", as fontes em §"Fontes da revisão de 25/11/2026" e o que falta em §"Não verificado". **Nenhum texto do jogo foi alterado**: as redações novas ficam marcadas como **pendente de redação** até o dono aprová-las.

Este documento limita o conteúdo ao que apoia a decisão do jogador. Separa fatos biológicos de regras abstratas do jogo e registra lacunas como **[PENDÊNCIA]**.

## Nanobô 2.0 — conteúdo vigente (13/11/2026)

> **Vigente.** Acrescenta e substitui, onde conflitar, o conteúdo das seções do M4 abaixo. Os objetivos e textos estão em [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md) (§"Nanobô 2.0"); a visão do jogo, no [`gdd.md`](gdd.md). Nível de ensino médio, sem termos moleculares no HUD.

### Fase 1 — bactérias na corrente sanguínea

- **Espécies** (misturam forma e Gram para a forma não decidir a arma):

  | Forma | Gram+ | Gram− |
  |---|---|---|
  | Coco | *Staphylococcus aureus*, *Streptococcus pyogenes* | *Moraxella catarrhalis* (diplococo) |
  | Bacilo | *Clostridium tetani* | *Escherichia coli*, *Pseudomonas aeruginosa* |

  - **Troca de 25/11:** *Moraxella catarrhalis* substitui *Neisseria meningitidis* e *Clostridium tetani* substitui *Bacillus cereus*, porque as duas antigas contrariavam a regra das armas (§"Espécies e a regra das armas"). *S. aureus* fica, com a nota de que muitas cepas resistem à penicilina.
  - **No jogo, todas as espécies aparecem juntas no sangue, por simplicidade.** Na vida real, *M. catarrhalis* vive nas vias respiratórias e *C. tetani* entra por ferimentos; não são típicas de corrente sanguínea (**Anotação a redigir, pendente de redação**).

- **Gram:** o corante roxo (cristal violeta) fica retido na parede grossa de peptidoglicano da Gram+. Na Gram− (parede fina e membrana externa), o álcool remove o roxo, e o segundo corante deixa a bactéria rosa. A cor vem do corante de laboratório, não da bactéria: o sprite é neutro e é tingido depois da 2ª camada.
- **Antibióticos, como regra de jogo:**
  - **Ruptura** (de parede, inspirada nos β-lactâmicos, como a penicilina) impede a montagem da parede e age na Gram+; a membrana externa da Gram− costuma dificultar a entrada de várias drogas ([OpenStax 14.3](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs); [CDC](https://www.cdc.gov/antimicrobial-resistance/about/index.html));
  - **Dissolução** (de membrana, inspirada nas polimixinas) **desorganiza** a membrana externa, que só a Gram− tem (OpenStax 14.3; "desfaz" é metáfora);
  - **Reserva** representa os antibióticos de reserva, usados quando os comuns falham, e é limitada para ensinar o uso racional. Até a Reserva pode deixar de funcionar se for usada à toa ([Bull. WHO 2025](https://pmc.ncbi.nlm.nih.gov/articles/PMC12706710/));
  - **O rótulo é "regra do jogo, simplificada, inspirada em penicilina e polimixina; na vida real há exceções e resistência"**. A divisão "Ruptura na Gram+, Dissolução na Gram−" não é farmacologia: não existe antibiótico real "de parede" ou "de membrana" com eficácia dividida assim por Gram.
- **Resistência:** a bactéria que sobrevive a um tratamento incompleto (1 dose de 2) se multiplica, e o mesmo antibiótico deixa de agir. **A mecânica das 2 doses e dos 6 s é regra do jogo**, não biologia: na realidade, a resistência surge por mutação ou por troca de genes e é **selecionada** pelo uso desnecessário ou mal feito: o antibiótico mata as sensíveis, as que já eram resistentes sobrevivem, se multiplicam e passam a resistência às filhas ([Fiocruz/IOC](https://www.ioc.fiocruz.br/noticias/cartilha-alerta-para-os-riscos-da-resistencia-aos-antibioticos); [Agência Brasil, 23/11/2026](https://agenciabrasil.ebc.com.br/saude/noticia/2026-11/uso-racional-de-antibioticos-pode-diminuir-bacterias-resistentes)). A divisão binária (uma bactéria vira duas) passa o DNA completo, e a resistência com ele ([OpenStax 9.1](https://openstax.org/books/microbiology/pages/9-1-how-microbes-grow)). A frase de saúde pública do jogo é **"siga a prescrição até o fim"**, alinhada à Agência Brasil e à Anvisa; o jogo não afirma que parar cedo cause resistência, ponto contestado na literatura ([BMJ 2017](https://www.bmj.com/content/358/bmj.j3418)). A resistência é da bactéria, não do corpo ([CDC](https://www.cdc.gov/antimicrobial-resistance/about/index.html)).
- **Microbiota:** a maioria das bactérias do corpo é inofensiva ou útil; o jogo trata só as que causam a infecção.
- **Comportamento pela forma (14/11):**
  - os bacilos do jogo (*E. coli*, *P. aeruginosa*, *C. tetani*) têm **flagelo** e nadam, por isso perseguem o Nanobô;
  - os cocos (*Staphylococcus*, *Streptococcus*, *Moraxella*) não têm flagelo: são levados pelo sangue, aderem ao tecido, colonizam e produzem **toxinas** que lesam as células.
  - Simplificação: há exceções na natureza, mas a regra vale para as seis espécies do jogo. Flagelo por espécie: *P. aeruginosa* (um flagelo polar) tem fonte; *C. tetani* tem fonte única (flagelos peritríquios, algumas cepas imóveis); a ausência de flagelo nos cocos e o flagelo da *E. coli* são **não verificados** em fonte lida (§"Não verificado"). Os cocos aderem por adesinas ([OpenStax 15.3](https://openstax.org/books/microbiology/pages/15-3-virulence-factors-of-bacterial-and-viral-pathogens)); "levado pelo sangue" é simplificação.
- **Biofilme:** bactérias aderidas produzem uma matriz (polissacarídeos) em que o antibiótico age menos, por difusão lenta e metabolismo baixo ([OpenStax 9.1](https://openstax.org/books/microbiology/pages/9-1-how-microbes-grow)). A proteção contra o sistema imune **não foi verificada** em fonte lida; não escrever isso em texto fixo sem fonte. No jogo, a bactéria dentro do biofilme pede uma dose a mais e a mancha deixa o Nanobô lento.
- **Endotoxina:** o **LPS** (lipopolissacarídeo) da membrana externa das Gram- é liberado quando a célula morre ou se divide e causa inflamação e febre ([OpenStax 15.3](https://openstax.org/books/microbiology/pages/15-3-virulence-factors-of-bacterial-and-viral-pathogens)); que o **antibiótico** acelere essa liberação não foi verificado. No jogo, a Gram- eliminada deixa uma nuvem que fere o Nanobô. **LPS fica de fora do texto do jogador** (decisão de 25/11): lá vira **"toxina da membrana externa"**; este documento mantém o termo técnico com a fonte.
- **Escala:** os sprites são **ilustrativos, fora de escala** (decisão de 25/11, substitui "escala biológica"); só a **ordem vírus < bactéria < célula** é mantida (vírus 16, bactéria 32, levedura 40, célula 64 px). Os tamanhos reais citados antes (vírus ~0,1 µm, bactéria 1–2 µm, célula humana 10–20 µm) são conhecimento geral **sem fonte lida** e não aparecem como medida do jogo; a razão real é muito maior que a dos sprites. Os tempos do jogo (12 s de divisão, 6 s de biofilme, 10 s de lisogênico) também não são tempos reais: a *E. coli* dobra em até ~20 min no laboratório ([OpenStax 9.1](https://openstax.org/books/microbiology/pages/9-1-how-microbes-grow)).

### Fase 2 — vírus nas vias aéreas

- **Ambiente e agente:** um vírus respiratório (como o da gripe) infecta o epitélio dos brônquios.
- **Muco e cílios:** o epitélio dos brônquios tem células com cílios que empurram o muco para cima, a "escada mucociliar". O vírus não nada: é levado pelo muco até as células. No jogo, a correnteza do muco empurra os vírus livres e o Nanobô.
- **O vírus não é célula:** é **acelular**, sem metabolismo nem ribossomos **próprios** (os ribossomos que traduzem o RNA viral são os da célula hospedeira), e só se multiplica dentro de uma célula ([Fiocruz/EPSJV, cap. 2](https://www.epsjv.fiocruz.br/sites/default/files/cap2.pdf)). Antibióticos agem em estruturas bacterianas e não tratam gripe nem resfriado ([SBI, 30/06/2026](https://infectologia.org.br/noticias/antibiotico-nao-trata-gripe-riscos-da-automedicacao-e-resistencia-bacteriana/)). Por isso, o antibiótico não age nele.
- **Ciclos (analogia didática dos bacteriófagos):** os ciclos lisogênico e lítico são descritos nos livros para **vírus de bactérias** ([OpenStax Microbiology 6.2](https://openstax.org/books/microbiology/pages/6-2-the-viral-life-cycle)); no jogo servem de analogia. **A nota entra no jogo** na Anotação da 1ª célula infectada (**pendente de redação**; decisão de 25/11).
  - **lisogênico:** o material genético do vírus fica na célula sem destruí-la por um tempo. Em vírus humanos o análogo real é a **latência** (herpes simples, varicela-zóster, que fica nos nervos e pode voltar como herpes-zóster, e Epstein-Barr; OpenStax 6.2). **Não foi achada fonte que atribua estado lisogênico ou latente ao vírus da gripe**; o jogo não afirma isso;
  - **lítico:** o vírus faz cópias, a célula se rompe e libera vírus novos, que infectam outras células. Nas células ciliadas do epitélio respiratório a infecção mata por morte celular, e não por "encher e estourar" ([PMC7516383](https://pmc.ncbi.nlm.nih.gov/articles/PMC7516383/), trecho); a ruptura do jogo é simplificação.
- **Imunidade adaptativa:**
  - os **anticorpos** (produzidos por linfócitos B e plasmócitos) se prendem ao vírus livre e o impedem de entrar nas células ([SBI/Fiocruz Rene Rachou](https://exposicaocovid19.sbi.org.br/imunidade/)); é a **Neutralização** do jogo. "O vírus some" é imagem de jogo: na vida real o vírus neutralizado fica inativo e é eliminado por fagócitos, e a resposta leva dias, não segundos (prazo exato não verificado);
  - o **linfócito T citotóxico** reconhece e destrói células infectadas; é a ferramenta hoje chamada **Lise**, aplicada pelo Nanobô. **O nome será trocado** (proposta: **"Ataque T"**, que evoca o linfócito T): "lise" é, no ensino médio, o rompimento da célula pelo vírus no ciclo lítico, e o nome confundia as duas coisas. A troca no código é posterior; até lá, este e os demais documentos ainda dizem "Lise".
  - O jogo não tem aliados: o Nanobô faz os dois papéis, e os cards dizem quem faz isso no corpo.

### Fase 3 — fungos no pulmão

- **Fungos são eucariontes:** têm núcleo, como as nossas células. A parede é de **quitina**, e não de peptidoglicano; que o antibiótico antibacteriano "não age por causa da quitina" é inferência razoável (alvo ausente), **sem fonte**: o texto seguro é "antibiótico de bactéria não tem alvo no fungo". A membrana tem **ergosterol** (a nossa tem colesterol), que é o alvo de muitos antifúngicos ([OpenStax Microbiology, Fungi](https://openstax.org/books/microbiology/pages/5-3-fungi)). O contraste com o colesterol explica por que o alvo é seletivo.
- **Perfuração** (inspirada na **anfotericina B**, um poliênico que se liga ao ergosterol e forma poros; [Pharmacology 2e](https://pressbooks.pub/uwecpharmacology2e/chapter/3-15-antifungals/), fonte secundária): o antifúngico do jogo. Descreve só os poliênicos: os azóis reduzem a produção de ergosterol e as equinocandinas atuam na parede. A anfotericina B é nefrotóxica, o que combina com a Perfuração custar vitais; o jogo não diz que o antifúngico é inofensivo.
- **Levedura:** *Cryptococcus neoformans*, unicelular, inalada; multiplica-se por **brotamento** (uma célula-filha cresce grudada e se solta).
- **Bolor:** *Aspergillus fumigatus*, formado por filamentos (**hifas**) que crescem e soltam **esporos** (no texto do jogo, "esporo", não "conídio"), que viram colônias novas. O conjunto de hifas é o micélio (OpenStax, Fungi).
- **Oportunismo:** os esporos estão no ar, a maioria das pessoas os inala sem adoecer, e a forma invasiva atinge sobretudo quem tem a defesa enfraquecida ([CDC, aspergilose](https://www.cdc.gov/aspergillosis/about/); [CDC, prevenção](https://www.cdc.gov/aspergillosis/prevention)). **Enquadramento correto (CC-05c): o risco vem da defesa fraca, não de mais esporos inalados**; a redação "quanto pior o paciente chega, mais esporos se instalam" inverte a causa e vira "quanto mais fraco o paciente, mais fácil para os esporos se instalarem" (**pendente de redação**). Em texto de criptococo, não escrever que ele "só" aproveita organismo enfraquecido: *C. gattii* também afeta pessoas saudáveis ([MSD Manual](https://www.msdmanuals.com/pt/casa/infec%C3%A7%C3%B5es/infec%C3%A7%C3%B5es-f%C3%BAngicas/criptococose); [Ministério da Saúde](https://www.gov.br/saude/pt-br/assuntos/saude-de-a-a-z/c/criptococose)). No jogo, a carga inicial de esporos depende dos sinais vitais ao fim da Fase 2 e das cargas de Reserva gastas: é **regra de jogo**. O jogo **não afirma** que o antibiótico causa aspergilose; o CDC liga "doses altas de antibióticos" a risco fúngico em geral, não à aspergilose especificamente ([CDC, fatores de risco](https://www.cdc.gov/fungal/risk-factors/index.html)).

## Fase 1 — bactérias e células próprias

| Agente | Tipo no jogo | Resistência do jogo | Informação curta para o scanner |
|---|---|---:|---|
| Bactéria Gram-positiva do cenário | Ameaça no cenário da missão | 2 pontos de resistência | pulso 1: “roxo · Staphylococcus aureus” (nome do agente); pulso 2: “Gram+ · parede espessa, rica em peptidoglicano”, com a revisão “Essa parede mais espessa distingue este grupo.” |
| Bactéria Gram-negativa do cenário | Ameaça no cenário da missão | 2 pontos de resistência | pulso 1: “vermelho · Escherichia coli”; pulso 2: “Gram- · parede fina sob membrana externa”, com a revisão “A membrana externa adicional distingue este grupo.” |
| Célula saudável própria | Célula a preservar; não é alvo | Não se aplica; ataques não a destroem | pulso 1: “célula do organismo · Célula epitelial”, sem coloração; pulso 2: “célula saudável do próprio organismo: preserve-a”, com a revisão “preserve a célula: as duas armas ferem o próprio organismo” |

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

## Verificação com fontes (25/11/2026)

Veredito geral da pesquisa: nenhuma afirmação dos documentos foi achada errada; os problemas são de **enquadramento**. Marcação: **(trecho)** = lido só no resumo de uma busca, página não aberta por completo; **NV** = sem fonte lida. Termos acima do ensino médio (LPS, β-lactamase, transpeptidase) ficam **só aqui**, com fonte; não vão para o texto do jogador.

### OA1 e OA2: procarionte × eucarionte, forma e coloração de Gram

- Gram+ roxa e Gram− rosa/vermelha: cristal violeta, iodo, descolorante e safranina; a parede espessa da Gram+ retém o complexo roxo e a fina da Gram− o perde ([OpenStax Microbiology 2.4](https://openstax.org/books/microbiology/pages/2-4-staining-microscopic-specimens)). O teste tem limites (cultura velha pode parecer Gram−).
- Gram+ com parede espessa de peptidoglicano; Gram− com parede fina sob uma membrana externa, onde fica o LPS ([OpenStax Microbiology 14.3](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs)). Fraseado escolar: [Prepara ENEM](https://www.preparaenem.com/biologia/bacterias-gram-positivas-e-negativas.htm) (não é autoridade).
- **Ajuste de CC-01a** ("um corante roxo tinge a parede"): "No laboratório, o corante roxo fica preso ou sai conforme a parede da bactéria." (**pendente de redação**.)
- "A forma não determina o Gram": correto; há coco Gram− (*Moraxella*, o meningococo) e coco Gram+ (*Staphylococcus*) ([CDC, meningococo](https://www.cdc.gov/meningococcal/about/index.html)). Nenhuma fonte tem a frase pronta.
- Núcleo × nucleoide e "bactéria sem núcleo" (OA1): **NV**, mas é biologia padrão do ensino médio.
- BNCC: os OA não aparecem nomeados (a base é por competências); o repositório os liga à competência 3 e à habilidade EM13CNT301 por reprodução secundária ([profez](https://profez.com.br/bncc/ciencias-da-natureza-e-suas-tecnologias/3-ano-em/em13cnt301)); o texto do MEC não foi aberto.

### OA3: as armas

- β-lactâmicos (penicilinas, cefalosporinas, carbapenêmicos) bloqueiam a ligação cruzada do peptidoglicano; como inibem a parede, são bactericidas, e a célula humana não faz peptidoglicano, o que torna o alvo seletivo. Penicilinas naturais agem sobretudo em Gram+; a vancomicina só em Gram+. Polimixinas desorganizam a membrana externa das Gram−, a ação não é seletiva, pode lesar rim e sistema nervoso, e a colistina é "droga de último recurso" ([OpenStax 14.3](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs)).
- **Reserva:** a classificação AWaRe da OMS define "Reserva" como last-resort, para multirresistentes; a lista de 2025 traz 10 de Reserva, entre eles colistina e polimixina B, e a resistência à colistina em *Klebsiella* cresce ([Bull. WHO 2025](https://pmc.ncbi.nlm.nih.gov/articles/PMC12706710/)); a definição está em revisão pela OMS (prevista para 2027). Analogia brasileira, "não usar uma bala de canhão para matar uma formiga" ([Agência Brasil, 23/11/2026](https://agenciabrasil.ebc.com.br/saude/noticia/2026-11/uso-racional-de-antibioticos-pode-diminuir-bacterias-resistentes)).
- **Regra do jogo, com rótulo:** a divisão "Ruptura na Gram+, Dissolução na Gram−" é simplificação aceitável como regra de jogo e **enganosa se lida como farmacologia**; por isso o rótulo "regra do jogo, simplificada, inspirada em" e o "em geral".

### Espécies e a regra das armas

Esta foi a descoberta mais forte: a regra falhava em *N. meningitidis* (inverte os dois lados) e em *B. cereus*, e era frágil em *S. aureus*. Tabela das **seis espécies atuais**, com o que as fontes dizem na vida real:

| Espécie (forma · Gram) | Penicilina (Ruptura) | Polimixina (Dissolução) | Segue a regra? | Fontes |
|---|---|---|---|---|
| *S. aureus* (coco · +) | **a maioria resiste** (β-lactamase *blaZ*): só 13,5% de 3.083 isolados dos EUA em 2013 eram sensíveis (trecho); 85–90% hospitalar e 65–70% comunitária em 2001 (trecho); MRSA tem PBP alterada | não age (Gram+) | **Parcial.** Gram+ correto, mas a penicilina simples falha. **Nota no jogo: "muitas cepas resistem à penicilina"** | [PMC6886895](https://pmc.ncbi.nlm.nih.gov/articles/PMC6886895) (URL a conferir), [CDC EID 2001](https://www.cdc.gov/eid/article/7/2/pdfs/70-0178.pdf) (PDF deu 404) |
| *S. pyogenes* (coco · +) | **sensível**: "nunca houve relato" de resistência clínica no grupo A; penicilina ou amoxicilina é a escolha | não age | **Sim** | [CDC, faringite](https://www.cdc.gov/groupastrep/diseases-hcp/strep-throat.html) |
| *M. catarrhalis* (diplococo · −) | **resiste**: mais de 95% dos isolados clínicos, por β-lactamase (BRO-1/BRO-2); cefalosporinas de 2ª e 3ª geração funcionam | a maioria das cepas é sensível à colistina, algumas resistentes (página arquivada do CDC, trecho; sem ponto de corte oficial) | **Sim**, com a fonte da polimixina fraca | [PMC7029268](https://pmc.ncbi.nlm.nih.gov/articles/PMC7029268), [CDC arquivo 2009](https://webarchive.library.unt.edu/eot2008/20090111022708/http:/www.cdc.gov/std/Gonorrhea/lab/Mcat.htm) |
| *C. tetani* (bacilo · +) | **sensível**: penicilina G cristalina ou metronidazol, sem evidência clara de superioridade | não age em Gram+ | **Sim** | [Guia de Vigilância em Saúde, MS, 6ª ed. 2023](https://www.sjp.pr.gov.br/wp-content/uploads/2024/07/TETANO-ACIDENTAL-GUIA-DE-VIGILANCIA-EM-SAUDE.pdf); polimixina: [PMC8406936](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8406936/), [PMC6519339](https://pmc.ncbi.nlm.nih.gov/articles/PMC6519339) |
| *E. coli* (bacilo · −) | variável (não pesquisado) | polimixinas agem em *E. coli*, *Klebsiella* e *Pseudomonas* (trecho) | **Sim** para a Dissolução | [PMC6440778](https://pmc.ncbi.nlm.nih.gov/articles/PMC6440778), [MSD Vet](https://www.msdvetmanual.com/pharmacology/antibacterial-agents/polymyxins) |
| *P. aeruginosa* (bacilo · −) | resistente à penicilina G simples (**NV**) | colistina/polimixina como último recurso em cepas multirresistentes (trecho) | **Sim** | [CDC, Pseudomonas](https://www.cdc.gov/pseudomonas-aeruginosa/about/index.html) |

Fatos de apoio:

- *S. aureus* é portador nasal comum, até 30% da população ([PubMed 30349525](https://pubmed.ncbi.nlm.nih.gov/30349525/), trecho); cerca de 1 em 10 pessoas carrega o meningococo na garganta sem adoecer ([CDC](https://www.cdc.gov/meningococcal/about/index.html)). A maioria das *E. coli* é inofensiva e útil no intestino, e só algumas cepas causam doença ([CDC, E. coli](https://www.cdc.gov/ecoli/about/index.html)); uma linha de Anotação pode dizer "no intestino quase sempre é útil; esta cepa é a que causa doença".
- *M. catarrhalis*: diplococo Gram−, colonizador comum das vias respiratórias superiores, causa otite média em crianças e infecção respiratória baixa em adultos com DPOC ou imunossuprimidos ([PMC7029268](https://pmc.ncbi.nlm.nih.gov/articles/PMC7029268)); não móvel (provável; [PMC163240](https://pmc.ncbi.nlm.nih.gov/articles/PMC163240), fonte primária não aberta); bacteremia é incomum (**NV**).
- *C. tetani*: bacilo Gram+ anaeróbio, esporulado, esporos persistem no ambiente por anos; reservatório no solo e no intestino de animais; os esporos entram por ferimentos e a toxina (tetanospasmina) causa espasmos; incubação típica de 5 a 15 dias; não passa de pessoa a pessoa (Guia MS, acima). Flagelos peritríquios em material universitário ([e-Disciplinas USP](https://edisciplinas.usp.br/mod/resource/view.php?id=2686866), trecho; algumas cepas são imóveis). A prevenção principal é a vacina ([CDC](https://www.cdc.gov/tetanus/hcp/clinical-guidance/index.html) diz que antibiótico não é recomendado para prevenir tétano em ferimento).
- **Limite da substituição:** *C. tetani* é anaeróbio de ferimentos e *M. catarrhalis* vem do trato respiratório; nenhuma das duas é típica de corrente sanguínea. No jogo é licença de simplificação, dita numa Anotação.
- **Espécies antes e por que saíram:** *N. meningitidis* é tratada com penicilina G ou ceftriaxona, mesmo sendo Gram−, e não responde à polimixina (o ágar Thayer-Martin usa colistina para eliminar outros Gram− que *Neisseria* tolera) ([MSD pt-BR](https://www.msdmanuals.com/pt-br/profissional/doen%C3%A7as-infecciosas/cocos-e-cocobacilos-gram-negativos/doen%C3%A7as-meningoc%C3%B3cicas), [CDC clínico](https://www.cdc.gov/meningococcal/hcp/clinical-guidance)); *B. cereus* é Gram+ com β-lactamase potente e resistência marcada a β-lactâmicos ([Clin. Microbiol. Rev., PMC2863360](https://pmc.ncbi.nlm.nih.gov/articles/PMC2863360/), trecho).
- **Alternativas pesquisadas e não usadas:** *Listeria monocytogenes* (Gram+ bacilo, ampicilina/penicilina como padrão; melhor ligação com infecção invasiva, menos conhecida; [CDC](https://www.cdc.gov/listeria/about/index.html)), *Corynebacterium diphtheriae* (bacilo Gram+, sensível à penicilina) e *Acinetobacter baumannii* (cocobacilo Gram−, usada com polimixina).

### OA4: resistência e uso racional

- O mecanismo é seleção: "o antibiótico mata as mais fraquinhas, mas as resistentes sobrevivem e tomam o espaço" ([Agência Brasil](https://agenciabrasil.ebc.com.br/saude/noticia/2026-11/uso-racional-de-antibioticos-pode-diminuir-bacterias-resistentes)). A Anvisa, citando a OMS, aponta o uso indevido e excessivo como principal fator ([Anvisa 2020](https://www.gov.br/anvisa/pt-br/assuntos/noticias-anvisa/2020/resistencia-microbiana-saiba-o-que-e-e-como-evitar)). A Fiocruz diz que vem de alterações aleatórias no material genético ou da aquisição de genes ([Fiocruz/IOC](https://www.ioc.fiocruz.br/noticias/cartilha-alerta-para-os-riscos-da-resistencia-aos-antibioticos)). O traço é herdável e o germe é que resiste, não o corpo ([CDC](https://www.cdc.gov/antimicrobial-resistance/about/index.html)).
- Posição brasileira: "siga a prescrição". A literatura internacional contesta que "parar cedo" cause resistência ([BMJ 2017](https://www.bmj.com/content/358/bmj.j3418); réplica pedindo cautela no [One Health Trust](https://onehealthtrust.org/news-media/blog/should-you-finish-the-course-of-antibiotics/), só resultado de busca). O jogo adota "siga a prescrição até o fim" e **não entra na controvérsia**. A frase da Anvisa "interromper tratamentos... nunca é uma boa ideia" **não apareceu no corpo da página**: não citar.
- **Ajustes (pendentes de redação):**
  - OA4: "Tratamento mal feito deixa sobreviventes; as que sobrevivem se multiplicam e passam a resistência às filhas", e a mecânica de 2 doses e 6 s apresentada **como regra do jogo**;
  - CC-02b "Esta ficou resistente...": "Esta resistiu: o mesmo antibiótico não age mais nela nem nas filhas." ("ficou" sugere mudança por exposição);
  - CC-02c: acrescentar "até a Reserva pode deixar de funcionar";
  - "mutação ou troca de genes, selecionada pelo uso desnecessário ou mal feito".
- Defesas bacterianas: flagelo gira (CC-07a "chicote que gira" é metáfora aceitável), cápsula ajuda a evitar fagocitose, biofilme é matriz de polissacarídeos, endotoxina é o LPS da Gram− ([OpenStax 3.3, 9.1 e 15.3](https://openstax.org/books/microbiology/pages/15-3-virulence-factors-of-bacterial-and-viral-pathogens)). CC-08a/b e CC-09a corretos; CC-09b ("até pelo antibiótico") correto na essência, "pelo antibiótico" **NV**.

### OA5 a OA7: anticorpos, vírus acelular, lisogênico e lítico

- Vírus: parasitas intracelulares obrigatórios, só ácido nucleico e capsídeo (às vezes envelope) ([Fiocruz/EPSJV, cap. 2](https://www.epsjv.fiocruz.br/sites/default/files/cap2.pdf)); [Brasil Escola](https://brasilescola.uol.com.br/biologia/virus-2.htm) (trecho).
- **RT-10:** "acelular: sem parede, ribossomos nem metabolismo" é absoluto demais; vira "acelular: sem metabolismo nem ribossomos próprios" (ASCII, sem acento maiúsculo; **pendente de redação**). CC-03a e RR-10 estão corretos.
- Anticorpos neutralizantes bloqueiam a entrada do vírus ([SBI/Fiocruz Rene Rachou](https://exposicaocovid19.sbi.org.br/imunidade/)); são produzidos por linfócitos B e plasmócitos ([PMC8706198](https://pmc.ncbi.nlm.nih.gov/articles/PMC8706198/), trecho). Dentro da célula atua o linfócito T citotóxico; sem antígeno viral, o anticorpo não tem alvo na célula saudável. Complemento opcional em 70 caracteres: "Anticorpos são feitos por plasmócitos (linfócitos B)."
- **Ciclos:** no lítico o fago faz cópias e a célula se rompe; no lisogênico o genoma do fago se integra como prófago, passa às filhas e fica latente até ser induzido (OpenStax 6.2). **CC-04a** ("fica dentro da célula, sem destruí-la ainda") é enganosa sem o rótulo de analogia; sugestão: "Escondido na célula, como o vírus da catapora fica nos nervos" (**pendente de redação**).
- **RT-13b** ("eclipse"): "eclipse" é conceito de virologia, **NV** em fonte lida; explicar em uma linha. O texto legado de `fase2-virus.md` ("o vírus de dentro eclode em até 3 novos"; "todo vírus nasce lisogênico") é enganoso (a célula libera os vírus) e já foi substituído em 14/11. "Célula rompe e solta 6 vírus": o 6 é de gameplay (uma célula produz muito mais). "irreversível: lisogênico → lítico não volta" é regra de jogo (nos fagos a indução é possível). Custo de −5 vitais da Lise: **NV** (dano colateral coerente com o jogo).
- Cenário: muco levado pelos cílios rumo à faringe está correto (resultados de busca, fontes em inglês e de baixa autoridade) e as células ciliadas são alvo de vírus respiratórios ([PMC11920889](https://pmc.ncbi.nlm.nih.gov/articles/PMC11920889/), trecho). "O vírus livre infecta qualquer célula livre" é simplificação (na realidade exige receptor específico).

### OA8: fungos

- Parede de quitina, membrana com ergosterol (a animal usa colesterol); levedura é unicelular, bolor é feito de hifas e o conjunto de hifas é o micélio ([OpenStax Microbiology, Fungi](https://openstax.org/books/microbiology/pages/5-3-fungi)); concordam materiais universitários brasileiros ([USP e-Disciplinas](https://edisciplinas.usp.br/pluginfile.php/6242027/mod_resource/content/1/Caract.%20ger.%20fungos%20Fisioterapia2021.pdf), [UFS](https://cesad.ufs.br/ORBI/public/uploadCatalago/10303304042012Microbiologia_Geral_Aula_9.pdf), ambos só por resumo de busca).
- Antifúngicos: poliênicos (anfotericina B, nistatina) ligam-se ao ergosterol e formam poros; azóis reduzem a produção de ergosterol; equinocandinas atuam na parede ([Pharmacology 2e](https://pressbooks.pub/uwecpharmacology2e/chapter/3-15-antifungals/), trecho, fonte secundária).
- *Cryptococcus* é inalado, vive em solo com fezes de pombo (*C. neoformans*) ou em árvores (*C. gattii*), atinge sobretudo imunodeprimidos, mas *C. gattii* também afeta pessoas saudáveis, e a doença não passa de pessoa a pessoa (MSD Manual e Ministério da Saúde, links acima). Aspergilose vem da inalação de esporos de um mofo comum (CDC, links acima).
- **Lacuna de conteúdo (gancho opcional OA3 e OA8):** a maioria dos fungos é inofensiva ou útil, e a penicilina vem de um fungo, *Penicillium*, descoberto por Fleming em 1928; a cepa original está na coleção de fungos da Fiocruz ([Folha de Pernambuco](https://www.folhape.com.br/noticias/fiocruz-colecao-de-fungos-da-instituicao-completa-100-anos-de/253773/), jornal, fonte secundária).
- Cápsula da levedura como proteção contra as 3 doses: metáfora de jogo; cápsula do *Cryptococcus* como proteção **NV** em fonte aberta. "Hifa invade o tecido": simplificação (o CDC fala de doença "invasiva" sem detalhar hifas).

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

## Fontes da revisão de 25/11/2026

Fontes brasileiras primeiro. Todas já estão citadas acima, no ponto em que sustentam cada afirmação; esta é a lista única.

**Brasileiras**
- [Ministério da Saúde, Guia de Vigilância em Saúde, 6ª ed. 2023 (tétano acidental)](https://www.sjp.pr.gov.br/wp-content/uploads/2024/07/TETANO-ACIDENTAL-GUIA-DE-VIGILANCIA-EM-SAUDE.pdf): *C. tetani*, penicilina G, vacina.
- [MSD Manual pt-BR, doenças meningocócicas](https://www.msdmanuals.com/pt-br/profissional/doen%C3%A7as-infecciosas/cocos-e-cocobacilos-gram-negativos/doen%C3%A7as-meningoc%C3%B3cicas) e [criptococose](https://www.msdmanuals.com/pt/casa/infec%C3%A7%C3%B5es/infec%C3%A7%C3%B5es-f%C3%BAngicas/criptococose); [Ministério da Saúde, criptococose](https://www.gov.br/saude/pt-br/assuntos/saude-de-a-a-z/c/criptococose).
- [Agência Brasil, 23/11/2026](https://agenciabrasil.ebc.com.br/saude/noticia/2026-11/uso-racional-de-antibioticos-pode-diminuir-bacterias-resistentes); [Anvisa 2020](https://www.gov.br/anvisa/pt-br/assuntos/noticias-anvisa/2020/resistencia-microbiana-saiba-o-que-e-e-como-evitar); [Fiocruz/IOC](https://www.ioc.fiocruz.br/noticias/cartilha-alerta-para-os-riscos-da-resistencia-aos-antibioticos); [SBI, 30/06/2026](https://infectologia.org.br/noticias/antibiotico-nao-trata-gripe-riscos-da-automedicacao-e-resistencia-bacteriana/).
- [SBI/Fiocruz Rene Rachou, imunidade](https://exposicaocovid19.sbi.org.br/imunidade/); [Fiocruz/EPSJV, cap. 2](https://www.epsjv.fiocruz.br/sites/default/files/cap2.pdf); [Brasil Escola, vírus](https://brasilescola.uol.com.br/biologia/virus-2.htm) (trecho).
- [USP e-Disciplinas, fungos](https://edisciplinas.usp.br/pluginfile.php/6242027/mod_resource/content/1/Caract.%20ger.%20fungos%20Fisioterapia2021.pdf) e [UFS, Microbiologia Geral](https://cesad.ufs.br/ORBI/public/uploadCatalago/10303304042012Microbiologia_Geral_Aula_9.pdf) (trecho); [Folha de Pernambuco, Fleming e a coleção da Fiocruz](https://www.folhape.com.br/noticias/fiocruz-colecao-de-fungos-da-instituicao-completa-100-anos-de/253773/) (jornal).
- [Prepara ENEM, Gram](https://www.preparaenem.com/biologia/bacterias-gram-positivas-e-negativas.htm) (fraseado escolar).

**Internacionais**
- OpenStax Microbiology: [2.4 coloração](https://openstax.org/books/microbiology/pages/2-4-staining-microscopic-specimens), [6.2 ciclo viral](https://openstax.org/books/microbiology/pages/6-2-the-viral-life-cycle), [9.1 crescimento](https://openstax.org/books/microbiology/pages/9-1-how-microbes-grow), [14.3 antibacterianos](https://openstax.org/books/microbiology/pages/14-3-mechanisms-of-antibacterial-drugs), [15.3 virulência](https://openstax.org/books/microbiology/pages/15-3-virulence-factors-of-bacterial-and-viral-pathogens), [5.3 fungos](https://openstax.org/books/microbiology/pages/5-3-fungi).
- CDC: [resistência](https://www.cdc.gov/antimicrobial-resistance/about/index.html), [meningococo](https://www.cdc.gov/meningococcal/about/index.html) e [guia clínico](https://www.cdc.gov/meningococcal/hcp/clinical-guidance), [faringite por *S. pyogenes*](https://www.cdc.gov/groupastrep/diseases-hcp/strep-throat.html), [*Pseudomonas*](https://www.cdc.gov/pseudomonas-aeruginosa/about/index.html), [*E. coli*](https://www.cdc.gov/ecoli/about/index.html), [tétano](https://www.cdc.gov/tetanus/hcp/clinical-guidance/index.html), [*Listeria*](https://www.cdc.gov/listeria/about/index.html), [aspergilose](https://www.cdc.gov/aspergillosis/about/) e [prevenção](https://www.cdc.gov/aspergillosis/prevention), [fatores de risco fúngicos](https://www.cdc.gov/fungal/risk-factors/index.html), [EID 2001](https://www.cdc.gov/eid/article/7/2/pdfs/70-0178.pdf) (PDF deu 404) e [arquivo 2009, *Moraxella*](https://webarchive.library.unt.edu/eot2008/20090111022708/http:/www.cdc.gov/std/Gonorrhea/lab/Mcat.htm) (trecho).
- [Bull. WHO 2025, AWaRe](https://pmc.ncbi.nlm.nih.gov/articles/PMC12706710/); [BMJ 2017](https://www.bmj.com/content/358/bmj.j3418); [One Health Trust](https://onehealthtrust.org/news-media/blog/should-you-finish-the-course-of-antibiotics/) (trecho).
- PMC: [7029268](https://pmc.ncbi.nlm.nih.gov/articles/PMC7029268) (*Moraxella*), [163240](https://pmc.ncbi.nlm.nih.gov/articles/PMC163240), [6886895](https://pmc.ncbi.nlm.nih.gov/articles/PMC6886895) (URL a conferir), [6440778](https://pmc.ncbi.nlm.nih.gov/articles/PMC6440778), [8406936](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8406936/), [6519339](https://pmc.ncbi.nlm.nih.gov/articles/PMC6519339), [2863360](https://pmc.ncbi.nlm.nih.gov/articles/PMC2863360/), [8706198](https://pmc.ncbi.nlm.nih.gov/articles/PMC8706198/), [7516383](https://pmc.ncbi.nlm.nih.gov/articles/PMC7516383/), [11920889](https://pmc.ncbi.nlm.nih.gov/articles/PMC11920889/); [PubMed 30349525](https://pubmed.ncbi.nlm.nih.gov/30349525/); [MSD Vet, polimixinas](https://www.msdvetmanual.com/pharmacology/antibacterial-agents/polymyxins); [Pharmacology 2e, antifúngicos](https://pressbooks.pub/uwecpharmacology2e/chapter/3-15-antifungals/).

## Não verificado

Lacunas da pesquisa de 25/11. Nada abaixo deve entrar em texto fixo do jogo como fato sem fonte.

- **Sem fonte lida (NV):** núcleo × nucleoide e hemácia madura sem núcleo (OA1); ausência de flagelo em *S. aureus*, *S. pyogenes* e *Moraxella* (só fontes de nível médio ou inferência; o meningococo usava pili tipo IV); flagelo da *E. coli*; Gram de *E. coli*, *P. aeruginosa* e *S. pyogenes* (conhecimento padrão); habitat e microbiota de *S. pyogenes* e *P. aeruginosa*; penicilina em *E. coli* e *P. aeruginosa*; porinas; proteção do biofilme contra o sistema imune; liberação de endotoxina pelo antibiótico; cápsula do *Cryptococcus*; "eclipse" em virologia; prazo real da resposta primária; tamanhos de vírus e células; dano colateral da Lise; bacteremia por *M. catarrhalis* e por *C. tetani*; ponto de corte da colistina para *M. catarrhalis*.
- **Só por resumo de busca (conferir antes de citar):** os 13,5% (a URL PMC6886895 foi atribuída pela ordem dos resultados) e os 85–90% e 65–70% do CDC EID 2001 (PDF deu 404); polimixina e *Neisseria* (PMC6440778 e MSD Vet); as páginas USP, UFS e Pharmacology 2e; vários PMC de imunologia e influenza; o arquivo do CDC sobre *Moraxella*; a frase da Anvisa sobre interromper tratamentos, **não confirmada**.
- **Fontes não abertas:** PDF do Ministério da Saúde sobre Gram, texto oficial da BNCC (MEC), PAN-BR 2026-2030, PDF da Fiocruz sobre resistência, livro AWaRe, notícia da Anvisa de 2024, páginas do CFF. Não há fonte brasileira de nível médio (livro do PNLD, BNCC) para fungos, nem material do Butantan ou da Fiocruz sobre ciclos virais e vacina; a base brasileira é forte em resistência, anticorpos e criptococose e quase ausente em morfologia e flagelo.
- **Próximo passo recomendado:** abrir o PDF do Ministério da Saúde sobre Gram e a OpenStax Biology 2e (caps. 4 e 22) para fechar OA1 e a membrana externa.
