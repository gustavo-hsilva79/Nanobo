# Catálogo de recursos do Nanobô

Inventário dos recursos necessários à execução atual, para orientar o carregador central da #54. Os recursos já presentes estão listados abaixo; pastas vazias e recursos ainda não escolhidos permanecem indicados como pendências.

## Recursos presentes e usados

| Recurso | Uso atual | Local |
|---|---|---|
| Sprite do Nanobô (32×32) | Personagem do jogador e, com tingimento por `al_draw_tinted_bitmap()`, os três agentes do encontro | `assets/sprites/nanobo.png`; carregado por `load_resources()` em `src/main.c` |
| Fonte embutida do Allegro | Todo o texto da interface: HUD, painel do scanner, menu, pausa e telas de fim | Criada em memória por `al_create_builtin_font()` em `src/main.c`; não há arquivo de fonte |

> O sprite tingido **não** substitui a arte final dos agentes: cada tipo de agente ainda precisa do seu próprio recurso, definido como **[PENDÊNCIA]** abaixo.

## Recursos previstos

| Tipo | Recursos necessários | Local definido | Estado |
|---|---|---|---|
| Sprites e animações | Arte final dos agentes (bactéria Gram+, bactéria Gram−, célula própria) e spritesheet animado do Nanobô | `assets/sprites/` | [PENDÊNCIA] Arquivos e especificações ainda não definidos |
| Cenários | Fundo/ambiente microscópico das fases | `assets/backgrounds/` | [PENDÊNCIA] Arquivos ainda não definidos |
| Fontes | Tipografia final da interface | `assets/fonts/` | [PENDÊNCIA] Fonte ainda não escolhida; a fonte embutida atual é provisória |
| Música | Trilha da partida | `assets/audio/music/` | [PENDÊNCIA] Arquivo ainda não definido |
| Efeitos sonoros | Feedback de ações e eventos | `assets/audio/sfx/` | [PENDÊNCIA] Arquivos ainda não definidos |
| Dados auxiliares | Configuração ou dados externos, caso necessários | `assets/data/` | [PENDÊNCIA] Nenhum arquivo necessário identificado até o momento |

## Observações para o carregador

- Carregar apenas recursos que tenham sido adicionados e sejam usados pelo jogo.
- Centralizar carregamento e liberação conforme #109 e #112.
- Não tratar diretórios `.gitkeep` como arquivos de recurso.
- Atualizar este catálogo quando novos arquivos forem escolhidos ou adicionados.

## Verificação

O roteiro reproduzível do carregamento e da liberação está em `docs/planejamento/qa-recursos.md`.
