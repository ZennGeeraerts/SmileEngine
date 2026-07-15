smile_option(GRAPHIC_TOOLS "Include graphical tools" ON)

if(SMILE_GRAPHIC_TOOLS)
    get_files_recursively(SOURCE_FILES "tools")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()