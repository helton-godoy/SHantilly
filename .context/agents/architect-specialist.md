# Architect Specialist Agent

## Foco

Tomar decisões arquiteturais para o **ShowBox**, garantindo qualidade, extensibilidade e manutenibilidade do código Qt6/C++.

## Princípios Arquiteturais

### 1. Separação de Responsabilidades

```diagram
┌─────────────────────────────────────────────┐
│  PRESENTATION (ShowBox, dialog_*.cc)        │
│  - Renderização GUI                         │
│  - Gerenciamento de layouts                 │
├─────────────────────────────────────────────┤
│  APPLICATION (commands/, parser*)           │
│  - Lógica de comandos                       │
│  - Processamento de argumentos              │
├─────────────────────────────────────────────┤
│  DOMAIN (ThemeManager, IconHelper)          │
│  - Regras de negócio                        │
│  - Serviços utilitários                     │
├─────────────────────────────────────────────┤
│  INFRASTRUCTURE (I/O, Qt Framework)         │
│  - Framework Qt6                            │
│  - Sistema de arquivos                      │
└─────────────────────────────────────────────┘
```

### 2. Padrões de Design Utilizados

| Padrão        | Uso no ShowBox           | Localização            |
|---------------|--------------------------|------------------------|
| **Command**   | Encapsular operações CLI | `commands/*.cpp`       |
| **Registry**  | Mapear nomes a comandos  | `command_registry.cpp` |
| **Builder**   | Construir diálogos       | `ShowBox::addXXX()`    |
| **Strategy**  | Temas intercambiáveis    | `theme_manager.cpp`    |
| **Composite** | Hierarquia de layouts    | Qt layouts             |

### 3. Extensibilidade

**Adicionar Novo Widget**:

1. Criar método `ShowBox::addNewWidget()`
2. Criar comando `AddNewWidgetCommand`
3. Registrar no parser
4. Criar exemplo em `examples/`

**Adicionar Nova Funcionalidade**:

1. Identificar camada apropriada
2. Definir interface pública
3. Implementar com testes
4. Documentar API

## Decisões Arquiteturais

### ADR-001: Uso de Qt6 sobre GTK

**Contexto**: Escolher toolkit GUI para aplicação desktop multiplataforma.

**Decisão**: Qt6 Framework

**Razões**:

- Performance superior em rendering
- Melhor suporte a gráficos (Qt Charts)
- API mais moderna (C++17)
- Licença LGPL permite uso comercial
- Melhor integração com KDE

**Consequências**:

- Dependência de Qt runtime
- Maior tamanho de binário
- Curva de aprendizado para não-Qt devs

---

### ADR-002: Command Pattern para CLI

**Contexto**: Como estruturar processamento de argumentos CLI.

**Decisão**: Command Pattern com Registry

**Razões**:

- Extensibilidade para novos comandos
- Testabilidade isolada
- Separação de parsing e execução
- Fácil manutenção

**Estrutura**:

```cpp
// Interface
class Command {
    virtual void execute(ExecutionContext&) = 0;
};

// Implementações específicas
class AddButtonCommand : public Command { ... };
class SetTitleCommand : public Command { ... };

// Registry despacha
registry.dispatch("add-button", context);
```

---

### ADR-003: Parent-Child Memory Management

**Contexto**: Gerenciamento de memória para widgets.

**Decisão**: Usar Qt parent-child ownership exclusivamente.

**Razões**:

- Simplicidade
- Previne memory leaks
- Segurança (Qt garante limpeza)
- Padrão Qt idiomático

**Regra**:

```cpp
// ✅ SEMPRE especificar parent
QWidget* widget = new QWidget(this);

// ❌ NUNCA criar sem parent
QWidget* widget = new QWidget();  // Banned
```

---

### ADR-004: UTF-8 como Encoding Padrão

**Contexto**: Suporte a múltiplos idiomas e caracteres especiais.

**Decisão**: UTF-8 em todo o sistema.

**Implementação**:

```cpp
// Input
QString text = QString::fromUtf8(cString);

// Output
fprintf(stdout, "%s\n", qString.toUtf8().constData());
```

---

### ADR-005: Single-Thread GUI

**Contexto**: Modelo de threading para GUI.

**Decisão**: GUI single-threaded (main thread apenas).

**Razões**:

- Simplicidade
- Thread-safety garantida
- Padrão Qt recomendado
- ShowBox não precisa de performance multi-thread

**Consequência**:

- Operações longas devem usar `QApplication::processEvents()`
- Ou mover para QThread com signals para atualizar GUI

## Componentes Críticos

### ShowBox (Classe Principal)

