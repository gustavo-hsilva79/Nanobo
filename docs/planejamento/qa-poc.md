# Nanobô — Prova de Conceito (M3)

> Atualização 22/10/2026: os números deste registro (6 agentes, janela 640x480) descrevem a build da época. O estado atual — 9 agentes, tela lógica 640x360 com escala inteira, mundo 1920x512 — está em docs/game-design/decisoes.md e docs/game-design/gdd.md.

> **Última revisão:** 07/10/2026.
>
> **Marco:** M3 — Prova de Conceito Técnica (entrega acadêmica de 07/10/2026).
>
> **Tarefas cobertas:** #27 (cenário mínimo), #28 (execução e registro), #120–#126 (validação), #8 (entrada de teclado), #9 (movimentação).
>
> **Critério central:** a Aula 6 pede a mecânica principal do jogo implementada e rodando no Visual Studio. A mecânica principal do Nanobô é **escanear → identificar** (ver `docs/game-design/scanner.md` e `docs/game-design/decisoes.md`).
>
> **Atualização de 17/10/2026:** este documento registra a PoC do M3 como foi validada em 07/10 (três agentes, HUD `0/3`, sem combate). O jogo de hoje tem **6 agentes** e HUD `identificados: 0/6`, mais vida, laser, defesa e célula própria; a seção 9 registra a extensão do M3.5. Os resultados da seção 4 continuam sendo os observados naquela data e não foram reescritos.

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

> A linha "Ferramenta" registra o ambiente de 07/10/2026. O `Nanobo.vcxproj` atual fixa o toolset **v143** e compila em C11; o roteiro continua válido.

## 3. Roteiro reproduzível

1. Compilar `Debug|x64`.
2. Executar `x64\Debug\Nanobo.exe` a partir da raiz do repositório.
3. Confirmar a tela de `MENU`.
4. Pressionar `ENTER` (ou clicar em `INICIAR`) e confirmar a entrada em `GAME`.
5. Segurar `↑` por cerca de um segundo: o Nanobô deve subir e parar de subir ao soltar.
6. Repetir com `↓`, `←` e `→`, e depois com `W`, `A`, `S`, `D`.
7. Levar o Nanobô até perto de uma borda e continuar andando para o mesmo lado: ele não deve sair da tela.
8. Longe de todos os agentes, pressionar `SPACE` duas vezes: o contador deve continuar `0/6`.
9. Aproximar-se de um agente e pressionar `SPACE` **uma** vez: aparece a primeira camada e o contador continua `0/6`.
10. Pressionar `SPACE` **mais uma** vez no mesmo agente: aparece a segunda camada e o contador vai para `1/6`.
11. Pressionar `SPACE` mais duas vezes no mesmo agente: contador permanece `1/6` e nenhuma camada nova aparece.
12. Ir até um segundo agente e pressionar `SPACE` duas vezes: contador vai para `2/6`.
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

Essa confirmação está aberta na **#181**, com o roteiro dos passos 5 a 7 e o critério de aceite. As capturas usadas como evidência estão anexadas lá.

Os passos 12, 13 e 14 não foram executados nesta revisão. O funcionamento do `ESC`/tempo congelado já havia sido registrado em `qa-maquina-estados.md` §5 (passos 9–10), que continua valendo.

## 5. Escopo do que **não** foi provado

A PoC não cobre, e não deveria cobrir, o núcleo completo do MVP. Ficaram fora desta entrega:

| Não provado | Onde entra |
|---|---|
| Colisão entre Nanobô e agentes | #10 — M3.5 |
| Vida, dano periódico e invulnerabilidade | #11 — M3.5 |
| Ataque, lasers, projétil, cooldown e defesa | #16, #17, #49, #62–#64 — M3.5 |
| Comportamento próprio de bactéria e célula | #13, #14 — M3.5/M4 |
| Geração e posicionamento dos agentes | #24, #25 — M3.5 |
| Condições reais de fim de fase, vitória e derrota | #18, #20, #26 — M4 |
| Pontuação, bônus e hi-score | #66, #67, #34 — M4/M5 |

