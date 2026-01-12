FROM ubuntu:24.04 AS builder

# Evita perguntas interativas durante a instalação
ENV DEBIAN_FRONTEND=noninteractive

# Instalar dependências necessárias
RUN apt-get update -qq && apt-get install -qqy --no-install-recommends \
    build-essential \
    cmake \
    git \
    wget \
    file \
    libfuse2 \
    python3-pip \
    qt6-base-dev \
    && rm -rf /var/lib/apt/lists/*

# Baixar o linuxdeploy
WORKDIR /tools
RUN wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage \
    && chmod +x linuxdeploy-x86_64.AppImage

# Instala Qt 6.6.1 via aqt
RUN pip install aqtinstall \
    && aqt install-qt linux desktop 6.6.1 gcc_64 -O /opt/qt

# Configura ambiente Qt
ENV PATH="/opt/qt/6.6.1/gcc_64/bin:${PATH}"
ENV QT_ROOT="/opt/qt/6.6.1/gcc_64"

WORKDIR /build

# Copiar código fonte
COPY . /build

# Construir o projeto (assumindo qmake)
RUN mkdir build && cd build && qmake .. && make

# Criar AppDir
RUN mkdir -p AppDir/usr/bin AppDir/usr/lib AppDir/usr/share/applications AppDir/usr/share/icons/hicolor/256x256/apps

# Copiar binário
RUN cp build/showbox AppDir/usr/bin/ || cp src/code/showbox/bin/showbox AppDir/usr/bin/

# Copiar bibliotecas Qt necessárias (simplificado)
RUN cp -r /opt/qt/6.6.1/gcc_64/lib/*.so* AppDir/usr/lib/ 2>/dev/null || true

# Criar desktop file
RUN echo "[Desktop Entry]\nName=ShowBox\nExec=showbox\nIcon=showbox\nType=Application\nCategories=Utility;" > AppDir/usr/share/applications/showbox.desktop

# Copiar ícone (se existir)
RUN cp src/code/showbox/icon.png AppDir/usr/share/icons/hicolor/256x256/apps/showbox.png 2>/dev/null || echo "Icon not found, skipping"

# Criar AppRun
RUN echo '#!/bin/bash\nexec "$APPDIR/usr/bin/showbox" "$@"' > AppDir/AppRun && chmod +x AppDir/AppRun

# Usar linuxdeploy para criar AppImage
RUN /tools/linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage

# Copiar AppImage para /dist
RUN mkdir -p /dist && cp *.AppImage /dist/