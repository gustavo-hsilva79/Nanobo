# Proposta de divisão de Issues — Nanobô

**Responsáveis propostos:** Gustavo Herick Silva (`gustavo-hsilva79`) e Eric Brito (`EricBrito-bcc`)  
**Status:** proposta para revisão da dupla; este documento não significa que as atribuições já foram aplicadas no GitHub.

## 1. Objetivo

Distribuir o trabalho do Nanobô de maneira equilibrada e sustentável, considerando esforço estimado, dependências técnicas, coesão entre tarefas e oportunidade de revisão cruzada. A meta não é fazer os dois integrantes trabalharem em áreas isoladas, mas estabelecer responsáveis principais claros e manter integração frequente.

## 2. Princípios da divisão

1. **Equilíbrio por esforço, não apenas por contagem.** Uma Issue grande pode exigir muito mais trabalho que várias pequenas. Story Points são estimativas relativas, não horas garantidas.
2. **Coesão técnica.** Tarefas relacionadas ficam preferencialmente com o mesmo responsável para reduzir mudanças simultâneas nos mesmos módulos e facilitar a manutenção.
3. **Dependências explícitas.** Issues bloqueadas não devem ser iniciadas como se fossem independentes. A sequência real deve seguir o Kanban e os milestones.
4. **Responsável não significa execução isolada.** O outro integrante pode revisar, testar e ajudar a resolver integração.
5. **Revisão cruzada.** Mudanças de gameplay devem ser revisadas por Eric; mudanças de interface, estados e integração devem ser revisadas por Gustavo, sempre que viável.
6. **Validação antes de fechar.** Compilar não basta: cada Issue precisa cumprir seus critérios de aceite e ter teste reproduzível quando aplicável.
7. **Épicos e Issues pai.** Se forem apenas agrupadores, não somar seus pontos novamente aos das sub-Issues. A estimativa do esforço deve evitar dupla contagem.
8. **Prioridade aos marcos.** O trabalho deve ser ordenado conforme os milestones oficiais e os riscos da PoC/MVP, e não apenas pela preferência individual.

## 3. Divisão proposta

> A distribuição abaixo é uma proposta inicial baseada no inventário consultado anteriormente. Antes de aplicar atribuições, conferir no GitHub o estado atual, as Issues abertas/fechadas, as estimativas, os vínculos pai/sub-Issue e os milestones. Os títulos resumidos não substituem a descrição de cada Issue.

### Gustavo Herick Silva — responsável principal por gameplay e mecânicas

**Gameplay e Fase 1**
- #10 — colisão entre Nanobô e agentes
- #11 — vida, dano e invulnerabilidade
- #13 — comportamento da bactéria
- #14 — comportamento da célula saudável
- #16 — ataque, raios e cooldown
- #17 — defesa e cooldown
- #18 — integração de identificação, pontuação e consequências
- #22 — personagem com placeholder
- #24 — composição dos agentes da Fase 1
- #25 — posicionamento semi-randomizado
- #26 — destino/conclusão da Fase 1
- #33 — estrutura para múltiplas fases
- #49 — colisão do projétil e resistência dos agentes
- #62–#67 — seleção de raio, projétil, cooldown, identificação única, pontuação e bônus

**Fase 2 — vírus**
- #55 e suas sub-Issues relacionadas à implementação da Fase 2
- #83–#91 — regras e estados virais, ciclo lisogênico/lítico, multiplicação limitada, scanner, combate, configuração, progressão e validação

**Tutorial**
- #58 e #103–#107 — introdução, demonstração guiada, tentativa com raio inadequado e integração/validação do tutorial

**Hi-score**
- #34 e #77–#82 — formato do arquivo, leitura, ordenação, gravação, entrada de nome e validação da persistência

**Recursos**
- #30 — spritesheet/animação
- #31 — efeitos sonoros
- #32 — música
- #35 — liberação centralizada de recursos

### Eric Brito (`EricBrito-bcc`) — responsável principal por interface, integração e qualidade

**Interface e fluxo**
- #7 — máquina de estados
- #19 — HUD
- #20 — integração do fluxo de telas/fases
- #50–#53 — menu, pausa, vitória e Game Over
- #57 — resultados entre fases
- #68–#76 — transições, reinício e validação do fluxo

**Memória**
- #36 e #114–#119 — cenário de referência, medições, investigação de vazamentos e evidências de uso de memória

**Regressão**
- #37 e #127–#133 — testes de estados, gameplay, scanner, combate, pontuação, áudio e consolidação dos resultados

**Teste com pessoa externa**
- #38 e #134–#138 — protocolo, preparação, observação, compreensão educacional e consolidação do feedback

**Estabilização**
- #39 e #139–#143 — inventário de bugs, reprodução, priorização, correções críticas e retestes

**Documentação e entrega**
- #40 e #144–#146 — documentação técnica, arquitetura, dependências, build e execução Windows
- #41 — revisão do One Sheet e do GDD
- #42 — build Release
- #43 — instalador Windows
- #44 — materiais de apresentação
- #46 — validação em máquina limpa
- #47 — ensaio da apresentação

**Verificação independente da PoC**
- #181 — confirmar manualmente a movimentação por teclado e registrar o resultado em `docs/planejamento/qa-poc.md`

## 4. Por que essa organização é uma boa escolha

Nenhuma divisão pode ser demonstrada como “a melhor possível” sem conhecer com precisão a disponibilidade, experiência, velocidade real e preferências de ambos. Esta proposta é, porém, uma opção justificável pelos seguintes motivos:

### 4.1 Reduz o custo de coordenação

O conjunto de mecânicas, agentes, scanner, ataque, identificação e pontuação tem dependências fortes. Concentrar a responsabilidade principal por esse conjunto com Gustavo reduz a quantidade de transferências de contexto e de alterações concorrentes nos mesmos comportamentos.

