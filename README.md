# VSC-CMake-CXX-Project-Template
A Template of Cross-Platform CMake-C++ Project for Visual Studio Code with Github Actions CI/CD.

## 1. Environment Setup

To run on Windows:

1. Install Visual Studio 2022 with the Desktop development with C++ workload.
2. To build, run the following command on powershell:
   ```pwsh
   .\scripts\msvc-bash.bat .\scripts\build.sh
   ```
3. To run:
  ```pwsh
  build\src\GrafixBoard.exe
  ```

To build & run on Linux:

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