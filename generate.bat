@echo off
echo Updating submodules...
git submodule update --init --recursive
tools\premake5 %* vs2019
pause