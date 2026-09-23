# Nanobô — Projeto de Software

> **Entrega:** Projeto Integrador II — Projeto de Software + Kanban
>
> **Data acadêmica:** 23/09/2026
>
> **Equipe:** Gustavo Herick Silva e Eric Brito Sampaio de Melo
>
> **Tecnologias:** C + Allegro 5 + Git/GitHub + Visual Studio

## 1. Objetivo

Este documento consolida o planejamento de software do Nanobô para a entrega de Projeto de Software + Kanban. Ele descreve escopo, requisitos, entregáveis, decomposição do trabalho, dependências, estimativas, marcos e critérios de validação.

O **GitHub Project** é a fonte operacional do andamento das tarefas. As Issues representam trabalho executável; este documento registra contexto, planejamento e relações entre os blocos de trabalho.

## 2. Visão do produto

Nanobô é um jogo educacional 2D de ação em que o jogador controla um nanorrobô e defende um organismo identificando ameaças biológicas.

### Loop central

**Explorar → Escanear → Identificar → Decidir → Agir → Consequência**

A aprendizagem deve participar da tomada de decisão durante o gameplay, e não aparecer apenas como um quiz separado.

### Pilares

- **Ação**
- **Aprendizado**
- **Descoberta**

## 3. Escopo

### MVP

O MVP permanece centrado no núcleo jogável da Fase 1:

- exploração e movimentação;
- scanner;
- identificação;
- interpretação das informações;
- escolha de ação/raio;
- combate;
- defesa;
- vida e dano;
- pontuação;
- progressão;
- vitória e derrota;
- fluxo de telas necessário para uma partida completa.

A Fase 1 trabalha bactérias e células saudáveis, incluindo a distinção entre Gram-positivas e Gram-negativas.

### Pós-MVP / entrega final

A expansão prevista inclui:

- segunda fase;
- vírus;
- ciclos lítico e lisogênico;
- multiplicação limitada;
- múltiplas fases;
- spritesheet/animação;
- música e efeitos sonoros;
- hi-score persistente;
- estabilização, testes e release;
- instalador Windows.

Funcionalidades adicionais que não possuem justificativa educacional, técnica ou de requisito permanecem fora do escopo até decisão da dupla.

## 4. Requisitos

### 4.1 Requisitos funcionais

| ID | Requisito | Validação |
|---|---|---|
| RF01 | Possuir menu inicial | Teste de navegação |
| RF02 | Permitir movimentação por teclado | Teste de controles |
| RF03 | Permitir interação por mouse | Teste de entrada |
| RF04 | Possuir scanner por alvo válido mais próximo | Teste de seleção |
| RF05 | Revelar informações em pulsos/camadas | Teste do scanner |
| RF06 | Permitir identificação única por agente | Teste de identificação |
| RF07 | Fazer a informação do scanner participar da decisão | Teste de gameplay |
| RF08 | Possuir ataque e defesa | Teste de combate |
| RF09 | Possuir vida, dano e consequências | Teste de colisões/combate |
| RF10 | Possuir pontuação e progressão | Teste de partida |
| RF11 | Possuir vitória e Game Over | Teste de estados |
| RF12 | Possuir múltiplas fases na versão final | Teste de progressão |
| RF13 | Possuir hi-score persistente na versão final | Teste entre execuções |
| RF14 | Possuir música e efeitos sonoros na versão final | Teste de áudio |
| RF15 | Utilizar spritesheet/animação na versão final | Inspeção/teste visual |
| RF16 | Possuir instalador Windows na entrega final | Teste em máquina limpa |

### 4.2 Requisitos não funcionais

| ID | Requisito | Validação |
|---|---|---|
| RNF01 | Implementação em C com Allegro 5 | Inspeção do projeto |
| RNF02 | Código versionado com Git/GitHub | Inspeção do repositório |
| RNF03 | Organização por módulos compatível com a estrutura atual | Inspeção |
| RNF04 | Uso máximo de aproximadamente 100 MB de RAM | Medição reproduzível |
| RNF05 | Funcionalidades concluídas devem possuir critério de aceite verificável | QA |
| RNF06 | Alterações maiores devem ser revisadas antes do merge | Processo Git/PR |

## 5. Arquitetura e organização técnica

A arquitetura deve permanecer simples e adequada a uma equipe de duas pessoas.

### Fluxo principal

```text
Process Input → Update → Render
                 ↑
             Delta Time
```

### Estados previstos

```text
MENU
  ↓
GAME ↔ PAUSE
  ↓
RESULTS
  ↓
VICTORY / GAME OVER
  ↓
nova partida / encerramento
```

Os estados devem ser centralizados e impedir atualização indevida do gameplay fora de GAME.

