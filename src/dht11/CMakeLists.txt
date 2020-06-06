cmake_minimum_required(VERSION 3.1.0)

set(HEADERS
    pi_2_dht_read.h
    common_dht_read.h
    pi_2_mmio.h
)

set(SOURCES
    common_dht_read.c
    pi_2_dht_read.c
    pi_2_mmio.c
)

add_library (dht11 STATIC ${HEADERS} ${SOURCES})
target_include_directories (dht11 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
