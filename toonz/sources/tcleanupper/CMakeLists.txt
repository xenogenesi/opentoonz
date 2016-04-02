add_executable(tcleanup
    tcleanupper.cpp)

target_link_libraries(tcleanup
    Qt5::Core
    Qt5::Widgets
    tfarm
    image)

if (UNIX AND NOT APPLE)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tcleanup DESTINATION usr/bin)
endif()
