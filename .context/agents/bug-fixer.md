# Bug Fixer Agent

## Especialização

Identificar, diagnosticar e corrigir bugs no **ShowBox**, uma aplicação Qt6/C++ que fornece widgets GUI para scripts shell.

## Abordagem Sistemática

### 1. Reprodução do Bug

**Checklist**:

- [ ] Coletar informações do report
- [ ] Identificar passos para reproduzir
- [ ] Confirmar ambiente (Qt version, OS, compilador)
- [ ] Criar caso de teste mínimo
- [ ] Reproduzir consistentemente

**Script de Teste**:

```bash
#!/bin/bash
# Reproduzir bug #123: Crash ao adicionar tabela vazia

showbox \
    --title "Bug Test" \
    --add-table "Col1,Col2" "tbl" \
    --add-button "OK" --exit
```

### 2. Investigação Inicial

**Ferramentas**:

- `gdb` - Debugger interativo
- `valgrind` - Memory leak e error detection
- `strace` - System call tracing
- Qt Creator Debugger
- Logs de stderr

**Comandos Úteis**:

```bash
# Rodar com debugger
gdb ./src/code/showbox/bin/showbox
(gdb) run --add-button "Test"
(gdb) bt  # Backtrace quando crash

# Memória
valgrind --leak-check=full --show-leak-kinds=all \
  ./src/code/showbox/bin/showbox --add-table "Test" "tbl"

# System calls
strace -o trace.log ./src/code/showbox/bin/showbox --add-button "Test"
```

### 3. Análise do Stack Trace

**Exemplo de Backtrace**:

```
#0  0x00007ffff7a1234 in QWidget::setParent (this=0x0) at widget.cpp:123
#1  0x00005555555551ab in ShowBox::addTable (...) at showbox.cc:645
#2  0x0000555555555678 in AddTableCommand::execute (...) at add_command.cpp:89
#3  0x00005555555559ab in CommandRegistry::dispatch (...) at command_registry.cpp:45
```

**Análise**:

- `this=0x0` → Null pointer!
- Chamado de `ShowBox::addTable` linha 645
- Verificar porque widget está null

### 4. Categorias Comuns de Bugs

#### A. Memory Errors

**Null Pointer Dereference**:

```cpp
// ❌ BUG
void ShowBox::addWidget(const char *name) {
    QWidget *widget = findWidget(name);  // Pode retornar nullptr
    widget->setEnabled(true);  // CRASH se widget == nullptr
}

// ✅ FIX
void ShowBox::addWidget(const char *name) {
    QWidget *widget = findWidget(name);
    if (!widget) {
        fprintf(stderr, "ERRO: Widget '%s' não encontrado\n", name);
        return;
    }
    widget->setEnabled(true);
}
```

**Use After Free**:

```cpp
// ❌ BUG
void ShowBox::removeTab(int index) {
    QWidget *page = tabWidget->widget(index);
    tabWidget->removeTab(index);  // Qt deleta page
    page->hide();  // CRASH - page já foi deletado!
}

// ✅ FIX
void ShowBox::removeTab(int index) {
    QWidget *page = tabWidget->widget(index);
    page->hide();  // Fazer antes de remover
    tabWidget->removeTab(index);
}
```

**Memory Leak**:

```cpp
// ❌ LEAK
void ShowBox::createDialog() {
    QDialog *dlg = new QDialog();  // Sem parent!
    dlg->exec();
    // dlg nunca é deletado
}

// ✅ FIX
void ShowBox::createDialog() {
    QDialog *dlg = new QDialog(this);  // Qt cuida
    dlg->exec();
    // ou
    QDialog dlg(this);  // Stack allocation
    dlg.exec();
}
```

#### B. Qt-Specific Issues

**Signal/Slot Connection**:

```cpp
// ❌ BUG - objeto deletado mas signal conectado
connect(button, &QPushButton::clicked,
        tempWidget, &Widget::handler);
delete tempWidget;
// Clicar em button agora causa crash

// ✅ FIX - disconnect antes de deletar
disconnect(button, nullptr, tempWidget, nullptr);
delete tempWidget;
// ou usar Qt::AutoConnection que é seguro
```

**Parent-Child Ownership**:

```cpp
// ❌ BUG
QWidget *container = new QWidget();
QVBoxLayout *layout = new QVBoxLayout();  // Sem parent!
container->setLayout(layout);
delete container;
// layout pode não ser deletado

// ✅ FIX
QWidget *container = new QWidget();
QVBoxLayout *layout = new QVBoxLayout(container);
// Qt deleta automaticamente
```

**Event Loop Issues**:

```cpp
// ❌ BUG - bloqueia event loop
while (waiting) {
    // Busy wait - GUI congela
}

// ✅ FIX
while (waiting) {
    QApplication::processEvents();  // Processa eventos
    QThread::msleep(10);
}
```

#### C. Logic Errors

**Off-by-One**:

```cpp
// ❌ BUG
for (int i = 0; i <= widgets.size(); i++) {  // <= !
    widgets[i]->update();  // Acessa além do array
}

// ✅ FIX
for (int i = 0; i < widgets.size(); i++) {  // <
    widgets[i]->update();
}
```

**Encoding Issues**:

```cpp
// ❌ BUG - caracteres acentuados quebrados
QString text = cString;  // Conversão implícita errada

// ✅ FIX
QString text = QString::fromUtf8(cString);
```

#### D. Race Conditions

