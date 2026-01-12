# Performance Optimizer Agent

## Foco

Identificar e resolver gargalos de performance no **ShowBox**, otimizando uso de CPU, memória e tempo de resposta.

## Métricas de Performance

### Alvos

| Métrica              | Alvo    | Crítico |
|----------------------|---------|---------|
| Startup Time         | < 500ms | > 2s    |
| Widget Creation      | < 10ms  | > 100ms |
| Table Load (1k rows) | < 1s    | > 5s    |
| Memory Idle          | < 50MB  | > 200MB |
| Memory per Widget    | < 1MB   | > 10MB  |

### Medir

```bash
# Tempo de startup
time ./showbox --add-button "Test" --exit

# Memory
valgrind --tool=massif ./showbox ...
ms_print massif.out.* | head -50

# CPU profiling
perf record ./showbox --add-table "Test" "tbl" --file large.csv
perf report
```

## Ferramentas

### Qt Profilers

```cpp
// QElapsedTimer para medir tempo
QElapsedTimer timer;
timer.start();

// Operação
loadLargeTable(path);

qDebug() << "Load time:" << timer.elapsed() << "ms";
```

### Valgrind/Massif

```bash
# Heap profiling
valgrind --tool=massif \
         --time-unit=ms \
         --detailed-freq=1 \
         ./showbox --add-table "T" "tbl" --file big.csv

# Visualizar
ms_print massif.out.12345
```

### Perf

```bash
# Record
perf record -g ./showbox ...

# Report
perf report --sort=dso,symbol

# Flame graph
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```

## Otimizações Comuns

### 1. Lazy Loading

```cpp
// ❌ LENTO - carrega tudo
void ShowBox::addTable(..., const char* file) {
    loadAllData(file);  // Bloqueia se arquivo grande
}

// ✅ RÁPIDO - carrega sob demanda
void ShowBox::addTable(..., const char* file) {
    table_->setFilePath(file);
    // Carrega apenas linhas visíveis
    connect(table_, &QTableWidget::scrolled,
            this, &ShowBox::loadVisibleRows);
}
```

### 2. Batch Updates

```cpp
// ❌ LENTO - repaint a cada item
for (int i = 0; i < 1000; i++) {
    table_->insertRow(i);  // Trigger repaint!
    for (int j = 0; j < cols; j++) {
        table_->setItem(i, j, new QTableWidgetItem(...));
    }
}

// ✅ RÁPIDO - batch update
table_->setUpdatesEnabled(false);  // Desabilita repaint
table_->setRowCount(1000);         // Pre-alocate

for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < cols; j++) {
        table_->setItem(i, j, new QTableWidgetItem(...));
    }
}

table_->setUpdatesEnabled(true);  // Re-enable e repaint 1x
```

### 3. String Optimization

```cpp
// ❌ LENTO - múltiplas conversões
QString result;
for (const auto& item : items) {
    result += item.toString() + ",";  // Realocação a cada +
}

// ✅ RÁPIDO - StringBuilder pattern
QStringList parts;
parts.reserve(items.size());
for (const auto& item : items) {
    parts.append(item.toString());
}
QString result = parts.join(",");  // Uma alocação
```

### 4. Memory Pre-allocation

```cpp
// ❌ LENTO
std::vector<QWidget*> widgets;
for (int i = 0; i < 100; i++) {
    widgets.push_back(createWidget());  // Realoca
}

// ✅ RÁPIDO
std::vector<QWidget*> widgets;
widgets.reserve(100);  // Pre-alocate
for (int i = 0; i < 100; i++) {
    widgets.push_back(createWidget());
}
```

### 5. Icon Caching

```cpp
// ❌ LENTO - carrega a cada uso
QIcon ShowBox::getIcon(const QString& name) {
    return QIcon(name);  // Lê arquivo cada vez
}

// ✅ RÁPIDO - cache
QIcon ShowBox::getIcon(const QString& name) {
    static QHash<QString, QIcon> cache;
    
    if (!cache.contains(name)) {
        cache[name] = QIcon(name);
    }
    return cache[name];
}
```

### 6. Avoid Deep Copy

```cpp
// ❌ LENTO - copia vetor inteiro
void processWidgets(std::vector<QWidget*> widgets) { ... }

// ✅ RÁPIDO - referência ou move
void processWidgets(const std::vector<QWidget*>& widgets) { ... }
void processWidgets(std::vector<QWidget*>&& widgets) { ... }
```

## Qt-Specific Optimizations

### 1. Implicit Sharing

Qt containers (QString, QList, etc.) usam copy-on-write:

