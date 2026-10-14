# Nanobô — Validação do carregamento e da liberação dos recursos (#113)

> **Última revisão:** 07/10/2026.
>
> **Tarefas cobertas:** #109 (carregamento centralizado), #110 (tratamento de falha), #111 (reutilização), #112 (liberação no encerramento).
>
> **Inventário de recursos:** `assets/catalogo-recursos.md` (#108).

## 1. Recursos carregados hoje

| Recurso | Origem | Carregado por |
|---|---|---|
| Fonte embutida do Allegro | criada em memória, sem arquivo | `load_resources()` |
| Sprite do Nanobô (`assets/sprites/nanobo.png`, 32×32) | `assets/sprites/` | `load_resources()` |

Não há áudio, fundo ou fonte de arquivo em uso nesta revisão. Os recursos previstos seguem marcados como **[PENDÊNCIA]** no catálogo.

## 2. Ponto central de carregamento

- `load_resources()` é chamado **uma única vez**, no `main()`, entre a criação da janela e o início do loop de eventos.
- Nenhuma função de carregamento é chamada dentro do loop por frame.
- Os recursos ficam em variáveis de arquivo (`font` e `nanobo`) e são usados por todos os estados que desenham.
- Cada recurso é carregado uma única vez por execução.

## 3. Tratamento de falha

- Cada recurso é verificado logo após o carregamento.
- Se um recurso falhar, `load_resources()` devolve `false`, imprime uma mensagem identificando o recurso e o `main()` **não prossegue**: libera o que já havia sido carregado, destrói janela, timer e fila de eventos e encerra com código 1.
- Um recurso que falhou nunca é usado por nenhum caminho de desenho, porque o loop não chega a começar.

## 4. Liberação

- `free_resources()` é o único ponto de liberação.
- A ordem é a inversa do carregamento: bitmap antes da fonte, e ambos antes da janela.
- Cada recurso é zerado para `NULL` depois de liberado, o que impede uma segunda liberação.
- `free_resources()` também é chamado no caminho de falha do carregamento, para liberar o recurso que já tinha sido carregado com sucesso.

## 5. Roteiro reproduzível

1. Compilar `Debug|x64`.
2. Executar `x64\Debug\Nanobo.exe` **a partir da raiz do repositório** e confirmar que o jogo abre e desenha.
3. Executar o mesmo executável **a partir de um diretório que não contenha `assets/sprites/nanobo.png`**; ler a saída e o código de saída do processo.
4. Dentro do jogo, percorrer `MENU → GAME → PAUSE → GAME → RESULTS → VICTORY → GAME` e observar que os recursos continuam disponíveis em todos os estados.
5. Encerrar pela moldura da janela e repetir a execução algumas vezes.

## 6. Resultado observado

| Passo | Resultado observado |
|---|---|
| 1 | `Debug\|x64` e `Release\|x64` compilam sem erro |
| 2 | o processo permanece em execução; nenhuma mensagem de erro na saída padrão |
| 3 | saída `couldn't load nanobo` e código de saída `1`; o processo não abre partida nem usa sprite inválido |
| 4 | as trocas de estado não produzem nova mensagem de carregamento e o desenho continua correto em todos os estados |
| 5 | o encerramento não gera mensagem de erro relacionada a descarte de recurso |

O passo 2 foi verificado executando o jogo por alguns segundos a partir da raiz do repositório. O passo 3 foi verificado executando o mesmo binário de um diretório temporário sem a pasta `assets`. Os passos 4 e 5 foram verificados por execução e por inspeção do código: o carregamento só aparece em `load_resources()`, chamado fora do loop, e a liberação só aparece em `free_resources()`.

Não foi feita medição de memória nesta revisão. O limite de memória é validado na #36/#114–#119 e não é presumido aqui a partir do tamanho dos arquivos.

## 7. Pendências

- Medição reproduzível de memória durante o ciclo completo.
- Recursos de áudio, fundo e fonte de arquivo, quando existirem.
- Repetição do teste depois que a #112 for exercitada por mais caminhos de encerramento.
