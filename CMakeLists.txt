cmake_minimum_required(VERSION 3.10)
project(UgolkiNew LANGUAGES CXX)

# Проверка на Visual Studio
if(NOT MSVC)
    message(FATAL_ERROR "Этот проект должен собираться в Microsoft Visual Studio (MSVC)")
endif()

# Включаем MFC
set(CMAKE_MFC_FLAG 2)

# Указываем исходники
set(SOURCES
    Board.cpp
    Board.h
    CellType.h
    CPlayer.cpp
    CPlayer.h
    CStartupDlg.cpp
    CStartupDlg.h
    HPlayer.cpp
    HPlayer.h
    MFCUgolkiTest.cpp
    MFCUgolkiTest.h
    MFCUgolkiTestDlg.cpp
    MFCUgolkiTestDlg.h
    MonteCarloEvaluator.cpp
    MonteCarloEvaluator.h
    RPlayer.cpp
    RPlayer.h
    UPlayer.cpp
    UPlayer.h
    UgolkiField.cpp
    UgolkiField.h
    MFCUgolkiTest.rc
    resource.h
    framework.h
    targetver.h
    pch.cpp
    pch.h
)
add_compile_options(/utf-8)
add_compile_definitions(_UNICODE UNICODE)
# Создаем исполняемый файл
add_executable(UgolkiNew WIN32 ${SOURCES})

# Указываем папку с заголовками
target_include_directories(UgolkiNew PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Поддержка предкомпилированных заголовков
target_precompile_headers(UgolkiNew PRIVATE pch.h)

# Добавляем ресурсы вручную
target_sources(UgolkiNew PRIVATE
    res/MFCUgolkiTest.ico
    res/MFCUgolkiTest.rc2
)
