@echo off

cmake --build build --config Release
cls
.\build\Release\tetris.exe