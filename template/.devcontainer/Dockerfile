ARG ARCH=amd64
FROM --platform=linux/$ARCH ubuntu:24.04

ARG ARCH

RUN export DEBIAN_FRONTEND=noninteractive && \
  apt-get update && \
  apt-get install -yqq --no-install-recommends \
          build-essential \
          curl \
          ca-certificates \
          git \
          gdb \
          libssl-dev \
          ninja-build \
          python3 \
          python3-invoke \
          tzdata \
          unzip \
          libasound2-dev \
          libpulse-dev && \
  update-ca-certificates && \
  apt-get autoremove -yqq --purge && \
  rm -rf /var/lib/apt/lists/*

# Install CMake 3.31.6
RUN CMAKE_ARCH=$(uname -m) && \
    curl -fsSL -o cmake_install.sh "https://github.com/Kitware/CMake/releases/download/v3.31.6/cmake-3.31.6-linux-${CMAKE_ARCH}.sh" && \
    chmod +x cmake_install.sh && \
    ./cmake_install.sh --skip-license --prefix=/usr/local/ && \
    rm cmake_install.sh

# Create invoke completion script on Linux
RUN inv --print-completion-script bash > ~/.invoke-completion.sh
# Source the script at startup of each shell
RUN echo "source ~/.invoke-completion.sh" >> ~/.bashrc

RUN git config --system --add safe.directory '*'

WORKDIR /switchboard
