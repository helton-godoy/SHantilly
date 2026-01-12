# Visão Geral do Projeto ShowBox

## Resumo

**ShowBox** é uma ferramenta de linha de comando baseada em Qt6 que fornece **widgets GUI avançados para scripts shell**. Ele permite que desenvolvedores criem interfaces gráficas ricas e interativas diretamente a partir de scripts Bash, oferecendo uma alternativa moderna a ferramentas como `dialog`, `zenity` ou `yad`.

## Propósito

O ShowBox democratiza o desenvolvimento de interfaces gráficas para administradores de sistemas e desenvolvedores de scripts, eliminando a necessidade de conhecimento profundo em programação GUI. Com simples comandos de linha de comando, é possível criar diálogos complexos, formulários, tabelas, gráficos e muito mais.

## Principais Características

### 🎨 Widgets Disponíveis

- **Botões**: PushButton, CheckBox, RadioButton
- **Entrada de Texto**: TextBox (normal e com senha)
- **Seleção**: ListBox, ComboBox com suporte a edição
- **Exibição**: Label (HTML/texto/imagem), TextView, Separator
- **Controles**: ProgressBar (normal e busy mode), Slider
- **Organizacionais**: GroupBox, Frame, Tabs, Page
- **Avançados**: Calendar, Table (com busca), Charts (Qt Charts)

### ⚙️ Recursos Técnicos

- **Sistema de Temas**: Suporte a temas claro/escuro via `ThemeManager`
- **Ícones Customizados**: Integração com `icon_helper` para ícones personalizados
- **Arquitetura Modular**: Baseada em padrão Command para extensibilidade
- **Parser Próprio**: Tokenização e parsing de comandos customizados
- **Widgets Customizados**: `CustomTableWidget` e `CustomChartWidget` especializados

## Arquitetura de Alto Nível

```diagram
┌─────────────────────────────────────────┐
│      Script Shell (Bash/sh)             │
│  (showbox --title "..." --add-button)   │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│   Tokenizer + Parser                    │
│   (Processa argumentos CLI)             │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│   Command Registry                      │
│   (Executa comandos registrados)        │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│   ShowBox (Qt6 QDialog)                 │
│   - Gerencia layouts                    │
│   - Cria widgets dinamicamente          │
│   - Aplica temas                        │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│   Widgets Qt6                           │
│   (Renderização nativa multi-plataforma)│
└─────────────────────────────────────────┘
```

## Tecnologias Utilizadas

- **Linguagem**: C++17
- **Framework GUI**: Qt6 (widgets, svg, charts)
- **Build System**: qmake (`.pro` files)
- **Containerização**: Docker (Debian Trixie)
- **Documentação**: Pandoc (geração de man pages)

## Estrutura do Projeto

```shell
showbox/
├── src/code/showbox/      # Código-fonte principal (C++/Qt6)
│   ├── showbox.{cc,h}     # Classe principal (~1500 linhas)
│   ├── commands/          # Implementação de comandos
│   ├── *.pro              # Arquivo de projeto Qt
│   └── obj/               # Objetos compilados
│
├── examples/              # Scripts de demonstração (a serem criados)
│   ├── showbox_calendar.sh
│   ├── showbox_charts.sh
│   └── ...
│
├── packaging/             # Empacotamento multiplataforma
│   ├── deb/               # Pacotes Debian
│   ├── rpm/               # Pacotes RedHat
│   ├── appimage/          # AppImage universal
│   ├── flatpak/           # Flatpak
│   ├── dmg/               # macOS
│   └── msix/              # Windows
│
├── tools/                 # Ferramentas de desenvolvimento
│   └── start-docker-*.sh  # Scripts Docker para builds
│
├── man/                   # Manuais (man pages)
├── docs/                  # Documentação do desenvolvedor
└── tests/                 # Testes (a serem implementados)
```

## Casos de Uso

### 1. Instaladores Interativos

```bash
#!/bin/bash
showbox --title "Instalador" \
  --add-textbox "Nome:" "username" \
  --add-checkbox "Instalar extras?" "extras" \
  --add-button "Instalar" --exit
```

### 2. Dashboards de Monitoramento

```bash
#!/bin/bash
showbox --title "Sistema" \
  --add-chart "CPU" \
  --add-progressbar "memoria" \
  --add-table "processos"
```

### 3. Formulários de Configuração

```bash
#!/bin/bash
showbox --title "Config" \
  --add-tabs "config_tabs" \
  --add-page "Rede" \
  --add-textbox "IP:" "ip_address"
```

## Estratégia de Desenvolvimento

### Ambiente de Desenvolvimento

O projeto utiliza **Docker** para garantir reprodutibilidade:

1. Clone do repositório
2. Execução de `tools/start-docker-dev.sh`
3. Container com todas as dependências disponível

O código dentro do container é mapeado para `src/code/` permitindo desenvolvimento tanto no container quanto no host.

### Workflow de Build

```bash
# Build local (dentro do Docker)
cd src/code/showbox
qmake6
make

# Build de pacotes
./tools/start-docker-build-deb.sh    # Debian
./tools/start-docker-build-rpm.sh    # RedHat
./tools/start-docker-appimage.sh     # AppImage
```

## Público-Alvo

- **Administradores de Sistemas**: Criação de ferramentas de configuração e manutenção
- **DevOps**: Construção de interfaces para pipelines e automações
- **Desenvolvedores Shell**: Adição de GUIs a scripts existentes
- **Usuários Avançados**: Customização de workflows sem aprender frameworks GUI

## Diferenciais

### vs Dialog/Zenity

- ✅ Widgets mais ricos (charts, tables com busca)
- ✅ Sistema de temas integrado
- ✅ Renderização Qt nativa

### vs Yad

- ✅ Melhor integração com Qt/KDE
- ✅ Arquitetura mais extensível
- ✅ Widgets customizados especializados

### vs Python/Tkinter

- ✅ Menor curva de aprendizagem
- ✅ Integração direta com shell scripts
- ✅ Não requer conhecimento de linguagens de programação

## Roadmap

### Curto Prazo (v1.0)

- [ ] Completar scripts de exemplo
- [ ] Implementar suite de testes
- [ ] Finalizar Makefile
- [ ] Criar documentação de usuário

### Médio Prazo (v1.5)

- [ ] Adicionar novos widgets (TreeView, FileDialog)
- [ ] Melhorar sistema de temas
- [ ] Plugins para extensão

### Longo Prazo (v2.0)

- [ ] Suporte a scripts Python/Ruby
- [ ] Editor visual de diálogos
- [ ] Biblioteca de templates

## Licença

*TODO: Definir licença (sugestão: GPL v3 ou MIT).*

## Contribuições

O projeto está em desenvolvimento ativo. Contribuições são bem-vindas especialmente em:

- Criação de exemplos práticos
- Testes de widgets
- Empacotamento para diferentes distribuições
- Documentação de uso

---

*Última atualização: 2026-01-12.*
