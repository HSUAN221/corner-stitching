# define library
add_library(${PROJECT_NAME} INTERFACE)

# add lcs
set(header_file
lcs.hpp
)
add_subdirectory(${PROJECT_NAME})