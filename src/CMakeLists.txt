cmake_minimum_required(VERSION 3.1.0)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick Mqtt REQUIRED)

set(HEADERS
    qmlmqttclient.h
)

set(SOURCES
    qmlmqttclient.cpp
    main.cpp
)

set(RESOURCES
    resources.qrc
)

add_executable (rpiclock ${HEADERS} ${SOURCES} ${RESOURCES})

target_compile_definitions(rpiclock PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(rpiclock
    Qt5::Core
    Qt5::Quick
    Qt5::Mqtt
)

install(TARGETS rpiclock DESTINATION bin)
