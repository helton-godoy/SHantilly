# Segurança do ShowBox

## Visão Geral

ShowBox processa entrada de usuário (argumentos CLI e arquivos) e executa código nativo C++. Embora não seja uma aplicação web ou serviço de rede, alguns aspectos de segurança devem ser considerados.

## Superfície de Ataque

### Entradas

1. **Argumentos CLI**: Strings passadas via linha de comando
2. **Arquivos**: Dados CSV/TSV carregados em tabelas
3. **Variáveis de Ambiente**: Configurações de tema/locale

### Processamento

- **Parser**: Tokenização e análise de comandos
- **Widgets Qt**: Renderização de conteúdo
- **File I/O**: Leitura de arquivos

## Vulnerabilidades Potenciais

### 1. Buffer Overflow

**Risco**: Baixo (Qt usa containers seguros)

**Prevenção**:

```cpp
// ✅ BOM - QString gerencia memória
QString text = QString::fromUtf8(input);

// ❌ EVITAR - buffer fixo
char buffer[100];
strcpy(buffer, input);  // Overflow se input > 100
```

### 2. Injection Attacks

**Command Injection** (baixo risco - ShowBox não executa shell):

```cpp
// ✅ BOM - sem exec de shell commands
QProcess::execute("/bin/bash", {"-c", userInput});  // Não fazemos isso
```

**SQL Injection**: N/A (sem banco de dados)

### 3. Path Traversal

**Risco**: Médio (ao carregar arquivos)

**Mitigação**:

```cpp
// Validar caminho de arquivo
bool ShowBox::loadFile(const QString& path) {
    QFileInfo fileInfo(path);
    
    // Verificar se arquivo existe
    if (!fileInfo.exists()) {
        fprintf(stderr, "ERRO: Arquivo não encontrado\n");
        return false;
    }
    
    // Verificar se é arquivo regular (não symlink malicioso)
    if (!fileInfo.isFile()) {
        fprintf(stderr, "ERRO: Caminho inválido\n");
        return false;
    }
    
    // Canonicalizar path (resolve .., symlinks)
    QString canonicalPath = fileInfo.canonicalFilePath();
    
    // TODO: Verificar se está em diretório permitido
    // if (!canonicalPath.startsWith("/home/user/allowed/")) {
    //     return false;
    // }
    
    QFile file(canonicalPath);
    // ...
}
```

### 4. Denial of Service

**Arquivos Enormes**:

```cpp
// Limitar tamanho de arquivo
const qint64 MAX_FILE_SIZE = 100 * 1024 * 1024;  // 100MB

bool loadTableData(const QString& path) {
    QFileInfo info(path);
    if (info.size() > MAX_FILE_SIZE) {
        fprintf(stderr, "ERRO: Arquivo muito grande\n");
        return false;
    }
    // ...
}
```

**Número Excessivo de Widgets**:

```cpp
// Limitar widgets
const int MAX_WIDGETS = 1000;

if (widgets.size() >= MAX_WIDGETS) {
    fprintf(stderr, "ERRO: Muitos widgets\n");
    return;
}
```

### 5. Memory Safety

**Use-After-Free** (ver `bug-fixer.md`):

```cpp
// ✅ BOM - Qt parent ownership
QWidget* widget = new QWidget(this);

// ❌ RUIM - manual delete
delete widget;
widget->show();  // CRASH
```

**Memory Leaks**:

```bash
# Verificar com Valgrind
valgrind --leak-check=full ./showbox ...
```

## Input Validation

### Argumentos CLI

```cpp
void ShowBox::addTextBox(const char* title, const char* name, ...) {
    // Validar name não-nulo
    if (!name || !*name) {
        fprintf(stderr, "ERRO: nome obrigatório\n");
        return;
    }
    
    // Validar comprimento
    if (strlen(name) > 255) {
        fprintf(stderr, "ERRO: nome muito longo\n");
        return;
    }
    
    // Sanitizar para objectName (Qt)
    QString qName = QString::fromUtf8(name);
    qName = qName.replace(QRegularExpression("[^a-zA-Z0-9_]"), "_");
    
    widget->setObjectName(qName);
}
```

### Arquivos

