smile_option(IMGUI "Include imgui" ON)

if(SMILE_IMGUI)
	add_subdirectory(${THIRDPARTY_PATH}/imgui imgui)

	set(INCLUDE_DIRS
		${THIRDPARTY_PATH}/imgui
		${THIRDPARTY_PATH}/ImGuizmo
	)

	set(LIB_LINKS
		imgui
	)

	target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
	target_link_libraries(smile PRIVATE ${LIB_LINKS})

	get_files_recursively(SOURCE_FILES "imgui")
	target_sources(smile PRIVATE ${SOURCE_FILES})
endif()