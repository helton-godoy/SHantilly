FROM debian:trixie-slim

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
# Build dependencies (dev packages)
RUN apt-get update -qq && apt-get install -qqy \
    build-essential \
    cmake \
    qmake6 \
    qt6-base-dev \
    qt6-charts-dev \
    libqt6charts6-dev \
    libqt6svg6-dev \
    libqt6opengl6-dev \
    libgl-dev \
    libopengl-dev \
    # Runtime libraries
    libqt6charts6 \
    libqt6svg6 \
    libqt6opengl6 \
    libqt6widgets6 \
    libqt6gui6 \
    libqt6core6t64 \
    libgtest-dev \
    # XCB dependencies for GUI rendering
    libxcb-cursor0 \
    libxcb-xinerama0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxkbcommon-x11-0 \
    # Icon themes for GUI icon support
    adwaita-icon-theme \
    hicolor-icon-theme \
    # Utilities
    git \
    locales \
    curl \
    wget \
    pandoc \
    clang-format \
    clang-tidy \
    cppcheck \
    doxygen \
    debhelper \
    python3-pip \
    mkdocs \
    mkdocs-material \
    lcov \
    default-jre-headless \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Install Sonar Scanner
RUN curl -sSLo /tmp/sonar-scanner.zip https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.2.1.4610-linux-x64.zip \
    && unzip -q /tmp/sonar-scanner.zip -d /opt \
    && ln -s /opt/sonar-scanner-6.2.1.4610-linux-x64/bin/sonar-scanner /usr/local/bin/sonar-scanner \
    && rm /tmp/sonar-scanner.zip

# Generate locale
RUN sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Set working directory
WORKDIR /app

# Default command
CMD ["/bin/bash"]

