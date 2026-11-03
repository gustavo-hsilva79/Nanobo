# Nanobô — textos do jogo (M4)

> **Criado em:** 03/11/2026. Fontes das regras: `docs/game-design/ensino-e-aprendizagem.md` (§"Feedback por situação" e §"Regras para qualquer texto") e as issues #105, #198 e #200.
> Todo texto novo entra aqui com ID, tela, limite de caracteres e de onde vem a regra. Nenhum texto usa PBP, transpeptidase, LPS ou ligações cruzadas.

## 1. Feedback da decisão (HUD da fase, `src/gameplay/gameplay.c`)

| ID | Situação | Texto | Caracteres | Origem |
|---|---|---|---|---|
| FB-01 | módulo A acerta Gram+, primeiro acerto | `módulo A atingiu a parede: falta 1 acerto` | 41 | spec, tabela de feedback |
| FB-02 | módulo B acerta Gram-, primeiro acerto | `módulo B atingiu a camada extra: falta 1 acerto` | 47 | spec, tabela de feedback |
| FB-03 | acerto que neutraliza, agente identificado | `ameaça neutralizada: +10 pontos` | 31 | já existia (#66) |
| FB-04 | acerto que neutraliza sem identificação | `neutralizada sem identificar: o bônus de 9/9 se perdeu` | 54 | já existia (#67) |
| FB-05 | primeiro erro de módulo | `que estrutura o scan mostrou? qual módulo tem esse alvo?` | 56 | spec, dica escalonada |
| FB-06 | segundo erro, B em Gram+ | `no jogo, a Gram+ não tem a camada extra atingida por B` | 54 | spec, dica escalonada |
| FB-07 | segundo erro, A em Gram- | `no jogo, a camada extra das Gram- dificulta a ação de A` | 55 | spec, dica escalonada |
| FB-08 | célula própria ferida pelo A | `no jogo, o módulo A fere a célula própria: -1 vida` | 50 | spec + D1 |
| FB-09 | célula própria ferida pelo B | `no jogo, o módulo B fere a célula própria: -1 vida` | 50 | spec + D1 |
| FB-10 | contato com bactéria | `contato com bactéria: -1 de vida` | 32 | já existia (#11) |

**Hífen, não menos tipográfico.** "Gram-" usa hífen ASCII (U+002D). O "−" (U+2212) que a spec traz não existe na fonte embutida (Latin-1): na tela ele saía como `^`, visto no QA de 03/11 e corrigido no commit `5e7cc75`. Ao editar estes textos, não reintroduza U+2212.

## 2. Controles, HUD e dicas (`src/main.c`)

| ID | Local | Texto | Caracteres | Origem |
|---|---|---|---|---|
| CT-01 | menu, título do painel | `Como jogar` | 10 | #201 |
| CT-02 | menu, grupo 1 | `Movimentação` | 12 | #201 |
| CT-03 | menu, grupo 1 | `setas ou WASD mover · cursor mirar` | 34 | #201 |
| CT-04 | menu, grupo 2 | `Combate` | 7 | #201 |
| CT-05 | menu, grupo 2 | `botão esquerdo atirar · botão direito escanear` | 46 | #201 |
| CT-06 | menu, grupo 3 | `Equipamentos` | 12 | #201 |
| CT-07 | menu, grupo 3 | `1 e 2 ou roda: módulo A/B` | 25 | #201 |
| CT-08 | menu, nota do grupo 3 | `A age na parede · B age na camada externa` | 41 | #201 |
| CT-09 | menu, painel do objetivo | `Objetivo da missão` | 18 | #201 |
| CT-10 | menu, painel do objetivo | `neutralizar as %d bactérias e chegar ao destino` | 47 | #201 |
| CT-11 | menu, painel do objetivo | `identificar os %d agentes vale +%d pontos` | 41 | #201 |
| CT-12 | menu, rodapé | `enter ou clique para começar` | 28 | #201 |
| CT-13 | menu, rodapé | `F11 tela cheia` | 14 | #201 |
| CT-14 | HUD, linha 0 | `ameaças %d/%d` | 11 | #199 |
| CT-15 | HUD, linha 1 | `identificados %d/%d` | 17 | #199 |
| CT-16 | HUD, linha 0 | `pontos %d` | 9 | #199 |
| CT-17 | HUD, linha 0 | `pronto` / `recarga` / `defesa` | 6 / 7 / 6 | #199 |
| CT-18 | scanner, sem alvo | `Nenhum alvo` | 11 | #199 |
| CT-19 | scanner, sem alvo | `Aproxime-se de um agente para escanear.` | 39 | #199 |
| CT-20 | scanner, qualquer estado | `botão direito sobre o agente` | 28 | #199 |
| CT-21 | scanner, alvo neutralizado | `Alvo neutralizado` / `Chegue perto de outro agente.` | 17 / 28 | #199 |
| CT-22 | scanner, alvo no alcance | `Alvo detectado` | 14 | #199 |
| CT-23 | scanner, alvo no alcance | `Clique com o botão direito para escanear.` | 41 | #199 |
| CT-24 | scanner, alvo no alcance | `o scan revela a estrutura do agente` | 35 | #199 |
| CT-25 | scanner, 1 camada | `Leitura 1 de 2` | 14 | #199 |
| CT-26 | scanner, 1 camada | `clique de novo para revelar a estrutura` | 39 | #199 |
| CT-27 | scanner, identificado | `Identificação concluída` | 23 | #199 |
| CT-28 | scanner, identificado | `interprete os dados antes de atacar` | 35 | #199 |
| CT-29 | scanner, cabeçalho | `célula` / `bactéria` | 6 / 7 | #199 |
| CT-30 | resultados | `Fase 1 concluída` / `resumo da fase` | 16 / 14 | #202 |
| CT-31 | resultados | `pontos` / `ameaças neutralizadas` | 6 / 21 | #202 |
| CT-32 | resultados | `agentes identificados` / `bônus de identificação` | 21 / 22 | #202 |
| CT-33 | resultados | `não obtido` | 10 | #202 |
| CT-34 | resultados | `enter também continua` | 21 | #202 |
| CT-35 | vitória | `Vitória` | 7 | #202 |

**Limite:** a fonte embutida é de **8 px por caractere** (medido com `al_get_text_width`, não estimado) e a área útil do painel do scanner é de 588 px. A linha mais longa desenhada na fase tem 47 caracteres (376 px); a mais longa de todas, `botão esquerdo atirar · botão direito escanear`, tem 46 caracteres em 536 px de painel.

## 2.1 Regra de tipografia (medida em 03/11/2026)

**Maiúscula acentuada não desenha.** A fonte embutida devolve glifo errado para `Í`, `Ç`, `Ã`, `Ó` maiúsculos — `FASE 1 CONCLUÍDA` saía `CONCLU f DA`, `MOVIMENTAÇÃO` saía `MOVIMENTAÇ%Ø` — enquanto as **minúsculas acentuadas saem corretas** (`ç`, `ã`, `ó`, `ê`, `í`).

Regras que valem para todo texto novo:

- Títulos e rótulos em **caixa de frase** (`Fase 1 concluída`, `Objetivo da missão`, `Identificação concluída`).
- Caixa alta só em texto **sem acento** (`INICIAR`, `CONTINUAR`, `COMO JOGAR` quando não houver acento).
- Nenhum texto desenhado pode ter maiúscula acentuada. `NANOBÔ` é exceção registrada: é o nome do jogo, desenhado em bitmap ampliado 3×, onde o glifo é legível mas não correto.
- Antes de commitar texto novo, rodar `python -c "print([c for c in 'TEXTO' if ord(c) > 0xFF])"` (codepoint fora de Latin-1 vira `^` na tela).
