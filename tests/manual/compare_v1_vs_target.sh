#!/bin/bash
set -e

# Ensure we are in project root
if [[ ! -f "CMakeLists.txt" ]]; then
	echo "Please run from SHantilly project root"
	exit 1
fi

# Build Legacy (QMake)
echo "=== Building Legacy SHantilly ==="
pushd src/code/SHantilly
if command -v qmake6 &>/dev/null; then
	qmake6 SHantilly.pro
else
	echo "qmake6 not found, skipping legacy build (assuming it is built or environment issue)"
fi
if [[ -f Makefile ]]; then
	make -j$(nproc)
fi
popd

# Build Target (CMake)
echo "=== Building Target SHantilly ==="
mkdir -p build
pushd build
if command -v cmake &>/dev/null; then
	cmake ..
	make -j$(nproc)
else
	echo "cmake not found, skipping target build"
fi
popd

echo "=== Launching Comparison ==="
echo "Legacy (expecting 'Legacy Button'):"
if [[ -f src/code/SHantilly/bin/SHantilly-legacy ]]; then
	echo 'add pushbutton "Legacy Button" btn1' | ./src/code/SHantilly/bin/SHantilly-legacy &
	LEGACY_PID=$!
else
	echo "Legacy binary not found."
fi

echo "Target (expecting 'Target Button'):"
if [[ -f build/bin/SHantilly ]]; then
	echo 'add pushbutton "Target Button" btn2' | ./build/bin/SHantilly &
	TARGET_PID=$!
else
	echo "Target binary not found."
fi

echo "Press Enter to close applications..."
read
if [[ -n ${LEGACY_PID} ]]; then kill "${LEGACY_PID}" 2>/dev/null; fi
if [[ -n ${TARGET_PID} ]]; then kill "${TARGET_PID}" 2>/dev/null; fi
