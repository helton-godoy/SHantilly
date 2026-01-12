# Security Auditor Agent

## Foco

Auditar e melhorar a segurança do **ShowBox**, identificando vulnerabilidades e implementando mitigações.

## Superfície de Ataque

### Entradas do ShowBox

```diagram
┌─────────────────────────────────────────────┐
│  CLI Arguments (argv)                       │
│  - Flags e valores de comandos              │
│  - Caminhos de arquivos                     │
│  - Dados de texto (títulos, labels)         │
├─────────────────────────────────────────────┤
│  Files (stdin, --file)                      │
│  - CSV/TSV para tabelas                     │
│  - Imagens para ícones                      │
│  - Arquivos de texto para TextView          │
├─────────────────────────────────────────────┤
│  Environment Variables                      │
│  - DISPLAY (X11)                            │
│  - HOME, USER                               │
│  - SHOWBOX_THEME (futuro)                   │
└─────────────────────────────────────────────┘
```

## Checklist de Auditoria

### 1. Input Validation

```cpp
// Verificar TODAS as entradas de usuário

// ✅ Validar nome de widget
void ShowBox::addWidget(const char* name) {
    if (!name || !*name) {
        fprintf(stderr, "ERRO: nome obrigatório\n");
        return;
    }
    
    // Limitar comprimento
    if (strlen(name) > MAX_NAME_LENGTH) {
        fprintf(stderr, "ERRO: nome muito longo\n");
        return;
    }
    
    // Sanitizar caracteres
    QString qName = QString::fromUtf8(name);
    if (qName.contains(QRegularExpression("[<>\"']"))) {
        fprintf(stderr, "ERRO: caracteres inválidos\n");
        return;
    }
}
```

### 2. Buffer Overflow

```cpp
// Usar containers Qt ao invés de buffers C

// ❌ VULNERÁVEL
char buffer[256];
strcpy(buffer, userInput);  // Overflow se > 256

// ✅ SEGURO
QString buffer = QString::fromUtf8(userInput);
// QString gerencia memória automaticamente
```

### 3. Path Traversal

```cpp
// Verificar caminhos de arquivo

bool ShowBox::loadFile(const QString& path) {
    QFileInfo info(path);
    
    // Canonicalizar (resolve .., symlinks)
    QString canonical = info.canonicalFilePath();
    
    // Verificar se existe e é arquivo regular
    if (!info.exists() || !info.isFile()) {
        fprintf(stderr, "ERRO: arquivo inválido\n");
        return false;
    }
    
    // Verificar permissões
    if (!info.isReadable()) {
        fprintf(stderr, "ERRO: sem permissão\n");
        return false;
    }
    
    // OPCIONAL: Restringir a diretório específico
    // if (!canonical.startsWith("/home/user/allowed/")) {
    //     return false;
    // }
    
    QFile file(canonical);
    return file.open(QIODevice::ReadOnly);
}
```

### 4. Integer Overflow

```cpp
// Verificar operações numéricas

// ❌ VULNERÁVEL
int totalSize = width * height;  // Pode overflow

// ✅ SEGURO
if (width > 0 && height > INT_MAX / width) {
    fprintf(stderr, "ERRO: tamanho excede limite\n");
    return;
}
int totalSize = width * height;
```

### 5. Memory Safety

```cpp
// Verificar uso de ponteiros

// ❌ VULNERÁVEL - use after free
QWidget* widget = findWidget(name);
delete widget;
widget->update();  // CRASH

// ❌ VULNERÁVEL - null dereference
QWidget* widget = findWidget(name);
widget->show();  // CRASH se nullptr

// ✅ SEGURO
QWidget* widget = findWidget(name);
if (widget) {
    widget->show();
    // Use deleteLater para Qt objects
    widget->deleteLater();
}
```

### 6. Resource Exhaustion

```cpp
// Limitar recursos

// Tamanho máximo de arquivo
const qint64 MAX_FILE_SIZE = 100 * 1024 * 1024;  // 100MB

bool loadData(const QString& path) {
    QFileInfo info(path);
    if (info.size() > MAX_FILE_SIZE) {
        fprintf(stderr, "ERRO: arquivo muito grande\n");
        return false;
    }
    // ...
}

// Número máximo de widgets
const int MAX_WIDGETS = 1000;

void ShowBox::addWidget(QWidget* w) {
    if (widgets_.size() >= MAX_WIDGETS) {
        fprintf(stderr, "ERRO: muitos widgets\n");
        return;
    }
    widgets_.push_back(w);
}

// Timeout para operações longas
QTimer::singleShot(30000, this, [this]() {
    if (stillLoading_) {
        cancelLoad();
        fprintf(stderr, "ERRO: timeout\n");
    }
});
```

## Ferramentas de Análise

### Static Analysis

```bash
# Cppcheck
cppcheck --enable=all \
         --suppress=missingIncludeSystem \
         src/code/showbox/

# Clang Static Analyzer
scan-build qmake && scan-build make

# Clang-Tidy
clang-tidy src/code/showbox/*.cc \
  -checks='*,-llvm*,-fuchsia*' \
  -- -I/usr/include/qt6
```

