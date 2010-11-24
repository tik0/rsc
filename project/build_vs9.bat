echo off

echo "Common tools directory: %VS90COMNTOOLS%"

call "%VS90COMNTOOLS%/vsvars32.bat"

cd build
IF %ERRORLEVEL% NEQ 0 (
	echo "Could not change to build directory"
	goto :cleanup
)

cmake /G "Visual Studio 9 2008" /DCMAKE_INSTALL_PREFIX=%1 /DCMAKE_BUILD_TYPE=debug ..
IF %ERRORLEVEL% NEQ 0 (
	echo "CMake error"
	goto :cleanup
)

msbuild /t:ALL_BUILD /v:d RSC.sln
IF %ERRORLEVEL% NEQ 0 (
	echo "Build error"
	goto :cleanup
)

REM TODO unit tests

echo "success"

:cleanup
cd ..