# Estratégia de Testes

## Visão Geral

O Showbox implementa uma estratégia abrangente de testes que cobre desde unidades individuais até fluxos completos de usuário, garantindo qualidade e confiabilidade em todas as camadas da aplicação.

## Pirâmide de Testes

```
      /\
     /  \
    / UI \
   /______\
  /        \
 /Integração\
---------------
|   Unidade   |
---------------

UI: 20% (Testes manuais + visuais)
Integração: 30% (Fluxos completos)
Unidade: 50% (Classes e funções)
```

## Testes Unitários

### Framework Utilizado

- **Qt Test**: Framework nativo do Qt
- **Estrutura**: Classe `QObject` com slots de teste
- **Execução**: `tst_units` binary

### Cobertura

- **Classes Core**: `ShowBox`, `DialogParser`, `CommandRegistry`
- **Utilitários**: `IconHelper`, `ThemeManager`
- **Widgets Custom**: `CustomTableWidget`, `CustomChartWidget`
- **Parsing**: Tokenização e validação

### Exemplos de Testes

#### Teste de Comando Add

```cpp
void TestCommands::testAddPushButton()
{
    ShowBox dialog("Test", nullptr, false);
    DialogCommand cmd = createCommand(AddCommand, PushButtonWidget,
                                    "OK", "btn_ok");

    dialog.executeCommand(cmd);

    QPushButton* btn = dialog.findChild<QPushButton*>("btn_ok");
    QVERIFY(btn != nullptr);
    QCOMPARE(btn->text(), QString("OK"));
}
```

#### Teste de Parsing

```cpp
void TestParser::testCommandParsing()
{
    DialogParser parser;
    QString input = "add pushbutton OK btn_ok apply exit";

    parser.processInput(input);

    QCOMPARE(parser.getLastCommand().command, AddCommand);
    QCOMPARE(parser.getLastCommand().control, PushButtonWidget);
}
```

### Métricas de Qualidade

- **Cobertura**: Mínimo 80% de linhas de código
- **Performance**: Testes executam em < 30s
- **Isolamento**: Cada teste independente
- **Determinístico**: Resultados consistentes

## Testes de Integração

### Cenários Testados

- **Fluxos Completos**: Criação → configuração → interação → saída
- **Parsing de Scripts**: Arquivos de exemplo completos
- **Múltiplas Plataformas**: Linux, Windows, macOS
- **Configurações**: Diferentes temas e layouts

### Testes End-to-End

```bash
# Script de teste
echo "add pushbutton OK btn_ok apply exit" | ./showbox
echo "add textbox Name name_field" | ./showbox
echo "add listbox Items list1" | ./showbox
```

### Validação de Saída

- **Format**: Valores separados por tabulação
- **Encoding**: UTF-8 consistente
- **Completude**: Todos os widgets reportados
- **Precisão**: Valores corretos retornados

## Testes Manuais

### Testes Visuais

- **Layout**: Alinhamento e posicionamento correto
- **Temas**: Renderização clara/escura
- **Responsividade**: Redimensionamento adequado
- **Internacionalização**: Textos em diferentes idiomas

### Testes de Usabilidade

- **Navegação**: Tab order e foco
- **Acessibilidade**: Suporte a leitores de tela
- **Performance**: Resposta a eventos em tempo real
- **Erros**: Tratamento graceful de entradas inválidas

### Matriz de Compatibilidade

| Sistema      | Qt Version | Status  | Notas                |
|--------------|------------|---------|----------------------|
| Ubuntu 20.04 | Qt5        | ✅      | Totalmente suportado |
| Ubuntu 22.04 | Qt6        | ✅      | Suporte experimental |
| Windows 10   | Qt5        | ✅      | Build automatizado   |
| macOS 12     | Qt5        | ✅      | Build via CI         |

## Testes de Performance

### Benchmarks