```cpp
bool CustomTableWidget::loadCSV(const QString& path) {
    QFile file(path);
    
    // Verificar permissões de leitura
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Não foi possível abrir arquivo:" << path;
        return false;
    }
    
    QTextStream stream(&file);
    int lineCount = 0;
    const int MAX_LINES = 100000;
    
    while (!stream.atEnd() && lineCount < MAX_LINES) {
        QString line = stream.readLine();
        
        // Sanitizar linha
        line = line.trimmed();
        
        // Processar
        processLine(line);
        lineCount++;
    }
    
    if (lineCount >= MAX_LINES) {
        qWarning() << "Arquivo truncado em" << MAX_LINES << "linhas";
    }
    
    return true;
}
```

## Encoding

### UTF-8 Sempre

```cpp
// ✅ BOM - explicit UTF-8
QString text = QString::fromUtf8(cString);
QByteArray bytes = qString.toUtf8();

// ❌ EVITAR - conversão implícita
QString text = cString;  // Pode ser Latin-1
```

### HTML/Escape

```cpp
// Se exibir conteúdo de usuário como HTML
QString safeText = userInput.toHtmlEscaped();
label->setText(safeText);
```

## Permissões

### File System

ShowBox **não deve** ser executado como root:

```bash
# ❌ RUIM
sudo showbox ...

# ✅ BOM
showbox ...
```

### Capabilities

Se precisar de permissões especiais (não atualmente):

```bash
# Ao invés de setuid root
sudo setcap cap_net_raw+ep /usr/bin/showbox
```

## Sandboxing

### Flatpak

Flatpak fornece sandbox automático:

- Filesystem isolado
- Network isolado (se não necessário)
- Dbus filtrado

**Manifest**:

```yaml
finish-args:
  - --filesystem=home  # Acesso a arquivos do usuário
  - --share=ipc        # X11/Wayland
  - --socket=x11
  - --socket=wayland
  # Sem --share=network (não precisa)
```

## Dependências

### Qt6

- Manter Qt atualizado para patches de segurança
- Monitorar CVEs: <https://www.qt.io/product/security>

### Sistema

```bash
# Verificar dependências vulneráveis (Debian)
apt list --upgradable | grep "qt6"

# Audit (futuro)
npm audit     # Não aplicável
cargo audit   # Não aplicável
```

## Reporting de Vulnerabilidades

**Procedimento**:

1. **NÃO** abrir issue público
2. Enviar email para: <security@showbox.dev>
3. Incluir:
   - Descrição da vulnerabilidade
   - Passos para reproduzir
   - Impacto potencial
   - Sugestão de correção (opcional)

**Timeline de Resposta**:

- Confirmação: 48h
- Avaliação: 7 dias
- Patch: 30 dias (depende da severidade)
- Divulgação: Após patch disponível

## Checklist de Segurança

### Para Cada Feature

- [ ] Input validado e sanitizado
- [ ] Limits impostos (tamanho, quantidade)
- [ ] Encoding UTF-8 correto
- [ ] Sem exec de comandos shell
- [ ] Memory safe (Valgrind clean)
- [ ] Paths canonicalizados
- [ ] Erro handling adequado

### Para Cada Release

- [ ] Audit de dependências
- [ ] Scan de vulnerabilidades
- [ ] Testes de fuzzing (futuro)
- [ ] Code review com foco em segurança
- [ ] Update de bibliotecas

## Ferramentas

### Static Analysis

```bash
# Cppcheck
cppcheck --enable=all src/code/showbox/

# Clang Static Analyzer
scan-build qmake && scan-build make
```

### Dynamic Analysis

```bash
# AddressSanitizer
qmake CONFIG+=sanitizer
make
./showbox --add-button "Test"

# Valgrind
valgrind --leak-check=full ./showbox ...
```

### Fuzzing (Futuro)

```bash
# AFL ou libFuzzer para fuzzing de parser
```

## Notas

- ShowBox **não é** uma aplicação de segurança crítica
- Principal foco: **evitar crashes** e **memory leaks**
- Usuário executa com suas próprias permissões
- Não há autenticação/autorização (single-user GUI)

---

**Defesa em Profundidade**: Múltiplas camadas de validação e proteção!
