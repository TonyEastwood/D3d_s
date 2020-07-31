mkdir BuildDir
cd BuildDir
REM Set up Microsoft Visual Studio 2017, where <arch> is amd64, x86, etc.
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
SET _ROOT=C:\Qt\Qt-5
SET PATH=%_ROOT%\qtbase\bin;%_ROOT%\gnuwin32\bin;%PATH%
REM Uncomment the below line when using a git checkout of the source repository
REM SET PATH=%_ROOT%\qtrepotools\bin;%PATH%

C:\Qt\5.15.0\msvc2019_64\bin\qmake.exe ../d3ds.pro -spec win32-msvc "CONFIG+=qtquickcompiler" && C:/Qt/Tools/QtCreator/bin/jom.exe -f Makefile 

cd BuildDir
for /r C:\Qt\5.15.0\msvc2019_64\bin %%a in (*) do if "%%~nxa"=="windeployqt.exe" set p=%%~dpnxa
if defined p (
echo %p%
cd release/
%p% D3ds.exe
) else (
echo File not found
)

echo Build Successfull!
echo Result in BuildDir/release/d3ds.exe
pause