No código atual, `RESULTS` e `VICTORY` ainda são alcançados por **teclas de teste** (`F` e `V`), não por condições de jogo; `GAME OVER` já é real por vida zerada (a tecla `G` continua forçando o estado). Isso é limitação conhecida e está registrada em `qa-maquina-estados.md` §6.

O combate foi deliberadamente deixado fora do escopo do M3 porque o cronograma da disciplina concentra os riscos técnicos da PoC na investigação (scanner e identificação), e não no combate. O trabalho restante foi remanejado para o milestone **M3.5 — Construção do Core Gameplay** (21/10/2026).

## 6. Limitações e pendências

- A movimentação precisa da confirmação manual da dupla — aberta na **#181** (seção 4).
- Não há suíte de testes automatizados no projeto, e nenhuma foi criada nesta revisão.
- A leitura por camadas não tem cooldown; o valor definitivo segue **[PENDÊNCIA]** em `docs/game-design/scanner.md`.
- O diâmetro desenhado do sprite (32 px) e o limite de borda usado na movimentação (16 px) são números provisórios.
- O loop usa **passo fixo de 1/60 s** (delta constante), não delta real: o rótulo “Delta Time” da §1 descreve o passo fixo. Em máquina que não sustenta 60 Hz, a simulação desacelera em vez de pular quadros.

## 7. Como reproduzir a entrega

