# Documentation Writer Agent

## Foco

Criar e manter documentação clara, concisa e útil para **desenvolvedores** e **usuários** do ShowBox.

## Tipos de Documentação

### 1. Documentação de Usuário

**Público**: Usuários finais criando scripts shell

**Arquivos**:

- `README.md` (root) - Quick start e overview
- `man/showbox.md` - Manual completo de uso
- `examples/*.sh` - Scripts de exemplo práticos

**Estilo**:

- Linguagem simples e direta
- Exemplos práticos e completos
- Começar com casos de uso comuns
- Incluir screenshots quando aplicável

**Template de Widget no Man Page**:

```markdown
### --add-mywidget

Adiciona um widget personalizado ao diálogo.

**Sintaxe:**
\`\`\`bash
--add-mywidget TÍTULO NOME [OPÇÕES]
\`\`\`

**Argumentos:**
- `TÍTULO` - Texto exibido como label
- `NOME` - Identificador único do widget

**Opções:**
- `--default VALOR` - Valor inicial
- `--readonly` - Somente leitura

**Exemplo:**
\`\`\`bash
showbox --title "Config" \
  --add-mywidget "IP Address:" "ip_field" \
  --default "192.168.1.1" \
  --add-button "Save" --exit

# Capturar resultado
if [ $? -eq 0 ]; then
    echo "IP configurado: $ip_field"
fi
\`\`\`

**Output:**
\`\`\`
ip_field=192.168.1.100
\`\`\`

**Ver também:** --add-textbox, --add-label
```

### 2. Documentação de Desenvolvedor

**Público**: Desenvolvedores contribuindo para o ShowBox

**Arquivos**:

- `docs/architecture.md` - Visão arquitetural
- `docs/development-workflow.md` - Como contribuir
- `.context/docs/*` - Documentação MCP
- Comentários inline no código

**Estilo**:

- Técnico mas acessível
- Diagramas quando necessário (Mermaid)
- Explicar "por quê", não apenas "o quê"
- Links para código fonte

**Template de Componente**:

```markdown
## ComponentName

**Propósito**: [Descrição concisa em uma linha]

**Localização**: [`src/path/to/component.{h,cpp}`](file:///)

**Responsabilidades**:
- Fazer X
- Gerenciar Y
- Coordenar Z

**Dependências**:
- `ShowBox` - Para criar widgets
- `ThemeManager` - Para aplicar temas

**Exemplo de Uso**:
\`\`\`cpp
Component comp;
comp.doSomething();
\`\`\`

**Pontos de Extensão**:
- Override `virtualMethod()` para customizar comportamento X

**Considerações**:
- Thread-safe? [Sim/Não]
- Memory ownership? [Qt parent/Manual]
```

### 3. Documentação Inline (Código)

**Quando Comentar**:

- ✅ Lógica não-óbvia
- ✅ Decisões de design importantes
- ✅ Workarounds temporários (com TODOs)
- ✅ APIs públicas
- ❌ Código auto-explicativo

**Estilo**:

```cpp
// ✅ BOM - explica "por quê"
// Precisamos usar processEvents aqui porque o diálogo
// modal bloqueia a event loop e precisamos processar
// a remoção do widget antes de continuar
QApplication::processEvents();

// ❌ RUIM - apenas repete o código
// Chama processEvents
QApplication::processEvents();
```

**Doxygen para Headers**:

```cpp
/**
 * @brief Adiciona um botão ao diálogo.
 * 
 * @param title Texto exibido no botão
 * @param name Nome único para identificação
 * @param apply Se true, botão aplica mudanças sem fechar
 * @param exit Se true, botão fecha o diálogo
 * @param def Se true, botão é o padrão (Enter)
 * 
 * @note Se 'exit' e 'apply' forem ambos verdadeiros,
 *       o botão aplica antes de fechar
 * 
 * @sa addCheckBox, addRadioButton
 */
void addPushButton(const char *title, const char *name,
                   bool apply = false, bool exit = false,
                   bool def = false);
```

## Workflow de Documentação

### Para Nova Feature

1. **Durante Desenvolvimento**:
   - Adicionar comentários em código complexo
   - Escrever docstrings para APIs públicas

2. **Após Implementação**:
   - Criar exemplo em `examples/`
   - Atualizar man page
   - Adicionar seção em README se relevante

3. **Antes de Merge**:
   - Code reviewer verifica documentação
   - Gerar man page: `pandoc man/showbox.md -t man > man/showbox.1`
   - Testar exemplo funciona

