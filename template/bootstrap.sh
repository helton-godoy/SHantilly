# Bootstrap script for new contributors
# -------------------------------------------------
# This script automates the initial setup of the SHantilly project.
# It assumes a Unix‑like environment (Linux/macOS) with git, curl, and a C++ compiler installed.
#
# Steps performed:
#   1. Clone the repository (if not already inside a clone).
#   2. Install vcpkg (if not present).
#   3. Install Trunk (if not present).
#   4. Install project dependencies via vcpkg.
#   5. Configure the project with CMake (using the vcpkg toolchain).
#   6. Build the project and run unit tests.
# -------------------------------------------------

set -euo pipefail

# Helper to print colored messages
info() { echo -e "\033[1;34m[INFO]\033[0m $*"; }
error() { echo -e "\033[1;31m[ERROR]\033[0m $*" >&2; }

# 1. Ensure we are inside the repository root
if [[ ! -f "CMakeLists.txt" ]]; then
	error "CMakeLists.txt not found. Run this script from the repository root."
	exit 1
fi

# 2. Install vcpkg if missing
if [[ ! -d "${HOME}/vcpkg" ]]; then
	info "Cloning vcpkg..."
	git clone https://github.com/microsoft/vcpkg.git "${HOME}/vcpkg"
	"${HOME}/vcpkg/bootstrap-vcpkg.sh"
else
	info "vcpkg already installed at ${HOME}/vcpkg"
fi

# 3. Install Trunk if missing
if ! command -v trunk >/dev/null 2>&1; then
	info "Installing Trunk..."
	curl https://get.trunk.io -fsSL | bash
else
	info "Trunk already installed"
fi

# 4. Install project dependencies via vcpkg
info "Installing project dependencies via vcpkg..."
"${HOME}/vcpkg/vcpkg" install --triplet x64-linux

# 5. Configure CMake with vcpkg toolchain
info "Configuring CMake..."
mkdir -p build && cd build
cmake .. \
	-DCMAKE_TOOLCHAIN_FILE="${HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DCMAKE_BUILD_TYPE=Release

# 6. Build the project
info "Building the project..."
cmake --build . -j$(nproc)

# 7. Run unit tests (if any)
if [[ -d "tests" ]]; then
	info "Running unit tests..."
	ctest --output-on-failure
fi

info "Setup complete! You can now run the application binaries from ./build/bin."
