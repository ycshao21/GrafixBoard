# VSC-CMake-CXX-Project-Template
A Template of Cross-Platform CMake-C++ Project for Visual Studio Code with Github Actions CI/CD.

## 1. Environment Setup

```bash
IMAGE_NAME=jamesnulliu/grafixboard:vulkan1.4.304-torch2.6-cuda12.6-ubuntu24.04
CONTAINER_NAME=whatever-you-like

docker pull ${IMAGE_NAME}

docker run -it --name ${CONTAINER_NAME} --gpus all --network=host \
  ${IMAGE_NAME} /bin/bash

# Exit the container ==========

docker start ${CONTAINER_NAME}
docker exec -it ${CONTAINER_NAME} /bin/bash

# Inside the container ==========

# [TODO] @jamesnulliu: 
#     This command should be done when building the image, not by users.
apt install -y libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config
```

## 2. How to Use this Template
### 2.1. Build 

After generation, build the project by:

```bash
bash ./scripts/build.sh
```

If you are using windows and want to use MSVC deliberately, run the following command in Powershell or CMD to build:

```pwsh
.\scripts\msvc-bash.bat .\scripts\build.sh --prune-env-path
```