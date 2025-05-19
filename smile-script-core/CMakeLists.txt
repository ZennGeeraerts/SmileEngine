set(MONO_XBUILD "../smile/mono/lib/mono/4.5/xbuild")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/resources/scripts/$<CONFIG>/smile-script-core.dll"
    COMMAND "${MONO_XBUILD}" "${CMAKE_CURRENT_SOURCE_DIR}/smile-script-core.csproj" /p:Configuration=$<CONFIG>
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Building C# smile script core as DLL"
)