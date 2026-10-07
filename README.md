<div align="center">

# Nanobô

**Jogo educacional 2D desenvolvido em C com Allegro 5.**

Um nanorrobô explora um ambiente microscópico inspirado no interior do corpo humano, analisa organismos por meio de um scanner e utiliza as informações obtidas para tomar decisões.

</div>

## Sobre o projeto

Nanobô é um projeto de jogo educacional desenvolvido para o Projeto Integrador II. A proposta combina exploração, investigação e tomada de decisão com conteúdos do Ensino Médio, fazendo com que o aprendizado participe do gameplay.

O ciclo central do jogo é:

**Explorar → Escanear → Identificar → Decidir → Agir → Consequência**

O jogador deve interpretar as informações obtidas pelo scanner para decidir como agir, em vez de responder apenas a um quiz separado.

> **Status:** em desenvolvimento.
>
> **PoC (M3 — 07/10/2026):** o núcleo de investigação está implementado e jogável — movimentação por teclado, scanner com seleção do alvo válido mais próximo, leitura em duas camadas e identificação única por agente. O roteiro reproduzível e o resultado observado estão em [Registro da PoC](docs/planejamento/qa-poc.md). Combate, vida, dano e agentes ainda não existem: pertencem ao marco interno M3.5.

## Núcleo do jogo

- Exploração de um ambiente microscópico.
- Scanner com seleção do alvo válido mais próximo e leitura em pulsos/camadas.
- Identificação única dos agentes encontrados.
- Interpretação das informações do scanner para orientar a decisão do jogador.
- Combate, defesa, vida, dano e consequências.
- Pontuação, bônus por identificação, progressão e hi-score persistente.
- Progressão por fases.

### Conteúdo planejado

**Fase 1 — bactérias e células saudáveis**

A primeira fase trabalha a diferenciação entre bactérias e células saudáveis, incluindo a abordagem de Gram-positivas e Gram-negativas. A informação obtida no scanner deve ter função prática na escolha da ação do jogador.

**Fase 2 — vírus**

A segunda fase é planejada para a entrega final, após o MVP. O conteúdo envolve os ciclos lítico e lisogênico e uma mecânica de multiplicação limitada. As regras biológicas e de balanceamento que ainda não foram validadas permanecem como pendências.

## Requisitos da disciplina

Os requisitos finais registrados no planejamento do projeto são:

- Menu inicial.
- Game Over.
- Múltiplas fases.
- Entrada por teclado e mouse.
- Animação por spritesheet.
- Música e efeitos sonoros.
- Hi-score persistente.
- Uso máximo de 100 MB de RAM.
- Instalador para Windows.

A entrega final deve cumprir esses requisitos. O MVP permanece deliberadamente centrado em uma fase para validar primeiro o núcleo jogável.

## Tecnologias

| Tecnologia | Uso |
| --- | --- |
| **C** | Linguagem principal |
| **Allegro 5** | Biblioteca para desenvolvimento do jogo 2D |
| **Visual Studio** | Ambiente de desenvolvimento atual |
| **Git / GitHub** | Versionamento e colaboração |

## Estrutura do projeto

```text
Nanobo/
├── src/                    # Código-fonte
│   ├── core/               # Núcleo e ciclo do jogo
│   ├── gameplay/           # Regras e mecânicas de gameplay
│   ├── entities/           # Entidades do jogo
│   ├── scenes/             # Cenas e estados de jogo
│   └── systems/            # Sistemas auxiliares
│
├── assets/                 # Recursos utilizados pelo jogo
│   ├── sprites/
│   ├── backgrounds/
│   ├── audio/
│   │   ├── music/
│   │   └── sfx/
│   ├── fonts/
│   └── data/
│
├── docs/                   # Documentação do projeto
│   ├── one-sheet/
│   ├── game-design/
│   └── planejamento/
│
├── Nanobo.sln              # Solução do Visual Studio
└── Nanobo.vcxproj          # Projeto do Visual Studio
```

> **Atenção:** apenas `src/main.c` e `src/core/game.c` entram na compilação — são os dois únicos `<ClCompile>` de `Nanobo.vcxproj`. Os demais módulos (`gameplay/`, `entities/`, `scenes/`, `systems/`) são a organização planejada e ainda não têm código compilado. Ao acrescentar código, coloque-o em um arquivo que já compila.

## Planejamento acadêmico

| Marco | Data | Natureza |
| --- | --- | --- |
| One Sheet Paper | 09/09/2026 | Entrega acadêmica |
| Projeto de Software + Kanban | 23/09/2026 | Entrega acadêmica |
| PoC | 07/10/2026 | Entrega acadêmica |
| MVP | 04/11/2026 | Entrega acadêmica |
| Entrega final + apresentação | 02/12/2026 | Entrega acadêmica |

Cada etapa corresponde a 20% da avaliação conforme o planejamento acadêmico registrado para o projeto. Entre os marcos acadêmicos existem dois marcos internos, **M3.5 — Construção do Core Gameplay** (21/10) e **M5 — Release Candidate** (29/11), que não compõem nota.

## Documentação

- [One Sheet](docs/one-sheet/one-sheet.md)
- [GDD](docs/game-design/gdd.md)
- [Escopo](docs/game-design/escopo.md)
- [Decisões de Game Design](docs/game-design/decisoes.md)
- [Conteúdo biológico e objetivos de aprendizagem](docs/game-design/conteudo-biologico.md)
- [Regras do scanner](docs/game-design/scanner.md)
- [Kanban](docs/planejamento/kanban.md)
- [Plano de Execução](docs/planejamento/plano-de-execucao.md)
- [Projeto de Software](docs/planejamento/projeto-de-software.md)
- [Milestones](docs/planejamento/milestones.md)
- [Fluxo de estados](docs/planejamento/fluxo-estados.md)
- [Registro da PoC](docs/planejamento/qa-poc.md)
- [Validação da máquina de estados](docs/planejamento/qa-maquina-estados.md)
- [Validação do carregamento de recursos](docs/planejamento/qa-recursos.md)
- [Catálogo de recursos](assets/catalogo-recursos.md)

## Desenvolvimento

O desenvolvimento é incremental e orientado por riscos. A prioridade é validar o núcleo técnico e educacional antes de investir em conteúdo e polimento.

A referência operacional das tarefas é o Kanban do projeto. Os documentos de design registram decisões e regras; Issues representam trabalho executável.

## Equipe

- **Gustavo Herick Silva**
- **Eric Brito Sampaio de Melo**

## Projeto Integrador

Projeto acadêmico desenvolvido como parte do **Projeto Integrador II — 2026**.

## Licença

Consulte o arquivo [LICENSE.txt](LICENSE.txt) para informações sobre a licença do projeto.

---

README inspirado em boas práticas e referências de organização apresentadas no [Awesome README](https://github.com/matiassingers/awesome-readme).
