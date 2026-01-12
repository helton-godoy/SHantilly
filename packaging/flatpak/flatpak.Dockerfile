 
# Dockerfile otimizado para construção de pacotes Flatpak
# Base: Debian Stable Slim
FROM debian:stable-slim

ENV DEBIAN_FRONTEND=noninteractive

# Instala apenas flatpak-builder e dependências básicas de rede
RUN apt-get update && apt-get install -y --no-install-recommends \
    flatpak-builder \
    ca-certificates \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
