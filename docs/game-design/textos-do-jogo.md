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

| ID | Tela | Texto | Caracteres | Origem |
|---|---|---|---|---|
| CT-01 | menu, linha 1 | `setas/wasd mover · cursor mira · x defender` | 43 | RF-11, RF-12 |
| CT-02 | menu, linha 2 | `direito escanear` / `esquerdo atirar` / `1 A` / `2 B` / `roda troca` | 16 / 15 / 3 / 3 / 10 | RF-12 |
| CT-03 | menu, linha 3 | `A parede (Gram+) · B camada extra (Gram-)` | 41 | spec, "Módulo A e B" |
| CT-04 | HUD | letra `A` ou `B` ao lado do ícone do módulo | 1 | #198 |
| CT-05 | painel do scanner, sem alvo | `chegue perto e clique com o botão direito` | 42 | #198 |
| CT-06 | painel do scanner, alvo neutralizado | `chegue perto de outro alvo e clique com o botão direito` | 57 | #198 |
| CT-07 | painel do scanner, dica de ação | `botão direito: escanear` / `botão direito: escanear de novo` | 23 / 30 | #198 |
| CT-08 | painel do scanner, próxima camada | `botão direito: revelar a próxima camada` | 40 | #198 |

**Limite:** a fonte embutida é 8×8, então o painel aceita ~70 caracteres por linha (640 px menos margens). Nenhuma linha acima passa de 57 caracteres.
