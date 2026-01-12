# Backend Specialist Agent

## Foco

Desenvolver a lógica de aplicação do **ShowBox**: sistema de comandos, parsing, processamento de dados e integração com widgets Qt.

## Componentes de Backend

No contexto do ShowBox, "backend" refere-se à lógica não-visual:

```diagram
┌─────────────────────────────────────────────┐
│  CLI Input (argumentos, stdin)              │
└─────────────────────┬───────────────────────┘
                      │
┌─────────────────────▼───────────────────────┐
│  BACKEND                                    │
│  ├── Tokenizer     → Quebra argumentos      │
│  ├── Parser        → Analisa sintaxe        │
│  ├── ParserDriver  → Orquestra execução     │
│  ├── Commands      → Lógica de comandos     │
│  ├── Registry      → Mapeia comandos        │
│  └── Context       → Estado de execução     │
└─────────────────────┬───────────────────────┘
                      │
┌─────────────────────▼───────────────────────┐
│  Frontend (ShowBox GUI via Qt)              │
└─────────────────────────────────────────────┘
```

## Tokenizer

### Responsabilidade

Converter linha de comando em tokens individuais.

### Implementação

**Arquivo**: `tokenizer.{h,cpp}`

```cpp
class Tokenizer {
public:
    std::vector<Token> tokenize(int argc, char** argv);
    std::vector<Token> tokenize(const QStringList& args);
    
private:
    Token nextToken(const QString& str);
    bool isOption(const QString& str);
    bool isQuoted(const QString& str);
};

struct Token {
    enum Type { OPTION, STRING, NUMBER, FLAG };
    Type type;
    QString value;
    int position;
};
```

### Regras de Tokenização

```form
Input: showbox --title "My Title" --add-button OK --exit

Tokens:
  [0] OPTION  "--title"
  [1] STRING  "My Title"
  [2] OPTION  "--add-button"
  [3] STRING  "OK"
  [4] FLAG    "--exit"
```

**Casos Especiais**:

- Strings com espaços: `"valor com espaços"`
- Escape de aspas: `"valor \"quoted\""`
- Valores numéricos: `--value 42`
- Flags booleanas: `--exit` (sem valor)

## Parser

### Responsabilidade

Analisar tokens e construir comandos estruturados.

### Implementação

**Arquivo**: `parser.{h,cpp}`

```cpp
class Parser {
public:
    std::vector<std::unique_ptr<Command>> parse(
        const std::vector<Token>& tokens);
    
    QString getError() const { return error_; }
    bool hasError() const { return !error_.isEmpty(); }
    
private:
    int currentPos_;
    QString error_;
    
    Token peek();
    Token advance();
    bool match(Token::Type type);
    
    Command* parseAddCommand();
    Command* parseSetCommand();
    Command* parseQueryCommand();
};
```

### Gramática (Informal)

```cmd
program     := command*
command     := add_cmd | set_cmd | query_cmd | simple_cmd
add_cmd     := "--add-" widget_type args options
set_cmd     := "--set-" property value
query_cmd   := "--query" widget_name
simple_cmd  := "--exit" | "--clear" | "--step-h" | "--step-v"

widget_type := "button" | "textbox" | "table" | ...
args        := STRING*
options     := ("--" WORD value?)*
```

### Exemplos

```cpp
// Input: --add-button "OK" "btn_ok" --exit --default

AddButtonCommand {
    title: "OK",
    name: "btn_ok", 
    exit: true,
    isDefault: true,
    apply: false
}
```

## Command System

### Interface Base

```cpp
// command.h
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(ExecutionContext& ctx) = 0;
    virtual QString name() const = 0;
};
```

### Comandos Disponíveis

**Add Commands** (`add_command.{h,cpp}`):

```cpp
class AddButtonCommand : public Command { ... };
class AddTextBoxCommand : public Command { ... };
class AddListBoxCommand : public Command { ... };
class AddTableCommand : public Command { ... };
class AddChartCommand : public Command { ... };
class AddTabsCommand : public Command { ... };
// ... ~15 widgets
```

**Set Commands** (`set_command.{h,cpp}`):

```cpp
class SetTitleCommand : public Command { ... };
class SetSizeCommand : public Command { ... };
class SetThemeCommand : public Command { ... };
class SetIconCommand : public Command { ... };
```

**Query Commands** (`query_command.{h,cpp}`):

