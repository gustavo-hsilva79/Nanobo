# Catálogo de recursos do Nanobô

Inventário dos recursos necessários à execução atual, para orientar o carregador central da #54. Os recursos já presentes estão listados abaixo; pastas vazias e recursos ainda não escolhidos permanecem indicados como pendências.

## Recursos presentes e usados

| Recurso | Uso atual | Local |
|---|---|---|
| Sprite do Nanobô (32×32) | Personagem do jogador | `assets/sprites/nanobo.png`; `load_resources()` em `src/main.c` |
| Bactéria Gram-positiva (32×32) | Agente `SPECIES_GRAM_POSITIVE` | `assets/sprites/bacteria_gram_positiva.png`; idem |
| Bactéria Gram-negativa (32×32) | Agente `SPECIES_GRAM_NEGATIVE` | `assets/sprites/bacteria_gram_negativa.png`; idem |
| Célula saudável própria (32×32) | Agente `SPECIES_CELL` | `assets/sprites/celula_propria.png`; idem |
| Ícone de coração (16×16) | HUD de vida | `assets/sprites/icone_coracao.png`; idem |
| Fonte embutida do Allegro | Todo o texto da interface: HUD, painel do scanner, menu, pausa e telas de fim | `al_create_builtin_font()` em `src/main.c`; não há arquivo de fonte |

`free_resources()` libera os cinco bitmaps na ordem inversa e depois a fonte. O projétil é desenhado com `al_draw_filled_circle()` (primitivas), sem bitmap.

## Arte final criada

Os três sprites de agente e o ícone de coração já são carregados pelo jogo. O fundo, a tira de quadros do Nanobô e os ícones de laser/scanner ainda não são. Cada recurso tem a fonte `.aseprite` editável ao lado do `.png` exportado; o `.png` é o arquivo que o jogo deve carregar.

Todos os sprites usam a mesma paleta, o mesmo tamanho de pixel e a mesma direção de luz (cima-esquerda), para manter a coesão visual.

### Agentes da Fase 1 — 32×32, PNG com transparência

| Arquivo | Agente | Cor | Forma | Pista visual do envelope |
|---|---|---|---|---|
| `assets/sprites/bacteria_gram_positiva.png` | Bactéria Gram-positiva | roxo | cacho de cocos esféricos | parede espessa e clara (2 px), sem membrana externa |
| `assets/sprites/bacteria_gram_negativa.png` | Bactéria Gram-negativa | vermelho | bacilo (bastonete) | membrana externa fina, separada do corpo |
| `assets/sprites/celula_propria.png` | Célula saudável própria | verde | célula maior e arredondada | núcleo delimitado com nucléolo e mitocôndrias |

As cores seguem a regra da #16: **roxo para Gram+** e **vermelho para Gram−**; a célula própria é verde. O HUD usa o mesmo roxo e vermelho nos projéteis, e o coração é carmim para não competir com o laser vermelho.

A forma e o tamanho também separam os agentes: as bactérias são pequenas e simples (sem organelas), enquanto a célula própria é maior e tem núcleo delimitado — a diferença procarioto/eucarioto.

### Nanobô animado — spritesheet

| Arquivo | Conteúdo |
|---|---|
| `assets/sprites/nanobo_spritesheet.png` | Tira horizontal de **256×32**: 8 quadros de 32×32 |
| `assets/sprites/nanobo_spritesheet.aseprite` | Fonte editável, com as etiquetas de animação |

| Etiqueta | Quadros | Duração | Movimento |
|---|---|---|---|
| `idle` | 1–4 | 220 ms cada | flutuação suave, pluma curta |
| `andar` | 5–8 | 110 ms cada | oscilação rápida, pluma longa e clara |

Cada quadro é um recorte de 32×32 na tira, na ordem dos quadros. Em Allegro, o recorte sai de `al_create_sub_bitmap(tira, quadro * 32, 0, 32, 32)`.

### Fundo do cenário

| Arquivo | Conteúdo |
|---|---|
| `assets/backgrounds/fundo_microscopico.png` | Ambiente microscópico, **640×480** — arte antiga, fora do tamanho da tela atual (640×360); precisa ser reexportado antes de entrar em `load_resources()` |
| `assets/backgrounds/fundo_microscopico.aseprite` | Fonte editável |

O fundo é escuro e de baixo contraste de propósito: os sprites precisam continuar legíveis por cima dele.

### Ícones de interface — 16×16, PNG com transparência

| Arquivo | Uso sugerido |
|---|---|
| `assets/sprites/icone_laser_roxo.png` | Laser correspondente à Gram+ (#16) |
| `assets/sprites/icone_laser_vermelho.png` | Laser correspondente à Gram− (#16) |
| `assets/sprites/icone_scanner.png` | Lupa, para indicar a ação de escanear |
| `assets/sprites/icone_coracao.png` | Vida, para o HUD |

> O coração é carmim, um vermelho mais frio que o da Gram−, para não competir com o laser vermelho no HUD.

## Recursos previstos

| Tipo | Recursos necessários | Local definido | Estado |
|---|---|---|---|
| Sprites e animações | Arte final dos agentes e spritesheet animado do Nanobô | `assets/sprites/` | Agentes carregados; spritesheet criado e não carregado |
| Cenários | Fundo/ambiente microscópico das fases | `assets/backgrounds/` | Criado; não carregado |
| Ícones de interface | Laser, scanner e vida | `assets/sprites/` | Criados; só o coração é carregado |
| Fontes | Tipografia final da interface | `assets/fonts/` | [PENDÊNCIA] Fonte ainda não escolhida; a fonte embutida atual é provisória |
| Música | Trilha da partida | `assets/audio/music/` | [PENDÊNCIA] Arquivo ainda não definido |
| Efeitos sonoros | Feedback de ações e eventos | `assets/audio/sfx/` | [PENDÊNCIA] Arquivos ainda não definidos |
| Dados auxiliares | Configuração ou dados externos, caso necessários | `assets/data/` | [PENDÊNCIA] Nenhum arquivo necessário identificado até o momento |

## Observações para o carregador

- Carregar apenas recursos que tenham sido adicionados e sejam usados pelo jogo.
- Centralizar carregamento e liberação conforme #109 e #112.
- Não tratar diretórios `.gitkeep` como arquivos de recurso.
- Os arquivos `.aseprite` são fontes de edição, **não** recursos de execução: o carregador deve ler os `.png`.
- Atualizar este catálogo quando novos arquivos forem escolhidos ou adicionados.

## Verificação

O roteiro reproduzível do carregamento e da liberação está em `docs/planejamento/qa-recursos.md`.
