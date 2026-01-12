#!/bin/bash

set -e

echo "Building AppImage using Docker..."

# Construir a imagem Docker
docker build -f packaging/appimage/appimage.Dockerfile -t showbox-appimage .

echo "Running container to build AppImage..."

# Executar o container e montar o diretório dist
docker run --rm -v $(pwd)/dist:/dist showbox-appimage

echo "AppImage build completed. Check dist/ directory for the .AppImage file."