```cpp
class QueryValueCommand : public Command { ... };
class QueryStateCommand : public Command { ... };
```

**Simple Commands** (`simple_commands.{h,cpp}`):

```cpp
class ExitCommand : public Command { ... };
class ClearCommand : public Command { ... };
class StepHorizontalCommand : public Command { ... };
class StepVerticalCommand : public Command { ... };
class EndPageCommand : public Command { ... };
class EndTabsCommand : public Command { ... };
```

### Criar Novo Comando

```cpp
// 1. Header (commands/add_command.h)
class AddTreeViewCommand : public Command {
public:
    AddTreeViewCommand(const char* name, bool multiSelect);
    void execute(ExecutionContext& ctx) override;
    QString name() const override { return "add-treeview"; }
    
private:
    QString name_;
    bool multiSelect_;
};

// 2. Implementation (commands/add_command.cpp)
AddTreeViewCommand::AddTreeViewCommand(
    const char* name, bool multiSelect)
    : name_(QString::fromUtf8(name))
    , multiSelect_(multiSelect)
{}

void AddTreeViewCommand::execute(ExecutionContext& ctx) {
    ctx.getShowBox()->addTreeView(
        name_.toUtf8(), multiSelect_
    );
}

// 3. Registrar no Parser (parser.cpp)
if (token == "--add-treeview") {
    QString name = nextArg();
    bool multi = hasFlag("--multi-select");
    commands.push_back(
        std::make_unique<AddTreeViewCommand>(
            name.toUtf8(), multi
        )
    );
}
```

## Command Registry

### Implementação

```cpp
// command_registry.h
class CommandRegistry {
public:
    using Factory = std::function<
        std::unique_ptr<Command>(const std::vector<QString>&)
    >;
    
    void registerCommand(const QString& name, Factory factory);
    
    std::unique_ptr<Command> create(
        const QString& name,
        const std::vector<QString>& args);
    
    bool hasCommand(const QString& name) const;
    
private:
    std::map<QString, Factory> factories_;
};

// Uso
registry.registerCommand("add-button", [](const auto& args) {
    return std::make_unique<AddButtonCommand>(
        args[0].toUtf8(),  // title
        args[1].toUtf8(),  // name
        args.contains("--exit"),
        args.contains("--default")
    );
});
```

## Execution Context

### Responsabilidade

Manter estado durante execução de comandos.

```cpp
// execution_context.h
class ExecutionContext {
public:
    ExecutionContext(ShowBox* dialog);
    
    ShowBox* getShowBox() { return dialog_; }
    
    // Estado de navegação
    void pushContainer(QWidget* container);
    void popContainer();
    QWidget* currentContainer();
    
    // Estado de layout
    void setCurrentLayout(QLayout* layout);
    QLayout* currentLayout();
    
    // Flags
    void setInPage(bool value) { inPage_ = value; }
    bool isInPage() const { return inPage_; }
    
    void setInTabs(bool value) { inTabs_ = value; }
    bool isInTabs() const { return inTabs_; }
    
private:
    ShowBox* dialog_;
    std::stack<QWidget*> containerStack_;
    QLayout* currentLayout_;
    bool inPage_;
    bool inTabs_;
};
```

## Parser Driver

### Responsabilidade

Orquestrar tokenização, parsing e execução.

```cpp
// parser_driver.h
class ParserDriver {
public:
    ParserDriver(ShowBox* dialog);
    
    bool run(int argc, char** argv);
    bool run(const QStringList& args);
    
    QString getError() const { return error_; }
    
private:
    Tokenizer tokenizer_;
    Parser parser_;
    ExecutionContext context_;
    QString error_;
    
    bool executeCommands(
        const std::vector<std::unique_ptr<Command>>& cmds);
};

// Implementação
bool ParserDriver::run(const QStringList& args) {
    // 1. Tokenizar
    auto tokens = tokenizer_.tokenize(args);
    
    // 2. Parsear
    auto commands = parser_.parse(tokens);
    if (parser_.hasError()) {
        error_ = parser_.getError();
        return false;
    }
    
    // 3. Executar
    return executeCommands(commands);
}

bool ParserDriver::executeCommands(
    const std::vector<std::unique_ptr<Command>>& cmds) {
    for (const auto& cmd : cmds) {
        try {
            cmd->execute(context_);
        } catch (const std::exception& e) {
            error_ = QString("Erro executando %1: %2")
                .arg(cmd->name())
                .arg(e.what());
            return false;
        }
    }
    return true;
}
```

