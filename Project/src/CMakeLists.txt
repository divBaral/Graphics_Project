find_package(SFML REQUIRED system window graphics)
include_directories(${myproject_SOURCE_DIR}/Project/include)
include_directories(${myproject_SOURCE_DIR}/glalib/include)
link_directories(${myproject_SOURCE_DIR}/glalib/src)
include_directories(${SFML_INCLUDE_DIR})
set(SOURCE_FILES main.cpp Camera.cpp)



add_compile_options("-O3")
add_compile_options("-std=c++20")



# we add the executable of the program
add_executable(myproject ${SOURCE_FILES})
target_link_libraries(myproject PRIVATE sfml-graphics sfml-system glalib)
