# Catálogo de recursos do Nanobô

Inventário dos recursos necessários à execução atual, para orientar o carregador central da #54. O repositório ainda não contém arquivos finais de imagem, fonte ou áudio; pastas vazias e recursos ainda não escolhidos permanecem indicados como pendências.

## Recursos presentes e usados

| Recurso | Uso atual | Local |
|---|---|---|
| Fonte embutida do Allegro | Texto provisório “Hello world!” | Criada em memória por `al_create_builtin_font()` em `src/main.c`; não há arquivo de fonte |

## Recursos previstos

| Tipo | Recursos necessários | Local definido | Estado |
|---|---|---|---|
| Sprites e animações | Nanobô, bactérias, células saudáveis e, na Fase 2, vírus | `assets/sprites/` | [PENDÊNCIA] Arquivos e especificações ainda não definidos |
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
