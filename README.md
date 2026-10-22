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
> **M3 — PoC (07/10/2026):** o núcleo de investigação está implementado e jogável — movimentação por teclado, scanner com seleção do alvo válido mais próximo, leitura em duas camadas e identificação única por agente. O roteiro reproduzível e o resultado observado estão em [Registro da PoC](docs/planejamento/qa-poc.md).
>
> **M3.5 — core gameplay (concluído, 18/10/2026):** o combate está no código — ataque com laser, projétil, resistência e neutralização, defesa com cooldown, vida em corações, dano por contato e Game Over por vida zerada. O encontro tem **9 agentes** (3 Gram+, 3 Gram− e 3 células), num mundo de 1920×512 que rola com a câmera: a tela lógica é **640×360**, ampliada na janela por escala inteira. A célula própria anda mais devagar e atacar uma célula custa 1 de vida ao Nanobô. Menu, pausa e tela de Game Over também estão prontos. Ainda faltam a pontuação e as condições reais de fim de fase (#18/#26); até lá, `RESULTS` e `VICTORY` são alcançados por teclas de teste. Os roteiros estão em [Registro da PoC](docs/planejamento/qa-poc.md).

## Núcleo do jogo

- Exploração de um ambiente microscópico.
- Scanner com seleção do alvo válido mais próximo e leitura em pulsos/camadas.
- Identificação única dos agentes encontrados.
- Interpretação das informações do scanner para orientar a decisão do jogador.
- Combate implementado: laser por espécie, projétil, resistência, defesa e vida.
- Pontuação, bônus por identificação, progressão e hi-score persistente *(planejados)*.
- Progressão por fases *(planejada)*.

### Conteúdo planejado

**Fase 1 — bactérias e células saudáveis**

A primeira fase trabalha a diferenciação entre bactérias e células saudáveis, incluindo a abordagem de Gram-positivas e Gram-negativas. A informação obtida no scanner deve ter função prática na escolha da ação do jogador.

**Fase 2 — vírus**

A segunda fase é planejada para a entrega final, após o MVP. O conteúdo envolve os ciclos lítico e lisogênico e uma mecânica de multiplicação limitada. As regras biológicas e de balanceamento que ainda não foram validadas permanecem como pendências.

## Controles atuais

| Tecla | Ação |
| --- | --- |
| setas ou `WASD` | mover o Nanobô |
| `ESPAÇO` | scanner: revela a próxima camada do alvo mais próximo |
| `J` | atacar na direção em que o Nanobô está olhando |
| `1` / `2` | escolher laser roxo (Gram+) / vermelho (Gram−) |
| `X` | defesa: 2 s de imunidade e 3 s de recarga |
| `ESC` | pausar / retomar |
| `ENTER` | iniciar partida, confirmar, nova partida |
| clique em `INICIAR` | iniciar partida no menu |
| `F` / `G` / `V` | teclas de teste de fluxo, saem quando #20/#26 fecharem |

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

## Como compilar e executar

Pré-requisitos: **Windows** e **Visual Studio 2022** com a carga de trabalho de desenvolvimento para desktop com C++.

1. Abra `Nanobo.sln` no Visual Studio.
2. Na primeira vez, deixe o Visual Studio **restaurar os pacotes NuGet** — `Allegro 5.2.11.3` e `AllegroDeps 1.16.0`, listados em `packages.config`. Se o build reclamar de `packages\Allegro*.targets` ausente, restaure os pacotes; não edite o projeto para contornar.
3. Escolha a configuração **Debug | x64** e compile com `Ctrl+Shift+B`. As configurações Win32 são legadas. O projeto compila em **C11** (`/std:c11`) com nível de aviso 4: código novo precisa compilar nos dois.
4. Execute a partir da **raiz do repositório** (a pasta onde está `Nanobo.sln`): o jogo carrega os recursos por caminho relativo, como `assets/sprites/nanobo.png`.

O executável fica em `x64\Debug\Nanobo.exe`, ou `x64\Release\Nanobo.exe` quando a configuração é Release.

## Estrutura do projeto

```text
Nanobo/
├── src/                    # Código-fonte
│   ├── core/               # Máquina de estados das cenas
│   ├── entities/           # Entidades do jogo
│   ├── gameplay/           # Regras e mecânicas de gameplay
│   ├── scenes/             # Cenas e estados de jogo
│   └── systems/            # Sistemas auxiliares (ainda só no papel)
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
│   ├── game-design/
│   ├── planejamento/
│   └── arquivo/            # o que não é mais referência viva
│
├── Nanobo.sln              # Solução do Visual Studio
└── Nanobo.vcxproj          # Projeto do Visual Studio
```

> **Atenção:** entram na compilação os `<ClCompile>` de `Nanobo.vcxproj`: `src/main.c`, `src/core/game.c`, `src/entities/entities.c` e `src/gameplay/gameplay.c`. O `src/scenes/scenes.h` também é compilado, porque `src/core/game.h` o inclui. O `src/systems/systems.h` é um cabeçalho vazio e continua fora do build: código escrito ali não compila.

## Planejamento acadêmico

| Marco | Data | Natureza |
| --- | --- | --- |
| One Sheet Paper | 09/09/2026 | Entrega acadêmica |
| Projeto de Software + Kanban | 23/09/2026 | Entrega acadêmica |
| PoC | 07/10/2026 | Entrega acadêmica |
| MVP | 04/11/2026 | Entrega acadêmica |
| Entrega final + apresentação | 02/12/2026 | Entrega acadêmica |

Cada etapa corresponde a 20% da avaliação conforme o planejamento acadêmico registrado para o projeto. Entre os marcos acadêmicos existem dois marcos internos, **M3.5 — Construção do Core Gameplay** (21/10) e **M5 — Release Candidate** (29/11), que não compõem nota.

A lista completa dos marcos, com o objetivo e o critério de foco de cada um, está em [Milestones](docs/planejamento/milestones.md).

## Documentação

> Documentos marcados como *registro* foram escritos na data da entrega e não são atualizados; o estado corrente do produto está neste README.

- [One Sheet](docs/one-sheet.md) *(registro de 09/09/2026)*
- [GDD](docs/game-design/gdd.md)
- [Escopo](docs/game-design/escopo.md)
- [Decisões de Game Design](docs/game-design/decisoes.md)
- [Conteúdo biológico e objetivos de aprendizagem](docs/game-design/conteudo-biologico.md)
- [Glossário](docs/game-design/glossario.md)
- [Regras do scanner](docs/game-design/scanner.md)
- [Referências biológicas das Issues #2 e #177](docs/planejamento/referencias-biologicas-issues-2-177.md)
- [Kanban](docs/planejamento/kanban.md)
- [Milestones](docs/planejamento/milestones.md)
- [Plano de Execução](docs/planejamento/plano-de-execucao.md)
- [Plano de execução do backlog restante](docs/planejamento/plano-backlog-restante.md)
- [Projeto de Software](docs/planejamento/projeto-de-software.md) *(registro de 23/09/2026)*
- [Divisão de Issues da dupla](docs/planejamento/divisao-issues-dupla.md)
- [Guia: mover código do main.c para os módulos](docs/planejamento/guia-refatoracao-modulos.md)
- [Fluxo de estados](docs/planejamento/fluxo-estados.md)
- [Interação por mouse](docs/planejamento/interacao-mouse.md)
- [Registro da PoC](docs/planejamento/qa-poc.md)
- [Validação da máquina de estados](docs/planejamento/qa-maquina-estados.md)
- [Validação do carregamento de recursos](docs/planejamento/qa-recursos.md)
- [Catálogo de recursos](assets/catalogo-recursos.md)
- [Documentos arquivados](docs/arquivo/README.md)

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
