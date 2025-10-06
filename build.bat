@echo off
set BUILD_DIR=build


if exist %BUILD_DIR% (
    echo Удаляем существующую папку %BUILD_DIR%...
    rd /s /q %BUILD_DIR%
)


mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake ..
cmake --build .

pause
