# Test Writer Agent

## Missão

Criar testes automatizados robustos para o **ShowBox** usando Qt Test framework.

## Framework de Testes

### Qt Test (QTest)

ShowBox usa **Qt Test**, framework oficial para testes Qt/C++.

**Estrutura**:

```shell
tests/
├── auto/
│   ├── test_showbox/
│   │   ├── test_showbox.pro
│   │   ├── test_showbox.cpp
│   │   └── test_showbox.h
│   ├── test_commands/
│   ├── test_parser/
│   └── test_widgets/
├── manual/
└── run_tests.sh
```

## Anatomia de um Teste

### Template Básico

```cpp
// test_showbox.h
#include <QtTest/QtTest>
#include "showbox.h"

class TestShowBox : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();    // Antes de todos os testes
    void init();            // Antes de cada teste
    void cleanup();         // Após cada teste
    void cleanupTestCase(); // Após todos os testes
    
    // Testes
    void testAddButton();
    void testAddTable();
    void testTheme();
};
```

```cpp
// test_showbox.cpp
#include "test_showbox.h"

void TestShowBox::initTestCase() {
    // Setup único
    qDebug() << "Starting ShowBox tests";
}

void TestShowBox::init() {
    // Setup antes de cada teste
}

void TestShowBox::testAddButton() {
    ShowBox dialog("Test", nullptr, false, stdout);
    
    // Adicionar botão
    dialog.addPushButton("OK", "btn_ok", false, true, false);
    
    // Verificar widget criado
    QPushButton* btn = dialog.findChild<QPushButton*>("btn_ok");
    QVERIFY(btn != nullptr);
    
    // Verificar propriedades
    QCOMPARE(btn->text(), QString("OK"));
}

void TestShowBox::cleanup() {
    // Limpeza após cada teste
}

void TestShowBox::cleanupTestCase() {
    qDebug() << "Finished ShowBox tests";
}

QTEST_MAIN(TestShowBox)
#include "test_showbox.moc"
```

### Arquivo .pro

```qmake
QT += testlib widgets svg charts
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += test_showbox.cpp \
           ../../../src/code/showbox/showbox.cc \
           ../../../src/code/showbox/dialog_main.cc
           
HEADERS += test_showbox.h \
           ../../../src/code/showbox/showbox.h

TARGET = test_showbox
```

## Macros do QTest

### Verificações (Assertions)

```cpp
// Verificar verdadeiro/falso
QVERIFY(condition);
QVERIFY2(condition, "Mensagem de erro");

// Comparar valores
QCOMPARE(actual, expected);
QCOMPARE(widget->text(), QString("Expected"));

// Verificar tipos
QVERIFY(qobject_cast<QPushButton*>(widget) != nullptr);

// Fuzzy compare (floats)
QCOMPARE(3.14, 3.14159);  // Com tolerância

// Verificar exceptions
QVERIFY_EXCEPTION_THROWN(code, ExceptionType);

// Skip teste
QSKIP("Not implemented yet");

// Falha explícita
QFAIL("This should not happen");

// Warnings esperados
QTest::ignoreMessage(QtWarningMsg, "Expected warning");
```

### Test Data

```cpp
void TestShowBox::testAddButton_data() {
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("exit");
    
    QTest::newRow("ok-button") << "OK" << "btn_ok" << true;
    QTest::newRow("cancel-button") << "Cancel" << "btn_cancel" << false;
    QTest::newRow("empty-title") << "" << "btn_empty" << false;
}

void TestShowBox::testAddButton() {
    QFETCH(QString, title);
    QFETCH(QString, name);
    QFETCH(bool, exit);
    
    ShowBox dialog;
    dialog.addPushButton(title.toUtf8(), name.toUtf8(), false, exit, false);
    
    QPushButton* btn = dialog.findChild<QPushButton*>(name);
    QVERIFY(btn != nullptr);
    QCOMPARE(btn->text(), title);
}
```

### Benchmarks

```cpp
void TestShowBox::benchmarkAddWidget() {
    ShowBox dialog;
    
    QBENCHMARK {
        dialog.addPushButton("Test", "btn", false, false, false);
    }
}
```

## Categorias de Testes

### 1. Testes Unitários

**Testar classe/método isoladamente**:

```cpp
void TestParser::testTokenize() {
    Tokenizer tokenizer;
    
    QStringList args = {"--title", "Test", "--add-button", "OK"};
    std::vector<Token> tokens = tokenizer.tokenize(args);
    
    QCOMPARE(tokens.size(), 4);
    QCOMPARE(tokens[0].type, Token::OPTION);
    QCOMPARE(tokens[0].value, QString("--title"));
    QCOMPARE(tokens[1].type, Token::STRING);
    QCOMPARE(tokens[1].value, QString("Test"));
}
```

### 2. Testes de Integração

**Testar interação entre componentes**:

```cpp
void TestIntegration::testCommandExecution() {
    // Setup
    ShowBox dialog;
    CommandRegistry registry;
    ExecutionContext ctx(&dialog);
    
    // Criar comando
    AddButtonCommand cmd("OK", "btn_ok", false, true, false);
    
    // Executar
    cmd.execute(ctx);
    
    // Verificar resultado
    QPushButton* btn = dialog.findChild<QPushButton*>("btn_ok");
    QVERIFY(btn != nullptr);
}
```

