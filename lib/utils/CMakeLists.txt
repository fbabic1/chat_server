add_library(lib_utils STATIC
	connection.cpp
	message.cpp
	user.cpp
)
target_include_directories(lib_utils PUBLIC
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}
)