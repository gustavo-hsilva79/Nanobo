# Nanobo — Plano Agente Externo (T01-T12) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Executar auditoria, organização e preparação do backlog Nanobô sem implementar o jogo nem decidir Game Design pela dupla.

**Architecture:** 4 frentes sequenciais com gates de validação da dupla: P0-Desbloqueio, A-Auditoria backlog, B-Entregas acadêmicas, C-Decisões/conflitos, D-PoC/MVP + relatório T12.

**Tech Stack:** GitHub Issues / Milestones / Project, C/Allegro5 Jogo.c, KANBAN.md, .github/KANBAN.md, GDD docx

**Spec:** Mensagem do usuário Nanobô Tasks para Agente Externo T01-T12 + limites + regra escalonamento.

## Global Constraints

- Não implementar o jogo inteiro.
- Não alterar decisões de Game Design sem registrar proposta + justificativa.
- Não inventar requisitos.
- Antes de alterar Issue, verificar se ainda é pendência real.
- Não fechar Issues só porque parecem conceitualmente resolvidas.
- Não fazer merge sem validação da dupla.
- Toda alteração pequena, rastreável e explicada.
- Decisões da dupla marcadas como PENDENTE DE DECISÃO DA DUPLA.
- Priorizar organização do backlog e documentação.
- Se exigir decisão de Game Design/escopo: PARAR → registrar dúvida → alternativas → solicitar decisão.

---

### P0 Desbloqueio — Brief

Ler `.github/KANBAN.md`, `KANBAN.md`, `Jogo.c`. Tentar GitHub `gustavo-hsilva79/Nanobo`. Localizar GDD `Nanobo_Projeto_Integrador_II_Atualizado.docx` (ausente localmente). Confirmar Project URL e colunas Backlog → To Do → In Dev → In Review → Done. Produzir `p0-desbloqueio.md` em workspace SDD, sem editar Issues.

### Task A Frente A: T01+T02+T05+T11

Arquivos: Issues #1-#55 (remoto), `.github/KANBAN.md:7-86`, `KANBAN.md:1-35`.
Produz: `frente-A-auditoria.md` com tabelas Issue|problema|ação|justificativa + inconsistências Project + milestones + recomendação KANBAN raiz vs .github. Não reorganizar Project sem motivo. Não apagar KANBAN sem justificar.

### Task B Frente B: T03+T04

Verificar existência Issues One Sheet e Projeto Software via search. Propor checklist T03 (high concept, pilares, história, mecânicas, features, interface, arte, música, audiência, integrantes, controles, objetivo) e T04 (escopo, RF/RNF, entregáveis, timeline, milestones, tarefas, estimativas, responsáveis). Lacunas como PENDENTE DE DECISÃO DA DUPLA. Produzir `frente-B-entregas.md`.

### Task C Frente C: T06+T07+T08

Listar sem responsável, agrupar por área, sugerir A/B/compartilhado + dependências sem atribuir definitivo. Tabela T07 decisão|estado|fonte|próxima ação (controles, mouse, mapa, HUD, conteúdo biológico, visual, feedback educativo, efeito Defesa). Tabela T08 conflito|fontes|impacto|proposta (hi-score, música, múltiplas fases vs MVP 1 fase, identificação, ataque/defesa). Produzir `frente-C-decisoes.md`.

### Task D Frente D: T09+T10+T12

Ordenar PoC por risco/dependência (loop, estados, movimento, colisão, agente, scanner, ataque, defesa, dano/vida, feedback, objetivo). Auditar critérios aceite MVP (objetivo, esperado, dependências, conclusão, teste). Compilar `RELATORIO_T01-T12.md` rascunho com Corrigir agora / Antes PoC / Antes MVP / Pós-MVP / Decisões dupla / Riscos / Alterações realizadas / Não realizadas. Não declarar MVP concluído.
