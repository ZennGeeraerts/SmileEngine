smile_option(RENDERER "Include renderer" ON)

if(SMILE_RENDERER)
	get_files_recursively(SOURCE_FILES "renderer")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()