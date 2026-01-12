# Glossário - ShowBox

## Termos Técnicos

### ShowBox

A aplicação principal. Classe C++ baseada em `QDialog` que gerencia a janela GUI e coordena todos os widgets.

### Widget

Componente GUI do Qt (ex: botão, caixa de texto, tabela). No ShowBox, widgets são criados dinamicamente a partir de comandos CLI.

### Command Pattern

Padrão de design onde operações são encapsuladas como objetos. No ShowBox, cada flag CLI (`--add-button`, `--set-title`) é um comando que implementa a interface `Command`.

### Command Registry

Registro central que mapeia nomes de comandos para suas implementações. Despacha execução quando parser identifica um comando.

### Tokenizer

Componente que divide argumentos CLI em tokens individuais (strings, flags, valores).

### Parser

Componente que analisa tokens e constrói uma árvore de comandos para execução.

### ParserDriver

Orquestrador que coordena tokenização, parsing e execução sequencial de comandos.

### ExecutionContext

Objeto de contexto passado durante execução de comandos, contendo referências ao ShowBox, layout atual, e estado.

### Layout

Sistema de organização de widgets do Qt. ShowBox usa `QVBoxLayout` (vertical) e `QHBoxLayout` (horizontal) para posicionar widgets.

### Signal/Slot

Mecanismo de comunicação do Qt. **Signals** são eventos emitidos (ex: botão clicado), **Slots** são métodos que respondem a signals.

### Theme Manager

Componente responsável por aplicar temas visuais (claro/escuro/sistema) a todos os widgets.

### Icon Helper

Utilitário para carregar ícones de arquivos ou do tema do sistema Qt.

### CustomTableWidget

Widget especializado baseado em `QTableWidget` com funcionalidades extras como busca integrada.

### CustomChartWidget

Widget especializado baseado em `QtCharts::QChart` para renderizar gráficos.

## Termos do Qt

### QWidget

Classe base para todos os elementos de interface gráfica no Qt.

### QDialog

Janela modal ou não-modal. ShowBox herda de `QDialog`.

### QVBoxLayout / QHBoxLayout

Layouts que organizam widgets verticalmente ou horizontalmente.

### QTabWidget

Widget de abas/tabs para navegação entre páginas.

### QPushButton / QCheckBox / QRadioButton

Componentes básicos de botão.

### QLineEdit

Campo de entrada de texto de uma linha.

### QTextEdit

Campo de entrada de texto multilinha.

### QTableWidget

Widget de tabela com células editáveis.

### QComboBox

Menu dropdown / caixa de combinação.

### QListWidget

Lista de itens selecionáveis.

### QSlider

Controle deslizante para valores numéricos.

### QProgressBar

Barra de progresso.

### QCalendarWidget

Widget de calendário para seleção de datas.

### QApplication

Classe principal que gerencia a aplicação Qt e event loop.

### Event Loop

Loop de eventos Qt que processa interações do usuário e atualiza a GUI.

### Parent-Child Ownership

Sistema de gerenciamento de memória do Qt onde objetos "filhos" são automaticamente deletados quando o "pai" é destruído.

### Meta-Object Compiler (moc)

Ferramenta Qt que processa headers e gera código para signals/slots.

## Termos de Build

### qmake

Sistema de build do Qt que processa arquivos `.pro` e gera Makefiles.

### .pro file

Arquivo de projeto qmake que especifica sources, headers, dependências, etc.

### VPATH

Caminho de busca para arquivos fonte no qmake.

### DESTDIR

Diretório de destino para binários compilados.

### OBJECTS_DIR

Diretório para arquivos objeto (.o) intermediários.

### MOC_DIR

Diretório para arquivos gerados pelo moc.

## Termos de CLI

### Flag

Argumento de linha de comando que começa com `--` (ex: `--title`, `--add-button`).

### Positional Argument

Argumento sem flag, identificado pela posição (ex: em `--add-button "OK"`, "OK" é posicional).

### stdin/stdout/stderr

Streams padrão de entrada, saída e erro em sistemas Unix/Linux.

### Exit Code

Código numérico retornado ao sair (0 = sucesso, != 0 = erro).

## Termos de Empacotamento

### AppImage

Formato de pacote universal Linux independente de distribuição.

### .deb

Formato de pacote Debian/Ubuntu.

### .rpm

Formato de pacote RedHat/Fedora/SUSE.

### Flatpak

Sistema de empacotamento e distribuição sandboxed para Linux.

### .dmg

Formato de imagem de disco para macOS.

### .msix

Formato de pacote moderno para Windows.

## Padrões de Design

### Builder Pattern

Padrão para construção de objetos complexos passo a passo. ShowBox usa isso com métodos `addXXX()`.

### Strategy Pattern

Padrão para definir família de algoritmos intercambiáveis. ThemeManager usa para temas.

### Registry Pattern

Padrão para manter registro central de objetos. CommandRegistry é um exemplo.

### Singleton Pattern

Padrão para garantir uma única instância. QApplication é essencialmente um singleton.

## Conceitos de C++

### Header (.h)

Arquivo de declaração com interfaces, classes, constantes.

### Implementation (.cpp/.cc)

Arquivo de implementação com código executável.

### const char*

Ponteiro para string C estilo (terminada em null).

### QString

Classe Qt para strings Unicode.

### nullptr

Ponteiro nulo em C++11+.

### override

Palavra-chave C++11 indicando que método sobrescreve método virtual da base.

### virtual

Método que pode ser sobrescrito por classes derivadas.

### Pure Virtual (= 0)

Método virtual sem implementação; torna a classe abstrata.

## Conceitos Shell

### Bash Script

Script executável em shell Bash (Bourne Again SHell).

### Shebang (#!/bin/bash)

Primeira linha de script indicando interpretador.

### Exit Status ($?)

Variável contendo código de saída do último comando.

### Pipe (|)

Conecta stdout de um comando ao stdin de outro.

### Redirect (>, <)

Redireciona streams de/para arquivos.

## Termos de Desenvolvimento

### Docker

Plataforma de containerização para ambientes reproduzíveis.

### Dockerfile

Arquivo de instruções para construir imagem Docker.

### Docker Volume

Sistema de persistência de dados entre container e host.

### Valgrind

Ferramenta de detecção de memory leaks e erros de memória.

### GDB

GNU Debugger para depuração de programas C/C++.

### Pandoc

Conversor de documentos (ex: Markdown → Man page).

### Man Page

Página de manual Unix acessível via comando `man`.

---

*Última atualização: 2026-01-12*
