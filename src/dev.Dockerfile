FROM debian:trixie-slim

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
RUN apt-get update -qq && apt-get install -qqy \
    build-essential \
    qt6-base-dev \
    libqt6charts6-dev \
    libqt6svg6-dev \
    qmake6 \
    git \
    locales \
    curl \
    wget \
    pandoc \
    debhelper \
    libxcb-cursor0 \
    && rm -rf /var/lib/apt/lists/*

# Generate locale
RUN sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Set working directory
WORKDIR /app

# Default command
CMD ["/bin/bash"]