**Responsabilidades**:

- Gerenciar QDialog principal
- Coordenar criação de widgets
- Manter estado de layouts
- Produzir output

**Interface Pública**:

```cpp
class ShowBox : public QDialog {
public:
    // Widgets
    void addPushButton(...);
    void addTextBox(...);
    void addTable(...);
    void addChart(...);
    
    // Navegação
    void addTabs(...);
    void addPage(...);
    void endPage();
    void endTabs();
    
    // Configuração
    void setThemeManager(ThemeManager*);
    void applyTheme(ThemeMode);
    
    // Layout
    void stepHorizontal();
    void stepVertical();
    
    // Limpeza
    void clear(char* name);
};
```

### CommandRegistry

**Pattern**: Registry + Factory

```cpp
class CommandRegistry {
    std::map<std::string, CommandFactory> factories;
    
public:
    void registerCommand(const std::string& name, CommandFactory f);
    Command* createCommand(const std::string& name, args...);
    void dispatch(const std::string& name, ExecutionContext& ctx);
};
```

### Parser Pipeline

```flow
Arguments → Tokenizer → Parser → AST → ParserDriver → Commands
```

## Pontos de Extensão

### 1. Novos Widgets

**Arquivo**: `showbox.cc`

```cpp
void ShowBox::addNewWidget(const char* name, ...) {
    // 1. Criar widget Qt
    // 2. Configurar propriedades
    // 3. Aplicar tema
    // 4. Conectar signals
    // 5. Adicionar ao layout
    // 6. Registrar para output
}
```

### 2. Novos Comandos

**Arquivo**: `commands/add_command.cpp`

```cpp
class AddNewWidgetCommand : public Command {
public:
    void execute(ExecutionContext& ctx) override {
        ctx.getShowBox()->addNewWidget(...);
    }
};
```

### 3. Novos Temas

**Arquivo**: `theme_manager.cpp`

```cpp
void ThemeManager::applyCustomTheme() {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(...));
    // ...
    QApplication::setPalette(palette);
}
```

## Trade-offs

### Simplicidade vs Flexibilidade

**Escolha**: Simplicidade

- API CLI intuitiva
- 1 widget = 1 comando
- Defaults sensíveis

### Performance vs Memory

**Escolha**: Memory (uso moderado)

- Widgets criados on-demand
- Sem pool de objetos
- Parent-child limpeza automática

### Extensibilidade vs Acoplamento

**Escolha**: Extensibilidade moderada

- Command pattern desacopla parsing
- ShowBox conhece todos widgets (acoplamento aceito)
- Helpers são stateless

## Métricas de Qualidade

### Complexidade

| Arquivo        | Linhas    | Funções | Complexidade      |
|----------------|-----------|---------|-------------------|
| showbox.cc     | 1516      | 52      | Alta (refatorar?) |
| dialog_*.cc    | ~400 cada | ~10     | Média             |
| commands/*.cpp | ~100 cada | 1-3     | Baixa ✅          |

### Acoplamento

- **ShowBox**: Alta (conhece todos widgets) - OK
- **Commands**: Baixa (só conhecem ShowBox interface) ✅
- **Parser**: Média (conhece Command types) - OK

### Coesão

- **ThemeManager**: Alta (só temas) ✅
- **IconHelper**: Alta (só ícones) ✅
- **ShowBox**: Média-Baixa (muitas responsabilidades) ⚠️

## Recomendações Futuras

### 1. Split ShowBox

Dividir `showbox.cc` (1516 linhas) em:

- `widget_factory.cpp` - Criação de widgets
- `layout_manager.cpp` - Gerenciamento de layouts
- `output_formatter.cpp` - Formatação de output
- `showbox.cpp` - Coordenação

### 2. Plugin System

Para extensibilidade sem recompilação:

```cpp
// showbox --load-plugin mywidget.so
class WidgetPlugin {
    virtual void registerWidgets(ShowBox&) = 0;
};
```

### 3. DSL para Diálogos

Arquivo `.sbox` declarativo:

```yaml
dialog:
  title: "Config"
  widgets:
    - textbox: {label: "Name", name: "txt_name"}
    - button: {text: "OK", exit: true}
```

## Checklist de Design Review

- [ ] Responsabilidade única por classe?
- [ ] Interface pública mínima?
- [ ] Dependências injetáveis?
- [ ] Testável isoladamente?
- [ ] Documentação adequada?
- [ ] Seguindo padrões existentes?
- [ ] Performance aceitável?
- [ ] Memory safe?

---

**Arquitetura é evolução, não revolução. Pequenas melhorias contínuas!**
