@echo off
pushd %~dp0\..\conan\
call conan install . -sbuild_type=Debug -of=Debug --build=missing
popd
PAUSE
