cmake_minimum_required(VERSION 3.0)

project(alpha-snake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_)

find_package(PkgConfig)
pkg_check_modules(CLUTTER REQUIRED clutter-1.0 glib-2.0 gobject-2.0)

add_executable(alpha-snake
        src/main.c
        src/affichage.c src/affichage.h src/list.c src/list.h
        src/struc.c src/struc.h
        src/menu.c src/alpha-button.c src/alpha-button.h)

target_link_libraries(alpha-snake ${CLUTTER_LIBRARIES})
include_directories(alpha-snake ${CLUTTER_INCLUDE_DIRS})
