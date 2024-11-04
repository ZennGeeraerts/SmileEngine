include(smile-engine)

set(INCLUDE_DIRS
    ${ENGINE_PATH}/thirdparty/PhysX/include
    ${ENGINE_PATH}/thirdparty/PhysX/include/PhysX   # For internal PhysX usage
)

set(LIB_LINKS
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysX_64.lib
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysXCharacterKinematic_static_64.lib
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysXCommon_64.lib
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysXCooking_64.lib
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysXExtensions_static_64.lib
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysXFoundation_64.lib
    ${ENGINE_PATH}/thirdparty/PhysX/lib/$<CONFIG>/PhysXPvdSDK_static_64.lib
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})