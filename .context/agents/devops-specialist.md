# DevOps Specialist Agent

## Foco

Gerenciar build, empacotamento, CI/CD e distribuição do **ShowBox** para múltiplas plataformas.

## Plataformas Suportadas

### Linux

- **Debian/Ubuntu** (.deb)
- **RedHat/Fedora** (.rpm)
- **Universal** (AppImage)
- **Flatpak** (sandboxed)

### macOS

- **DMG** (disk image)

### Windows

- **MSIX** (Microsoft Store)

## Sistema de Build

### qmake (Qt Build System)

**Arquivo Principal**: `src/code/showbox/showbox.pro`

```qmake
TEMPLATE = app
CONFIG += qt thread release c++17
QT += widgets svg charts

TARGET = showbox
DESTDIR = ../../bin
OBJECTS_DIR = ../../obj

HEADERS += showbox.h ...
SOURCES += showbox.cc ...

# Man page automática
man_page.commands = pandoc -s -f markdown -t man ...
QMAKE_EXTRA_TARGETS += man_page
```

**Build Manual**:

```bash
cd src/code/showbox
qmake6
make -j$(nproc)
make install INSTALL_ROOT=/tmp/install
```

### Docker Development

**Ambiente Reproduzível**:

```bash
# Iniciar dev container
./tools/start-docker-dev.sh

# Dentro do container
cd /app/src/code/showbox
qmake6 && make
```

**Dockerfile** (`src/dev.Dockerfile`):

```dockerfile
FROM debian:trixie-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    qt6-base-dev \
    libqt6charts6-dev \
    libqt6svg6-dev \
    qmake6 \
    pandoc

WORKDIR /app
CMD ["/bin/bash"]
```

## Empacotamento

### 1. Debian Package (.deb)

**Diretório**: `packaging/deb/`

**Estrutura**:

```shell
deb/
├── build.sh          # Script de build
├── control           # Metadados do pacote
└── debian/
    ├── changelog
    ├── compat
    ├── control
    ├── rules
    └── showbox.install
```

**Build**:

```bash
./tools/start-docker-build-deb.sh

# Ou manualmente
cd packaging/deb
./build.sh

# Resultado: showbox_1.0.0_amd64.deb
```

**Instalação**:

```bash
sudo dpkg -i showbox_1.0.0_amd64.deb
sudo apt-get install -f  # Fix dependencies
```

**control file**:

```header
Package: showbox
Version: 1.0.0
Section: utils
Priority: optional
Architecture: amd64
Depends: libqt6core6, libqt6gui6, libqt6widgets6, libqt6charts6, libqt6svg6
Maintainer: Showbox Team <team@showbox.dev>
Description: GUI widgets for shell scripts
 ShowBox provides rich Qt6-based GUI widgets that can be
 controlled from shell scripts via command line interface.
```

### 2. RPM Package

**Diretório**: `packaging/rpm/`

**Build**:

```bash
./tools/start-docker-build-rpm.sh
```

**SPEC file** (`showbox.spec`):

```spec
Name:           showbox
Version:        1.0.0
Release:        1%{?dist}
Summary:        GUI widgets for shell scripts

License:        GPLv3
URL:            https://github.com/showbox/showbox
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtcharts-devel
Requires:       qt6-qtbase
Requires:       qt6-qtcharts

%description
ShowBox provides rich Qt6-based GUI widgets for shell scripts.

%prep
%autosetup

%build
cd src/code/showbox
qmake6
make %{?_smp_mflags}

%install
cd src/code/showbox
make install INSTALL_ROOT=%{buildroot}

%files
%{_bindir}/showbox
%{_mandir}/man1/showbox.1*

%changelog
* Sun Jan 12 2026 Team <team@showbox.dev> - 1.0.0-1
- Initial release
```

### 3. AppImage (Universal Linux)

**Diretório**: `packaging/appimage/`

**Vantagens**:

- Roda em qualquer distribuição
- Não precisa instalação
- Self-contained

**Build**:

```bash
./tools/start-docker-appimage.sh

# Resultado: ShowBox-1.0.0-x86_64.AppImage
```

**Estrutura**:

```shell
AppDir/
├── AppRun
├── showbox.desktop
├── showbox.png
├── usr/
    ├── bin/showbox
    └── lib/
        └── (bibliotecas Qt6)
```

**Execução**:

```bash
chmod +x ShowBox-1.0.0-x86_64.AppImage
./ShowBox-1.0.0-x86_64.AppImage --add-button "Test"
```

### 4. Flatpak

**Diretório**: `packaging/flatpak/`

**Manifest** (`com.showbox.ShowBox.yaml`):

```yaml
app-id: com.showbox.ShowBox
runtime: org.kde.Platform
runtime-version: '6.6'
sdk: org.kde.Sdk
command: showbox

modules:
  - name: showbox
    buildsystem: qmake
    sources:
      - type: dir
        path: ../../src/code/showbox
```

**Build**:

```bash
./tools/start-docker-flatpak.sh

flatpak-builder --force-clean build-dir \
  packaging/flatpak/com.showbox.ShowBox.yaml
```

