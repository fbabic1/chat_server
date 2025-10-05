add_library(lib_config INTERFACE)
target_include_directories(lib_config INTERFACE
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}
)
