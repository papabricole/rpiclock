cmake_minimum_required(VERSION 3.1.0)

if(BUILD_DHT11)
    add_compile_definitions(BUILD_DHT11)
    add_subdirectory(dht11)
    set(EXTRA_LIBS dht11 pthread)
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(HEADERS
    DHT11.h
)

set(SOURCES
    DHT11.cpp
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
    ${EXTRA_LIBS}
)

install(TARGETS rpiclock DESTINATION bin)