- **Parsing**: 1000+ comandos/segundo
- **Renderização**: < 100ms para layouts complexos
- **Memória**: < 50MB para aplicações típicas
- **Startup**: < 500ms para inicialização

### Profiling

```cpp
// Medição de performance
QElapsedTimer timer;
timer.start();

// Operação a ser medida
dialog.executeCommand(cmd);

qint64 elapsed = timer.elapsed();
QVERIFY(elapsed < 100); // Máximo 100ms
```

### Memory Leaks

```bash
# Valgrind check
valgrind --leak-check=full --show-leak-kinds=all ./tst_units

# Address sanitizer
export ASAN_OPTIONS=detect_leaks=1
./tst_units
```

## Testes de Segurança

### Validações de Entrada

- **Buffer Overflow**: Limites de 1024 bytes
- **Injection**: Sanitização de comandos
- **Path Traversal**: Validação de caminhos de arquivo
- **Null Pointers**: Checks em todas as operações

### Sandboxing

- **Container**: Execução em ambientes isolados
- **Permissions**: Acesso mínimo ao sistema
- **Network**: Bloqueio de conexões externas
- **Filesystem**: Restrição a diretórios permitidos

## Automação de Testes

### CI/CD Integration

```yaml
# .github/workflows/test.yml
- name: Run Unit Tests
  run: |
    cd src/code/showbox
    qmake CONFIG+=test
    make
    ./tst_units

- name: Run Integration Tests
  run: |
    cd examples
    ./run_all_examples.sh

- name: Performance Benchmarks
  run: |
    ./benchmarks/run_benchmarks.sh
```

### Test Scripts

```bash
# Run all tests
npm run test

# Run with coverage
npm run test:coverage

# Run specific suite
npm run test:unit
npm run test:integration
npm run test:performance
```

## Métricas e Relatórios

### Cobertura de Código

```bash
# Gerar relatório
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report/

# Verificar mínimo
coverage=$(lcov --summary coverage.info | grep lines | awk '{print $2}')
if (( $(echo "$coverage < 80.0" | bc -l) )); then
    echo "Coverage too low: $coverage%"
    exit 1
fi
```

### Relatórios de Performance

- **Throughput**: Comandos processados por segundo
- **Latency**: Tempo de resposta a eventos
- **Memory**: Pico de uso e vazamentos
- **CPU**: Utilização durante operações intensas

## Estratégia de Regressão

### Testes de Smoke

- **Build**: Compilação bem-sucedida
- **Basic UI**: Janela abre corretamente
- **Simple Commands**: Add/remove widgets básicos
- **Output**: Valores retornados corretamente

### Testes de Regressão

- **Known Issues**: Casos que já foram corrigidos
- **Edge Cases**: Condições limite validadas
- **Platform Specific**: Bugs específicos por SO
- **Version Compatibility**: Funciona com versões anteriores

## Plano de Contingência

### Falhas de Teste

1. **Análise**: Investigar causa raiz
2. **Isolamento**: Criar teste reprodutível
3. **Correção**: Implementar fix
4. **Verificação**: Testar correção
5. **Prevenção**: Adicionar testes para prevenir recorrência

### Métricas de Qualidade

- **Defect Density**: Bugs por 1000 linhas de código
- **Mean Time To Fix**: Tempo médio para correção
- **Test Effectiveness**: Porcentagem de bugs encontrados por testes
- **Customer Satisfaction**: Feedback de usuários

## Ferramentas e Infraestrutura

### Ambiente de Teste

- **Docker**: Containers para isolamento
- **VMs**: Para testes multi-plataforma
- **CI Services**: GitHub Actions, GitLab CI
- **Cloud**: AWS Device Farm para testes móveis

### Ferramentas de Análise

- **Valgrind**: Memory leaks e profiling
- **AddressSanitizer**: Buffer overflows
- **Clang Analyzer**: Análise estática
- **Qt Creator**: Debug integrado