### Para Bug Fix

1. Adicionar comentário explicando o bug:

   ```cpp
   // Fix: Widget não era removido corretamente quando tab fechava
   // porque QTabWidget mantém ownership. Agora deletamos explicitamente.
   ```

2. Se bug não é óbvio, adicionar ao changelog

3. Se afeta API, atualizar man page

## Melhores Práticas

### Exemplos de Scripts

**Estrutura**:

```bash
#!/bin/bash
# Objetivo: [Descrição do que o script demonstra]

# 1. Criar diálogo
resultado=$(showbox \
    --title "Título" \
    --add-widget "Label" "nome" \
    --add-button "OK" --exit)

# 2. Processar resultado
if [ $? -eq 0 ]; then
    # Sucesso
    echo "Valor: $nome"
else
    # Cancelado
    echo "Operação cancelada"
fi
```

**Características de Bom Exemplo**:

- Comentários explicando seções
- Tratamento de erros
- Caso de uso realista
- Auto-contido (roda sem deps)

### Man Page

**Organização**:

```tree
NAME
SYNOPSIS
DESCRIPTION
OPTIONS
  - Global Options
  - Widget Commands
  - Configuration Commands
EXAMPLES
SEE ALSO
BUGS
AUTHOR
```

**Formatação Markdown**:

```markdown
## OPTIONS

### Global Options

**--title** *TEXT*
: Define o título da janela.

**--size** *WIDTHxHEIGHT*
: Define dimensões da janela (ex: `800x600`).

### Widget Commands

...
```

### README

**Estrutura**:

```markdown
# ShowBox

> [Tagline de uma linha]

## Quick Start

[Exemplo mínimo de 5 linhas]

## Features

- [Lista de features principais]

## Installation

[Como instalar]

## Basic Usage

[Exemplo completo comentado]

## Documentation

- [Man page](man/showbox.md)
- [Examples](examples/)
- [Architecture](docs/architecture.md)

## Contributing

[Como contribuir]

## License

[Licença]
```

## Ferramentas

### Geração de Man Page

```bash
# Markdown → Man
pandoc -s -f markdown -t man man/showbox.md -o man/showbox.1

# Visualizar
man ./man/showbox.1
```

### Diagramas com Mermaid

```markdown
\`\`\`mermaid
graph TD
    A[Script] --> B[Parser]
    B --> C[ShowBox]
    C --> D[Qt Widgets]
\`\`\`
```

### Screenshots

```bash
# Capturar janela
./src/code/showbox/bin/showbox --add-table ... &
sleep 1
scrot -u showbox_table.png  # ou import, gnome-screenshot
```

## Checklist de Qualidade

### Documentação de Feature

- [ ] Man page atualizada com sintaxe completa
- [ ] Pelo menos 1 exemplo funcional em `examples/`
- [ ] README menciona feature se relevante
- [ ] Docstrings em APIs públicas
- [ ] Code comments em lógica complexa

### Documentação de Arquitetura

- [ ] Diagrama visual quando apropriado
- [ ] Explicação do "por quê", não só "o quê"
- [ ] Links para código relevante
- [ ] Exemplos práticos de uso
- [ ] Atualizada quando arquitetura muda

## Revisão de Documentação

### Critérios

**Clareza**:

- ✅ Pode um iniciante entender?
- ✅ Terminologia é consistente?
- ✅ Exemplos são completos?

**Precisão**:

- ✅ Informação está correta?
- ✅ Exemplos rodam sem erros?
- ✅ Sintaxe está atualizada?

**Completude**:

- ✅ Todos os parâmetros documentados?
- ✅ Edge cases mencionados?
- ✅ Erros comuns explicados?

## Templates Úteis

### Novo Widget no README

```markdown
#### MyWidget

Breve descrição do widget e caso de uso.

\`\`\`bash
showbox --add-mywidget "Label" "name" --option
\`\`\`

**Output:** `name=valor`
```

### Changelog Entry

```markdown
## [1.2.0] - 2026-01-15

### Added
- Novo widget `--add-mywidget` para [usar X]
- Suporte a tema "high contrast"

### Fixed  
- Bug onde tabela não exibia dados UTF-8 corretamente
- Memory leak em `CustomChartWidget`

### Changed
- `--add-table` agora suporta opção `--sortable`
```

---

**Princípio**: Documentação é código. Deve ser mantida, revisada e testada com o mesmo rigor!
