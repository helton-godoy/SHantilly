# Refactoring Specialist Agent

## Foco

Melhorar a estrutura e qualidade do código **ShowBox** sem alterar comportamento externo, reduzindo débito técnico e aumentando manutenibilidade.

## Sinais para Refatorar

### Code Smells

| Smell                   | Exemplo no ShowBox                   | Ação                       |
|-------------------------|--------------------------------------|----------------------------|
| **Long Method**         | `showbox.cc` tem funções > 50 linhas | Extract Method             |
| **Large Class**         | `ShowBox` tem 52 métodos             | Extract Class              |
| **Duplicate Code**      | Pattern repetido em `addXXX()`       | Extract template           |
| **Long Parameter List** | 6+ parâmetros em funções             | Introduce Parameter Object |
| **Feature Envy**        | Método acessa muito outra classe     | Move Method                |
| **Comments**            | Comentário explica código ruim       | Clarify code               |

### Métricas

- **Arquivo > 500 linhas**: Considerar split
- **Função > 30 linhas**: Considerar extract
- **Classe > 20 métodos**: Considerar split
- **Parâmetros > 4**: Considerar object
- **Indentação > 4 níveis**: Considerar extract

## Refatorações Comuns

### 1. Extract Method

```cpp
// ❌ ANTES - método longo
void ShowBox::addTable(const char* headers, const char* name,
                       const char* file, bool readonly,
                       bool selection, bool search) {
    // Validação (10 linhas)
    if (!name || !*name) { ... }
    if (!currentLayout) { ... }
    
    // Criação (15 linhas)
    CustomTableWidget* table = new CustomTableWidget(this);
    table->setObjectName(...);
    table->setEditTriggers(...);
    
    // Headers (10 linhas)
    QString headerStr = QString::fromUtf8(headers);
    QStringList headerList = headerStr.split(',');
    table->setColumnCount(headerList.size());
    table->setHorizontalHeaderLabels(headerList);
    
    // Load data (20 linhas)
    if (file && *file) {
        QFile f(QString::fromUtf8(file));
        // ... carregar dados
    }
    
    // Layout (5 linhas)
    currentLayout->addWidget(table);
    widgets_.push_back(table);
}

// ✅ DEPOIS - métodos extraídos
void ShowBox::addTable(const char* headers, const char* name,
                       const char* file, bool readonly,
                       bool selection, bool search) {
    if (!validateWidgetCreation(name)) return;
    
    auto* table = createTable(name, readonly, selection, search);
    configureTableHeaders(table, headers);
    
    if (file && *file) {
        loadTableData(table, file);
    }
    
    addWidgetToLayout(table);
}

bool ShowBox::validateWidgetCreation(const char* name) {
    if (!name || !*name) {
        fprintf(stderr, "ERRO: nome obrigatório\n");
        return false;
    }
    if (!currentLayout) {
        fprintf(stderr, "ERRO: sem layout\n");
        return false;
    }
    return true;
}

CustomTableWidget* ShowBox::createTable(...) { ... }
void ShowBox::configureTableHeaders(...) { ... }
void ShowBox::loadTableData(...) { ... }
void ShowBox::addWidgetToLayout(QWidget* widget) { ... }
```

### 2. Extract Class

```cpp
// ❌ ANTES - ShowBox faz tudo
class ShowBox {
    // Layout management (deveria ser separado)
    void stepHorizontal();
    void stepVertical();
    QVBoxLayout* mainLayout;
    QHBoxLayout* currentRow;
    
    // Output formatting (deveria ser separado)
    void outputValues();
    QString formatValue(QWidget*);
};

// ✅ DEPOIS - classes separadas
class LayoutManager {
public:
    void stepHorizontal();
    void stepVertical();
    void addWidget(QWidget*);
    QLayout* currentLayout();
    
private:
    QVBoxLayout* mainLayout_;
    QHBoxLayout* currentRow_;
    std::stack<QLayout*> layoutStack_;
};

class OutputFormatter {
public:
    QString format(const std::vector<QWidget*>& widgets);
    void writeToFile(FILE* out);
    
private:
    QString formatWidget(QWidget*);
};

class ShowBox {
    LayoutManager layoutManager_;
    OutputFormatter outputFormatter_;
};
```

### 3. Introduce Parameter Object

```cpp
// ❌ ANTES - muitos parâmetros
void ShowBox::addPushButton(
    const char* title,
    const char* name,
    bool apply,
    bool exit,
    bool isDefault,
    const char* icon,
    const char* tooltip);

// ✅ DEPOIS - objeto
struct ButtonConfig {
    QString title;
    QString name;
    bool apply = false;
    bool exit = false;
    bool isDefault = false;
    QString icon;
    QString tooltip;
    
    static ButtonConfig fromArgs(const char* title, const char* name) {
        ButtonConfig cfg;
        cfg.title = QString::fromUtf8(title);
        cfg.name = QString::fromUtf8(name);
        return cfg;
    }
};

void ShowBox::addPushButton(const ButtonConfig& config);

// Uso
ButtonConfig cfg = ButtonConfig::fromArgs("OK", "btn_ok");
cfg.exit = true;
cfg.isDefault = true;
dialog.addPushButton(cfg);
```

### 4. Replace Conditional with Polymorphism

