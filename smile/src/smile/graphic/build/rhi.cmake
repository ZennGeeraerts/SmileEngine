smile_option(RHI "Include rendering hardware interface" ON)

if(SMILE_RHI)
	get_files_recursively(SOURCE_FILES "rhi")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()