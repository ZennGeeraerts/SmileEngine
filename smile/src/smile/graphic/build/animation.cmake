smile_option(ANIMATION "Include animation API" ON)

if(SMILE_ANIMATION)
	get_files_recursively(SOURCE_FILES "animation")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()