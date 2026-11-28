# Nanobô — Regressão final (#37, #127 a #133)

Data: 28/11/2026, madrugada. Build: `master` em `6787f3f` (contenção, selos, toxina com antecipação), o mesmo empacotado no `dist\Nanobo-1.0.0.msi` regerado nesta data (ver `release.md`, "Atualização de 28/11").

Este registro reúne o que foi verificado de forma automática ou roteirizada nesta máquina. **Não substitui o teste manual da dupla**: o que exige ouvir, mirar ou julgar a sensação está listado em "O que esta regressão não cobre", no fim.

## 1. Ambiente e portões

| Portão | Resultado |
|---|---|
| `Release\|x64` (Rebuild pelo `montar_distribuicao.ps1`) | 0 avisos, 0 erros |
| `Debug\|x64` (Rebuild) | 0 avisos de compilador; só o MSB8028 das cópias de conflito do OneDrive em `Nanobo\x64\Debug\` (ambiental, conhecido) |
| Harness de regras (`../Nanobo-testes/build_harness.sh`) | **606 passaram, 0 falharam** |
| Bots da Fase 1 (`bots 1000`) | leitor vence 67%, chute 4% (meta: 55–75% e ≤ 10%); 2 tempos esgotados do leitor, 0,0 resistente sobrando |
| Bots da Fase 2 (`bots2 1000 77` / `60`) | leitor 100% / 98%, chute 7% / 0%; 0 derrotas pela integridade |
| Registro de textos (`conferir_textos.py`) | 0 literais sem linha, 0 linhas sem texto |
| Instalador (`gerar_instalador.ps1`) | `.vdproj` e `dist\Nanobo` conferem (55 arquivos); `.msi` gerado sem `ERROR`/`WARNING` |

Os roteiros de tela usaram `../Nanobo-testes/qa_jogo.ps1` (teclas e cliques sintéticos, capturas da janela) sobre o `x64\Debug\Nanobo.exe`, que tem as teclas de QA (`F` conclui a fase, `G` força o game over). A janela, a tela cheia e a memória usaram o `x64\Release\Nanobo.exe`. O `nanobo_ranking.txt` foi copiado antes e restaurado depois (a Vitória do roteiro gravou uma entrada de teste).

## 2. Menu, estados e reinício (#127)

| Critério | Resultado | Evidência |
|---|---|---|
| Menu inicia partida | ok: `ENTER` e clique em INICIAR levam ao briefing; `ESC` pula para a Fase 1 | roteiros A2 e C |
| Pausa congela e a retomada preserva o estado | ok: relógio em 00:02 na pausa, 00:02 depois de abrir e fechar o "Como jogar", 00:04 depois de 1,5 s de jogo retomado | roteiro C, capturas 02–05 |
| Resultados, Vitória e Game Over alcançáveis | ok: Resultados das três fases, Vitória com Top 10 e Game Over (tecla `G`) | roteiros A2 e C |
| Nova partida reinicia o estado transitório | ok: depois da Vitória e depois do Game Over, a Fase 1 abre com 0 pontos, 0/6 ameaças, 0/9 identificados e barras cheias | A2 captura 10, C captura 08 |
| Telas desenham sem texto cortado ou sobreposto | ok nas capturas de menu, briefing, Fase 1/2/3 com aviso de objetivo e protocolo, pausa, "Como jogar" (4 páginas), Recordes, Resultados ×3, Vitória com entrada de nome, Game Over | sheets em `.hermes/regressao-2026-11-28/` |
| Hover e foco de teclado distintos | ok: foco = borda âmbar; hover = fundo preenchido (menu com o cursor sobre COMO JOGAR e o foco em INICIAR) | primeira captura do menu |
| "Como jogar" abre do menu e da pausa e volta à origem | ok: do menu volta ao menu; da pausa volta à pausa com o jogo congelado | roteiros B e C |
| Interface íntegra ao redimensionar ao vivo | ok com a área cliente em 2560×1411 (inicial), 1278×709, 1938×1069 e 640×360, no menu e na Fase 1 | roteiro D |
| Janela não desce abaixo de 640×360 | ok: pedido de 300×200 resultou em área cliente de **640×360** | roteiro D |
| `F11` ida e volta | ok: 2560×1440 em tela cheia, volta para janela com a interface íntegra e o foco preservado | roteiro D, capturas 01–03 |
| HUD legível | ok nas três fases (integridade, vitais, pontos, tempo, arma, recarga, ameaças, identificados) | capturas de fase |

**Escala 3× e 4×:** com a tela de 1280×720 (27/11), o 3× e o 4× da base antiga de 640×360 correspondem a 1,5× e 2×, que foram cobertos (1938×1069 e 2560×1440). Um monitor 4K não estava disponível.

## 3. Gameplay e progressão (#128)

| Critério | Resultado |
|---|---|
| Partida inicia e permanece jogável | ok nas três fases |
| Progressão na ordem prevista | ok: Fase 1 (sangue) → Fase 2 (vias aéreas, com a narrativa "Enquanto isso, no hospital") → Fase 3 (pulmão, com a narrativa e a Anotação 1) → Vitória |
| Pontuação preservada entre fases | ok: 110 ao fim da Fase 1, 190 ao fim da Fase 2 e 300 ao fim da Fase 3; 300 na Vitória |
| Objetivos e selos | ok: o aviso de cada fase mostra o objetivo e os três selos da fase; os Resultados mostram o bloco "Protocolo da fase" |
| Conclusão e Game Over | ok |
| Regras de conclusão (contenção das resistentes, metas por fase) | cobertas pelo harness; a tecla `F` força a conclusão e não exercita a regra |

## 4. Combate, colisões e defesa (#130)

Não houve combate jogado por pessoa nesta regressão. As regras são cobertas pelo harness, que passou inteiro: 10 casos de recarga, 4 de invulnerabilidade, 14 de defesa, 24 de dose, 32 de resistência (inclusive "contato na invulnerabilidade não tira integridade de novo", "defesa bloqueia sem gastar recarga", "defesa impede o dano" e a nova toxina com antecipação). Os bots das Fases 1 e 2 jogam com essas regras e ficaram nas metas.

## 5. Pontuação, bônus e Top 10 (#131)

- **Harness:** 39 casos de pontos e bônus (+10 por ameaça inicial, 0 por filha e broto, +15 com tudo identificado, bônus dos vitais, isolamento sem pontos) e 13 de ranking (10 posições, empate mantém o mais antigo em cima, `.tmp`/`.bak`).
- **Tela:** a Vitória pediu o nome ("novo recorde no Top 10!"), gravou 300 na posição 2, destacou a linha e mostrou "posição 2 no Top 10"; o painel RECORDES do menu e o "recorde 377 · Amaciante" do menu continuaram certos; a nova partida começou com 0 pontos sem apagar o ranking.
- **Bônus de identificação:** "não obtido" nas três fases do roteiro (0 lidos), como esperado.

## 6. Áudio e recursos (#132)

| Critério | Resultado |
|---|---|
| Recursos carregam | ok: o jogo só abre se todos os PNG e WAV carregarem (`load_resources()` sai com código 1 se faltar um); abriu em Debug, em Release e na pasta instalada pelo `.msi` |
| Troca de mapa por fase | ok: os três mapas apareceram em sequência sem travamento |
| Encerramento libera sem erro | ok: fechar pelo X (`WM_CLOSE`) encerrou o Release com **código 0** |
| Memória | pico de **90,01 MB** de working set no Release (meta ≤ 100 MB), última amostra 82,16 MB; 69 amostras em 40 s com menu, tela cheia, redimensionamento e Fase 1 |
| Música e SFX | **não verificado de ouvido** (ver §8) |

## 7. Falhas e observações

Nenhuma falha bloqueante. Registros:

1. **Projeto de instalação desatualizado (corrigido).** O `.vdproj` ainda listava `allegro_ttf-5.2.dll`, `icone_coracao.png` e `icone_defesa.png`, que o jogo não usa mais, e não tinha os 12 PNG do redesenho da interface de 28/11. O `gerar_instalador.ps1` recusou o build, como deve. O `.vdproj` foi atualizado (3 entradas removidas, 12 incluídas) e o instalador foi regerado.
2. **`F11` volta 4 px menor (cosmético).** A janela abriu maximizada em 2560×1411 e, depois de `F11` ida e volta, ficou em 2556×1407. A interface continua íntegra e responde.
3. **Glifo do `ê` apertado (anterior, da fonte).** "resistência" e "lê" mostram o circunflexo colado à letra na fonte embutida do Allegro. Já era assim antes; é legível.

## 8. O que esta regressão não cobre

- **Áudio de ouvido:** música em loop e os seis SFX nos eventos certos. Pede uma pessoa com som ligado (é da #132).
- **Combate jogado:** acerto e erro de arma, contato persistente e defesa sob pressão real, sentidos por quem joga. O harness prova a regra, não a sensação (#130).
- **Selo e contenção jogados até o fim:** o roteiro concluiu as fases com `F`, que não exercita a regra do isolamento das resistentes nem a perda de um selo.
- **Máquina limpa** (#162 a #164) e **teste com pessoa externa** (#38, #134 a #138): continuam pendentes.

## 9. Evidências

Fora do git, em `.hermes/regressao-2026-11-28/`: as folhas de capturas dos roteiros A2 (progressão completa), B (recordes e "Como jogar"), C (clique, pausa, Game Over e reinício) e D (janela e tela cheia), o `memoria.csv` e os logs `install.log`/`uninstall.log` do `.msi`.
