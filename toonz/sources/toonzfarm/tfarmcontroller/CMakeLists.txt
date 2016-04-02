include_directories(../include)

add_executable(tfarmcontroller
    tfarmcontroller.cpp)

target_link_libraries(tfarmcontroller
    Qt5::Core
    tfarm)

if (UNIX AND NOT APPLE)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tfarmcontroller DESTINATION usr/bin)
endif()
