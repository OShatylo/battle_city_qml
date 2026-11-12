cmake_minimum_required(VERSION 3.16)

project(BattleCity
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    Multimedia
)

qt_standard_project_setup()

qt_add_executable(battle_city
    main.cpp
    shape.cpp
    shape.h
    game.cpp
    game.h
    directionenumqml.cpp
    directionenumqml.h
    shapetypeenumqml.cpp
    shapetypeenumqml.h
    qml.qrc
)

target_link_libraries(battle_city PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Multimedia
)
