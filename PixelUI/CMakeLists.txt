cmake_minimum_required(VERSION 3.5)

project(PixelUI)

file(GLOB_RECURSE PixelUISrc
"Widget/*.h"
"Widget/*.cpp"
"*.cpp"
"Shaders/*.sc"
"Runtime/*.bin")

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PixelUISrc})

add_executable(PixelUI ${PixelUISrc})

target_link_libraries(PixelUI PUBLIC glfw bgfx bx)

# foreach(Item ${PixelUISrc})
# 	message(${Item})
# endforeach()

# target_include_directories(PixelUI PUBLIC
# "../ThirdParty/glfw/include")

# set(${ShaderRcGeneratedFile} "")

# add_subdirectory(../ThirdParty/bgfx/tools/shaderc shaderc)

# file(GLOB_RECURSE PixelShader
# "Shaders/*.sc")

# set(GeneratedShader "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/vs_triangle.bin")

set(GeneratedShader "")

foreach(Item ${PixelUISrc})
	get_filename_component(EXTENSION ${Item} EXT) # get externsion, if it is bin, then skip
	get_filename_component(BASE_NAME ${Item} NAME_WE)
	if(${EXTENSION} STREQUAL ".sc" AND NOT ${Item} STREQUAL "varying.def.sc")
		list(APPEND GeneratedShader "${BASE_NAME}.bin") # fs_triangle.bin vs_triangle.bin
	endif()
endforeach()

# foreach(GeneratedItem ${GeneratedShader})
# 	message(${GeneratedItem})
# endforeach()

# ------generate bin file, in the first time------
foreach(Item ${PixelUISrc})
	get_filename_component(EXTENSION ${Item} EXT) # get externsion, if it is bin, then skip
	get_filename_component(BASE_NAME ${Item} NAME_WE)
	if(${EXTENSION} STREQUAL ".sc" AND NOT ${Item} STREQUAL "varying.def.sc")
		file(WRITE "Runtime/${BASE_NAME}.bin" "")
	endif()
endforeach()
# ------generate bin file, in the first time------

foreach(Item ${PixelUISrc})
	get_filename_component(BASE_NAME ${Item} NAME_WE)
	get_filename_component(BASE_DIRECTORY ${Item} DIRECTORY)
	get_filename_component(BASE_EXT ${Item} EXT)
	string(FIND ${Item} "vs" VsPos)
	if(NOT ${VsPos} EQUAL -1 AND NOT ${BASE_EXT} STREQUAL ".bin")
		add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/${BASE_NAME}.bin"
		DEPENDS ${Item}
		COMMAND call $<TARGET_FILE:shaderc>
		-f ${Item}
		-o "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/${BASE_NAME}.bin"
		--varyingdef "${BASE_DIRECTORY}/varying.def.sc"
		--platform windows
		-p vs_5_0
		-O 3
		--type vertex
		-O3
		-i "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/common")
	endif()
endforeach()

foreach(Item ${PixelUISrc})
	get_filename_component(BASE_NAME ${Item} NAME_WE)
	get_filename_component(BASE_DIRECTORY ${Item} DIRECTORY)
	get_filename_component(BASE_EXT ${Item} EXT)
	string(FIND ${Item} "fs" fsPos)
	if(NOT ${fsPos} EQUAL -1 AND NOT ${BASE_EXT} STREQUAL ".bin")
		add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/${BASE_NAME}.bin"
		DEPENDS ${Item}
		COMMAND call $<TARGET_FILE:shaderc>
		-f ${Item}
		-o "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/${BASE_NAME}.bin"
		--varyingdef "${BASE_DIRECTORY}/varying.def.sc"
		--platform windows
		-p ps_5_0
		-O 3
		--type fragment
		-O3
		-i "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/common")
	endif()
endforeach()

# add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/vs_triangle.bin"
# DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Triangle/vs_triangle.sc"
# COMMAND call $<TARGET_FILE:shaderc>
# -f "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Triangle/vs_triangle.sc"
# -o "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/vs_triangle.bin"
# --varyingdef "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Triangle/varying.def.sc"
# --platform windows
# -p vs_4_0
# -O 3
# --type vertex
# -O3
# -i "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/common")

# add_custom_target(MyTarget ALL
# DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Runtime/vs_triangle.bin")

if(MSVC)
	set_property(TARGET PixelUI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