```cpp
// ❌ ANTES - switch em tipo
QString ShowBox::getWidgetValue(QWidget* widget) {
    if (auto* text = qobject_cast<QLineEdit*>(widget)) {
        return text->text();
    }
    if (auto* check = qobject_cast<QCheckBox*>(widget)) {
        return check->isChecked() ? "true" : "false";
    }
    if (auto* combo = qobject_cast<QComboBox*>(widget)) {
        return combo->currentText();
    }
    // ... mais 10 tipos
    return QString();
}

// ✅ DEPOIS - interface
class ValueProvider {
public:
    virtual QString getValue() const = 0;
};

class TextBoxValueProvider : public ValueProvider {
    QLineEdit* widget_;
public:
    QString getValue() const override {
        return widget_->text();
    }
};

// Widget wrapper sabe seu provider
class WidgetWrapper {
    QWidget* widget_;
    std::unique_ptr<ValueProvider> provider_;
public:
    QString getValue() const {
        return provider_->getValue();
    }
};
```

### 5. Pull Up / Push Down

```cpp
// Se múltiplos addXXX() têm código comum
// PULL UP para método base ou template

// ❌ ANTES - código duplicado
void ShowBox::addPushButton(...) {
    if (!validateName(name)) return;
    QPushButton* btn = new QPushButton(this);
    btn->setObjectName(QString::fromUtf8(name));
    // config específica
    applyTheme(btn);
    currentLayout->addWidget(btn);
    widgets_.push_back(btn);
}

void ShowBox::addCheckBox(...) {
    if (!validateName(name)) return;
    QCheckBox* box = new QCheckBox(this);
    box->setObjectName(QString::fromUtf8(name));
    // config específica
    applyTheme(box);
    currentLayout->addWidget(box);
    widgets_.push_back(box);
}

// ✅ DEPOIS - template method
template<typename W, typename... Args>
W* ShowBox::addWidget(const char* name, Args&&... args) {
    if (!validateName(name)) return nullptr;
    
    W* widget = new W(std::forward<Args>(args)..., this);
    widget->setObjectName(QString::fromUtf8(name));
    
    return widget;
}

void ShowBox::finalizeWidget(QWidget* widget) {
    applyTheme(widget);
    currentLayout->addWidget(widget);
    widgets_.push_back(widget);
}

void ShowBox::addPushButton(const char* title, const char* name, ...) {
    auto* btn = addWidget<QPushButton>(name, QString::fromUtf8(title));
    if (!btn) return;
    // config específica
    finalizeWidget(btn);
}
```

## Workflow de Refatoração

### 1. Preparação

```bash
# Garantir testes passando
cd tests && ./run_tests.sh
# Commit estado atual
git add -A && git commit -m "Before refactoring"
```

### 2. Identificar Alvo

```bash
# Métricas de complexidade
cppcheck --enable=style src/code/showbox/

# Linhas por arquivo
wc -l src/code/showbox/*.cc | sort -n
```

### 3. Refatorar Incrementalmente

```tasks
1. Fazer UMA pequena mudança
2. Compilar
3. Rodar testes
4. Commit se passou
5. Repetir
```

### 4. Validar

```bash
# Todos os testes
./run_tests.sh

# Memory leaks
valgrind --leak-check=full ./showbox --add-button "Test"

# Funcionalidade
./examples/showbox_table.sh
```

## Padrões de Refatoração

### Safe Refactoring Steps

**Rename** (IDE suporta):

```cpp
// F2 em Qt Creator, rename symbol em VS Code
oldMethodName() → newMethodName()
```

**Extract Variable**:

```cpp
// ❌
if (widget->objectName().startsWith("btn_") && widget->isEnabled()) { }

// ✅
QString name = widget->objectName();
bool isButton = name.startsWith("btn_");
bool isActive = widget->isEnabled();
if (isButton && isActive) { }
```

**Inline Variable** (oposto):

```cpp
// ❌ Variável usada 1x
QString name = widget->objectName();
setName(name);

// ✅
setName(widget->objectName());
```

## Técnicas Específicas Qt

### Replace QObject::connect

```cpp
// ❌ ANTIGO - string-based
connect(sender, SIGNAL(clicked()), 
        receiver, SLOT(handleClick()));

// ✅ NOVO - type-safe
connect(sender, &QPushButton::clicked,
        receiver, &MyClass::handleClick);
```

### Modernize Loops

```cpp
// ❌
for (int i = 0; i < list.size(); ++i) {
    process(list[i]);
}

// ✅
for (const auto& item : list) {
    process(item);
}

// ✅ Com índice se necessário
for (int i = 0; const auto& item : list) {
    process(item, i++);
}
```

### Use auto

```cpp
// ❌ Repetitivo
QMap<QString, QWidget*>::iterator it = map.begin();

// ✅ Cleaner
auto it = map.begin();
```

## Checklist de Refatoração

### Antes

- [ ] Código compilando
- [ ] Testes passando
- [ ] Commit do estado atual
- [ ] Entendi o que o código faz

### Durante

- [ ] Mudanças pequenas e incrementais
- [ ] Compilar após cada mudança
- [ ] Testar frequentemente
- [ ] Não mudar comportamento

### Depois

- [ ] Todos testes passando
- [ ] Sem warnings novos
- [ ] Valgrind clean
- [ ] Código mais legível
- [ ] Documentação atualizada

## Métricas de Sucesso

| Antes                   | Depois       | Melhoria |
|-------------------------|--------------|----------|
| showbox.cc: 1516 linhas | < 500 linhas | -67%     |
| ShowBox: 52 métodos     | < 20 métodos | -62%     |
| max função: 80 linhas   | < 30 linhas  | -63%     |
| Duplicação: 15%         | < 5%         | -67%     |

## Riscos

- **Introduzir bugs**: Mitigar com testes
- **Over-engineering**: Manter YAGNI
- **Tempo excessivo**: Timebox refactoring
- **Quebrar API**: Manter compatibilidade CLI

---

**Refatorar é como jardinagem: pequenos cuidados frequentes previnem grandes problemas!**
