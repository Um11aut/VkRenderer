@echo off

for /r %%i in (*.frag, *.vert) do (
    echo Compiling: %%i
    %VULKAN_SDK%\Bin\glslangValidator.exe -V -o "out\%%~ni.spv" %%i
)


echo Comleted