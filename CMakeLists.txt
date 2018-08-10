cmake_minimum_required (VERSION 3.8)
project (quixo)
set (CMAKE_CXX_STANDARD 17)

####################################################
# Function:                 EXCLUDE_FILES_FROM_DIR_IN_LIST
# Description:              Exclude all files from a list under a specific directory.
# Param _InFileList:        Input and returned List 
# Param _excludeDirName:    Name of the directory, which shall be ignored.
# Param _verbose:           Print the names of the files handled

FUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName _verbose)
  foreach (ITR ${_InFileList})
    if ("${_verbose}")
      message(STATUS "ITR=${ITR}")
    endif ("${_verbose}")

    if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")                   # Check if the item matches the directory name in _excludeDirName
    #  message(STATUS "Remove Item from List:${ITR}")
      list (REMOVE_ITEM _InFileList ${ITR})                              # Remove the item from the list
    endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

  endforeach(ITR)
  set(SOURCE_FILES ${_InFileList} PARENT_SCOPE)                          # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST)


####################################################
# find all source and header files
file(GLOB_RECURSE SOURCE_FILES "src/*.hpp" "src/*.cpp" )

OPTION(USE_GRAPHICS "build and link SFML to have a grahpical interface" OFF)

if(NOT USE_GRAPHICS)
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "/graphics/" FALSE)
endif(NOT USE_GRAPHICS)

# organize the files according to the organisation in the file system
# from: http://cmake.3232098.n2.nabble.com/Keep-folder-structure-in-Visual-Studio-Project-generated-with-CMake-td7586044.html
macro(GroupSources curdir)
   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
   foreach(child ${children})
          if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                  GroupSources(${curdir}/${child})
          else()
                  string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
          endif()
   endforeach()
endmacro()

# execute tha macro
GroupSources(src)

if(MSVC)
	add_compile_options( /MP )
endif(MSVC)


# faster compilation with multithreading
if(MSVC)
	add_compile_options( /std:c++17 )
	
	set(OPTIMIZATION_OPTIONS 
    $<$<CONFIG:Debug>:>
    $<$<CONFIG:RelWithDebInfo>:>
    $<$<CONFIG:Release>: /O2 /Ob2 /Oi /Ot /GL > 
    $<$<CONFIG:MinSizeRel>:>
    )
	
	add_compile_options( "${OPTIMIZATION_OPTIONS}" )
endif(MSVC)

include_directories("src")
add_executable(quixo ${SOURCE_FILES})

if(USE_GRAPHICS)
	add_subdirectory("dependencies/SFML")
	include_directories("dependencies/SFML/include")
	target_link_libraries(quixo sfml-system sfml-graphics sfml-window)
endif(USE_GRAPHICS)