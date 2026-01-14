# Documentação Técnica - Showbox

## Índice da Documentação

Bem-vindo à documentação técnica completa do Showbox. Esta seção contém todos os guias necessários para entender, desenvolver e contribuir com o projeto.

## 📋 Visão Geral

- **[Visão Geral do Projeto](./project-overview.md)**: Introdução completa ao Showbox, objetivos, arquitetura e casos de uso
- **[Arquitetura](./architecture.md)**: Detalhes técnicos da implementação, padrões de design e fluxo de dados
- **[Workflow de Desenvolvimento](./development-workflow.md)**: Guia para configuração do ambiente e processos de desenvolvimento
- **[Estratégia de Testes](./testing-strategy.md)**: Cobertura completa de testes unitários, integração e qualidade

## 🏗️ Arquitetura e Design

### Componentes Principais

- **ShowBox**: Classe principal da interface Qt
- **DialogParser**: Parser de comandos assíncrono
- **Command Registry**: Sistema de registro de comandos
- **Theme Manager**: Gerenciamento de temas claro/escuro

### Padrões Implementados

- Command Pattern para processamento de comandos
- Factory Pattern para criação de widgets
- Observer Pattern para comunicação assíncrona
- Strategy Pattern para diferentes tipos de widget

## 🚀 Guia de Desenvolvimento

### Configuração Inicial

```bash
git clone https://github.com/helton-godoy/showbox.git
cd showbox
./tools/start-docker-dev.sh
```

### Build e Testes

```bash
# Build
cd src/code/showbox
qmake && make

# Testes
npm run test
npm run test:coverage
```

### Debugging

```bash
# Com GDB
gdb ./showbox
(gdb) break ShowBox::executeCommand
(gdb) run < examples/script.sh
```

## 🧪 Estratégia de Qualidade

### Pirâmide de Testes

- **50% Unitários**: Classes e funções individuais
- **30% Integração**: Fluxos completos
- **20% UI**: Testes manuais e visuais

### Métricas de Qualidade

- Cobertura de código: mínimo 80%
- Performance: < 100ms para operações críticas
- Memory leaks: zero tolerância
- Compatibilidade: Linux, Windows, macOS

## 📚 Referências Técnicas

### APIs Principais

#### ShowBox Class

```cpp
class ShowBox : public QDialog {
public:
    // Adição de widgets
    void addPushButton(const char* title, const char* name, ...);
    void addTextBox(const char* title, const char* name, ...);
    void addListBox(const char* title, const char* name, ...);

    // Comandos de controle
    void setOptions(QWidget* widget, unsigned int options, ...);
    void clear(char* name);
    QWidget* findWidget(char* name);

    // Sistema de layout
    void stepHorizontal();
    void stepVertical();
    void addStretch(int stretch = 1);
};
```

#### DialogParser Class

```cpp
class DialogParser : public QThread {
public:
    DialogParser(ShowBox* parent = nullptr, FILE* in = stdin);

signals:
    void sendCommand(DialogCommand command);

protected:
    void run() override; // Loop principal de parsing
};
```

### Sintaxe de Comandos

#### Adição de Widgets

```
add pushbutton "OK" btn_ok apply exit
add textbox "Nome" name_field "João Silva"
add listbox "Itens" items_list activation selection
add checkbox "Aceitar termos" terms true
```

#### Configuração

```
set enabled btn_ok false
set visible panel1 true
set stylesheet main "QWidget { background: #f0f0f0; }"
```

#### Consulta

```
query value name_field
query selection items_list
query checked terms
```

## 🔧 Troubleshooting

### Problemas Comuns

#### Erro de Display X11

```bash
# Verificar conexão
echo $DISPLAY
xeyes

# Solução: configurar X11 forwarding no SSH
ssh -X user@host
```

#### Falha de Build

```bash
# Limpar e rebuild
make clean
rm -rf obj/
qmake && make
```

#### Memory Leaks

```bash
# Verificar com Valgrind
valgrind --leak-check=full ./showbox < script.sh
```

## 📈 Métricas e Monitoramento

### Performance Benchmarks

- Parsing: 1000+ comandos/segundo
- Renderização: < 100ms para layouts complexos
- Memória: < 50MB para aplicações típicas
- Startup: < 500ms

### Qualidade do Código

- Cobertura de testes: > 80%
- Complexidade ciclomática: < 10 por função
- Densidade de defeitos: < 1 por 1000 LOC
- Tempo médio de correção: < 4 horas

## 🤝 Contribuição

### Processo de Contribuição

1. Fork do repositório
2. Branch feature/fix
3. Desenvolvimento com testes
4. Pull Request com review
5. Merge após aprovação

### Guidelines

- Commits: Conventional Commits em português
- Código: Qt style guidelines
- Testes: Obrigatórios para novas funcionalidades
- Documentação: Atualizar docs para mudanças significativas

## 📞 Suporte

- **Issues**: Bugs e solicitações de feature
- **Discussions**: Dúvidas gerais e discussões
- **Wiki**: Guias detalhados e tutoriais
- **Discord/Slack**: Comunicação em tempo real

## 🔗 Links Úteis

- [Repositório Principal](https://github.com/helton-godoy/showbox)
- [Documentação de Comandos](../examples/README.md)
- [Exemplos Práticos](../examples/)
- [Scripts de Build](../packaging/)
- [Testes Automatizados](../tests/)

---

**Última atualização**: Janeiro 2026
**Versão documentada**: Showbox 1.0
**Mantenedor**: Andriy Martynets
