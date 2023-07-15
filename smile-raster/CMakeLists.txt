project("smile-raster" LANGUAGES CUDA CXX)

find_package(CUDAToolkit)

file(GLOB_RECURSE SOURCE_FILES 
	src/*.cu
	src/*.cuh)

set(INCLUDE_DIRS
    src
	thirdparty/glm
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME}
                       PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_ARCHITECTURES "50")
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER Dependencies)

target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cudart)