# Arquitetura do Showbox

## Visão Geral da Arquitetura

O Showbox segue uma arquitetura modular baseada em padrões de design bem estabelecidos, com separação clara de responsabilidades entre parsing, execução e interface gráfica.

## Diagrama de Componentes

```
[Script Shell] --> stdin --> [DialogParser] --> [Command Registry] --> [ShowBox]
                                   |                    |                  |
                                   v                    v                  v
                             [Tokenização]      [Command Handlers]   [Qt Widgets]
                                   |                    |                  |
                                   v                    v                  v
                         [Sintaxe Validation]    [Business Logic]   [GUI Rendering]
```

## Componentes Principais

### 1. DialogParser (Thread de Parsing)

**Responsabilidades:**

- Leitura sequencial de comandos via stdin
- Tokenização e validação sintática
- Execução assíncrona para não bloquear UI
- Gerenciamento de estado de parsing

**Características Técnicas:**

- Herda de `QThread` para execução concorrente
- Buffer circular de 1024 bytes para processamento
- Estados finitos para diferentes estágios de parsing
- Sinalização Qt para comunicação com UI

### 2. ShowBox (Classe Principal)

**Herança:** `QDialog` (Qt)

**Principais Métodos:**

- `add*()`: Métodos para criação de widgets (addPushButton, addTextBox, etc.)
- `setOptions()`: Configuração de propriedades
- `findWidget()`: Localização de widgets por nome
- `report()`: Saída de valores via stdout

**Gerenciamento de Layout:**

- Layout vertical principal (`QVBoxLayout`)
- Suporte a layouts aninhados (grupos, frames)
- Sistema de coordenadas relativas
- Auto-posicionamento inteligente

### 3. Command Registry

**Padrão:** Command Pattern + Factory Pattern

**Estrutura:**

- `Command` enum com flags para diferentes operações
- `Control` enum para tipos de widgets
- Registry de handlers para cada comando
- Validação de parâmetros por tipo de comando

### 4. Sistema de Widgets

**Tipos Suportados:**

- **Básicos**: PushButton, CheckBox, RadioButton, Label
- **Entrada**: TextBox, ListBox, ComboBox
- **Contêineres**: GroupBox, Frame, Tabs, Page
- **Avançados**: ProgressBar, Slider, TextView, Calendar
- **Custom**: Table, Chart (widgets personalizados)

**Características:**

- Criação dinâmica baseada em comandos
- Propriedades configuráveis (enabled, visible, style)
- Eventos e sinais Qt nativos
- Suporte a ícones e temas

## Padrões de Design Utilizados

### Command Pattern

```cpp
struct DialogCommand {
    char buffer[BUFFER_SIZE];
    unsigned int command;
    unsigned int control;
    // ... campos de dados
};
```

### Factory Pattern

```cpp
class CommandRegistry {
    static std::map<std::string, CommandFactory*> factories;
    // Registro dinâmico de comandos
};
```

### Observer Pattern

- Sinais Qt para comunicação widget ↔ parser
- Slots para tratamento de eventos
- Conexões assíncronas entre componentes

## Fluxo de Dados

### Entrada (Parsing)

1. Comando textual chega via stdin
2. `DialogParser` tokeniza e valida sintaxe
3. Comando é convertido para `DialogCommand` struct
4. Sinal `sendCommand()` é emitido

### Processamento

1. `ShowBox::executeCommand()` recebe o comando
2. Command registry identifica handler apropriado
3. Handler executa operação no widget correspondente
4. Layout é atualizado automaticamente

### Saída

1. Valores são coletados via `report()` ou sinais
2. Dados formatados são enviados para stdout
3. Aplicação termina com código apropriado

## Gerenciamento de Estado

### Estados do Parser

- `StageCommand`: Aguardando identificação do comando
- `StageType`: Processando tipo de widget
- `StageTitle`: Lendo título/texto do widget
- `StageName`: Lendo nome identificador
- `StageText`: Processando conteúdo adicional
- `StageOptions`: Aplicando opções/propriedades

### Estados da UI

- Layout corrente (`currentLayout`)
- Widget de lista ativo (`currentListWidget`)
- Grupo ativo (`groupLayout`)
- Página/tab ativa (`currentTabsWidget`)

## Considerações de Performance

### Otimizações

- **Lazy Loading**: Widgets criados apenas quando necessários
- **Buffer Circular**: Processamento eficiente de entrada
- **Threading**: Parsing não bloqueia interface
- **Cache de Widgets**: Busca otimizada por nome

### Limitações

- Interface síncrona (bloqueante por design)
- Memória limitada pelo buffer de comandos
- Dependente de event loop Qt

## Extensibilidade

### Adição de Novos Widgets

1. Definir novo valor no enum `Control`
2. Implementar método `add*()` correspondente
3. Adicionar handler no command registry
4. Registrar propriedades específicas se necessário

### Novos Comandos

1. Adicionar entrada no enum `Command`
2. Implementar classe handler
3. Registrar no factory
4. Definir sintaxe de parsing

## Segurança e Validação

### Validações Implementadas

- Limites de buffer para prevenir overflow
- Validação de nomes de widgets
- Type checking para propriedades
- Sanitização de entrada de usuário

### Considerações de Segurança

- Execução em sandbox (container Docker recomendado)
- Validação de caminhos de arquivo
- Limitação de operações perigosas