### Estrutura considerada

```text
src/
├── core/
├── gameplay/
├── entities/
├── scenes/
└── systems/

assets/
├── sprites/
├── backgrounds/
├── audio/
├── fonts/
└── data/

docs/
├── one-sheet/
├── game-design/
└── planejamento/
```

A estrutura existente do repositório deve ser preservada sempre que possível; novas abstrações só devem ser criadas quando houver necessidade real.

## 6. Decomposição do trabalho — EAP

### E1 — Planejamento e escopo
Issues principais: #1, #2, #3, #45, #177.

### E2 — Fundação técnica
Issues principais: #4, #5, #6, #7, #92–#96, #102.

### E3 — Jogador, entrada e colisões
Issues principais: #8, #9, #10, #11, #22, #23.

### E4 — Agentes e combate
Issues principais: #13, #14, #16, #17, #49, #62–#64.

### E5 — Scanner e identificação
Issues principais: #3, #59–#61, #48.

### E6 — Fase 1 e aprendizagem
Issues principais: #18, #24–#26, #65–#67, #177, #178.

### E7 — PoC e validação do núcleo
Issues principais: #27, #28, #120–#126.

### E8 — Fluxo integrado e interface
Issues principais: #19, #20, #50–#53, #57, #58, #68–#76, #103–#107.

### E9 — Múltiplas fases e Fase 2
Issues principais: #33, #55, #83–#91.

### E10 — Recursos, persistência, arte e áudio
Issues principais: #30–#32, #34, #54, #77–#82, #108–#113.

### E11 — QA, memória e estabilização
Issues principais: #36–#39, #114–#143.

### E12 — Documentação, release e apresentação
Issues principais: #40–#47, #144–#176.

> A numeração acima é uma referência de agrupamento. O status real das Issues deve ser consultado no GitHub Project.

## 7. User Stories representativas

| ID | User Story | Critério de aceite resumido |
|---|---|---|
| US01 | Como jogador, quero movimentar o Nanobô para explorar o ambiente. | Movimento reproduzível por teclado. |
| US02 | Como jogador, quero escanear um agente para obter informações. | Alvo válido mais próximo é selecionado e informação é revelada em camadas. |
| US03 | Como jogador, quero identificar agentes uma única vez por encontro. | Identificação não é contabilizada novamente para o mesmo agente. |
| US04 | Como jogador, quero usar a informação do scanner para escolher o raio. | A informação permite ao jogador relacionar classificação e ação. |
| US05 | Como jogador, quero sofrer consequências quando tomar decisões inadequadas. | A consequência ocorre de acordo com as regras da fase e possui feedback. |
| US06 | Como jogador, quero concluir uma fase e visualizar seu resultado. | Vitória/derrota e resultado são alcançáveis de forma reproduzível. |
| US07 | Como jogador, quero enfrentar uma segunda fase na versão final. | A transição para a Fase 2 funciona e seu conteúdo educacional é jogável. |
| US08 | Como jogador, quero ter minha pontuação preservada no hi-score. | Resultado permanece disponível após nova execução. |

As User Stories devem ser decompostas em Issues/Tasks executáveis no GitHub.

## 8. Dependências críticas

```text
Fundação
   ↓
Jogador / entidades / colisões
   ↓
Combate
   ↓
Scanner + identificação
   ↓
Fase 1
   ↓
PoC
   ↓
Fluxo integrado / MVP
   ↓
Múltiplas fases / Fase 2
   ↓
Persistência + áudio + arte
   ↓
QA + memória + release
   ↓
Instalador + apresentação
```

Alguns trabalhos podem ocorrer em paralelo, especialmente documentação, arte, áudio e preparação de recursos, desde que suas dependências estejam satisfeitas e não haja conflito de arquivos.

## 9. Estimativas

As Issues existentes utilizam **Story Points** com escala Fibonacci:

**1, 2, 3, 5, 8, 13**

Regra de planejamento:

- tarefas pequenas e bem definidas: 1–3;
- tarefas com integração ou risco moderado: 5;
- tarefas grandes ou com incerteza significativa: 8;
- 13 indica trabalho que deve ser reavaliado e, quando possível, decomposto.

As estimativas atuais das Issues são a referência operacional. A dupla deve validar se continuam realistas conforme o trabalho avance.

## 10. Milestones e plano de entregas

| Marco | Data | Objetivo |
|---|---:|---|
| M1 — One Sheet Paper | 09/09/2026 | Conceito, escopo e visão do jogo |
| M2 — Projeto de Software + Kanban | 23/09/2026 | Planejamento, EAP, requisitos, tarefas, estimativas e Kanban |
| M3 — PoC | 07/10/2026 | Provar os maiores riscos técnicos e o núcleo da mecânica |
| M4 — MVP | 04/11/2026 | Demonstrar o núcleo jogável e educacional integrado |
| M5 — Meta interna | 29/11/2026 | Estabilização e preparação da release |
| M6 — Entrega final + apresentação | 02/12/2026 | Release, documentação, instalador e apresentação |

