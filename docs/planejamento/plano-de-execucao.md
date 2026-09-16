# Nanobô — Plano de Execução

## Estratégia

Desenvolver o núcleo jogável primeiro, validando os maiores riscos técnicos antes de investir em conteúdo e polimento.

## Ordem de desenvolvimento

1. **Base técnica:** Game Loop, inicialização e Delta Time.
2. **Estados e entrada:** máquina de estados, teclado e mouse.
3. **Movimentação e entidades:** Nanobô, agentes, colisões e geração dos agentes.
4. **Scanner e aprendizagem:** seleção do alvo válido mais próximo, pulsos/camadas, identificação única e apresentação das informações.
5. **Decisão e combate:** interpretação, ataque, defesa, vida, dano, cooldowns e consequências.
6. **Fase 1 completa:** fluxo de encontro, pontuação, progressão, vitória e derrota.
7. **MVP jogável:** HUD, menu, pausa, reinício, Game Over e integração das telas.
8. **Pós-MVP:** segunda fase com vírus, ciclos lítico/lisogênico e multiplicação limitada.
9. **Release:** spritesheet/animação, áudio, hi-score, liberação de recursos, testes, memória, build e instalador Windows.

## Estratégia da PoC

A PoC deve provar os riscos técnicos e de gameplay mais relevantes antes da expansão do conteúdo. Priorizar:

- Game Loop e Delta Time;
- estados/cenas;
- entrada por teclado e mouse;
- movimentação e colisões;
- scanner e seleção de alvo;
- identificação única;
- combate, vida e consequências;
- fluxo mínimo da Fase 1.

A PoC não deve ser tratada como uma versão final reduzida; seu objetivo é reduzir incerteza técnica e validar o núcleo.

## Regra de execução

Cada tarefa deve ter responsável, objetivo e critério de conclusão claros. Alterações maiores devem ser desenvolvidas em branch e revisadas antes do merge.

A Issue é a unidade de trabalho executável; o documento registra contexto, decisões e planejamento.

## Critério de conclusão

Não considerar uma funcionalidade pronta apenas porque compila. A Issue deve ter seu critério de aceite comprovado por teste reproduzível, o código deve estar versionado e a alteração não deve quebrar funcionalidades já validadas.

## Referência de acompanhamento

O Kanban em `docs/planejamento/kanban.md` é a referência para o estado das tarefas e milestones.
