# Nanobô — Prova de Conceito (M3)

> **Última revisão:** 07/10/2026.
>
> **Marco:** M3 — Prova de Conceito Técnica (entrega acadêmica de 07/10/2026).
>
> **Tarefas cobertas:** #27 (cenário mínimo), #28 (execução e registro), #120–#126 (validação), #8 (entrada de teclado), #9 (movimentação).
>
> **Critério central:** a Aula 6 pede a mecânica principal do jogo implementada e rodando no Visual Studio. A mecânica principal do Nanobô é **escanear → identificar** (ver `docs/game-design/scanner.md` e `docs/game-design/decisoes.md`).

## 1. O que a PoC prova

| # | O que precisa ficar provado | Onde está |
|---|---|---|
| 1 | Game Loop com Delta Time, sem depender da taxa de quadros | `main()`, `update_match()` |
| 2 | Máquina de estados com despacho de entrada e de render por estado | `src/core/game.c`, `render()`, `handle_key_down()` |
| 3 | Entrada de teclado e de mouse | `handle_key_down()`, `handle_key_up()`, `handle_mouse_click()` |
| 4 | Movimentação do Nanobô em quatro direções | `update_match()` |
| 5 | Scanner: alvo **válido mais próximo** dentro do alcance | `pick_target()` |
| 6 | Scanner: leitura em **2 camadas** | `use_scanner()`, `draw_scan_info()` |
| 7 | **Identificação única** por agente no encontro | `agents[i].identified`, `identified_count` |

## 2. Ambiente do teste

| Item | Valor |
|---|---|
| Sistema | Windows |
| Ferramenta | Visual Studio 18 Community, toolset v170, plataforma x64 |
| Build | `Debug\|x64` e `Release\|x64` via MSBuild sobre `Nanobo.vcxproj` |
| Executável | `x64\Debug\Nanobo.exe` |
| Janela | 640×480, fechável pela moldura |
| Execução | a partir da raiz do repositório, para que `assets/sprites/nanobo.png` resolva |

## 3. Roteiro reproduzível

1. Compilar `Debug|x64`.
2. Executar `x64\Debug\Nanobo.exe` a partir da raiz do repositório.
3. Confirmar a tela de `MENU`.
4. Pressionar `ENTER` (ou clicar em `INICIAR`) e confirmar a entrada em `GAME`.
5. Segurar `↑` por cerca de um segundo: o Nanobô deve subir e parar de subir ao soltar.
6. Repetir com `↓`, `←` e `→`, e depois com `W`, `A`, `S`, `D`.
7. Levar o Nanobô até perto de uma borda e continuar andando para o mesmo lado: ele não deve sair da tela.
8. Longe de todos os agentes, pressionar `SPACE` duas vezes: o contador deve continuar `0/3`.
9. Aproximar-se de um agente e pressionar `SPACE` **uma** vez: aparece a primeira camada e o contador continua `0/3`.
10. Pressionar `SPACE` **mais uma** vez no mesmo agente: aparece a segunda camada e o contador vai para `1/3`.
11. Pressionar `SPACE` mais duas vezes no mesmo agente: contador permanece `1/3` e nenhuma camada nova aparece.
12. Ir até um segundo agente e pressionar `SPACE` duas vezes: contador vai para `2/3`.
13. Pressionar `ESC`, aguardar alguns segundos e comparar o tempo do HUD antes e depois.
14. Pressionar `ESC` para retomar e fechar a janela pela moldura.

## 4. Resultado observado

| Passo | Resultado observado |
|---|---|
| 1 | `Debug\|x64` e `Release\|x64` compilam sem erro e sem aviso originado pelo código novo |
| 2 | o processo permanece em execução; nenhuma mensagem de erro na saída padrão |
| 3 | `MENU` desenha o título, o subtítulo, o botão `INICIAR` e a dica de interação |
| 4 | `GAME` desenha os três agentes, o Nanobô, o HUD (`identificados: 0/3`) e a dica `setas ou wasd para andar` |
| 8 | contador permanece `0/3` — ativação sem alvo dentro do alcance não consome camada |
| 10 | contador vai para `1/3` e as duas camadas aparecem no painel inferior |
| 11 | contador permanece `1/3` e nenhuma camada nova aparece — a identificação não se repete |

Os passos 3, 4, 8, 10 e 11 foram verificados por captura da área de cliente da janela durante a execução.
O passo 10 foi confirmado com quatro pulsos consecutivos no agente mais próximo do ponto de partida: o contador ficou em `1/3`, e não em `2/3` nem `4/3`.

### Verificação que não pôde ser automatizada

Os passos 5, 6 e 7 dependem de um teclado físico: nesta revisão a automação disponível injetou teclas que o Allegro não reportou como pressionadas (`handle_key_down` recebeu apenas códigos espúrios), então **a movimentação foi conferida por inspeção do código, não por execução assistida**. O roteiro da seção 3 existe justamente para que a dupla confirme esses passos com o teclado na mão antes da apresentação.

Os passos 12, 13 e 14 não foram executados nesta revisão. O funcionamento do `ESC`/tempo congelado já havia sido registrado em `qa-maquina-estados.md` §5 (passos 9–10), que continua valendo.

## 5. Escopo do que **não** foi provado

A PoC não cobre, e não deveria cobrir, o núcleo completo do MVP. Ficaram fora desta entrega:

| Não provado | Onde entra |
|---|---|
| Colisão entre Nanobô e agentes | #10 — M3.5 |
| Vida, dano periódico e invulnerabilidade | #11 — M3.5 |
| Ataque, raios, projétil, cooldown e defesa | #16, #17, #49, #62–#64 — M3.5 |
| Comportamento próprio de bactéria e célula | #13, #14 — M3.5/M4 |
| Geração e posicionamento dos agentes | #24, #25 — M3.5 |
| Condições reais de fim de fase, vitória e derrota | #18, #20, #26 — M4 |
| Pontuação, bônus e hi-score | #66, #67, #34 — M4/M5 |

No código atual, `RESULTS`, `VICTORY` e `GAME OVER` ainda são alcançados por **teclas de teste** (`F`, `G` e `V`), não por condições de jogo. Isso é limitação conhecida e está registrada em `qa-maquina-estados.md` §6.

O combate foi deliberadamente deixado fora do escopo do M3 porque o cronograma da disciplina concentra os riscos técnicos da PoC na investigação (scanner e identificação), e não no combate. O trabalho restante foi remanejado para o milestone **M3.5 — Construção do Core Gameplay** (21/10/2026).

## 6. Limitações e pendências

- A movimentação precisa da confirmação manual da dupla (seção 4).
- Não há suíte de testes automatizados no projeto, e nenhuma foi criada nesta revisão.
- A leitura por camadas não tem cooldown; o valor definitivo segue **[PENDÊNCIA]** em `docs/game-design/scanner.md`.
- O diâmetro desenhado do sprite (32 px) e o limite de borda usado na movimentação (16 px) são números provisórios.

## 7. Falhas encontradas

Nenhuma falha reproduzível foi encontrada nos itens efetivamente verificados. A ausência de verificação automatizada da movimentação está registrada na seção 4 e não é, por si só, uma falha do produto.
