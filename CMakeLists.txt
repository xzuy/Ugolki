cmake_minimum_required(VERSION 3.10)
project(CornersGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED SFML_DIR)
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
endif()

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(corners_sfml WIN32 main.cpp ai.cpp)

target_link_libraries(corners_sfml sfml-graphics sfml-window sfml-system)

set(SFML_DLL_DIR "C:/SFML/bin")
set(FONT_FILE "${CMAKE_SOURCE_DIR}/DejaVuSans-Bold.ttf")


add_custom_command(TARGET corners_sfml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLL_DIR}/sfml-graphics-d-2.dll
        ${SFML_DLL_DIR}/sfml-window-d-2.dll
        ${SFML_DLL_DIR}/sfml-system-d-2.dll
        ${FONT_FILE}
		${CMAKE_SOURCE_DIR}/bg.png
        $<TARGET_FILE_DIR:corners_sfml>
)


add_custom_command(TARGET corners_sfml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/bg.png
        $<TARGET_FILE_DIR:corners_sfml>
)

add_executable(test_board test_board.cpp)
add_executable(test_resetGame test_resetGame.cpp)
add_executable(test_drawBoard test_drawBoard.cpp)
target_include_directories(test_drawBoard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "C:/SFML/include")
target_link_libraries(test_drawBoard sfml-graphics sfml-window sfml-system)


# EXE для теста Даша
add_executable(test_checkWin test_checkWin.cpp ai.cpp)
add_executable(test_distanceToCorner test_distanceToCorner.cpp ai.cpp)
add_executable(test_isValidMove test_isValidMove.cpp ai.cpp)



target_include_directories(test_board PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME BoardTests COMMAND test_board)
add_test(NAME DrawBoard COMMAND test_drawBoard)
add_test(NAME ResetGame COMMAND test_resetGame)


#Тест Даша
add_test(NAME IsValidMove COMMAND test_isValidMove)
add_test(NAME CheckWin COMMAND test_checkWin)
add_test(NAME DistanceToCorner COMMAND test_distanceToCorner)


