# Catálogo de recursos do Nanobô

Inventário dos recursos necessários à execução atual, para orientar o carregador central da #54. Os recursos já presentes estão listados abaixo; pastas vazias e recursos ainda não escolhidos permanecem indicados como pendências.

## Recursos presentes e usados

| Recurso | Uso atual | Local |
|---|---|---|
| Sprite do Nanobô (32×32, 8 quadros) | Personagem do jogador | `assets/sprites/nanobo_spritesheet.png` (256×32, 8 quadros de 32×32), o bitmap que `load_resources()` abre e `draw_game()` recorta (#30); `nanobo.png` fica como histórico |
| Bactéria Gram-positiva (32×32) | Agente `SPECIES_GRAM_POSITIVE` | `assets/sprites/bacteria_gram_positiva.png`; idem |
| Bactéria Gram-negativa (32×32) | Agente `SPECIES_GRAM_NEGATIVE` | `assets/sprites/bacteria_gram_negativa.png`; idem |
| Célula saudável própria (32×32) | Agente `SPECIES_CELL` | `assets/sprites/celula_propria.png`; idem |
| Ícone de coração (16×16) | HUD de vida | `assets/sprites/icone_coracao.png`; idem |
| Ícone de scanner (16×16) | Cabeçalho do painel do scanner e bloco "como jogar" do menu | `assets/sprites/icone_scanner.png`; idem |
| Ícone da arma Ruptura (16×16) | Indicador da arma na HUD: parede de tijolos, cor neutra e forma própria (04/11) | `assets/sprites/icone_ruptura.png` |
| Ícone da arma Dissolução (16×16) | Indicador da arma na HUD: anéis concêntricos, cor neutra e forma própria (04/11) | `assets/sprites/icone_dissolucao.png` |
| Vírus da Fase 2 (32×32) | Agente `SPECIES_VIRUS`: corpo laranja com espinhos e núcleo | `assets/sprites/virus_fase2.png` |
| Ícone de defesa (16×16) | Linha de controles do menu | `assets/sprites/icone_defesa.png`; idem |
| Fonte embutida do Allegro | Todo o texto da interface: HUD, painel do scanner, menu, pausa e telas de fim | `al_create_builtin_font()` em `src/main.c`; não há arquivo de fonte |
| Mapa do mundo da Fase 1 (1920×512) | Fundo do mundo, desenhado atrás de destino, agentes e Nanobô | `assets/backgrounds/mapa_fase1.png`; idem |

`load_resources()` abre os onze bitmaps, a fonte e, quando o áudio está instalado, os seis efeitos de `assets/audio/sfx/*.wav` e a música `assets/audio/music/fundo.wav`. `free_resources()` libera os onze bitmaps na ordem inversa, depois os seis efeitos e a música — os samples saem antes de `al_uninstall_audio()` — e por fim a fonte. O título do menu (`NANOBÔ`) é um bitmap montado em tempo de carregamento a partir da fonte (`make_text_bitmap`), também liberado ali. O projétil é desenhado com primitivas do Allegro, sem bitmap; com a cor neutra, os dois projéteis se distinguem pela **forma**: quadrado na Ruptura, círculo na Dissolução. Os ícones `icone_laser_roxo.png` e `icone_laser_vermelho.png` saíram do `load_resources()` e continuam no disco como histórico.

## Arte final criada

Os quatro sprites de agente (três da Fase 1 e o vírus da Fase 2), o coração, os quatro ícones de interface e o mapa do mundo são carregados pelo jogo. O bitmap do jogador é `assets/sprites/nanobo_spritesheet.png` (256×32, 8 quadros, uma direção), desenhado por região; `nanobo.png` fica como histórico. A arte antiga de fundo (`fundo_microscopico.png`) ficou como referência. Os recursos anteriores a 04/11 têm a fonte `.aseprite` editável ao lado do `.png`; os três novos (ícones neutros e vírus) foram gerados por script e ainda não têm `.aseprite` (pendência de arte). O `.png` é o arquivo que o jogo carrega.

Todos os sprites usam a mesma paleta, o mesmo tamanho de pixel e a mesma direção de luz (cima-esquerda), para manter a coesão visual.

### Agentes da Fase 1 — 32×32, PNG com transparência

| Arquivo | Agente | Cor | Forma | Pista visual do envelope |
|---|---|---|---|---|
| `assets/sprites/bacteria_gram_positiva.png` | Bactéria Gram-positiva | roxo | cacho de cocos esféricos | parede espessa e clara (2 px), sem membrana externa |
| `assets/sprites/bacteria_gram_negativa.png` | Bactéria Gram-negativa | vermelho | bacilo (bastonete) | membrana externa fina, separada do corpo |
| `assets/sprites/celula_propria.png` | Célula saudável própria | verde | célula maior e arredondada | núcleo delimitado com nucléolo e mitocôndrias |

Cada espécie pode ter **mais de um sprite** para variar o cenário: mesmo número de variantes por grupo (senão a variante vira pista), 32×32, a mesma hitbox, e a variante só aparece **depois** da camada 1 — antes dela todas as bactérias são o mesmo disco cinza. A quantidade de variantes fica para o M5.

As cores da **coloração** são **roxo para Gram+** e **vermelho para Gram−**; a célula própria é verde. Desde 03/11 (noite) o HUD **não** usa roxo/vermelho para a arma: as armas Ruptura e Dissolução têm cor neutra, e ícone e projétil se distinguem pela forma. O coração continua carmim. Desde 04/11 os ícones neutros (parede/anéis) são os que o HUD desenha.

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

### Mapa da Fase 1

| Arquivo | Conteúdo |
|---|---|
| `assets/backgrounds/mapa_fase1.png` | Mapa do mundo inteiro, **1920×512** (60×16 tiles de 32 px), opaco |
| `assets/backgrounds/mapa_fase1.aseprite` | Fonte editável, cinco camadas: `fundo`, `tecido`, `corredores`, `bordas`, `marcos` |

O mapa cobre exatamente `WORLD_W`×`WORLD_H` e desenha a estrutura que o gameplay já usa, sem tocar nas regras: dois corredores horizontais de largura variável, a veia central, o atalho da esquerda, a clareira de nascimento em `(960, 256)` com a doca tracejada e o poro do destino em `(1856, 448)`, com a única luz quente do cenário. O tecido é **decoração**: não existe colisão, o Nanobô anda por cima de tudo.

O mapa foi desenhado pelo MCP do Aseprite em cinco scripts (um por camada), para poder ser regerado ou ajustado por script.

### Fundo do cenário (arte antiga)

| Arquivo | Conteúdo |
|---|---|
| `assets/backgrounds/fundo_microscopico.png` | Ambiente microscópico, **640×480** — arte antiga, fora do tamanho da tela atual (640×360) e fora do mundo 1920×512; fica no repositório como referência |
| `assets/backgrounds/fundo_microscopico.aseprite` | Fonte editável |

O fundo é escuro e de baixo contraste de propósito: os sprites precisam continuar legíveis por cima dele.

### Ícones de interface — 16×16, PNG com transparência

| Arquivo | Uso sugerido |
|---|---|
| `assets/sprites/icone_laser_roxo.png` | Histórico: laser da Gram+ (#16), substituído pelo ícone neutro da Ruptura |
| `assets/sprites/icone_laser_vermelho.png` | Histórico: laser da Gram− (#16), substituído pelo ícone neutro da Dissolução |
| `assets/sprites/icone_scanner.png` | Lupa, para indicar a ação de escanear |
| `assets/sprites/icone_coracao.png` | Vida, para o HUD |

> O coração é carmim, um vermelho mais frio que o da Gram−, para não competir com o laser vermelho no HUD.

## Recursos previstos

| Tipo | Recursos necessários | Local definido | Estado |
|---|---|---|---|
| Sprites e animações | Arte final dos agentes, **sprite neutro das bactérias** e spritesheet animado do Nanobô | `assets/sprites/` | Agentes carregados; spritesheet animado carregado e desenhado (#30); o disco cinza até a camada 1 é desenhado por primitiva, sem asset novo |
| Cenários | Mapa do mundo da Fase 1, 1920×512 | `assets/backgrounds/` | Criado e carregado (`ca4845a`) |
| Ícones de interface | Armas (Ruptura/Dissolução), scanner, vida e defesa | `assets/sprites/` | Scanner, vida, defesa e as duas armas prontos; o HUD desenha `icone_ruptura.png` e `icone_dissolucao.png` |
| Fontes | Tipografia final da interface | `assets/fonts/` | [PENDÊNCIA] Fonte ainda não escolhida; a fonte embutida atual é provisória |
| Música | Trilha da partida | `assets/audio/music/` | Presente: `fundo.wav`, 16 s em loop (705 644 B), WAV PCM 22 050 Hz mono 16 bits, carregado por `load_resources()` (#32) |
| Efeitos sonoros | Feedback de ações e eventos | `assets/audio/sfx/` | Presente: 6 WAVs PCM 22 050 Hz mono 16 bits (4 a 11,5 KB) — `scanner`, `disparo`, `impacto`, `neutralizacao`, `penalidade` e `defesa` — gerados por `../Nanobo-testes/gerar_audio.py` e carregados por `load_resources()` (#31) |
| Dados auxiliares | Configuração ou dados externos, caso necessários | `assets/data/` | [PENDÊNCIA] Nenhum arquivo necessário identificado até o momento |

## Observações para o carregador

- Carregar apenas recursos que tenham sido adicionados e sejam usados pelo jogo.
- Centralizar carregamento e liberação conforme #109 e #112.
- Não tratar diretórios `.gitkeep` como arquivos de recurso.
- Os arquivos `.aseprite` são fontes de edição, **não** recursos de execução: o carregador deve ler os `.png`.
- Atualizar este catálogo quando novos arquivos forem escolhidos ou adicionados.

## Verificação

O roteiro reproduzível do carregamento e da liberação está em `docs/planejamento/qa-recursos.md`.
