smile_option(SHADER "Include shader sources" ON)

if(SMILE_SHADER)
    get_files_recursively(SOURCE_FILES "shader")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()