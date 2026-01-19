# SHantilly

[![CI - Build and Test](https://github.com/helton-godoy/SHantilly/actions/workflows/ci.yml/badge.svg)](https://github.com/helton-godoy/SHantilly/actions/workflows/ci.yml)
[![CodeQL](https://github.com/helton-godoy/SHantilly/actions/workflows/codeql.yml/badge.svg)](https://github.com/helton-godoy/SHantilly/actions/workflows/codeql.yml)
[![Release](https://github.com/helton-godoy/SHantilly/actions/workflows/release.yml/badge.svg)](https://github.com/helton-godoy/SHantilly/actions/workflows/release.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

**GUI Widgets para Shell Scripts** - Transforme seus scripts bash/shell em aplicações visuais modernas e nativas com facilidade. Crie janelas, diálogos e formulários complexos usando a robustez do Qt6, sem precisar escrever uma linha de C++.

## 🚀 Início Rápido

```bash
# Clonar o repositório
git clone https://github.com/helton-godoy/SHantilly.git
cd SHantilly

# Build via Docker (recomendado)
make build

# Testar com exemplo
./examples/SHantilly_demo.sh
```

## 📋 Pré-requisitos

- Docker (recomendado) ou
- Qt6 (QtWidgets, QtCharts, QtSVG)
- CMake 3.16+
- C++17 compiler

## 🔧 Comandos Principais

| Comando | Descrição |
|---------|-----------|
| `make build` | Compila o projeto |
| `make test` | Executa os testes |
| `make lint` | Verifica qualidade do código |
| `make format` | Formata o código |
| `make docs` | Gera documentação |
| `make coverage` | Gera relatório de cobertura |
| `make pkg-deb` | Cria pacote DEB |
| `make pkg-appimage` | Cria AppImage |
| `make help` | Lista todos os comandos |

## 📁 Estrutura do Projeto

```
SHantilly/
├── src/                    # Código-fonte principal
│   └── code/SHantilly/     # Implementação C++/Qt6
├── tests/                  # Testes unitários e integração
├── examples/               # Scripts de exemplo
├── docs/                   # Documentação
├── packaging/              # Scripts de empacotamento
│   ├── deb/                # Pacotes Debian/Ubuntu
│   ├── appimage/           # AppImage
│   ├── rpm/                # RPM (Fedora/RHEL)
│   └── flatpak/            # Flatpak
├── libs/                   # Bibliotecas auxiliares
├── man/                    # Páginas de manual
├── .github/workflows/      # CI/CD GitHub Actions
├── CMakeLists.txt          # Build system
└── Makefile                # Automação de desenvolvimento
```

## 📖 Documentação

- [Instalação](docs/installation.md)
- [Guia de Início](docs/getting-started.md)
- [Referência de Widgets](docs/widgets-reference.md)
- [Layouts](docs/layouts.md)
- [Recursos Avançados](docs/advanced-features.md)
- [Troubleshooting](docs/troubleshooting.md)
- [Segurança](docs/security.md)

## 🤖 Para Agentes de IA

Consulte o arquivo [AGENTS.md](AGENTS.md) para instruções detalhadas sobre como contribuir com este projeto. Cada diretório também contém seu próprio `README.md` com informações específicas.

## 🔗 Projetos Relacionados

- [SHantilly-Studio](https://github.com/helton-godoy/SHantilly-Studio) - IDE Visual para criação de interfaces

## 📄 Licença

Este projeto está licenciado sob a GPLv3+ - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 🙏 Créditos

Baseado no trabalho original de Andriy Martynets (dialogbox).
