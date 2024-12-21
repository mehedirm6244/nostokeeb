cmake_minimum_required(VERSION 3.31)
project(nostokeeb)

find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(include)
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(nostokeeb
    src/main.cpp
    src/window.cpp
)

target_link_libraries(nostokeeb ${GTKMM_LIBRARIES} X11 Xtst)
