FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

USER root

RUN apt-get update && apt-get install -y \
    cmake \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*