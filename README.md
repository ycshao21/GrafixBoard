# VSC-CMake-CXX-Project-Template
A Template of Cross-Platform CMake-C++ Project for Visual Studio Code with Github Actions CI/CD.

## 1. Environment Setup

### 1.1. Linux with GCC and Clang

Install the following softwares:

```bash
export LLVM_VERSION=20

apt-get install -y \
    git apt-utils lsb-release software-properties-common gnupg  \
    ninja-build pkg-config build-essential gdb
  
cd /usr/local && git clone https://github.com/microsoft/vcpkg.git && \ 
    cd vcpkg && ./bootstrap-vcpkg.sh  && \
    wget -O /tmp/kitware-archive.sh \
        https://apt.kitware.com/kitware-archive.sh && \
    bash /tmp/kitware-archive.sh && \
    apt-get update && apt-get install -y cmake && \
    wget -O /tmp/llvm.sh https://apt.llvm.org/llvm.sh && \
    chmod +x /tmp/llvm.sh && /tmp/llvm.sh ${LLVM_VERSION} && \
    apt-get install -y libomp-${LLVM_VERSION}-dev  
```

### 1.2. Windows with MSVC

If you are using Windows, first install [Visual Studio](https://visualstudio.microsoft.com/), and then choose the `Desktop development with C++` workload. 

After that, open Powershell, install [scoop](https://scoop.sh/) and the following softwares:

```pwsh
# Allow script execution
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
# Install scoop
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
# Install softwares
scoop install cmake ninja git vcpkg
```

### 1.3. Windows with MinGW

You can skip the Visual Studio installation but install MinGW with scoop directly:

```pwsh
# Allow script execution
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
# Install scoop
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
# Install softwares
scoop install cmake ninja git mingw vcpkg
```

## 2. How to Use this Template
### 2.1. Create Your Own Repository

1. Click `Use this template` -> `Create a new repository` on the top right corner of this page.
2. Clone your repository to your local machine.


### 2.2 Generate a Project

Parameters:

- `<project-name>`: Name of your project. Can be any valid identifier.
- `<project-type>`: Type of the project you want to use. Can be:
  - `0`: cxx_exe, a simple C++ executable project.
  - `1`: cxx_lib, a C++ lib & test project.
  - `2`: cuda_exe, a CUDA executable project.
  - `3`: cuda_lib, a CUDA lib & test project.

```bash
python ./.templates/gen_project.py -n <project-name> -t <project-type>
```

### 2.3. Build a Executable or Library

After generation, build the project by:

```bash
bash ./scripts/build.sh
```

Or if you are using windows and want to use MSVC deliberately, run the following command in Powershell or CMD:

```pwsh
.\scripts\msvc-bash.bat .\scripts\build.sh --prune-env-path
```

### 2.4. Reset Project

If you chose a wrong template, don't be worry. Reset the project by:

```bash
python ./.templates/gen_project.py --reset
```

Or directly switch to a new template:

```bash
python ./.templates/gen_project.py -n <project-name> -t <project-type>
```

## 5. Remove Template

After generating your project, you can remove the ".template" directory by deleting it directly, or use the following command:

```bash
python ./.template/gen_project.py --remove-templates
```
