smile_option(LZ4 "Add LZ4 compression" ON)

if(SMILE_LZ4)
	get_files_recursively(SOURCE_FILES compression/lz4)

	find_package(lz4 REQUIRED)
	target_link_libraries(smile PRIVATE lz4::lz4)
endif()
