@echo off
cd %%~dp0
set PATH=%%~dp0bin;%%PATH%%
bin\main.exe %%*
