cmake_minimum_required(VERSION 3.1.0)

OPTION(BUILD_DHT11 "DHT11 temperature/humidity support" ON)

project(rpiclock)

add_subdirectory(src)