### Dynamic Analysis

```bash
# AddressSanitizer (buffer overflow, use-after-free)
qmake CONFIG+=sanitizer SANITIZERS=address
make
./showbox --add-button "Test"

# UndefinedBehaviorSanitizer
qmake CONFIG+=sanitizer SANITIZERS=undefined
make

# MemorySanitizer (uninitialized memory)
qmake CONFIG+=sanitizer SANITIZERS=memory
make
```

### Fuzzing

```cpp
// Fuzzer para parser (libFuzzer)
extern "C" int LLVMFuzzerTestOneInput(
    const uint8_t* data, size_t size) {
    
    std::string input(reinterpret_cast<const char*>(data), size);
    QStringList args = QString::fromStdString(input).split(' ');
    
    Tokenizer tok;
    Parser parser;
    
    try {
        auto tokens = tok.tokenize(args);
        parser.parse(tokens);
    } catch (...) {
        // Não deve crashar
    }
    
    return 0;
}
```

```bash
# Compilar com fuzzing
clang++ -fsanitize=fuzzer,address parser_fuzz.cpp \
  parser.cpp tokenizer.cpp -o parser_fuzzer

# Rodar
mkdir corpus
./parser_fuzzer corpus/ -max_len=1000
```

## Vulnerabilidades Específicas Qt

### 1. QProcess

```cpp
// ❌ VULNERÁVEL - command injection
QString cmd = QString("ls %1").arg(userInput);
QProcess::execute(cmd);  // Se userInput = "; rm -rf /"

// ShowBox NÃO usa QProcess para executar comandos do usuário
// Mas se precisar no futuro:

// ✅ SEGURO - argumentos separados
QProcess process;
process.start("ls", {userPath});  // userPath é argumento, não comando
```

### 2. QWebEngine

```cpp
// Se ShowBox adicionar componente web:

// ❌ VULNERÁVEL - XSS
webView->setHtml(QString("<p>%1</p>").arg(userInput));

// ✅ SEGURO - escape
QString safe = userInput.toHtmlEscaped();
webView->setHtml(QString("<p>%1</p>").arg(safe));
```

### 3. QSettings

```cpp
// Se ShowBox salvar configurações:

// Usar apenas tipos primitivos
settings.setValue("theme", themeName);
settings.setValue("size", windowSize);

// NÃO serializar objetos complexos de entrada do usuário
```

## Relatório de Vulnerabilidade

**Template**:

```markdown
## Vulnerabilidade: [Nome]

### Severidade
[Crítica | Alta | Média | Baixa]

### Descrição
[O que é a vulnerabilidade]

### Localização
- Arquivo: `showbox.cc`
- Função: `ShowBox::loadFile()`
- Linha: 234

### Reprodução
1. Executar: `showbox --add-table "T" "t" --file "../../../etc/passwd"`
2. Observar: [resultado]

### Impacto
[O que um atacante pode fazer]

### Mitigação
[Como corrigir]

### Status
[ ] Reportado
[ ] Confirmado
[ ] Corrigido
[ ] Verificado
[ ] Deployed
```

## Processo de Disclosure

### Responsável

1. **Não** abrir issue público
2. Enviar para: <security@showbox.dev>
3. Incluir:
   - Descrição detalhada
   - PoC se possível
   - Impacto estimado
4. Aguardar confirmação (48h)
5. Coordenar disclosure

### Timeline

- T+0: Vulnerabilidade reportada
- T+48h: Confirmação de recebimento
- T+7d: Avaliação inicial
- T+30d: Patch disponível (depende da severidade)
- T+90d: Divulgação pública coordenada

## Hardening

### Compilação

```bash
# Flags de segurança
QMAKE_CXXFLAGS += -fstack-protector-strong
QMAKE_CXXFLAGS += -D_FORTIFY_SOURCE=2
QMAKE_CXXFLAGS += -Wformat -Wformat-security
QMAKE_LFLAGS += -Wl,-z,relro,-z,now
```

### Sandboxing (Flatpak)

```yaml
# Permissões mínimas
finish-args:
  - --filesystem=home:ro     # Apenas leitura
  - --share=ipc              # X11
  - --socket=wayland
  # SEM --share=network      # Não precisa rede
```

## Checklist de Auditoria

### Código

- [ ] Todas entradas validadas
- [ ] Sem buffer overflows possíveis
- [ ] Paths canonicalizados
- [ ] Recursos limitados
- [ ] Ponteiros verificados
- [ ] Sem comandos shell executados
- [ ] Encoding consistente (UTF-8)

### Build

- [ ] Flags de segurança habilitadas
- [ ] Sem symbols em release
- [ ] ASLR habilitado (padrão moderno)
- [ ] Stack protector ativo

### Testes

- [ ] Fuzzing do parser
- [ ] AddressSanitizer clean
- [ ] Valgrind clean
- [ ] Testes de edge cases

### Documentação

- [ ] Política de segurança (SECURITY.md)
- [ ] Processo de disclosure
- [ ] Changelog de security fixes

---

**Segurança é responsabilidade de todos. Quando em dúvida, valide!**
