# Code Reviewer Agent

## Missão

Realizar code reviews detalhados de mudanças no **ShowBox**, garantindo qualidade, consistência e aderência aos padrões do projeto.

## Checklist de Code Review

### 1. Correção Funcional

- [ ] O código resolve o problema proposto?
- [ ] Não introduz novos bugs?
- [ ] Edge cases estão tratados?
- [ ] Validação de inputs está correta?

### 2. Qualidade de Código

**Legibilidade:**

- [ ] Nomes de variáveis/funções são descritivos?
- [ ] Lógica complexa está comentada?
- [ ] Código segue convenções de nomenclatura?

**Estrutura:**

- [ ] Funções têm responsabilidade única?
- [ ] Evita duplicação de código?
- [ ] Usa padrões de design apropriados?

**Qt/C++ Específico:**

```cpp
// ❌ RUIM - memory leak potencial
QPushButton *btn = new QPushButton();
layout->addWidget(btn);  // Sem parent!

// ✅ BOM - Qt parent ownership
QPushButton *btn = new QPushButton(this);
```

### 3. Performance

- [ ] Evita operações caras em loops?
- [ ] Usa Qt containers apropriadamente?
- [ ] Signals/slots bem conectados (sem ciclos)?
- [ ] Lazy loading quando apropriado?

```cpp
// ❌ RUIM - realocação constante
for (int i = 0; i < 1000; i++) {
    list.append(item);  // Realoca cada vez
}

// ✅ BOM - reserva memória
list.reserve(1000);
for (int i = 0; i < 1000; i++) {
    list.append(item);
}
```

### 4. Segurança de Memória

- [ ] Sem use-after-free?
- [ ] Ponteiros sempre verificados (`if (ptr)`)?
- [ ] Qt parent-child ownership correto?
- [ ] Sem double-delete?

### 5. Thread Safety

- [ ] Widgets só modificados na GUI thread?
- [ ] Sem race conditions?
- [ ] Mutexes usados corretamente (se multi-thread)?

### 6. Consistência com Codebase

**Padrões de Código:**

- [ ] Indentação consistente (4 espaços)?
- [ ] Chaves em estilo K&R?
- [ ] Linhas <= 100 caracteres?

**Arquitetura:**

- [ ] Segue padrão Command para novos comandos?
- [ ] Widgets adicionados via `ShowBox::addXXX()`?
- [ ] Registrado corretamente no `CommandRegistry`?

### 7. Testes

- [ ] Testes unitários existem?
- [ ] Testes cobrem edge cases?
- [ ] Exemplo funcional em `examples/`?
- [ ] Sem warnings do compilador?

### 8. Documentação

- [ ] Código comentado adequadamente?
- [ ] Man page atualizada?
- [ ] README atualizado se necessário?
- [ ] Exemplo de uso fornecido?

## Padrões Específicos do ShowBox

### Widget Creation

```cpp
// Template para adicionar widget
void ShowBox::addMyWidget(const char *title, const char *name) {
    // 1. Validar inputs
    if (!name || !*name) {
        fprintf(stderr, "ERRO: nome obrigatório\n");
        return;
    }
    
    // 2. Criar widget com parent
    QMyWidget *widget = new QMyWidget(this);
    widget->setObjectName(QString::fromUtf8(name));
    
    // 3. Configurar
    if (title && *title) {
        widget->setProperty("title", QString::fromUtf8(title));
    }
    
    // 4. Aplicar tema
    if (themeManager_) {
        applyWidgetTheme(widget);
    }
    
    // 5. Conectar signals
    connect(widget, &QMyWidget::changed,
            this, &ShowBox::onWidgetChanged);
    
    // 6. Adicionar ao layout
    if (currentLayout_) {
        currentLayout_->addWidget(widget);
    }
    
    // 7. Registrar
    widgets_.push_back(widget);
}
```

### String Handling

```cpp
// ✅ BOM - sempre UTF-8
QString text = QString::fromUtf8(cStr);
fprintf(stdout, "%s\n", qStr.toUtf8().constData());

// ❌ RUIM - conversão implícita
QString text = cStr;  // Pode ter problema com encoding
```

### Error Handling

```cpp
// ✅ BOM - mensagens para stderr
if (error) {
    fprintf(stderr, "ERRO: %s\n", errorMsg);
    return;
}

// ❌ RUIM - throw (ShowBox não usa exceções)
throw std::runtime_error("Error");
```

## Problemas Comuns

### 1. Memory Leaks

```cpp
// ❌ LEAK
void ShowBox::bad() {
    QWidget *w = new QWidget();  // Sem parent!
    // w nunca é deletado
}

// ✅ FIX
void ShowBox::good() {
    QWidget *w = new QWidget(this);  // Qt cuida do delete
}
```

### 2. Signal/Slot Issues

```cpp
// ❌ RUIM - disconnect esquecido
connect(btn, &QPushButton::clicked, this, &ShowBox::handler);
// Se btn for destruído antes de ShowBox, crash!

// ✅ BOM - Qt::AutoConnection (default) é seguro
connect(btn, &QPushButton::clicked,
        this, &ShowBox::handler, Qt::AutoConnection);
```

### 3. Iterator Invalidation

```cpp
// ❌ PERIGOSO
for (auto widget : widgets_) {
    if (condition) {
        widgets_.remove(widget);  // Invalida iterator!
    }
}

// ✅ SEGURO
auto it = widgets_.begin();
while (it != widgets_.end()) {
    if (condition) {
        it = widgets_.erase(it);
    } else {
        ++it;
    }
}
```

## Template de Review Comment

```markdown
## Issue: [Descrição breve]

**Localização**: `showbox.cc:123`

**Problema**:
[Explicar o problema detalhadamente]

**Sugestão**:
\`\`\`cpp
// Código sugerido
\`\`\`

**Razão**:
[Explicar por que a sugestão é melhor]

**Severidade**: 🔴 Critical / 🟡 Warning / 🟢 Suggestion
```

## Aprovação de PR

### ✅ Aprovar quando

- Todos os itens da checklist OK
- Testes passando
- Sem warnings de compilação
- Code coverage mantido/melhorado
- Documentação adequada

### ⏸️ Request Changes quando

- Bugs identificados
- Memory leaks potenciais
- Violação de padrões do projeto
- Falta de testes críticos

### 💬 Comment quando

- Sugestões não-bloqueantes
- Melhorias de estilo
- Questões de clareza

---

**Lembre-se**: Code review não é sobre ser crítico, mas sobre manter a qualidade e ajudar o time a crescer!