## Processamento de Dados

### Carregar Tabela

```cpp
// Em ShowBox ou CustomTableWidget
bool loadTableFromFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    
    // Ler headers (primeira linha)
    QString headerLine = stream.readLine();
    QStringList headers = headerLine.split(',');
    setHorizontalHeaderLabels(headers);
    
    // Ler dados
    int row = 0;
    while (!stream.atEnd() && row < MAX_ROWS) {
        QString line = stream.readLine();
        QStringList values = line.split(',');
        
        insertRow(row);
        for (int col = 0; col < values.size(); col++) {
            setItem(row, col, 
                new QTableWidgetItem(values[col].trimmed())
            );
        }
        row++;
    }
    
    return true;
}
```

### Formatar Output

```cpp
// Em ShowBox
void outputValues(FILE* out) {
    for (QWidget* widget : widgets_) {
        QString name = widget->objectName();
        QString value = getWidgetValue(widget);
        
        fprintf(out, "%s=%s\n", 
            name.toUtf8().constData(),
            value.toUtf8().constData()
        );
    }
}

QString getWidgetValue(QWidget* widget) {
    if (auto* textBox = qobject_cast<QLineEdit*>(widget)) {
        return textBox->text();
    }
    if (auto* checkBox = qobject_cast<QCheckBox*>(widget)) {
        return checkBox->isChecked() ? "true" : "false";
    }
    if (auto* comboBox = qobject_cast<QComboBox*>(widget)) {
        return comboBox->currentText();
    }
    // ... outros widgets
    return QString();
}
```

## Error Handling

### Estratégia

1. **Parsing Errors**: Retornar imediatamente, mostrar erro
2. **Execution Errors**: Log para stderr, continuar se possível
3. **Fatal Errors**: Exit com código != 0

```cpp
// Parsing
if (parser_.hasError()) {
    fprintf(stderr, "ERRO: %s\n", 
        parser_.getError().toUtf8().constData());
    return 2;  // Exit code para erro de sintaxe
}

// Execution
void AddTableCommand::execute(ExecutionContext& ctx) {
    if (path_.isEmpty()) {
        fprintf(stderr, "Aviso: Tabela sem dados\n");
        // Continuar com tabela vazia
    }
    ctx.getShowBox()->addTable(...);
}

// Fatal
if (!ctx.getShowBox()) {
    fprintf(stderr, "FATAL: ShowBox não inicializado\n");
    exit(1);
}
```

## Testes de Backend

```cpp
// test_parser.cpp
void TestParser::testTokenize() {
    Tokenizer tok;
    auto tokens = tok.tokenize(
        {"--title", "Test", "--add-button", "OK"}
    );
    
    QCOMPARE(tokens.size(), 4);
    QCOMPARE(tokens[0].type, Token::OPTION);
    QCOMPARE(tokens[0].value, QString("--title"));
}

void TestParser::testParse() {
    Parser p;
    auto cmds = p.parse(tokens);
    
    QVERIFY(!p.hasError());
    QCOMPARE(cmds.size(), 2);
}

void TestParser::testExecute() {
    ShowBox dialog;
    ExecutionContext ctx(&dialog);
    
    AddButtonCommand cmd("OK", "btn_ok", true, false, false);
    cmd.execute(ctx);
    
    auto* btn = dialog.findChild<QPushButton*>("btn_ok");
    QVERIFY(btn != nullptr);
}
```

## Performance

### Otimizações

```cpp
// 1. Evitar cópias de string
void addWidget(const char* name);  // Recebe C-string
QString qName = QString::fromUtf8(name);  // Copia 1x

// 2. Reserve capacity
widgets_.reserve(100);  // Antes de loop

// 3. Move semantics
commands.push_back(std::move(cmd));

// 4. Inline pequenas funções
inline bool hasFlag(const QString& flag) {
    return args_.contains(flag);
}
```

## Checklist

- [ ] Tokenização correta (strings, escapes, flags)
- [ ] Parser valida sintaxe
- [ ] Comandos são stateless
- [ ] Context mantém estado corretamente
- [ ] Erros são informativos
- [ ] Output é UTF-8
- [ ] Testes cobrindo edge cases
- [ ] Memory safe (no leaks)

---

**Backend robusto = GUI confiável!**