1. Baixar `Nanobo-poc-m3.zip` na [release `poc-m3`](https://github.com/gustavo-hsilva79/Nanobo/releases/tag/poc-m3) e extrair a pasta inteira.
2. Abrir `Nanobo.exe`. A pasta `assets/` precisa ficar ao lado do executável.
3. Seguir o roteiro da seção 3.

O zip contém `Nanobo.exe`, as DLLs do Allegro 5 e `assets/sprites/nanobo.png`. Não é preciso ter Visual Studio instalado para executar.

## 8. Falhas encontradas

Nenhuma falha reproduzível foi encontrada nos itens efetivamente verificados. A ausência de verificação automatizada da movimentação está registrada na seção 4 e não é, por si só, uma falha do produto.

## 9. PoC do combate (M3.5) — defesa, ataque e cooldown

> **Adicionado em 16/10/2026.** Estende esta PoC para o escopo do M3.5. A regra da defesa está fechada em `docs/game-design/decisoes.md`; o mapa de teclas é `X` = defesa, `J` = ataque, `ESPAÇO` = scanner.

### 9.1 Pré-requisitos

1. Fechar qualquer `Nanobo.exe` aberto (o linker não regrava o executável em uso).
2. Compilar `Debug|x64` no Visual Studio e executar a partir da **raiz do repositório**, para `assets/` ser encontrada.
3. Apertar `ENTER` no menu para iniciar a partida.

### 9.2 Defesa — roteiro numerado (#17)

| # | Ação | Resultado esperado |
|---|---|---|
| 1 | Observar o Nanobô sem apertar nada | nenhum anel; a defesa está pronta |
| 2 | Apertar `X` com o Nanobô longe de agentes | anel **ciano** aparece; some (vira cinza) depois de ~2 s |
| 3 | Apertar `X` de novo durante o anel cinza | nada acontece; o anel continua cinza até o fim da recarga |
| 4 | Cronometrar do fim do anel ciano até o anel desaparecer | ~3 s de recarga |
| 5 | Deixar uma bactéria encostar durante o anel ciano | a **vida não cai** e a bactéria é **empurrada** para longe |
| 6 | Repetir o contato sem defesa | a vida cai 1 e a bactéria **não** é empurrada |
| 7 | Encostar numa célula saudável (ids 3, 5 ou 6) durante a defesa | a célula **não** é empurrada e a vida não cai |
| 8 | Pausar com `ESC` durante o anel ciano e voltar | a defesa continua de onde parou (timer congelado) |
| 9 | Deixar a vida chegar a 0 sem defesa | a partida chega a GAME OVER |
| 10 | Voltar ao menu e iniciar partida nova | nenhum estado de defesa vaza (anel ausente, vida 3) |

### 9.3 Ataque — roteiro numerado (#62, #63, #64, #49)

| # | Ação | Resultado esperado |
|---|---|---|
| 1 | Apertar `J` | um projétil sai na direção do `facing` |
| 2 | Apertar `J` duas vezes em menos de 1 s | o segundo disparo é ignorado (cooldown) |
| 3 | Acertar a bactéria com o raio da cor certa | a resistência cai; ao zerar, o agente sai da cena |
| 4 | Acertar com o raio da cor errada | nada acontece com a bactéria (regra da Fase 1) |
| 5 | Atirar no meio da defesa | nenhum projétil é criado |
| 6 | Apertar `X` com projétil em voo | o projétil continua e acerta |

### 9.4 Resultado observado

| Item | Esperado | Observado | Data | Quem verificou |
|---|---|---|---|---|
| Defesa 2 s / recarga 3 s | anel ciano 2 s, cinza 3 s | coberto por 9.5 | 17/10/2026 | harness + captura |
| Imunidade durante a defesa | vida não cai | coberto por 9.5 | 17/10/2026 | harness + captura |
| Empurrão da bactéria | agente sai de cima do Nanobô | coberto por 9.5 | 17/10/2026 | harness + captura |
| Célula não é empurrada | célula ignorada | coberto por 9.5 | 17/10/2026 | harness + captura |
| Pausa congela os timers | defesa continua ao voltar | _a preencher_ | | |
| Contato sem defesa | vida cai 1 (regra da #11) | coberto por 9.5 | 17/10/2026 | harness + captura |
| Disparo e cooldown | 1 projétil por segundo | coberto por 9.5 | 17/10/2026 | harness + captura |
| Acerto válido | resistência cai e neutraliza | coberto por 9.5 | 17/10/2026 | harness + captura |

### 9.5 Evidência automatizada e por captura (17/10/2026)

| Fonte | O que prova |
|---|---|
| Harness de regras puras (29 casos, fora do repositório) | janela de 2 s, recarga de 3 s, aperto bloqueado na recarga, imunidade, empurrão só em bactéria, célula intacta, dano da #11 preservado, 1 projétil por disparo, cooldown de 1 s, raio errado sem resistência, neutralização, projétil fora da tela |
| Captura do jogo | anel ciano durante os 2 s e cinza na recarga; HUD `raio: roxo (Gram+)`; um único projétil roxo avançando com o `facing`; segundo `J` em 130 ms sem criar outro; 1 coração aceso e 2 escurecidos depois de dois contatos |

O harness não prova desenho e a captura não prova regra com precisão de quadro: por isso os dois. A passada manual da tabela 9.4 continua sendo de quem joga — ela mede sensação, não regra.

### 9.6 Limitações desta PoC

- Sem automação: a injeção de tecla no Allegro não é confiável (registro da #181), então cada item é conferido com tecla apertada à mão.
- A janela de 2 s e a recarga de 3 s são medidas a olho e cronômetro; o anel mostra o estado, não o número.
- O empurrão depende do estado do timer do agente; se ele sortear direção nova no mesmo quadro, o deslocamento visível varia.
- As teclas de teste `F`, `G` e `V` ainda existem: `F` e `V` continuam sendo o único caminho para RESULTADOS e VITÓRIA até a #20/#26; `G` força GAME OVER, que já é real por vida zerada.

### 9.7 Célula própria (#14) — 18/10/2026

Roteiro reproduzível:

1. Iniciar a partida e localizar uma célula (ids 3, 5 ou 6).
2. Observar por alguns segundos: ela anda mais devagar que as bactérias, em direção aleatória.
3. Mirar nela e apertar `J`.
4. Confirmar que a célula continua na cena e que um coração apaga.
5. Repetir o disparo em menos de 1 s: a vida não cai de novo (invulnerabilidade).
6. Mirar de novo e confirmar que ela continua identificável pelo scanner.

Evidência automatizada (18/10/2026): harness de regras fora do repositório com 21 checagens — `CELL_SPEED 15`, deslocamento de 15 px/s contra 30 px/s da bactéria, ataque tira 1 de vida, célula permanece ativa e com resistência 0, invulnerabilidade bloqueia o segundo dano, scanner revela 2 camadas e identifica uma vez. O build `Debug|x64` compila.
