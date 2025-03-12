# CMake Parameters

## Optional Parameters

- `BUILD_SHARED_LIBS`: Whether to build shared libaraies.  
  See: [lib/CMakeLists.txt](../lib/CMakeLists.txt).  
  Default: `OFF` (to build static libraries).  
  Note: This may affact all sub-projects.
- `CMAKE_CXX_SCAN_FOR_MODULES`: Whether to use C++ modules.  
  See: [cmake/compilers/cxx-compiler-configs.cmake](../cmake/compilers/cxx-compiler-configs.cmake).  
  Default: `OFF`.  
  Possible values: `ON`, `OFF`.
- `CMAKE_CXX_STANDARD`: C++ standard.  
  See: [cmake/compilers/cxx-compiler-configs.cmake](../cmake/compilers/cxx-compiler-configs.cmake).  
  Default: *Auto-Detect*.  
  Possible values: `14`, `17`, `20`, `23`.
- `CMAKE_EXECUTABLE_SUFFIX`: Executable suffix.  
  See: [CMakelists.txt](../CMakeLists.txt).  
  Default: `.exe`.
- `ENV{CC}`: C compiler.  
  See: [cmake/compilers/cxx-compiler-configs.cmake](../cmake/compilers/cxx-compiler-configs.ckmake).  
  Default: *Auto-Detect*.  
  Possible values: `gcc`, `clang`, `cl`, etc.
- `ENV{CXX}`: C++ compiler.  
  See: [cmake/compilers/cxx-compiler-configs.cmake](../cmake/compilers/cxx-compiler-configs.cmake).  
  Default: *Auto-Detect*.  
  Possible values: `gcc++`, `clang++`, `cl`, etc.
- `PROJECT_NAMESPACE`: Project namespace; Library targets should have an alias with project namespace as the prefix.  
  See: [CMakelists.txt](../CMakeLists.txt).  
  Default: `_template_project_name_`;
- `STACK_SIZE`: Stack size for the executable targets (if any).  
  See: [cmake/compilers/cxx-compiler-configs.cmake](../cmake/compilers/cxx-compiler-configs.cmake).  
  Default: `1048576` (Bytes).
- `STDOUT_IS_TERMINAL`: Whether the standard output is a terminal; Controls the output color.
  See: [CMakelists.txt](../CMakeLists.txt); [cmake/compilers/cxx-compiler-configs.cmake](../cmake/compilers/cxx-compiler-configs.cmake); [cmake/utils/logging.cmake](../cmake/utils/logging.cmake).  
  Default: `OFF`.  
  Possible values: `ON`, `OFF`.