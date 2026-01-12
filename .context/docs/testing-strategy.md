# Estratégia de Testes

## Visão Geral

ShowBox utiliza uma abordagem multi-camadas de testes para garantir qualidade e estabilidade.

## Pirâmide de Testes

```diagram
       /\
      /  \
     /E2E \ ← End-to-End (poucos)
    /------\
   /Integr. \ ← Integração (moderado)
  /----------\
 /Unitários   \ ← Unitários (muitos)
/--------------\
```

### 1. Testes Unitários (~70%)

**Foco**: Funções e métodos isolados

**Framework**: Qt Test

**Exemplos**:

- Tokenizer parseando argumentos
- CommandRegistry registrando comandos
- ThemeManager aplicando paletas

**Localização**: `tests/auto/test_*`

### 2. Testes de Integração (~20%)

**Foco**: Interação entre componentes

**Exemplos**:

- Parser → CommandRegistry → ShowBox
- Criação de widget via comando CLI
- Aplicação de tema ao diálogo completo

### 3. Testes End-to-End (~10%)

**Foco**: Fluxo completo do usuário

**Exemplos**:

- Script bash executando showbox
- Verificação de output correto
- Testes de diferentes plataformas

## Ferramentas

### Qt Test Framework

```cpp
#include <QtTest/QtTest>

class TestWidget : public QObject {
    Q_OBJECT
private slots:
    void testCreation();
    void testProperties();
};

QTEST_MAIN(TestWidget)
```

### Valgrind

```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         ./test_showbox
```

### Code Coverage

```bash
qmake CONFIG+=coverage
make
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

## Alvos de Qualidade

| Métrica             | Alvo  | Atual |
|---------------------|-------|-------|
| Cobertura de Código | ≥ 80% | TBD   |
| Cobertura de Branch | ≥ 70% | TBD   |
| Memory Leaks        | 0     | TBD   |
| Warnings Compilação | 0     | TBD   |
| Testes Passando     | 100%  | TBD   |

## CI/CD

### GitHub Actions

- Build em múltiplas plataformas
- Testes automatizados
- Verificação de memory leaks
- Code coverage reporting

### Pre-commit Hooks

```bash
# .git/hooks/pre-commit
#!/bin/bash
cd tests
./run_tests.sh || exit 1
```

## Categorias de Testes

### Funcionais

- Widgets são criados corretamente
- Comandos executam como esperado
- Output está no formato correto

### Não-Funcionais

- **Performance**: Benchmark de operações
- **Memória**: Sem leaks
- **Segurança**: Validação de inputs
- **Usabilidade**: Testes manuais

### Regressão

- Teste para cada bug corrigido
- Previne reintrodução de bugs

## Testes Manuais

### Checklist de Release

- [ ] Testar em Debian 12
- [ ] Testar em Ubuntu 24.04
- [ ] Testar em Fedora 40
- [ ] Testar todos os widgets visuais
- [ ] Testar temas claro/escuro
- [ ] Testar exemplos funcionam
- [ ] Verificar man page correta

## Relatórios de Bugs

**Template**:

```markdown
### Descrição
[Descrição clara do bug]

### Passos para Reproduzir
1. ...
2. ...

### Comportamento Esperado
[O que deveria acontecer]

### Comportamento Atual
[O que acontece]

### Ambiente
- OS: Ubuntu 24.04
- Qt: 6.6.1
- ShowBox: 1.0.0

### Logs/Stack Trace
\```
[colar logs]
\```
```

---

Testes são investimento, não custo!
