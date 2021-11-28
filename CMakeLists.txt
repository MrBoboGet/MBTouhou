project(MBTouhou)
set(PROJECT_NAME "MBTouhou")
##BEGIN MBPM_VARIABLES
set(MBPM_DEPENDENCIES 
	MrBoboGraphicsEngine
	stb
	assimp
	irrxml
	zlib
	glfw
	gl3w
	MBUtility
)
set(MBPM_TARGET_EXTPACKET_LIBRARIES )
set(MBPM_TARGET_COMPILE_OPTIONS )
set(MBPM_TARGET_LINK_OPTIONS )
#MBPM_Functions#variables MBPM_DEPENDENCIES
#MBPM_TARGET_EXTPACKET_LIBRARIES
#MBPM_TARGET_COMPILE_OPTIONS
#MBPM_TARGET_LINK_OPTIONS
#Environment variables MBPM_PACKETS_INSTALL_DIRECTORY
set(MBPM_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(MBPM_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
set(MBPM_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
macro(MBPM_UpdateLibraries TargetToUpdate LinkType BuildType)
	set(MBPM_TARGET_EXTPACKET_LIBRARIES "")
	foreach(PACKET IN LISTS MBPM_DEPENDENCIES)
		set(NewLibraryName "")
		set(NewLibraryToAdd "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/${PACKET}/MBPM_Builds/")
		if(${LinkType} STREQUAL "STATIC")
			set(NewLibraryName "${PACKET}_S")
		elseif(${LinkType} STREQUAL "DYNAMIC")
			set(NewLibraryName "${PACKET}_D")
		endif()
		if(${BuildType} STREQUAL "DEBUG")
			set(NewLibraryName "${NewLibraryName}D")
		elseif(${BuildType} STREQUAL "RELEASE")
			set(NewLibraryName "${NewLibraryName}R")
		endif()
		if(UNIX)
			set(NewLibraryName "lib${NewLibraryName}.")
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}a")
			else()
				set(NewLibraryName "${NewLibraryName}so")
			endif()
		else()
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}.lib")
			else()
				set(NewLibraryName "${NewLibraryName}.dll")				
			endif()
		endif()
		set(NewLibraryToAdd "${NewLibraryToAdd}${NewLibraryName}")
		list(APPEND MBPM_TARGET_EXTPACKET_LIBRARIES ${NewLibraryToAdd})
	endforeach()
endmacro()

macro(MBPM_UpdateTargetVariables TargetToUpdate LinkType BuildType)
	MBPM_UpdateLibraries(${TargetToUpdate} ${LinkType} ${BuildType})
endmacro()

macro(MBPM_UpdateOutput TargetToUpdate)
	set_target_properties(${TargetToUpdate} 
	PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	)
	#set_target_properties(${TargetToUpdate} PROPERTIES PREFIX "")
	#set_target_properties(${TargetToUpdate} PROPERTIES SUFFIX "")
	set_target_properties(${TargetToUpdate} PROPERTIES OUTPUT_NAME ${TargetToUpdate})
endmacro()

macro(MBPM_SetTargetCompileOptions TargetToUpdate LinkType BuildType)
	set(CMAKE_CXX_FLAGS " ")
	set(CMAKE_CXX_FLAGS_DEBUG " ")
	set(CMAKE_CXX_FLAGS_RELEASE " ")
	set(CMAKE_C_FLAGS " ")
	set(CMAKE_C_FLAGS_DEBUG " ")
	set(CMAKE_C_FLAGS_RELEASE " ")

	if(${BuildType} STREQUAL "DEBUG" AND ${LinkType} STREQUAL "STATIC")
		set(NEW_TARGET_COMPILE_OPTIONS "${MBPM_CXX_FLAGS} ${MBPM_CXX_FLAGS_DEBUG}")
	else()
		set(NEW_TARGET_COMPILE_OPTIONS "${MBPM_CXX_FLAGS} ${MBPM_CXX_FLAGS_RELEASE}")
	endif()
	string(REPLACE " " ";" NEW_TARGET_COMPILE_OPTIONS_LIST ${NEW_TARGET_COMPILE_OPTIONS})
	target_compile_options(${TargetToUpdate} PRIVATE ${NEW_TARGET_COMPILE_OPTIONS_LIST})
	message(${TargetToUpdate} " Compiled with follwing flags: ""${NEW_TARGET_COMPILE_OPTIONS_LIST}")
endmacro()

macro(MBPM_LinkTargetPacketLibraries Target)
	target_link_libraries(${Target} ${MBPM_TARGET_EXTPACKET_LIBRARIES})
endmacro()

macro(MBPM_SetTargetIncludeDirectories Target)
	target_include_directories(${Target} PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/MBUtility/"
	)
endmacro()

macro(MBPM_ApplyTargetConfiguration TargetToUpdate LinkType BuildType)
	MBPM_UpdateTargetVariables(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_UpdateOutput(${TargetToUpdate})
	MBPM_SetTargetCompileOptions(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_SetTargetIncludeDirectories(${TargetToUpdate})
endmacro()
##END MBPM_VARIABLES
set(PROJECT_SOURCES 

	"${CMAKE_CURRENT_SOURCE_DIR}//Source/bildhantering/stb_image.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Classes.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/EgenUtility/MinaStringOperations.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/Engine_GenericComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/SoundEngine/TouhouSoundEngine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/SpriteAnimationRenderer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/TextRendering.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Gameobject.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Hitbox.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/IndexBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Math.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Renderer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Shader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Source.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjekt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/Enemy_Bullet_Template.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/Johan.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/Level_1_Enemy_2.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/Level_1_Enemy_3.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/MainMenu.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/PausMenu.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/Player_Attack_BigShot.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/SpelarenFunktioner1.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Texture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Update.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Vector2D.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/VertexArray.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/VertexBuffer.cpp"
)
set(PROJECT_HEADERS 
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Engine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Gameobject.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Hitbox.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/IndexBuffer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/MinMath.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Renderer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Shader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/SpelObjekt/Enemy_Bullet_Template.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/SpelObjekt/Level_1_Enemy_2.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/SpelObjekt/Level_1_Enemy_3.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/SpelObjekt/Player_Attack_BigShot.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/SpelObjekt.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Texture.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/Vector2D.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/VertexArray.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/VertexBuffer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Headers/VertexBufferLayout.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/bildhantering/BildHantering.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/EgenUtility/MinaStringOperations.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/EngineExternals.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/Engine_GenericComponent.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/SoundEngine/TouhouSoundEngine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/SpriteAnimationRenderer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/Engine/TextRendering.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/Johan.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/MainMenu.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Source/SpelObjektcpp/PausMenu.h"
)
set(COMMON_FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})
set(COMMON_DYNAMIC_LIBRARIES
	#"${CMAKE_CURRENT_SOURCE_DIR}/_Libs/fmod_vc.lib"
	#"${CMAKE_CURRENT_SOURCE_DIR}/_Libs/fmodL_vc.lib"
)
set(COMMON_STATIC_LIBRARIES 
	#"${CMAKE_CURRENT_SOURCE_DIR}/_Libs/glew32s.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/_Libs/glew32.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/_Libs/glfw3.lib"
	"opengl32.lib"
)

set(COMMON_INCLUDE_DIRECTORIES
	"${CMAKE_CURRENT_SOURCE_DIR}/Headers/"
	"${CMAKE_CURRENT_SOURCE_DIR}/Headers/SpelObjekt/"
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/SoundEngine/"
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/"
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/SpelObjektcpp/"
	"${CMAKE_CURRENT_SOURCE_DIR}/_Libs/Includes/"
)

set(COMMON_LIBRARIES ${COMMON_STATIC_LIBRARIES} ${COMMON_DYNAMIC_LIBRARIES})

add_library(MBTouhou_SD STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBTouhou_SD" "STATIC" "DEBUG")
target_compile_features("MBTouhou_SD" PRIVATE cxx_std_17)
target_link_libraries("MBTouhou_SD" ${COMMON_LIBRARIES})
target_include_directories("MBTouhou_SD" PRIVATE ${COMMON_INCLUDE_DIRECTORIES})

add_library(MBTouhou_SR STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBTouhou_SR" "STATIC" "RELEASE")
target_compile_features("MBTouhou_SR" PRIVATE cxx_std_17)
target_link_libraries("MBTouhou_SR" ${COMMON_LIBRARIES})
target_include_directories("MBTouhou_SR" PRIVATE ${COMMON_INCLUDE_DIRECTORIES})

add_executable(MBTouhou ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBTouhou" "STATIC" "RELEASE")
target_compile_features("MBTouhou" PRIVATE cxx_std_17)
target_link_libraries("MBTouhou" ${COMMON_LIBRARIES})
target_include_directories("MBTouhou" PRIVATE ${COMMON_INCLUDE_DIRECTORIES})
message(${COMMON_LIBRARIES})

