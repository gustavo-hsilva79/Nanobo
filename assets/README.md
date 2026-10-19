# Assets

Organização dos recursos utilizados pelo Nanobô.

- `sprites/`: protagonista, agentes, animações e demais elementos visuais.
- `backgrounds/`: cenários e planos de fundo das fases.
- `audio/music/`: músicas do jogo.
- `audio/sfx/`: efeitos sonoros.
- `fonts/`: fontes utilizadas pela interface.
- `data/`: dados auxiliares e arquivos carregados pelo jogo.

## Carregados hoje

`nanobo.png`, `bacteria_gram_positiva.png`, `bacteria_gram_negativa.png`, `celula_propria.png` e `icone_coracao.png`, mais a fonte embutida do Allegro. O inventário completo, com o que ainda não é carregado, está em [catalogo-recursos.md](catalogo-recursos.md).

## Organização

Os assets devem ser adicionados conforme as funcionalidades forem implementadas. Evitar adicionar arquivos sem uso definido no jogo.

## Requisitos relacionados

A versão final deve contemplar spritesheet/animação, música e efeitos sonoros. A validação de memória deve considerar os recursos efetivamente carregados pelo jogo.