### 3. Testes de Widget

**Testar comportamento GUI**:

```cpp
void TestWidgets::testButtonClick() {
    ShowBox dialog;
    dialog.addPushButton("OK", "btn_ok", false, true, false);
    
    QPushButton* btn = dialog.findChild<QPushButton*>("btn_ok");
    QVERIFY(btn != nullptr);
    
    // Simular clique
    QSignalSpy spy(btn, &QPushButton::clicked);
    QTest::mouseClick(btn, Qt::LeftButton);
    
    // Verificar signal emitido
    QCOMPARE(spy.count(), 1);
}
```

### 4. Testes de Memória

**Verificar leaks com Valgrind**:

```bash
#!/bin/bash
# tests/check_memory.sh

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --error-exitcode=1 \
         ./test_showbox

if [ $? -eq 0 ]; then
    echo "✅ No memory leaks detected"
else
    echo "❌ Memory leaks found!"
    exit 1
fi
```

### 5. Testes de CLI

**Testar interface linha de comando**:

```cpp
void TestCLI::testFullCommand() {
    QProcess process;
    process.start("./showbox", {
        "--title", "Test",
        "--add-button", "OK",
        "--exit"
    });
    
    process.waitForStarted();
    process.waitForFinished(5000);
    
    QCOMPARE(process.exitCode(), 0);
}
```

## Padrões de Teste

### Setup e Teardown

```cpp
class TestShowBox : public QObject {
    Q_OBJECT
    
private:
    ShowBox* dialog;
    
private slots:
    void init() {
        dialog = new ShowBox("Test", nullptr, false, stdout);
    }
    
    void cleanup() {
        delete dialog;
        dialog = nullptr;
    }
    
    void testSomething() {
        // dialog já está criado
        dialog->addPushButton("OK", "btn", false, false, false);
        QVERIFY(dialog->findChild<QPushButton*>("btn") != nullptr);
    }
};
```

### Fixtures

```cpp
class WidgetFixture {
public:
    ShowBox dialog;
    QPushButton* button;
    
    WidgetFixture() : dialog("Test", nullptr, false, stdout) {
        dialog.addPushButton("OK", "btn_ok", false, false, false);
        button = dialog.findChild<QPushButton*>("btn_ok");
    }
};

void TestWidgets::testWithFixture() {
    WidgetFixture fixture;
    QVERIFY(fixture.button != nullptr);
    QCOMPARE(fixture.button->text(), QString("OK"));
}
```

### Mocks (Simulação)

```cpp
class MockOutputStream {
public:
    QStringList lines;
    
    static int write(const char* str) {
        lines.append(QString::fromUtf8(str));
        return 0;
    }
};

void TestOutput::testWidgetOutput() {
    MockOutputStream mockOut;
    ShowBox dialog("Test", nullptr, false, nullptr);
    
    dialog.addTextBox("Name", "txt_name", "John", nullptr, false);
    
    // Simular output
    dialog.outputValues();
    
    QVERIFY(mockOut.lines.contains("txt_name=John"));
}
```

## Cobertura de Testes

### Gerar Relatório

```bash
# Compilar com coverage
qmake CONFIG+=coverage
make

# Rodar testes
./test_showbox

# Gerar relatório
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html

# Abrir relatório
firefox coverage_html/index.html
```

### Target de Cobertura

- **Métodos críticos**: 100%
- **Código total**: >= 80%
- **Branches**: >= 70%

## Testes de Regressão

**Sempre adicionar teste quando corrigir bug**:

```cpp
// Bug #123: Crash ao adicionar tabela vazia
void TestBugFixes::testBug123_EmptyTable() {
    ShowBox dialog;
    
    // Não deve crashar
    dialog.addTable("Col1,Col2", "tbl", nullptr, false, false, false);
    
    CustomTableWidget* table = dialog.findChild<CustomTableWidget*>("tbl");
    QVERIFY(table != nullptr);
    QCOMPARE(table->rowCount(), 0);
}
```

## Continuous Integration

### GitHub Actions

```yaml
# .github/workflows/tests.yml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Qt
        run: sudo apt-get install -y qt6-base-dev qt6-charts-dev
      
      - name: Build Tests
        run: |
          cd tests/auto
          qmake6 && make
      
      - name: Run Tests
        run: |
          cd tests
          ./run_tests.sh
      
      - name: Check Memory
        run: |
          sudo apt-get install -y valgrind
          ./tests/check_memory.sh
```

## Checklist de Teste Completo

- [ ] Teste unitário criado
- [ ] Teste de integração (se aplicável)
- [ ] Test data para múltiplos casos
- [ ] Edge cases cobertos
- [ ] Memory leaks verificados
- [ ] Performance acceptable
- [ ] Testes passando localmente
- [ ] Testes passando em CI
- [ ] Cobertura adequada

---

**Lei do Teste**: Se não está testado, está quebrado!
