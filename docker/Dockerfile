FROM intel/oneapi-vtune:2025.0.2-0-devel-ubuntu24.04

# https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2023-0/error-pmu-resources-unavailable.html

LABEL maintainer="JamesNULLiu jamesnulliu@gmail.com"
LABEL version="0.1"

ARG DEBIAN_FRONTEND=noninteractive
ENV LANGUAGE=en_US.UTF-8
ENV LANG=en_US.UTF-8
ARG LLVM_VERSION=21

# Some basic tools
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    git apt-utils lsb-release software-properties-common gnupg  \
    vim-gtk3 wget p7zip-full ninja-build curl jq pkg-config \
    build-essential gdb htop libxinerama-dev libxcursor-dev xorg-dev \
    libglu1-mesa-dev libnss3-dev

# Vcpkg, Cmake
RUN cd /usr/local && git clone https://github.com/microsoft/vcpkg.git && \
    cd vcpkg && ./bootstrap-vcpkg.sh  && \
    wget -O /tmp/kitware-archive.sh \
    https://apt.kitware.com/kitware-archive.sh && \
    bash /tmp/kitware-archive.sh && \
    apt-get update && apt-get install -y cmake 

# LLVM
RUN wget -O /tmp/llvm.sh https://apt.llvm.org/llvm.sh && \
    chmod +x /tmp/llvm.sh && apt-get update && \
    /tmp/llvm.sh ${LLVM_VERSION} && \
    apt-get install -y libomp-${LLVM_VERSION}-dev

# Install Miniconda3 and conda env
RUN wget -O /tmp/miniconda3.sh \
    https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh && \
    mkdir -p /root/miniconda3 && \
    bash /tmp/miniconda3.sh -b -u -p /root/miniconda3 && \
    \. /root/miniconda3/bin/activate && \
    conda init --all && \
    conda upgrade -y libstdcxx-ng -c conda-forge

# Some final steps
COPY data/.vimrc data/.inputrc data/.bashrc.in /tmp/
RUN touch /root/.bashrc && cat /tmp/.bashrc.in >> /root/.bashrc && \
    mv /tmp/.vimrc /root/.vimrc && \
    mv /tmp/.inputrc /root/.inputrc && \
    apt-get update && apt-get upgrade -y && apt-get autoremove -y && \
    apt-get clean && rm -rf /var/lib/apt/lists/* \
    git config --system --unset-all user.name || true && \
    git config --system --unset-all user.email || true && \
    git config --global --unset-all user.name || true && \
    git config --global --unset-all user.email || true
