# Visão Geral do Projeto Showbox

## Descrição

O **Showbox** é uma biblioteca de widgets GUI para scripts shell, desenvolvida em C++ utilizando o framework Qt. Permite criar interfaces gráficas interativas através de comandos enviados via entrada padrão (stdin), facilitando a criação de diálogos e formulários a partir de scripts de shell.

## Objetivo Principal

Fornecer uma ponte entre scripts shell e interfaces gráficas modernas, permitindo que desenvolvedores criem aplicações com interface visual sem necessidade de conhecimento avançado em linguagens de programação GUI.

## Arquitetura Técnica

- **Linguagem**: C++ com Qt Framework
- **Paradigma**: Programação orientada a objetos
- **Padrão**: Command Pattern para processamento de comandos
- **Interface**: Parsing de comandos textuais via stdin
- **Saída**: Valores dos widgets via stdout

## Componentes Principais

### Classe ShowBox

Classe principal que herda de `QDialog` e gerencia:

- Criação dinâmica de widgets Qt
- Layout automático e posicionamento
- Gerenciamento de eventos e sinais
- Suporte a temas (claro/escuro)

### DialogParser

Thread responsável por:

- Parsing de comandos textuais
- Execução sequencial de operações
- Comunicação assíncrona com interface

### Sistema de Comandos

Implementa pattern de command registry com suporte a:

- `add`: Adicionar widgets (botões, caixas de texto, listas, etc.)
- `set`: Configurar propriedades de widgets
- `query`: Consultar valores de widgets
- `position`: Reposicionar elementos na interface

## Ambiente de Desenvolvimento

- **Containerização**: Docker para ambiente reprodutível
- **Build System**: Qt qmake/Makefile
- **Testes**: Framework Qt Test
- **Empacotamento**: Suporte a múltiplas distribuições (DEB, RPM, AppImage, DMG, MSI)

## Casos de Uso

- Scripts de instalação e configuração
- Ferramentas administrativas
- Interfaces para comandos complexos
- Formulários interativos em shell

## Vantagens

- **Simplicidade**: Interface baseada em texto
- **Flexibilidade**: Suporte a diversos tipos de widgets
- **Integração**: Fácil incorporação em scripts existentes
- **Portabilidade**: Funciona em múltiplas plataformas
- **Customização**: Suporte a temas e layouts complexos

## Limitações Técnicas

- Dependente do Qt e X11/Wayland
- Execução síncrona (bloqueante)
- Parsing limitado a formato específico
- Sem suporte nativo a expressões complexas

## Estado do Projeto

- **Versão**: 1.0
- **Licença**: GPL v3
- **Mantenedor**: Andriy Martynets
- **Status**: Estável e maduro