**Observação:** M5 é meta interna do projeto, não entrega oficial da disciplina.

### M2 — foco imediato

Para a entrega de 23/09, o foco documental é:

- Projeto de Software;
- EAP;
- requisitos;
- User Stories/Tasks;
- dependências;
- estimativas;
- milestones/plano de entregas;
- relação com o Kanban.

O andamento efetivo das tarefas deve ser mantido no GitHub Project.

## 11. Estratégia da PoC

A PoC deve reduzir as maiores incertezas antes da expansão do projeto.

Prioridades:

1. Game Loop e Delta Time;
2. máquina de estados;
3. entrada;
4. movimentação;
5. colisões;
6. agentes;
7. scanner e seleção de alvo;
8. identificação;
9. relação informação → decisão → ação;
10. vida, dano e combate;
11. fluxo mínimo da Fase 1;
12. teste reproduzível dos riscos.

A PoC não é considerada concluída apenas porque o programa compila.

## 12. Critério geral de Done

Uma Issue só deve ser considerada **Done** quando:

1. o critério de aceite da própria Issue foi comprovado;
2. o teste é reproduzível;
3. código/documentação relevante está versionado;
4. a alteração não quebra funcionalidades já validadas;
5. o resultado foi refletido no GitHub Project.

## 13. Kanban e colaboração

Fluxo definido:

**Backlog → To Do → In Dev → In Review → Done**

WIP recomendado: **2 tarefas simultâneas**.

Regras:

- GitHub Project é a fonte operacional do status;
- Issues representam trabalho executável;
- branches são usadas para trabalhos independentes;
- Pull Requests devem ser revisados antes do merge;
- commits devem ser pequenos e descritivos;
- segredos não devem ser versionados.

## 14. Riscos principais

| Risco | Impacto | Estratégia |
|---|---|---|
| Scanner não produzir informação útil | Alto | Validar na PoC |
| Relação informação → decisão não ficar clara | Alto | Testar gameplay e feedback |
| Colisões/combate instáveis | Alto | Validar antes da expansão |
| Máquina de estados gerar fluxo inconsistente | Alto | Testes por estado e transição |
| Escopo crescer durante desenvolvimento | Alto | Usar MVP e Issues como fronteira |
| Recursos serem carregados/liberados incorretamente | Médio/alto | Carregamento centralizado + testes |
| Memória ultrapassar limite | Alto | Medição antes da release |
| Fase 2 atrasar o núcleo | Alto | Só iniciar expansão após Fase 1/MVP estáveis |
| Instalação falhar em máquina limpa | Alto | Teste de release dedicado |

## 15. QA e evidências

A validação deve cobrir:

- controles;
- movimentação;
- colisões;
- scanner;
- identificação;
- combate;
- defesa;
- pontuação;
- progressão;
- estados;
- menu;
- pausa;
- Game Over;
- vitória;
- múltiplas fases;
- áudio;
- hi-score;
- memória;
- instalação;
- regressão;
- compreensão do conteúdo educacional.

Falhas relevantes devem ser reproduzíveis e registradas em Issues.

## 16. Documentos de referência

- One Sheet: `docs/one-sheet/one-sheet.md`
- GDD: `docs/game-design/gdd.md`
- Escopo: `docs/game-design/escopo.md`
- Decisões: `docs/game-design/decisoes.md`
- Kanban: `docs/planejamento/kanban.md`
- Milestones: `docs/planejamento/milestones.md`
- Plano de execução: `docs/planejamento/plano-de-execucao.md`
- Sequência operacional: `docs/planning/sequencia-issues.md`

## 17. Limites e pendências

As seguintes decisões continuam dependentes de validação da dupla/PoC quando ainda não houver evidência:

- cooldown definitivo do scanner;
- mitigação definitiva da defesa;
- balanceamento da Fase 2;
- espécies bacterianas finais;
- detalhes da arquitetura modular;
- formato concreto do arquivo de hi-score;
- fases posteriores à Fase 2;
- valores finais de balanceamento.

Não preencher essas lacunas por suposição. Quando necessário, registrar explicitamente como **[PENDÊNCIA]**.

---

**Fonte operacional do andamento:** GitHub Project do repositório.

**Fonte de planejamento:** este documento + Issues + documentos de design.

**Regra central:** o planejamento deve refletir o projeto real; nenhuma decisão de design ou resultado de teste deve ser inventado para completar a documentação.
