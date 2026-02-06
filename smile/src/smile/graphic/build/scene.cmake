smile_option(SCENE "Include scene package" ON)

if(SMILE_SCENE)
	get_files_recursively(SOURCE_FILES "scene")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()