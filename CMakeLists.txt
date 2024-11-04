cmake_minimum_required(VERSION 3.29)
project(afk)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
add_executable(afk main.cpp  Alert.mm
        Alert.mm)

if(APPLE)
    target_link_libraries(afk "-framework ApplicationServices")
    target_link_libraries(afk "-framework Cocoa")
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(afk ${X11_LIBRARIES} Xtst)
endif()
