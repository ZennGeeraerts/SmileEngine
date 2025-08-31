smile_option(YAML "Add a YAML parser" ON)

if(SMILE_YAML)
	get_files_recursively(SOURCE_FILES yaml)

	find_package(yaml-cpp REQUIRED)
	target_link_libraries(smile PUBLIC yaml-cpp)
endif()