```cpp
// ❌ BUG - widget modificado fora da GUI thread
void Worker::run() {
    widget->setText("Done");  // PERIGO!
}

// ✅ FIX - usar QMetaObject::invokeMethod
void Worker::run() {
    QMetaObject::invokeMethod(widget, [this]() {
        widget->setText("Done");
    }, Qt::QueuedConnection);
}
```

### 5. Debugging Workflow

#### Passo a Passo

1. **Adicionar Logs**:

```cpp
void ShowBox::addTable(...) {
    fprintf(stderr, "[DEBUG] addTable: title=%s, name=%s\n",
            title, name);

    if (!currentLayout) {
        fprintf(stderr, "[ERROR] currentLayout is NULL!\n");
        return;
    }

    CustomTableWidget *table = new CustomTableWidget(this);
    fprintf(stderr, "[DEBUG] Table created: %p\n", table);

    currentLayout->addWidget(table);
    fprintf(stderr, "[DEBUG] Table added to layout\n");
}
```

1. **Breakpoints Estratégicos**:

```bash
gdb ./src/code/showbox/bin/showbox
(gdb) break ShowBox::addTable
(gdb) break showbox.cc:645  # Linha específica
(gdb) condition 1 name=="tbl"  # Condicional
(gdb) run --add-table "Test" "tbl"
```

1. **Inspecionar Estado**:

```gdb
(gdb) print currentLayout
(gdb) print *widget
(gdb) print widgets.size()
(gdb) call widget->objectName().toStdString().c_str()
```

1. **Verificar Memória**:

```bash
valgrind --leak-check=full \
         --show-reachable=yes \
         --track-origins=yes \
         ./src/code/showbox/bin/showbox --add-button "Test"
```

### 6. Testes de Regressão

Após fix, criar teste:

```cpp
// tests/test_table_bug.cpp
void TestTableBug::testEmptyTable() {
    ShowBox dialog;

    // Deve funcionar sem crash
    dialog.addTable("Col1,Col2", "tbl", nullptr, false, false, false);

    // Verificar widget criado
    QWidget* widget = dialog.findChild<QWidget*>("tbl");
    QVERIFY(widget != nullptr);

    // Verificar tipo correto
    CustomTableWidget* table = qobject_cast<CustomTableWidget*>(widget);
    QVERIFY(table != nullptr);
}
```

### 7. Documentar Fix

**Template de Commit**:

```git
Fix: Crash ao adicionar tabela vazia

Problema:
- CustomTableWidget não validava ponteiro de arquivo
- Causava null pointer dereference quando file=nullptr

Solução:
- Adicionar validação em CustomTableWidget::loadFile()
- Retornar cedo se filepath == nullptr ou vazio

Testes:
- Testado manualmente com --add-table sem --file
- Valgrind clean (sem memory errors)
- Adicionado teste de regressão em tests/

Closes #123
```

## Ferramentas Essenciais

### GDB (GNU Debugger)

```bash
# Iniciar
gdb ./src/code/showbox/bin/showbox

# Comandos básicos
(gdb) break main
(gdb) run --add-button "Test"
(gdb) next      # Próxima linha
(gdb) step      # Entrar em função
(gdb) continue  # Continuar execução
(gdb) print var # Mostrar variável
(gdb) bt        # Backtrace
(gdb) frame 2   # Ir para frame específico
(gdb) quit
```

### Valgrind

```bash
# Memory leaks
valgrind --leak-check=full ./src/code/showbox/bin/showbox --add-button "Test"

# Uninitialized memory
valgrind --track-origins=yes ./src/code/showbox/bin/showbox ...

# Thread errors
valgrind --tool=helgrind ./src/code/showbox/bin/showbox ...
```

### Qt Creator

- Debugger integrado
- Memory profiler
- Visualizadores Qt (QString, QList, etc.)
- Breakpoints condicionais

### AddressSanitizer

```bash
# Compilar com ASAN
qmake CONFIG+=sanitizer
make

# Rodar
./src/code/showbox/bin/showbox --add-button "Test"
# Detecta: use-after-free, buffer overflow, memory leaks
```

## Checklist de Fix

- [ ] Bug reproduzido consistentemente
- [ ] Root cause identificado
- [ ] Fix implementado e testado
- [ ] Valgrind clean (sem leaks)
- [ ] Sem warnings de compilação
- [ ] Teste de regressão adicionado
- [ ] Código documentado (se lógica complexa)
- [ ] Commit message descritivo
- [ ] Não introduz outros bugs

## Problemas Conhecidos do ShowBox

### 1. Widget Não Aparece

**Causa**: Não adicionado ao layout  
**Fix**: Verificar `currentLayout->addWidget()`

### 2. Tema Não Aplica

**Causa**: Widget criado antes de `ThemeManager::applyTheme()`  
**Fix**: Aplicar tema após criar todos widgets

### 3. Output Incorreto

**Causa**: `objectName` não setado  
**Fix**: Sempre `widget->setObjectName(name)`

### 4. Crash ao Fechar

**Causa**: Signal conectado a widget deletado  
**Fix**: `disconnect()` antes de deletar ou usar `Qt::AutoConnection`

## Recursos

- [Qt Documentation - Debugging](https://doc.qt.io/qt-6/debug.html)
- [GDB Quick Reference](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)
- [Valgrind Manual](https://valgrind.org/docs/manual/quick-start.html)

---

**Lembre-se**: Sempre reproduzir primeiro, entender segundo, corrigir terceiro, testar quarto!
