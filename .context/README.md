# ShowBox - Contexto MCP

Este diretório contém o **contexto semântico** do projeto ShowBox, gerenciado pelo MCP (Model Context Protocol) via servidor `ai-context`.

## 📚 Documentação (`docs/`)

Documentação técnica detalhada do projeto:

- **[project-overview.md](docs/project-overview.md)** - Visão geral completa do ShowBox, propósito, arquitetura e roadmap
- **[architecture.md](docs/architecture.md)** - Arquitetura técnica detalhada, padrões de design, componentes e fluxos
- **[data-flow.md](docs/data-flow.md)** - Pipeline de dados, entrada/saída, processamento e integrações
- **[development-workflow.md](docs/development-workflow.md)** - Processo de desenvolvimento, ferramentas e práticas
- **[glossary.md](docs/glossary.md)** - Terminologia técnica do projeto
- **[testing-strategy.md](docs/testing-strategy.md)** - Estratégia de testes e QA
- **[tooling.md](docs/tooling.md)** - Ferramentas de desenvolvimento e build
- **[security.md](docs/security.md)** - Considerações de segurança

## 🤖 Agentes (`agents/`)

Playbooks para agentes especializados de IA:

### Desenvolvimento

- **[feature-developer.md](agents/feature-developer.md)** - Desenvolvimento de novas features e widgets
- **[bug-fixer.md](agents/bug-fixer.md)** - Correção de bugs e resolução de problemas
- **[refactoring-specialist.md](agents/refactoring-specialist.md)** - Refatoração e melhoria de código

### Qualidade

- **[code-reviewer.md](agents/code-reviewer.md)** - Code review de mudanças
- **[test-writer.md](agents/test-writer.md)** - Criação de testes automatizados
- **[performance-optimizer.md](agents/performance-optimizer.md)** - Otimização de performance

### Arquitetura

- **[architect-specialist.md](agents/architect-specialist.md)** - Decisões arquiteturais
- **[frontend-specialist.md](agents/frontend-specialist.md)** - GUI Qt6 e UX (aplicável ao ShowBox)
- **[backend-specialist.md](agents/backend-specialist.md)** - Lógica de aplicação e comandos

### Operações

- **[devops-specialist.md](agents/devops-specialist.md)** - Build, empacotamento e CI/CD
- **[security-auditor.md](agents/security-auditor.md)** - Auditoria de segurança

### Documentação

- **[documentation-writer.md](agents/documentation-writer.md)** - Criação e manutenção de documentação

### Outros

- **[database-specialist.md](agents/database-specialist.md)** - N/A para ShowBox (sem banco de dados)
- **[mobile-specialist.md](agents/mobile-specialist.md)** - N/A para ShowBox (desktop)

## 📋 Planos (`plans/`)

Planos de implementação para features e mudanças maiores serão criados aqui.

## 🎯 Como Usar

### Para Desenvolvedores

1. **Começando**: Leia [`project-overview.md`](docs/project-overview.md) para entender o projeto
2. **Arquitetura**: Consulte [`architecture.md`](docs/architecture.md) antes de mudanças grandes
3. **Desenvolvimento**: Use [`agents/feature-developer.md`](agents/feature-developer.md) como guia
4. **Code Review**: Siga checklist em [`agents/code-reviewer.md`](agents/code-reviewer.md)

### Para Agentes de IA

Ao trabalhar no ShowBox:

1. **Contextualize-se** lendo a documentação relevante em `docs/`
2. **Consulte o playbook** apropriado em `agents/` para sua tarefa
3. **Siga os padrões** documentados
4. **Atualize a documentação** quando necessário

### Atualização do Contexto

```bash
# Verificar status
mcp_ai-context_checkScaffolding --repoPath /home/helton/git/showbox

# Preencher templates vazios
mcp_ai-context_fillScaffolding --repoPath /home/helton/git/showbox

# Criar novo plano
mcp_ai-context_scaffoldPlan --planName "nova-feature" --summary "..."
```

## 🔄 Manutenção

Este contexto deve ser mantido atualizado:

- **Após features**: Atualizar docs relevantes
- **Mudanças arquiteturais**: Atualizar `architecture.md` e diagramas
- **Novos padrões**: Documentar em playbooks de agentes
- **Lições aprendidas**: Adicionar ao glossário ou docs

## 📝 Contribuindo para Documentação

Veja [`agents/documentation-writer.md`](agents/documentation-writer.md) para guidelines detalhados sobre:

- Estilo de escrita
- Formatação
- Exemplos
- Diagramas

---

**Gerado**: 2026-01-12  
**MCP Server**: ai-context  
**Projeto**: ShowBox v1.0
