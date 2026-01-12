# Ferramentas de Desenvolvimento

## Build System

### qmake (Qt Build)

**Arquivo**: `showbox.pro`

```bash
# Configurar build
qmake6

# Compilar
make -j$(nproc)

# Instalar
sudo make install
```

**Configurações**:

- `CONFIG+=debug` - Build de debug
- `CONFIG+=release` - Build otimizado
- `CONFIG+=coverage` - Com code coverage

### Make

```bash
# Build completo
make

# Build específico
make showbox

# Limpar
make clean

# Instalação em diretório custom
make install INSTALL_ROOT=/tmp/install
```

## Desenvolvimento

### Docker

**Iniciar ambiente**:

```bash
./tools/start-docker-dev.sh
```

**Dockerfile**: `src/dev.Dockerfile`

**Vantagens**:

- Ambiente reproduzível
- Todas dependências inclusas
- Isolamento do host

### Qt Creator

IDE oficial Qt com:

- Autocomplete Qt-aware
- Debugger integrado
- Designer visual (não usado no ShowBox)
- Profiler

**Abrir projeto**:

```bash
qtcreator src/code/showbox/showbox.pro
```

### VS Code

**Extensões recomendadas**:

- C/C++ (Microsoft)
- C++ Intellisense
- CMake Tools
- Qt for Python (syntax highlight)

**Configuração** (`.vscode/c_cpp_properties.json`):

```json
{
  "configurations": [{
    "name": "Linux",
    "includePath": [
      "${workspaceFolder}/**",
      "/usr/include/x86_64-linux-gnu/qt6"
    ],
    "compilerPath": "/usr/bin/g++",
    "cStandard": "c17",
    "cppStandard": "c++17"
  }]
}
```

## Debug

### GDB

```bash
gdb ./src/code/showbox/bin/showbox
(gdb) run --add-button "Test"
(gdb) bt  # Backtrace
(gdb) print var
(gdb) break showbox.cc:123
```

### Valgrind

**Memory leaks**:

```bash
valgrind --leak-check=full ./showbox ...
```

**Thread errors**:

```bash
valgrind --tool=helgrind ./showbox ...
```

### AddressSanitizer

```bash
gdb ./src/code/showbox/bin/showbox
(gdb) run --add-button "Test"
# Capturar janela
```

## Documentação

### Pandoc

Converte Markdown → Man page:

```bash
pandoc -s -f markdown -t man \
  man/showbox.md -o man/showbox.1
```

### Doxygen (Futuro)

Para gerar documentação API:

```bash
doxygen Doxyfile
firefox docs/html/index.html
```

## Empacotamento

### Scripts Automatizados

```bash
# Debian
./tools/start-docker-build-deb.sh

# RPM
./tools/start-docker-build-rpm.sh

# AppImage
./tools/start-docker-appimage.sh

# Flatpak
./tools/start-docker-flatpak.sh
```

### Ferramentas

- **dpkg-deb** - Criar pacotes .deb
- **rpmbuild** - Criar pacotes .rpm
- **appimagetool** - Criar AppImage
- **flatpak-builder** - Criar Flatpak

## Controle de Versão

### Git

```bash
# Clone
git clone https://github.com/showbox/showbox.git

# Branch
git checkout -b feature/nova-feature

# Commit
git commit -m "feat: descrição"

# Push
git push origin feature/nova-feature
```

### Git GUI

- **GitKraken**
- **GitHub Desktop**
- **git gui**

## Performance

### Profiling

```bash
# CPU profiling
perf record ./showbox ...
perf report

# QML Profiler (não aplicável)
```

### Benchmarking

```cpp
QBENCHMARK {
    dialog.addPushButton("Test", "btn", false, false, false);
}
```

## Úteis

### Tree (Estrutura)

```bash
tree -L 2 src/
```

### Find (Busca)

```bash
find . -name "*.cpp" -exec grep "ShowBox" {} +
```

### cloc (Linhas de Código)

```bash
cloc src/
```

---

**Produtividade**: Conhecer suas ferramentas acelera o desenvolvimento!
