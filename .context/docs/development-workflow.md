# Workflow de Desenvolvimento

## Ambiente de Desenvolvimento

O Showbox utiliza um ambiente containerizado com Docker para garantir reprodutibilidade e consistência entre diferentes máquinas de desenvolvimento.

### Pré-requisitos

- Docker Engine instalado
- Git para controle de versão
- Conhecimento básico de C++ e Qt

### Configuração Inicial

```bash
# Clonar repositório
git clone https://github.com/helton-godoy/showbox.git
cd showbox

# Executar script de configuração Docker
./tools/start-docker-dev.sh
```

### Estrutura do Container

- **Base**: Ubuntu/Debian com Qt5/Qt6
- **Volumes**: Código fonte montado em `/src`
- **Ferramentas**: qmake, make, gcc, gdb
- **Display**: Suporte a X11 forwarding

## Processo de Build

### Compilação Local

```bash
# Dentro do container
cd /src/code/showbox
qmake
make
```

### Build de Distribuição

```bash
# Build completo
npm run build

# Build específico de plataforma
npm run build:linux
npm run build:windows
npm run build:macos
```

### Artefatos Gerados

- Binário executável (`showbox`)
- Bibliotecas compartilhadas (`.so`/`.dll`)
- Arquivos de configuração
- Pacotes de instalação (`.deb`, `.rpm`, `.appimage`)

## Testes

### Suite de Testes Automatizados

```bash
# Executar todos os testes
npm run test

# Testes com watch mode (desenvolvimento)
npm run test:watch

# Testes específicos
npm run test:unit
npm run test:integration
```

### Testes Manuais

```bash
# Scripts de exemplo
cd examples
./showbox_textbox.sh
./showbox_buttons.sh

# Testes visuais
cd examples/demos
./visual_layout_test.sh
```

### Cobertura de Testes

- **Unitários**: Classes individuais e funções
- **Integração**: Fluxos completos de comandos
- **Funcionais**: Exemplos práticos e casos de uso
- **Performance**: Benchmarks de parsing e renderização

## Controle de Qualidade

### Linting e Formatação

```bash
# Verificar estilo de código
npm run lint

# Formatar código
npm run format

# Verificar conformidade
npm run check
```

### Análise Estática

```bash
# Clang Static Analyzer
npm run analyze:clang

# Valgrind (memory leaks)
npm run analyze:valgrind

# Sanitizers
npm run analyze:sanitizers
```

## Versionamento e Releases

### Conventional Commits

```bash
# Exemplos de commits
git commit -m "feat: add calendar widget support"
git commit -m "fix: resolve memory leak in parser"
git commit -m "docs: update command reference"
git commit -m "refactor: simplify command registry"
```

### Processo de Release

1. **Desenvolvimento**: Feature branches com PRs
2. **Staging**: Merge para `develop` com testes
3. **Release**: Merge para `main` com versionamento
4. **Distribuição**: Build e upload para todas as plataformas

### Versionamento Semântico

- **MAJOR**: Quebra de compatibilidade
- **MINOR**: Novos recursos compatíveis
- **PATCH**: Correções de bugs

## Debugging

### Debug com GDB

```bash
# Dentro do container
cd /src/code/showbox
gdb ./showbox

# Comandos úteis
(gdb) break ShowBox::executeCommand
(gdb) run < examples/showbox_buttons.sh
```

### Logging e Tracing

```bash
# Executar com debug logging
./showbox --debug < input.txt

# Analisar logs
tail -f /tmp/showbox.log
```

### Profiling

```bash
# Valgrind para performance
valgrind --tool=callgrind ./showbox < script.sh

# Analisar resultados
kcachegrind callgrind.out.*
```

## Contribuição

### Fluxo de Contribuição

1. **Fork** do repositório
2. **Branch** para feature/fix
3. **Desenvolvimento** com testes
4. **Pull Request** com descrição detalhada
5. **Code Review** e aprovação
6. **Merge** após CI passar

### Guidelines

- **Commits**: Mensagens claras em português
- **Branches**: `feature/nome`, `fix/issue-id`
- **PRs**: Template preenchido, testes incluídos
- **Issues**: Labels apropriadas, reprodutores quando possível

## CI/CD Pipeline

### GitHub Actions

- **Build**: Compilação para todas as plataformas
- **Test**: Execução da suite completa
- **Lint**: Verificação de código
- **Release**: Upload automático de artefatos

### Docker Images

- **Base**: Ubuntu com Qt dependencies
- **Build**: Imagem otimizada para compilação
- **Test**: Imagem com ferramentas de análise
- **Release**: Imagens específicas por plataforma

## Troubleshooting

### Problemas Comuns

#### Erro de Display

```bash
# Verificar X11 forwarding
echo $DISPLAY
xeyes  # Testar conectividade
```

#### Problemas de Build

```bash
# Limpar build
make clean
rm -rf obj/

# Rebuild completo
qmake && make
```

#### Falhas de Teste

```bash
# Executar teste específico
./tst_commands -functions
gdb ./tst_commands core
```

### Suporte

- **Issues**: Para bugs e solicitações
- **Discussions**: Para dúvidas gerais
- **Wiki**: Documentação detalhada
- **Discord/Slack**: Comunicação síncrona
