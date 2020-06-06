cmake_minimum_required(VERSION 3.1.0)

add_subdirectory(dht11)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(HEADERS
    RpiClock.h
)

set(SOURCES
    RpiClock.cpp
    main.cpp
)

set(RESOURCES
    resources.qrc
)

add_executable (rpiclock ${HEADERS} ${SOURCES} ${RESOURCES})

target_link_libraries(rpiclock
    Qt5::Widgets
    Qt5::Network
    dht11
)

install(TARGETS rpiclock DESTINATION bin)
