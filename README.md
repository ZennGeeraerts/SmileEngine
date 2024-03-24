# SmileEngine

## Getting started

1. Install CMake from https://cmake.org/ or get "C++ CMake tools for Windows" from the Visual Studio installer under "Individual components"
   It is recommended that you use Visual Studio 2022 with CMake tools for Windows since it's tested using this
2. Install Python3 from https://www.python.org/downloads/
3. Make sure the Python environment variables are added to PATH
4. Install conan via commandline by entering:
```
    pip install conan
```
5. Go to 3_Software/conan/ and run:
```
    conan profile detect
```
    this will create a default profile
6. Edit the default conan profile under C:\User\.conan2\profiles\default
   - Use the MSVC compiler
   - The project uses C++ 17
   - Select the correct compiler version for your installed Visual Studio version, in this case 193 is Visual Studio 2022
```
   compiler=msvc
   compiler.cppstd=17
   compiler.version=193
```
7. Now run the following command to get the packages in debug configuration:
```
    conan install . -sbuild_type=Debug -of=Debug --build=missing
```
   Always use the configuration name as the output folder name so CMakePresets can find it
   Debug/ or Release/ folders will also get ignored by git because of the .gitignore file
8. Open 3_Software/ as a folder in Visual Studio 2022
   If you have "C++ CMake tools for Windows" installed, CMake will automaticly generate the project
9. Tell Visual Studio to use the CMakePresets.json file under Options > CMake > General > CMake configuration file: > Always use CMake Presets
10. Regenerate with CMake and build the project