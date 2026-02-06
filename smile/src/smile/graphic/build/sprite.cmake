smile_option(SPRITE "Include 2D sprites API" ON)

if(SMILE_SPRITE)
	get_files_recursively(SOURCE_FILES "sprite")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()