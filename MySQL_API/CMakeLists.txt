find_package(mysql-concpp)

add_library(MySQL_API MySQL_DB.cpp MySQL_DB.h immutable_variables.h)

#target_compile_definitions(
#	MySQL_API
#	PRIVATE -DMY_DLL_BUILD)

#set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

target_link_libraries(MySQL_API mysql::concpp)