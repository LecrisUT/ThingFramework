set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)
doxygen_add_docs(ThingsFramework_doxygen
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_BINARY_DIR}/include)

# TODO: Export to CMakeExtraUtils
find_package(Python3 REQUIRED)
get_filename_component(_PYTHON_DIR ${Python3_EXECUTABLE} DIRECTORY)
find_program(SPHINX_EXECUTABLE REQUIRED
		NAMES sphinx-build sphinx-build.exe
		HINTS ${_PYTHON_DIR})
mark_as_advanced(SPHINX_EXECUTABLE)
if (NOT DEFINED SPHINX_OUTPUT_DIR)
	set(SPHINX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/html)
endif ()
add_custom_target(ThingsFramework_docs
		DEPENDS ThingsFramework_doxygen
		COMMAND ${SPHINX_EXECUTABLE} ${SPHINX_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR} ${SPHINX_OUTPUT_DIR}
		)
