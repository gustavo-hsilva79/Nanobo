<div align="center">

# Nanobô

**Jogo educacional 2D desenvolvido em C com Allegro 5.**

Um nanorrobô explora um ambiente microscópico inspirado no interior do corpo humano, analisa organismos por meio de um scanner e utiliza as informações obtidas para tomar decisões.

</div>

## Sobre o projeto

Nanobô é um projeto de jogo educacional desenvolvido para o Projeto Integrador. A proposta combina exploração, investigação e tomada de decisão com conteúdos do Ensino Médio, buscando transformar o aprendizado em parte da própria experiência de jogo.

O jogador controla o Nanobô, explora o ambiente, coleta e interpreta informações e utiliza essas informações para decidir como agir diante dos organismos encontrados.

> **Status:** em desenvolvimento.

## Principais elementos

- Exploração de um ambiente microscópico.
- Scanner para análise de organismos.
- Interpretação de informações apresentadas durante o jogo.
- Tomada de decisões e recomendações pelo jogador.
- Progressão por múltiplas fases.
- Sistema de pontuação e hi-score.
- Entrada por teclado e mouse.
- Animações com spritesheets.
- Música e efeitos sonoros.
- Estados de jogo, incluindo menu e Game Over.

As funcionalidades podem evoluir durante o desenvolvimento conforme o planejamento, os testes e as necessidades do projeto.

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
├── Nanobo.sln             # Solução do Visual Studio
└── Jogo.vcxproj           # Projeto do Visual Studio
```

A estrutura acompanha a evolução do projeto; diretórios e módulos podem ser refinados conforme as funcionalidades forem implementadas.

## Documentação

- [One Sheet](docs/one-sheet/one-sheet.md)
- [GDD](docs/game-design/gdd.md)
- [Escopo](docs/game-design/escopo.md)
- [Decisões de Game Design](docs/game-design/decisoes.md)
- [Kanban](docs/planejamento/kanban.md)
- [Plano de Execução](docs/planejamento/plano-de-execucao.md)
- [Milestones](docs/planejamento/milestones.md)

## Desenvolvimento

O projeto está sendo desenvolvido de forma incremental. O planejamento utiliza um fluxo de Kanban e milestones para acompanhar a evolução desde a definição do conceito até a versão final.

O código atualmente parte de um protótipo inicial em Allegro 5. As próximas implementações devem priorizar os riscos técnicos e o núcleo jogável antes de funcionalidades secundárias.

## Equipe

- **Gustavo Herick Silva**
- **Eric Brito Sampaio de Melo**

## Projeto Integrador

Projeto acadêmico desenvolvido como parte do **Projeto Integrador II — 2026**.

## Licença

Consulte o arquivo [LICENSE.txt](LICENSE.txt) para informações sobre a licença do projeto.

---

README inspirado em boas práticas e referências de organização apresentadas no [Awesome README](https://github.com/matiassingers/awesome-readme).
