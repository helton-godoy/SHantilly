# Guia de Instalação

Este guia cobre todas as formas de instalar o SHantilly no seu sistema.

---

## Requisitos do Sistema

### Dependências de Runtime

- **Qt6 Base** (6.2 ou superior)
- **Qt6 Charts** (para widgets de gráfico)
- **Sistema Linux** x86_64

### Dependências Opcionais

- **Fontes de ícones** (Adwaita, Breeze) para ícones padrão

---

## Métodos de Instalação

### 1. Pacote DEB (Debian/Ubuntu)

A forma mais fácil para sistemas baseados em Debian.

```bash
# Baixar o pacote mais recente
wget https://github.com/seu-usuario/SHantilly/releases/latest/download/SHantilly_1.0_amd64.deb

# Instalar
sudo dpkg -i SHantilly_1.0_amd64.deb

# Resolver dependências (se necessário)
sudo apt-get install -f
```

**Sistemas suportados:**

- Debian 12 (Bookworm) ou superior
- Debian 13 (Trixie)
- Ubuntu 22.04 LTS ou superior
- Ubuntu 24.04 LTS

---

### 2. AppImage (Universal)

Executável portátil que funciona em qualquer distribuição Linux moderna.

```bash
# Baixar
wget https://github.com/seu-usuario/SHantilly/releases/latest/download/SHantilly-1.0-x86_64.AppImage

# Tornar executável
chmod +x SHantilly-1.0-x86_64.AppImage

# Executar diretamente
./SHantilly-1.0-x86_64.AppImage

# Ou mover para o PATH
sudo mv SHantilly-1.0-x86_64.AppImage /usr/local/bin/SHantilly
```

> **Nota**: AppImage inclui todas as dependências Qt6 embutidas.

---

### 3. Compilação do Código-Fonte

Para desenvolvedores ou sistemas não suportados.

#### 3.1 Instalar Dependências de Build

**Debian/Ubuntu:**

```bash
sudo apt-get install build-essential qt6-base-dev qt6-charts-dev \
    libgl1-mesa-dev cmake
```

**Fedora:**

```bash
sudo dnf install qt6-qtbase-devel qt6-qtcharts-devel gcc-c++ make cmake
```

**Arch Linux:**

```bash
sudo pacman -S qt6-base qt6-charts base-devel cmake
```

#### 3.2 Clonar e Compilar

```bash
# Clonar repositório
git clone https://github.com/seu-usuario/SHantilly.git
cd SHantilly

# Compilar
cd src/code/SHantilly
qmake6 SHantilly.pro
make -j$(nproc)

# O binário estará em:
# ./bin/SHantilly
```

#### 3.3 Instalar (opcional)

```bash
# Instalar no sistema
sudo make install

# Ou adicionar ao PATH manualmente
echo 'export PATH="$PATH:/caminho/para/SHantilly/src/code/SHantilly/bin"' >> ~/.bashrc
source ~/.bashrc
```

---

## Verificação da Instalação

Após instalar, verifique se está funcionando:

```bash
# Verificar versão
SHantilly --version

# Teste rápido
echo 'add label "Instalação bem sucedida!" | add pushbutton "OK" exit default' | SHantilly
```

Se uma janela aparecer com a mensagem, a instalação foi bem sucedida! 🎉

---

## Configuração Pós-Instalação

### Configuração de Tema (Opcional)

O SHantilly detecta automaticamente o tema Qt do sistema. Para forçar um tema específico:

```bash
# Usar tema Fusion (neutro)
export QT_STYLE_OVERRIDE=Fusion

# Ou via argumento
SHantilly --style fusion < comandos.txt
```

### Variáveis de Ambiente

| Variável            | Descrição                            |
| ------------------- | ------------------------------------ |
| `QT_STYLE_OVERRIDE` | Força um estilo Qt específico        |
| `QT_SCALE_FACTOR`   | Escala da interface (HiDPI)          |
| `SHANTILLY_RC`      | Caminho para arquivo de configuração |

---

## Desinstalação

### Pacote DEB

```bash
sudo apt-get remove SHantilly
```

### AppImage

```bash
rm /usr/local/bin/SHantilly
```

### Compilação Manual

```bash
cd SHantilly/src/code/SHantilly
sudo make uninstall
```

---

## Próximos Passos

Instalação concluída? Siga para o [Início Rápido](getting-started.md) para criar seu primeiro diálogo!
