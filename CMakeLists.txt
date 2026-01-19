cmake_minimum_required(VERSION 3.16)

project(SHantilly-suite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuração Global do Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts PrintSupport Network Svg)

# Diretórios de Saída
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Subprojetos
add_subdirectory(libs/SHantilly-ui)
add_subdirectory(src/code/SHantilly)
# add_subdirectory(apps/studio) # Habilitaremos quando iniciarmos o Studio

enable_testing()
add_subdirectory(tests)

add_executable(poc_modern_cli
    src/poc_modern_cli.cpp
    src/code/SHantilly/core/parser/ParserMain.cpp
    src/code/SHantilly/core/parser/ParserMain.h
    src/code/SHantilly/core/parser/CommandParser.cpp
    src/code/SHantilly/core/parser/Tokenizer.cpp
    src/code/SHantilly/core/parser/WidgetParserUtils.cpp
)

target_link_libraries(poc_modern_cli PRIVATE
    Qt6::Widgets
    SHantilly-ui
)

target_include_directories(poc_modern_cli PRIVATE
    src/code/SHantilly/core/parser
    src/code/SHantilly/core/builder
)

set_property(TARGET poc_modern_cli PROPERTY AUTOMOC ON)

add_executable(visual_verifier
    src/visual_verifier.cpp
)

target_link_libraries(visual_verifier PRIVATE
    Qt6::Widgets
    SHantilly-ui
)
