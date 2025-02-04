# Minimum CMake version and project name
cmake_minimum_required(VERSION 3.31)
project(nostokeeb)

# Find required packages
find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Include directories
include_directories(include)
include_directories(${GTKMM_INCLUDE_DIRS})

# Define the executable and its source files
add_executable(nostokeeb
    src/main.cpp
    src/NK_Window.cpp
    src/NK_AboutDialog.cpp
)

# Link libraries
target_link_libraries(nostokeeb ${GTKMM_LIBRARIES} X11 Xtst)

# Installation rules
install(TARGETS nostokeeb DESTINATION /usr/local/bin)

# Configure and install the desktop entry file
configure_file(
    data/nostokeeb.desktop.in 
    ${CMAKE_BINARY_DIR}/nostokeeb.desktop 
    @ONLY
)
install(FILES ${CMAKE_BINARY_DIR}/nostokeeb.desktop DESTINATION /usr/share/applications)

# Install icons
install(FILES 
    data/icons/scalable/nostokeeb.svg
    DESTINATION /usr/share/icons/hicolor/scalable/apps/
)

# Configure and add the uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)
