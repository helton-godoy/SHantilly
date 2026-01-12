# Workflow de Desenvolvimento

Este documento descreve o processo de desenvolvimento padrão para contribuições ao **ShowBox**.

## Configuração Inicial

### 1. Fork e Clone

```bash
# Fork no GitHub
# Depois:
git clone https://github.com/SEU_USER/showbox.git
cd showbox

# Adicionar upstream
git remote add upstream https://github.com/showbox/showbox.git
```

### 2. Ambiente de Desenvolvimento

**Opção A: Docker (Recomendado).**

```bash
./tools/start-docker-dev.sh
# Container inicia com todas as dependências
```

**Opção B: Local.**

```bash
# Debian/Ubuntu
sudo apt-get install -y \
    build-essential \
    qt6-base-dev \
    libqt6charts6-dev \
    libqt6svg6-dev \
    qmake6 \
    git \
    pandoc

# Fedora
sudo dnf install -y \
    qt6-qtbase-devel \
    qt6-qtcharts-devel \
    qt6-qtsvg-devel
```

## Git Workflow

### Branching Strategy

```shell
main
  ├── develop (integração)
  ├── feature/add-treeview-widget
  ├── feature/improve-theme-system
  ├── fix/crash-on-empty-table
  └── release/1.1.0
```

**Convenções**:

- `feature/` - Novas features
- `fix/` - Bug fixes
- `refactor/` - Refatorações
- `docs/` - Documentação
- `release/` - Preparação de release

### Criar Feature Branch

```bash
git checkout develop
git pull upstream develop
git checkout -b feature/minha-feature
```

## Ciclo de Desenvolvimento

### 1. Planejar

- [ ] Criar issue no GitHub descrevendo feature/bug
- [ ] Discutir abordagem se mudança grande
- [ ] Atualizar `task.md` com checklist

### 2. Desenvolver

```bash
# Fazer mudanças
vim src/code/showbox/showbox.cc

# Build frequente
cd src/code/showbox
qmake6 && make -j$(nproc)

# Testar
./bin/showbox --add-button "Test"
```

### 3. Testar

```bash
# Testes unitários
cd tests/auto
qmake6 && make
./test_showbox

# Memory leaks
valgrind --leak-check=full ../src/code/showbox/bin/showbox ...

# Lint
# (adicionar linter no futuro)
```

### 4. Documentar

```bash
# Criar exemplo
echo '#!/bin/bash
showbox --add-mywidget "Test" "widget"' > examples/showbox_mywidget.sh
chmod +x examples/showbox_mywidget.sh

# Atualizar man page
vim man/showbox.md
pandoc -s -f markdown -t man man/showbox.md -o man/showbox.1
```

### 5. Commit

**Convenção de Commit Messages**:

```git
Tipo: Descrição curta (máx 50 chars)

Descrição detalhada do que mudou e por quê.
Pode ter múltiplas linhas.

Closes #123
```

**Tipos**:

- `feat:` - Nova feature
- `fix:` - Bug fix
- `refactor:` - Refatoração
- `docs:` - Documentação
- `test:` - Testes
- `chore:` - Tarefas de manutenção

**Exemplos**:

```bash
git commit -m "feat: Add TreeView widget

Implementa widget TreeView para exibir hierarquias de dados.
Suporta expansão/colapso de nós e seleção.

Closes #45"

git commit -m "fix: Crash ao adicionar tabela vazia

Valida ponteiro de arquivo antes de carregar dados.

Closes #123"

git commit -m "docs: Update man page with TreeView examples"
```

### 6. Push e Pull Request

```bash
git push origin feature/minha-feature
```

**No GitHub**:

1. Criar Pull Request para `develop`
2. Preencher template de PR
3. Aguardar CI passar
4. Solicitar review

## Code Review

### Para Autor

- Responder comentários prontamente
- Fazer mudanças solicitadas
- Marcar conversas como resolvidas

### Para Reviewer

- Usar checklist de [`code-reviewer.md`](.context/agents/code-reviewer.md)
- Ser construtivo nos comentários
- Aprovar quando satisfeito

## Merge

**Squash and Merge** para manter histórico limpo:

```bash
git checkout develop
git merge --squash feature/minha-feature
git commit -m "feat: Descrição da feature"
git push upstream develop
```

## Release Process

### 1. Preparação

```bash
git checkout develop
git pull
git checkout -b release/1.1.0

# Atualizar versão
vim src/code/showbox/showbox.pro  # VERSION = 1.1.0

# Atualizar changelog
vim CHANGELOG.md
```

### 2. Testes Finais

```bash
# Build completo
cd src/code/showbox
qmake6 && make clean && make -j$(nproc)

# Todos os testes
cd tests
./run_all_tests.sh

# Empacotamento
./tools/start-docker-build-deb.sh
./tools/start-docker-build-rpm.sh
./tools/start-docker-appimage.sh
```

### 3. Tag e Release

```bash
git tag -a v1.1.0 -m "Release 1.1.0"
git push upstream v1.1.0

# GitHub Actions cria assets automaticamente
```

## Boas Práticas

### Commits

- ✅ Commits pequenos e focados
- ✅ Mensagens descritivas
- ✅ Testar antes de commitar
- ❌ Não commitar código quebrado
- ❌ Não commitar commented code

### Código

- ✅ Seguir style guide
- ✅ Adicionar testes
- ✅ Documentar código complexo
- ✅ Verificar memory leaks
- ❌ Não deixar TODOs sem issue

### Pull Requests

- ✅ Descrição clara
- ✅ Referência a issue
- ✅ Screenshots para mudanças visuais
- ✅ Changelog entry
- ❌ Não fazer PRs gigantes

## Ferramentas Recomendadas

### IDEs/Editores

- **Qt Creator** - IDE oficial Qt
- **VS Code** - Com extensões C++/Qt
- **CLion** - IDE JetBrains

### Git GUI

- **GitKraken**
- **GitHub Desktop**
- **git gui** (built-in)

### Debug

- **gdb** - CLI debugger
- **valgrind** - Memory profiler
- **Qt Creator Debugger** - GUI debugger

## Troubleshooting

### Build Fails

```bash
# Limpar e rebuildar
make clean
rm -rf src/code/showbox/obj src/code/showbox/bin
qmake6 && make
```

### Tests Fail

```bash
# Rodar teste individual com verbose
./test_showbox -v2
```

### Git Issues

```bash
# Sincronizar com upstream
git fetch upstream
git rebase upstream/develop

# Resolver conflitos
git mergetool
```

---

**Dúvidas?** Abra uma issue ou pergunte no Discord!
