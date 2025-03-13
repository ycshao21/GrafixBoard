@echo off
setlocal

call scripts\msvc-setup.bat

call bash %*

endlocal