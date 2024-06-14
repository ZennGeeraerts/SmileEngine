@echo off
pushd %~dp0\..\conan\
call conan install . -sbuild_type=Release -of=Release --build=missing
popd
PAUSE