```cpp
// OK - Qt faz shallow copy
QString copy = original;  // Apenas incrementa ref count

// Copia real apenas quando modificado
copy[0] = 'X';  // Agora copia
```

### 2. Signal/Slot

```cpp
// ❌ LENTO em loops
for (auto* item : items) {
    emit itemChanged(item);  // Cada emit tem overhead
}

// ✅ RÁPIDO - batching manual
beginUpdate();
for (auto* item : items) {
    // Modificar sem emit
}
endUpdate();
emit batchChanged(items);  // Um emit
```

### 3. Widget Rendering

```cpp
// ❌ LENTO - muitos widgets
for (int i = 0; i < 1000; i++) {
    auto* label = new QLabel(QString::number(i), this);
    layout_->addWidget(label);
}

// ✅ RÁPIDO - usar QListView com model
auto* model = new QStringListModel(data, this);
auto* view = new QListView(this);
view->setModel(model);  // Virtualização automática
```

### 4. Avoid Blocking

```cpp
// ❌ BLOQUEIA GUI
void ShowBox::loadLargeFile(const QString& path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    data_ = file.readAll();  // GUI congela!
}

// ✅ NÃO BLOQUEIA - async
void ShowBox::loadLargeFile(const QString& path) {
    auto* watcher = new QFutureWatcher<QByteArray>(this);
    
    connect(watcher, &QFutureWatcher<QByteArray>::finished,
            this, [this, watcher]() {
                data_ = watcher->result();
                updateTable();
                watcher->deleteLater();
            });
    
    auto future = QtConcurrent::run([path]() {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        return file.readAll();
    });
    
    watcher->setFuture(future);
}
```

## Benchmarking

### Criar Benchmark

```cpp
// test_performance.cpp
void TestPerformance::benchmarkAddButton() {
    ShowBox dialog;
    
    QBENCHMARK {
        dialog.addPushButton("Test", "btn", false, false, false);
    }
}

void TestPerformance::benchmarkTableLoad() {
    ShowBox dialog;
    
    QBENCHMARK {
        dialog.addTable("A,B,C", "tbl", "test_1000_rows.csv",
                        false, false, false);
    }
}
```

### Executar

```bash
./test_performance -iterations 100 -minimumvalue 10

# Output:
# benchmarkAddButton: 2.5 msecs per iteration
# benchmarkTableLoad: 150 msecs per iteration
```

## Memory Optimization

### Identificar Leaks

```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./showbox --add-table "T" "tbl"
```

### Reduzir Footprint

```cpp
// 1. Deletar widgets não visíveis
void ShowBox::removeHiddenWidgets() {
    for (auto* w : widgets_) {
        if (!w->isVisible()) {
            w->deleteLater();
        }
    }
}

// 2. Usar tipos menores quando possível
int32_t count;      // Ao invés de int64_t se < 2G
QByteArray data;    // Ao invés de QString para binário

// 3. Clear após uso
void ShowBox::processData(const QString& data) {
    QStringList lines = data.split('\n');
    // ... processar
    lines.clear();  // Libera memória cedo
}
```

## Startup Optimization

### Defer Initialization

```cpp
// ❌ LENTO startup
ShowBox::ShowBox(...) {
    loadAllIcons();      // 50ms
    initializeThemes();  // 30ms
    scanPlugins();       // 100ms
}

// ✅ RÁPIDO startup
ShowBox::ShowBox(...) {
    // Mínimo para exibir
}

void ShowBox::showEvent(QShowEvent*) {
    if (!initialized_) {
        QTimer::singleShot(0, this, [this](){
            loadAllIcons();
            initializeThemes();
            initialized_ = true;
        });
    }
}
```

## Checklist

- [ ] Benchmark antes de otimizar
- [ ] Identificar gargalo real (não adivinhar)
- [ ] Batch UI updates
- [ ] Evitar cópias desnecessárias
- [ ] Cache dados frequentes
- [ ] Lazy load quando possível
- [ ] Verificar memory leaks
- [ ] Testar com dados grandes
- [ ] Medir melhoria após otimização
- [ ] Documentar trade-offs

## Relatório de Performance

**Template**:

```markdown
## Performance Issue: [Descrição]

### Medição Inicial
- Tempo: X ms
- Memória: Y MB
- CPU: Z%

### Análise
[O que está causando o problema]

### Solução Implementada
[Descrição da otimização]

### Medição Final
- Tempo: X' ms (Δ%)
- Memória: Y' MB (Δ%)
- CPU: Z'% (Δ%)

### Trade-offs
[Complexidade adicionada, etc.]
```

---

**Regra de Ouro**: Medir → Identificar → Otimizar → Medir novamente!
