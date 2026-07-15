smile_option(GRAPHIC_RESOURCE "Include graphic resources" ON)

if(SMILE_GRAPHIC_RESOURCE)
	find_package(stb REQUIRED)

	set(LIB_LINKS
		stb::stb
	)

	target_link_libraries(smile PRIVATE ${LIB_LINKS})

	get_files_recursively(SOURCE_FILES "resource")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()