**Instalação**:

```bash
flatpak install showbox.flatpak
flatpak run com.showbox.ShowBox
```

### 5. macOS DMG

**Diretório**: `packaging/dmg/`

**Requerimentos**:

- macOS ou hackintosh
- Qt6 for macOS
- create-dmg tool

**Build**:

```bash
cd src/code/showbox
qmake -spec macx-clang
make
macdeployqt ../../bin/showbox.app -dmg

# Resultado: showbox.dmg
```

### 6. Windows MSIX

**Diretório**: `packaging/msix/`

**Requerimentos**:

- Windows SDK
- Qt6 for Windows
- Visual Studio

**Build**:

```powershell
cd src\code\showbox
qmake -tp vc
msbuild showbox.vcxproj
```

## CI/CD

### GitHub Actions

**Arquivo**: `.github/workflows/build.yml`

```yaml
name: Build ShowBox

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  build-deb:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y qt6-base-dev libqt6charts6-dev
      
      - name: Build
        run: |
          cd src/code/showbox
          qmake6
          make -j$(nproc)
      
      - name: Test
        run: |
          cd tests
          ./run_tests.sh
      
      - name: Package
        run: |
          cd packaging/deb
          ./build.sh
      
      - name: Upload Artifact
        uses: actions/upload-artifact@v3
        with:
          name: showbox-deb
          path: packaging/deb/*.deb

  build-rpm:
    runs-on: ubuntu-latest
    container: fedora:latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Dependencies
        run: |
          dnf install -y qt6-qtbase-devel qt6-qtcharts-devel rpm-build
      
      - name: Build RPM
        run: |
          cd packaging/rpm
          ./build.sh

  build-appimage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Build AppImage
        run: |
          ./tools/start-docker-appimage.sh
      
      - name: Upload
        uses: actions/upload-artifact@v3
        with:
          name: showbox-appimage
          path: "*.AppImage"

  release:
    needs: [build-deb, build-rpm, build-appimage]
    runs-on: ubuntu-latest
    if: startsWith(github.ref, 'refs/tags/v')
    steps:
      - name: Download Artifacts
        uses: actions/download-artifact@v3
      
      - name: Create Release
        uses: softprops/action-gh-release@v1
        with:
          files: |
            showbox-deb/*.deb
            showbox-rpm/*.rpm
            showbox-appimage/*.AppImage
```

### Build Matrix

| Platform     | Builder | Output   | Deploy   |
|--------------|---------|----------|----------|
| Debian 11    | Docker  | .deb     | apt repo |
| Debian 12    | Docker  | .deb     | apt repo |
| Ubuntu 22.04 | Docker  | .deb     | apt repo |
| Ubuntu 24.04 | Docker  | .deb     | apt repo |
| Fedora 39    | Docker  | .rpm     | dnf repo |
| Fedora 40    | Docker  | .rpm     | dnf repo |
| Universal    | Docker  | AppImage | GitHub   |
| Flatpak      | Docker  | .flatpak | Flathub  |

## Distribuição

### Repositório APT (Debian/Ubuntu)

**Estrutura**:

```shell
apt-repo/
├── dists/
│   └── stable/
│       └── main/
│           └── binary-amd64/
│               └── Packages.gz
└── pool/
    └── main/
        └── s/
            └── showbox/
                └── showbox_1.0.0_amd64.deb
```

**Comandos**:

```bash
# Gerar Packages file
dpkg-scanpackages pool/ | gzip > dists/stable/main/binary-amd64/Packages.gz

# Assinar repositório
gpg --armor --detach-sign -o Release.gpg Release
```

**Uso**:

```bash
# Adicionar repositório
echo "deb https://repo.showbox.dev/apt stable main" | \
  sudo tee /etc/apt/sources.list.d/showbox.list

# Instalar
sudo apt update
sudo apt install showbox
```

### Flathub

**Submit**:

```bash
# Fork flathub repo
git clone https://github.com/flathub/com.showbox.ShowBox

# Add manifest
cp packaging/flatpak/com.showbox.ShowBox.yaml .

# Submit PR
git commit -m "Add ShowBox 1.0.0"
git push
```

## Versionamento

**Scheme**: `MAJOR.MINOR.PATCH`

- **MAJOR**: Breaking changes na CLI
- **MINOR**: Novos widgets/features
- **PATCH**: Bug fixes

**Tagging**:

```bash
git tag -a v1.0.0 -m "Release 1.0.0"
git push origin v1.0.0
```

## Checklist de Release

- [ ] Versão atualizada em todos `showbox.pro`
- [ ] Changelog atualizado
- [ ] Documentação atualizada
- [ ] Testes passando
- [ ] Build em todas plataformas OK
- [ ] Packages gerados sem erros
- [ ] Tag criado no Git
- [ ] Release notes escritos
- [ ] Artifacts uploaded para GitHub
- [ ] Repositórios atualizados

---

**Automação é chave**: Quanto mais automatizado, menos erros humanos!
