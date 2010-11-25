echo off

echo Common tools directory: %VS90COMNTOOLS%

call "%VS90COMNTOOLS%/vsvars32.bat"

cd build
IF %ERRORLEVEL% NEQ 0 (
	echo "Could not change to build directory"
	goto :error
)

echo [calling CMake]

cmake /G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%1 -DCMAKE_BUILD_TYPE=debug ..
IF %ERRORLEVEL% NEQ 0 (
	echo [CMake error]
	goto :error
)

echo [building project]

nmake
IF %ERRORLEVEL% NEQ 0 (
	echo [build error]
	goto :error
)

echo [running tests]
call "testpath.bat"
ctest -V
IF %ERRORLEVEL% NEQ 0 (
	echo [test error]
	goto :error
)

echo [build and tests successful]

echo [installing project]

nmake install
IF %ERRORLEVEL% NEQ 0 (
	echo [install error]
	goto :error
)

cd ..

exit /b 0

:error
cd ..
exit /b 1
