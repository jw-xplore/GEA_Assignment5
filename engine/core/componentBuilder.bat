set filename=EComponents.h
set folder=Components
set compbase=ComponentBase
break> %filename%

set COUNTER=0
for %%f in ("%folder%\*.h") do set /A COUNTER += 1

:: Inlcudes
echo #include "ComponentBase.h" >> %filename%

echo.>>%filename%

:: Define enum
echo enum EComponents >> %filename%
echo { >> %filename%
for %%f in ("%folder%\*.h") do echo %%~nfId, >> "%filename%"
echo EComponentsCount >> %filename%
echo }; >> %filename%

echo.>>%filename%

:: Forward declar component classes
for %%f in ("%folder%\*.h") do echo class %%~nf : public %compbase% { public: %%~nf() {} };>> "%filename%"

echo.>>%filename%

:: Define component blueprints
echo ComponentBase blueprints[%COUNTER%] =>> %filename%
echo { >> %filename%
for %%f in ("%folder%\*.h") do echo %%~nf(), >> "%filename%"
echo }; >> %filename%

:: FOR %%i IN (%folder%\*.h) DO echo %%i>>%filename%