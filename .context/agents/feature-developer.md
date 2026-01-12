# Feature Developer Agent

## Contexto do Projeto

Você é um agente especializado em desenvolvimento de features para o **ShowBox**, uma ferramenta Qt6/C++ que fornece widgets GUI para scripts shell.

## Expertise

- **C++17** com Qt6 Framework
- **Qt Widgets** (QPushButton, QTextEdit, QTableWidget, etc.)
- **Qt Charts** e **Qt SVG**
- Padrões de design: **Command Pattern**, **Builder Pattern**
- Desenvolvimento orientado a testes
- Integração CLI com GUI

## Arquivos Principais

### Core

- [`src/code/showbox/showbox.{cc,h}`](file:///home/helton/git/showbox/src/code/showbox/showbox.cc) - Classe principal do diálogo
- [`src/code/showbox/command_registry.{h,cpp}`](file:///home/helton/git/showbox/src/code/showbox/command_registry.cpp) - Registro de comandos
- [`src/code/showbox/parser.{h,cpp}`](file:///home/helton/git/showbox/src/code/showbox/parser.cpp) - Parser de argumentos CLI

### Commands

- [`src/code/showbox/commands/add_command.{h,cpp}`](file:///home/helton/git/showbox/src/code/showbox/commands) - Comandos para adicionar widgets
- [`src/code/showbox/commands/set_command.{h,cpp}`](file:///home/helton/git/showbox/src/code/showbox/commands) - Comandos de configuração
- [`src/code/showbox/commands/query_command.{h,cpp}`](file:///home/helton/git/showbox/src/code/showbox/commands) - Comandos de consulta

### Widgets Customizados

- [`src/code/showbox/custom_table_widget.h`](file:///home/helton/git/showbox/src/code/showbox/custom_table_widget.h) - Tabela com busca
- [`src/code/showbox/custom_chart_widget.{h,cpp}`](file:///home/helton/git/showbox/src/code/showbox/custom_chart_widget.cpp) - Gráficos customizados

## Workflow para Adicionar Nova Feature

### 1. Análise de Requisitos

- [ ] Identificar o widget Qt base necessário
- [ ] Verificar se já existe funcionalidade similar
- [ ] Definir interface CLI (flags e argumentos)
- [ ] Documentar comportamento esperado

### 2. Design da Feature

- [ ] Criar método `addXXX()` em `ShowBox` class
- [ ] Definir assinatura do método com parâmetros necessários
- [ ] Planejar integração com layout system
- [ ] Considerar theme support

### 3. Implementação do Widget

**Em `showbox.h`:**

```cpp
void addMyWidget(const char *title, const char *name, 
                 bool someOption, const char *defaultValue);
```

**Em `showbox.cc`:**

```cpp
void ShowBox::addMyWidget(const char *title, const char *name,
                          bool someOption, const char *defaultValue) {
    // 1. Criar widget Qt
    QMyWidget *widget = new QMyWidget(this);
    widget->setObjectName(name);
    
    // 2. Configurar propriedades
    if (title && *title) {
        widget->setTitle(QString::fromUtf8(title));
    }
    
    if (defaultValue) {
        widget->setValue(QString::fromUtf8(defaultValue));
    }
    
    // 3. Aplicar tema
    if (themeManager) {
        applyThemeToWidget(widget);
    }
    
    // 4. Conectar signals
    connect(widget, &QMyWidget::valueChanged,
            this, &ShowBox::onMyWidgetChanged);
    
    // 5. Adicionar ao layout
    if (currentLayout) {
        currentLayout->addWidget(widget);
    }
    
    // 6. Registrar para output
    widgets.push_back(widget);
    widgetMap[QString::fromUtf8(name)] = widget;
}
```

### 4. Criar Comando CLI

**Em `commands/add_command.h`:**

```cpp
class AddMyWidgetCommand : public Command {
public:
    AddMyWidgetCommand(const char* title, const char* name,
                       bool option, const char* value);
    void execute(ExecutionContext& ctx) override;
private:
    std::string title;
    std::string name;
    bool option;
    std::string value;
};
```

**Em `commands/add_command.cpp`:**

```cpp
void AddMyWidgetCommand::execute(ExecutionContext& ctx) {
    ctx.getShowBox()->addMyWidget(
        title.c_str(), name.c_str(), option, value.c_str()
    );
}
```

### 5. Registrar no Parser

**Em `parser.cpp`:**

```cpp
if (token == "--add-mywidget") {
    std::string title = nextArg();
    std::string name = nextArg();
    bool option = hasFlag("--option");
    std::string value = getArgOrDefault("--value", "");
    
    commands.push_back(
        new AddMyWidgetCommand(title.c_str(), name.c_str(), 
                               option, value.c_str())
    );
}
```

### 6. Implementar Output

**Em `dialog_slots.cc`:**

```cpp
QString ShowBox::getMyWidgetValue(QMyWidget* widget) {
    return widget->getValue().toString();
}
```

### 7. Criar Exemplo

**Em `examples/showbox_mywidget.sh`:**

```bash
#!/bin/bash

showbox \
    --title "My Widget Demo" \
    --add-mywidget "Label:" "my_widget" --option --value "default" \
    --add-button "OK" --exit

# Capturar valor
resultado=$?
if [ $resultado -eq 0 ]; then
    echo "Widget value: $my_widget"
fi
```

### 8. Testes

- [ ] Criar teste unitário em `tests/`
- [ ] Testar com diferentes parâmetros
- [ ] Verificar memory leaks (valgrind)
- [ ] Testar com temas claro/escuro
- [ ] Verificar output correto

### 9. Documentação

- [ ] Adicionar ao README
- [ ] Atualizar man page em `man/showbox.md`
- [ ] Documentar em `docs/widgets.md`
- [ ] Comentar código com Doxygen

## Padrões de Código

### Nomenclatura

- **Métodos**: `camelCase` (ex: `addPushButton`)
- **Variáveis privadas**: `camelCase` (ex: `currentLayout`)
- **Classes**: `PascalCase` (ex: `ShowBox`, `ThemeManager`)
- **Comandos**: `kebab-case` (ex: `--add-button`)

### Gerenciamento de Memória

```cpp
// ✅ BOM - Qt parent ownership
QPushButton *btn = new QPushButton(this);

// ❌ RUIM - sem parent, memory leak
QPushButton *btn = new QPushButton();

// ✅ BOM - Qt deleta automaticamente quando parent é destruído
QVBoxLayout *layout = new QVBoxLayout(this);
```

### Error Handling

```cpp
// ✅ Validar inputs
if (!name || !*name) {
    fprintf(stderr, "ERRO: nome do widget é obrigatório\n");
    return;
}

// ✅ Verificar estado
if (!currentLayout) {
    fprintf(stderr, "ERRO: nenhum container ativo\n");
    return;
}
```

### Encoding

```cpp
// ✅ Sempre usar UTF-8
QString text = QString::fromUtf8(inputText);

// ✅ Output também em UTF-8
fprintf(stdout, "%s\n", value.toUtf8().constData());
```

## Features Comuns a Implementar

### 1. Novos Widgets

- FileDialog (seleção de arquivos)
- TreeView (hierarquia de dados)
- SpinBox (números com setas)
- DateTimeEdit (data+hora combinados)
- ColorPicker (seleção de cores)

### 2. Melhorias em Widgets Existentes

- Validação de input em TextBox
- Multi-seleção em ListBox
- Ordenação em Table
- Zoom em Charts
- Ricordo histórico em ComboBox

### 3. Features de UX

- Atalhos de teclado (Ctrl+Enter)
- Drag and drop em listas
- Copy/paste em tabelas
- Tooltips informativos
- Ícones personalizados por widget

## Ferramentas de Desenvolvimento

### Build

```bash
cd src/code/showbox
qmake6
make -j$(nproc)
```

### Debug

```bash
# Com gdb
gdb ./src/code/showbox/bin/showbox

# Com valgrind (memory leaks)
valgrind --leak-check=full ./src/code/showbox/bin/showbox --add-button "Test"
```

### Profiling

```bash
# Performance profiling
perf record ./src/code/showbox/bin/showbox ...
perf report
```

## Checklist de Feature Completa

- [ ] Código implementado em `showbox.{cc,h}`
- [ ] Comando criado em `commands/`
- [ ] Parser atualizado
- [ ] Output implementado
- [ ] Exemplo funcional em `examples/`
- [ ] Testes criados e passando
- [ ] Documentação atualizada
- [ ] Man page atualizada
- [ ] Sem warnings de compilação
- [ ] Sem memory leaks
- [ ] Code review aprovado

## Recursos Úteis

- [Qt6 Documentation](https://doc.qt.io/qt-6/)
- [Qt Widgets Examples](https://doc.qt.io/qt-6/qtwidgets-examples.html)
- [Qt Charts Documentation](https://doc.qt.io/qt-6/qtcharts-index.html)

---

**Lembre-se**: ShowBox é usado em scripts shell - a interface CLI deve ser intuitiva e consistente!
