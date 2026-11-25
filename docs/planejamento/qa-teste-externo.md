# Nanobô — Protocolo do teste com pessoa externa

> **Vigente (25/11/2026, #134).** Substitui o protocolo de playtest de `qa-mvp.md` §6 e o pré/pós-teste de `ensino-e-aprendizagem.md` §"Playtest com pré e pós-teste", que são do M4 (objetivos antigos, "raio", "laser") e ficam como histórico. A issue-mãe é a #38; a preparação é a #135, a sessão é a #136, a análise é a #137 e a consolidação é a #138.

Este documento basta para conduzir o teste sem perguntar nada aos desenvolvedores: quem conduz lê os trechos entre aspas literalmente e anota na ficha da §7.

## 1. O que o teste responde

1. Uma pessoa que não participou do desenvolvimento entende **o objetivo** e **os controles** sem ajuda?
2. As informações do jogo (scanner, cards, feedback, relatório) bastam para ela **tomar as decisões** de cada objetivo de ensino?
3. Ela **sai sabendo mais** do que entrou, objetivo por objetivo (pré e pós-teste da §6)?

**Cobertura:** Fases 1 e 2, objetivos **OA1 a OA7** (`ensino-e-aprendizagem.md` §"Nanobô 2.0"). A Fase 3 e o **OA8** (fungos) ficam fora: não existem no build de 25/11. Se o build testado tiver a Fase 3, acrescente a pergunta Q8 da §6 e as linhas de OA8 da §7.

O resultado é **formativo**: com 1 a 5 pessoas, ele aponta problemas, não mede eficácia.

## 2. Participante

**Perfil mínimo**
- não participou do desenvolvimento, não jogou o Nanobô e não viu capturas nem a apresentação;
- tem a biologia do ensino médio (cursando ou concluída) — é o público do jogo;
- joga com teclado e mouse.

**Registre sempre**, sem nome (use P1, P2…): faixa de idade, se joga videogame com frequência, se já estudou microbiologia além do ensino médio e **se é colega da turma do SENAC**. Colega da turma é um desvio aceito em 25/11: a pessoa conhece jogos parecidos e o resultado vale menos como "sem explicação prévia" (#136). Anote isso no cabeçalho da sessão.

**Consentimento (verbal):** "Você topa jogar uns 20 minutos e responder umas perguntas? Não vou gravar seu nome; anoto o que acontece no jogo e algumas frases suas. Pode parar quando quiser." Sem o sim, não há teste.

## 3. Papéis e materiais

- **Condutor:** lê o roteiro, aplica as perguntas e não ajuda.
- **Observador:** preenche a ficha da §7. Se houver só uma pessoa da dupla, o condutor também observa e anota depois de cada fase, durante a tela de resultados.
- **Materiais:** este documento aberto em outra tela ou impresso, papel ou arquivo para a ficha, relógio com cronômetro.

## 4. Preparação (antes de chamar o participante)

A #135 registra o que for usado aqui.

1. **Build:** `x64/Release/Nanobo.exe` gerado a partir de um commit conhecido. Anote o hash curto (`git log -1 --format=%h`) no cabeçalho. Nunca use o Debug: ele tem as teclas de QA (`F`, `G`, `H`, `J`, `K`, `L`).
2. **Rodar da pasta do repositório** (os recursos carregam por caminho relativo) ou da pasta do pacote, se já existir.
3. **Ranking limpo:** **mova** o `nanobo_ranking.txt` (e também `nanobo_ranking.txt.bak` e o antigo `nanobo_hiscore.txt`, se existirem) para **fora** da pasta do jogo e devolva ao fim. Não renomeie para `.bak`: o jogo lê o `.bak` quando o arquivo principal falta, e importa o `nanobo_hiscore.txt`.
4. **Som ligado** em volume confortável; feche notificações e outras janelas.
5. Abra o jogo uma vez, confira que o menu aparece, e feche. O participante começa do **menu**, com o jogo recém-aberto.
6. **`F1` desligado** (o painel de QA não pode aparecer). Não use `F11` a não ser que o participante peça.

## 5. Roteiro da sessão (~40 min)

| Etapa | Tempo | O que acontece |
|---|---|---|
| Consentimento e cabeçalho | 3 min | §2 e cabeçalho da §7 |
| Pré-teste | 5 min | perguntas da §6, por escrito, sem comentar as respostas |
| Instrução única | 1 min | o texto abaixo, e nada mais |
| Jogo | até 20 min | da tela de menu até a Vitória, o Game Over ou o fim do tempo |
| Pós-teste | 5 min | as mesmas perguntas, em outra ordem |
| Entrevista | 5 min | perguntas abertas da §6.2 |

**Instrução única (ler literalmente):**
"Este é um jogo educativo feito por nós. Jogue como quiser, do jeito que você jogaria em casa. Tente falar em voz alta o que está pensando — o que você acha que está acontecendo e o que vai tentar. Eu não vou explicar nada durante o jogo, porque quero ver o que o próprio jogo consegue ensinar. Se o jogo travar, me avise."

**Fim do jogo:** pare na Vitória, no Game Over ou aos 20 minutos, o que vier primeiro. No Game Over antes de 10 minutos, diga: "Se quiser, pode tentar de novo." Registre a segunda tentativa como tal. Na Vitória, deixe a pessoa digitar o nome que quiser no Top 10 (é parte do fluxo); não registre esse nome na ficha.

### 5.1 Regra de não indução

**Pode dizer, e só isto:**
- "Faça como achar melhor." / "O que você acha?" — resposta a qualquer pergunta sobre o jogo;
- "O que você está tentando fazer agora?" — só se a pessoa ficar **3 minutos** sem progresso (sem scan, sem tiro, sem sair do lugar); conta como **intervenção de nível 1**;
- "Pode continuar." — se ela parar esperando aprovação.

**Não pode, em nenhuma hipótese:**
- explicar controles, regras, armas, cards ou biologia;
- apontar para a tela, ler textos do jogo em voz alta ou traduzir o que está escrito;
- reagir a acerto ou erro (elogio, careta, "quase", riso);
- dizer o que vem depois ("agora vem a Fase 2").

**Intervenção técnica** (o jogo travou, perdeu o foco, sem som, janela fora da tela): resolva o problema técnico sem comentar o jogo e registre como **intervenção técnica**. Se a pessoa ficar presa **6 minutos** mesmo depois do nível 1, o condutor pode dizer **uma** dica de controle (só a tecla ou o botão, nunca a regra), registrada como **intervenção de nível 2**. Cada intervenção vai para a ficha com o minuto e a frase exata.

## 6. Pré e pós-teste

Aplique **por escrito**, sem ajuda e sem comentar. No pós-teste, use a ordem **Q7, Q3, Q5, Q1, Q6, Q2, Q4**. Diga antes: "Responda com suas palavras; 'não sei' é uma resposta válida."

| ID | Pergunta | Resposta que vale 1 ponto | OA |
|---|---|---|---|
| Q1 | Qual a diferença entre uma célula do corpo humano e uma bactéria em relação ao lugar onde fica o DNA? | A célula humana tem núcleo (que guarda o DNA); a bactéria não tem núcleo, o DNA fica solto. | OA1 |
| Q2 | Dá para saber se uma bactéria é Gram-positiva ou Gram-negativa só pela forma dela (bolinha ou bastão)? Explique. | Não: a forma não define o grupo; é preciso a coloração de Gram (roxo para Gram+, rosa para Gram-). | OA2 |
| Q3 | Por que um antibiótico que age na parede da bactéria pode não funcionar numa Gram-negativa? | A Gram- tem uma membrana externa que protege a parede e barra o antibiótico (a Gram+ tem parede grossa sem membrana externa). | OA3 |
| Q4 | O que pode acontecer se um tratamento com antibiótico é interrompido antes da hora? | As bactérias que sobraram podem ficar resistentes / a resistência aumenta; por isso o tratamento é completo e o antibiótico "de reserva" é poupado. | OA4 |
| Q5 | O que um anticorpo faz com um vírus? | Se prende ao vírus e o impede de entrar em outra célula. | OA5 |
| Q6 | Antibiótico funciona contra vírus? Por quê? | Não, porque o vírus não é uma célula: não tem parede, ribossomos nem metabolismo próprio. | OA6 |
| Q7 | Qual a diferença entre o ciclo lisogênico e o ciclo lítico de um vírus? | No lisogênico o vírus fica dentro da célula sem rompê-la; no lítico ele se multiplica, rompe a célula e libera novos vírus. | OA7 |
| Q8 *(só com a Fase 3)* | Por que um antibiótico não age num fungo? | A parede do fungo é de quitina, não de peptidoglicano; o fungo é eucarionte e o alvo é outro (ergosterol da membrana). | OA8 |

**Pontuação:** 1 = a ideia central da coluna está presente, mesmo com palavras próprias; 0 = ausente, errada ou "não sei". Na dúvida, anote "parcial" ao lado do 0 e decida na análise (#137), com a dupla. Registre **por OA**, nunca só a soma.

**Leitura:** um OA que fica em 0 no pós-teste é lacuna a investigar na #137 (o jogo mostrou? a pessoa viu? entendeu?). Um OA que já era 1 no pré-teste não mostra aprendizagem, só conhecimento prévio — anote assim.

### 6.2 Entrevista final (perguntas abertas)

Leia na ordem e anote as respostas com as palavras da pessoa:

1. "Em uma frase: qual era o seu objetivo no jogo?"
2. "Como você decidia qual arma usar?"
3. "Teve alguma coisa que você não entendeu?"
4. "Em que momento ficou chato ou confuso?"
5. "Qual decisão te fez pensar mais?"
6. "O que você aprendeu que não sabia antes?"

## 7. Ficha de observação

Copie esta ficha para a §9, uma por participante. **Só fatos:** o que a pessoa fez e disse, com o minuto. Interpretação ("ela não entendeu a coloração") fica para a #137, em outra coluna.

**Cabeçalho:** participante (P1…), data, hora de início, build (hash), condutor, observador, perfil (§2), colega da turma (sim/não), mouse e teclado (sim/não).

**Marcos (preencher o minuto, ou "não")**

| Marco | Minuto / resposta |
|---|---|
| Saiu do menu (clique ou ENTER) | |
| Leu ou pulou os cards do briefing (ESC/PULAR) | |
| Primeiro movimento | |
| Primeiro scan (botão direito) | |
| Primeira 2ª camada lida (coloração) | |
| Primeiro tiro | |
| Primeira troca de arma (1/2/3 ou roda) | |
| Usou a defesa (X) | |
| Chegou ao destino da Fase 1 | |
| Fim da Fase 1 (resultado e pontos) | |
| Fim da Fase 2 ou fim do jogo (Vitória / Game Over e qual barra zerou) | |

**Decisões por objetivo (contar ou descrever)**

| OA | O que observar | Registro |
|---|---|---|
| OA1 | Atirou em célula do paciente? Quantas vezes? Mudou depois do feedback? | |
| OA2 | Escolheu a arma pela forma (camada 1) ou esperou a coloração (camada 2)? Errou num coco Gram- ou bacilo Gram+? | |
| OA3 | Quantas doses no grupo certo e no errado? Leu o feedback do erro (parou, falou dele)? | |
| OA4 | Deixou tratamento incompleto (resistência)? Usou a Reserva (3)? Esgotou? | |
| OA5 | Usou a Neutralização em vírus livre? | |
| OA6 | Atirou antibiótico em vírus? Mudou depois do feedback? | |
| OA7 | Usou a Lise em célula infectada antes de romper? Alguma célula rompeu? | |

**Cards de conceito:** para cada card que apareceu, anote se a pessoa leu (tempo parado) ou passou direto.

**Ocorrências livres**

| Minuto | Fase | O que fez / disse (literal) | Intervenção (nível ou técnica, frase dita) |
|---|---|---|---|
| | | | |

**Falas literais:** pelo menos 3, entre aspas, sem identificar a pessoa.

**Pré/pós por OA:** uma linha por OA com pré (0/1), pós (0/1) e "parcial", se houver.

## 8. Depois da sessão

1. Devolva o `nanobo_ranking.txt` à pasta do jogo (§4, passo 3) e confira que o Top 10 voltou.
2. Passe a ficha a limpo na §9 no mesmo dia.
3. **#137 (análise):** para cada OA, cruze pré/pós com as decisões observadas e responda: o jogo deu a informação? a pessoa viu? usou na decisão? Mantenha duas colunas, "observado" e "interpretação da dupla".
4. **#138 (consolidação):** classifique cada achado como **bug**, **problema de compreensão** ou **sugestão**. É **crítico** o que impede terminar uma fase, trava ou fecha o jogo, ou faz um OA ser aprendido errado por causa de informação ausente ou errada no jogo. Cada crítico ganha issue própria ou decisão explícita da dupla; sugestão do participante não vira requisito sozinha.

## 9. Registro das sessões

### Sessão 1 — preparação (#135, 25/11/2026)

- **Versão:** `x64/Release/Nanobo.exe` recompilado do zero (`-t:Rebuild`) em 25/11 a partir do commit `d1b0be5`. O código do jogo é o de `d822eab` (16/11), porque os commits seguintes só mexem em docs. Build com **0 avisos e 0 erros**; harness de regras com **334 passaram, 0 falharam**.
- **Conteúdo:** Fases 1 e 2 (OA1–OA7). Sem Fase 3.
- **Como rodar:** de dentro da pasta do repositório (os recursos carregam por caminho relativo), com duplo clique em `x64\Release\Nanobo.exe` ou pelo Explorer com a pasta do repositório como pasta de trabalho.
- **Cenário inicial:** jogo recém-aberto no menu, sem partida anterior, com o ranking fora da pasta (§4, passo 3). Hoje o menu mostra `recorde 220 · Recorde`; com o ranking fora, esse rodapé muda.
- **Conferido em 25/11:** o menu abre, `ENTER` leva ao briefing, `ESC` pula para a Fase 1, a dica `botão direito: escanear` aparece sobre o primeiro agente e o Nanobô anda com as setas.
- **Recursos:** teclado, mouse com roda e botão direito, som ligado, este documento aberto para o condutor.
- **Explicações planejadas durante o jogo:** nenhuma além da §5.1.
