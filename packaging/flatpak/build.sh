#!/bin/bash
#
# SHantilly Flatpak Builder
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
DIST_DIR="${PROJECT_ROOT}/dist"

echo "=== SHantilly Flatpak Builder ==="

# Install Flatpak SDK if needed
flatpak install -y flathub org.kde.Platform//6.6 org.kde.Sdk//6.6 || true

# Create dist directory
mkdir -p "${DIST_DIR}"

cd "${SCRIPT_DIR}"

# Build Flatpak
flatpak-builder --force-clean --repo=repo build-dir io.github.SHantilly.yaml

# Create bundle
flatpak build-bundle repo "${DIST_DIR}/SHantilly.flatpak" io.github.SHantilly

echo ""
echo "=== Build Complete ==="
echo "Flatpak bundle: ${DIST_DIR}/SHantilly.flatpak"
