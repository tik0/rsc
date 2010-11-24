echo off

echo "Common tools directory: %VS90COMNTOOLS%"

call "%VS90COMNTOOLS%/vsvars32.bat"

cd build
IF %ERRORLEVEL% NEQ 0 (
	echo "Could not change to build directory"
	goto :error
)

cmake /G "Visual Studio 9 2008" /DCMAKE_INSTALL_PREFIX=%1 /DCMAKE_BUILD_TYPE=debug ..
IF %ERRORLEVEL% NEQ 0 (
	echo "CMake error"
	goto :error
)

msbuild /t:ALL_BUILD /v:d RSC.sln
IF %ERRORLEVEL% NEQ 0 (
	echo "Build error"
	goto :error
)

REM TODO unit tests

echo "success"

exit /b 0

:error
cd ..
exit /b 1