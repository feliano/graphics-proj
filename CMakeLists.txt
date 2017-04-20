# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (Graphics)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile libs dependencies 
add_subdirectory (libs)

include_directories(
	libs/glfw-3.1.2/include/GLFW/
	libs/glm-0.9.7.1/
	libs/glew-1.13.0/include/
	.
)

# SETS THE ALL_LIBS VARIABLE
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

# Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Graphics 
add_executable(Graphics 
	common/shader.cpp
	common/shader.hpp


	shaders/SimpleFragmentShader.fragmentshader
	shaders/SimpleVertexShader.vertexshader
	shaders/grid.fshader
	shaders/grid.vshader
	shaders/colorShading.fshader
	shaders/colorShading.vshader
	Main.cpp
	Transform.h
	Camera.h
	Object3D.h
	Object3D.cpp

	Cube.h
	Cube.cpp
	Skybox.h
	Grid.h
	Grid.cpp
	Terrain.h
	FrameBuffer.h
	HeightMap.h

)

target_include_directories(Graphics PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/shaders
)

target_link_libraries(Graphics
	${ALL_LIBS}
)


file(GLOB SHADERS ${CMAKE_SOURCE_DIR}/shaders/*.fshader
                  ${CMAKE_SOURCE_DIR}/shaders/*.vshader
                  ${CMAKE_SOURCE_DIR}/shaders/*.vertexshader
                  ${CMAKE_SOURCE_DIR}/shaders/*.fragmentshader)





#get_target_property(MyTargetLocation Graphics LOCATION)
#get_filename_component(MyTargetDir $<TARGET_FILE_DIR:Graphics> PATH)
#get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)


#add_custom_command(TARGET Graphics POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#                       ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Graphics>/shaders
#                   COMMENT 'Copying shaders to build folders'
#                   )


# TODO SOLVE: DOESN'T CREATE NEW FOLDER IF BUILD FOLDER IS EMPTY...
# TEMPORARY: CREATE FOLDERS MANUALLY...
# shader files
add_custom_command(TARGET Graphics PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Graphics>/shaders)


# texture files
add_custom_command(TARGET Graphics PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:Graphics>/textures)


# copy object files
add_custom_command(TARGET Graphics PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/objects $<TARGET_FILE_DIR:Graphics>/objects)



# Copy shaders on every build
add_custom_target(copy)
foreach(Shader ${SHADERS})
  add_custom_command(TARGET copy POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${Shader} ${CMAKE_CURRENT_BINARY_DIR}/shaders
                     COMMENT "Copying ${Shader} to build folder")
endforeach()

add_dependencies(Graphics copy)