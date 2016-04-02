add_executable(tconverter
    tconverter.cpp)

target_link_libraries(tconverter
    Qt5::Core
    toonzlib
    image)

if (UNIX AND NOT APPLE)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tconverter DESTINATION usr/bin)
endif()
