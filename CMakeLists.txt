cmake_minimum_required(VERSION 3.10)
project(CornersGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Укажи путь по умолчанию для SFML
if(NOT DEFINED SFML_DIR)
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
endif()

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(corners_sfml corners_sfml.cpp)

target_link_libraries(corners_sfml sfml-graphics sfml-window sfml-system)

# Путь к SFML DLL-файлам
set(SFML_DLL_DIR "C:/SFML/bin")
set(FONT_FILE "${CMAKE_SOURCE_DIR}/arial.ttf")


# Копировать DLL-файлы рядом с .exe после сборки
add_custom_command(TARGET corners_sfml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLL_DIR}/sfml-graphics-d-2.dll
        ${SFML_DLL_DIR}/sfml-window-d-2.dll
        ${SFML_DLL_DIR}/sfml-system-d-2.dll
		${FONT_FILE}
        $<TARGET_FILE_DIR:corners_sfml>
)