Da mesma forma, estados, telas, HUD, fluxo, validação e preparação da entrega têm interfaces entre si. Manter Eric como responsável principal por esse eixo torna mais claro quem integra e valida o fluxo completo.

### 4.2 Equilibra o esforço estimado

A proposta anterior apontou cerca de **143 Story Points para Gustavo e 145 para Eric**, diferença de apenas 2 pontos (aproximadamente 1,4% da média dos dois totais). Isso sugere equilíbrio nas estimativas disponíveis, mas não prova igualdade de horas ou dificuldade real.

**Condição importante:** esses números só são confiáveis se forem recalculados a partir do estado atual e se Issues pai não forem contadas junto com as estimativas das respectivas sub-Issues. A Issue #181 não tinha estimativa identificada e deve ser estimada ou tratada separadamente.

### 4.3 Dá a cada integrante uma área de responsabilidade compreensível

Cada integrante consegue explicar quais módulos lidera, quais resultados precisa entregar e quais interfaces deve combinar com o colega. Isso ajuda na organização do Kanban e na prestação de contas do trabalho, sem impedir ajuda mútua.

### 4.4 Mantém a qualidade como responsabilidade compartilhada

Eric não deve ser o único a testar o jogo, nem Gustavo deve ser o único a verificar as mecânicas que implementou. A divisão separa a responsabilidade principal, mas a revisão cruzada reduz o risco de o autor deixar passar um erro de interpretação ou integração.

### 4.5 Favorece a validação dos riscos técnicos

A implementação do núcleo de gameplay e a verificação de estados, controles, memória e regressão ficam claramente atribuídas. Isso permite descobrir cedo problemas que afetariam a PoC ou o MVP, em vez de deixar todos os testes para o final.

### 4.6 Evita que a contagem bruta de Issues engane

Uma pessoa pode receber mais Issues pequenas e outra menos Issues grandes. O equilíbrio deve ser avaliado por estimativa, dependências, risco e trabalho concluído, e não por igualdade numérica artificial.

## 5. Limitações e riscos da proposta

- **Inventário precisa ser atualizado:** Issues podem ter sido fechadas, reabertas, renumeradas em novos contextos ou alteradas desde a consulta anterior.
- **Story Points são relativos:** não equivalem a horas e podem refletir entendimentos diferentes entre os integrantes.
- **Possível dupla contagem:** Issues pai/épicos não devem ter seus pontos somados novamente quando apenas agrupam subtarefas estimadas.
- **Possível desequilíbrio por etapa:** mesmo com totais próximos, um milestone pode concentrar mais trabalho de um integrante. Reavaliar a cada milestone.
- **Dependências entre módulos:** gameplay, HUD, estados e progressão precisam de contratos claros e integração frequente.
- **Teste e documentação não são trabalho residual:** devem ter critérios de aceite, tempo reservado e evidências registradas.

## 6. Mudanças a fazer no GitHub

A aplicação deve ser feita somente depois de validar a lista de Issues atual.

- [ ] Confirmar os logins `gustavo-hsilva79` e `EricBrito-bcc` e a permissão de ambos no repositório.
- [ ] Listar Issues abertas e fechar/retirar da distribuição as já concluídas.
- [ ] Conferir os vínculos entre Issues pai e sub-Issues.
- [ ] Conferir Story Points; estimar a #181 ou mantê-la explicitamente sem estimativa.
- [ ] Recalcular o esforço por responsável sem dupla contagem.
- [ ] Aplicar os responsáveis às Issues executáveis, preservando responsáveis existentes quando houver motivo documentado para isso.
- [ ] Evitar atribuir automaticamente todas as subtarefas e o épico à mesma pessoa se o épico for apenas agrupador.
- [ ] Confirmar que cada Issue está no milestone correto, especialmente as tarefas prioritárias da PoC/MVP.
- [ ] Registrar dependências e critérios de aceite que estejam ausentes.
- [ ] Conferir o Kanban depois das atribuições e garantir que a prioridade venha dos milestones e bloqueios reais.
- [ ] Revisar a distribuição ao final de cada milestone, transferindo tarefas se houver bloqueios ou desequilíbrio comprovado.

## 7. Acordo de trabalho sugerido

1. Cada responsável abre uma branch para uma tarefa independente e faz commits pequenos.
2. Issues que alterem interfaces compartilhadas devem ter o contrato combinado antes da implementação.
3. Ao terminar, o responsável registra como testar e quais critérios de aceite foram cumpridos.
4. O colega revisa o código ou reproduz o teste antes do fechamento, quando a tarefa justificar revisão.
5. Bugs encontrados durante a integração viram Issues ou comentários vinculados à Issue original; não devem ser ocultados para manter o quadro aparentemente limpo.
6. A dupla reavalia a carga semanalmente e em cada marco, usando o trabalho restante e os bloqueios reais.

## 8. Critério para considerar a divisão validada

A divisão será considerada validada quando:
- as Issues ativas tiverem responsáveis claros;
- o esforço tiver sido recalculado sem dupla contagem;
- os bloqueios e milestones estiverem coerentes;
- ambos concordarem com a distribuição;
- a carga real, observada após um ciclo de trabalho, não indicar desequilíbrio relevante;
- as tarefas continuarem sendo revisadas e testadas em conjunto.

**Conclusão:** esta é uma proposta sólida por coesão técnica, clareza de responsabilidade e estimativas próximas. Não é possível afirmar honestamente que seja a melhor divisão absoluta sem medir a disponibilidade e a experiência de cada integrante e observar o desempenho real. A melhor decisão prática é adotá-la como hipótese inicial, validar o inventário e ajustá-la com evidências durante o desenvolvimento.
