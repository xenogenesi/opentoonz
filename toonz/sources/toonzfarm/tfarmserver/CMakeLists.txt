include_directories(../include)

add_executable(tfarmserver
    tfarmserver.cpp)

target_link_libraries(tfarmserver
    Qt5::Core
    tfarm)

if (UNIX AND NOT APPLE)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tfarmserver DESTINATION usr/bin)
endif()
