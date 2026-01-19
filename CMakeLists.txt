cmake_minimum_required(VERSION 3.16)

# Integração automática com vcpkg (Manifest Mode)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE
   AND EXISTS "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(SHantilly-suite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configuração Global do Qt (via vcpkg)
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts PrintSupport Network Svg)

# Diretórios de Saída
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================================
# Cobertura de Código (Coverage)
# ============================================================================
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                             "Clang")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
  endif()
endif()

# Subprojetos
add_subdirectory(libs/SHantilly-ui)
add_subdirectory(src/code/SHantilly)
# add_subdirectory(apps/studio) # Habilitaremos quando iniciarmos o Studio

enable_testing()
add_subdirectory(tests)

add_executable(
  poc_modern_cli
  src/poc_modern_cli.cpp
  src/code/SHantilly/core/parser/ParserMain.cpp
  src/code/SHantilly/core/parser/ParserMain.h
  src/code/SHantilly/core/parser/CommandParser.cpp
  src/code/SHantilly/core/parser/Tokenizer.cpp
  src/code/SHantilly/core/parser/WidgetParserUtils.cpp)

target_link_libraries(poc_modern_cli PRIVATE Qt6::Widgets SHantilly-ui)

target_include_directories(
  poc_modern_cli PRIVATE src/code/SHantilly/core/parser
                         src/code/SHantilly/core/builder)

set_property(TARGET poc_modern_cli PROPERTY AUTOMOC ON)

add_executable(visual_verifier src/visual_verifier.cpp)

target_link_libraries(visual_verifier PRIVATE Qt6::Widgets SHantilly-ui)

# ============================================================================
# Instalação
# ============================================================================
include(GNUInstallDirs)

install(TARGETS SHantilly poc_modern_cli visual_verifier
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY libs/SHantilly-ui/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SHantilly)

install(
  TARGETS SHantilly-ui
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# ============================================================================
# CPack - Empacotamento DEB
# ============================================================================
set(CPACK_PACKAGE_NAME "shantilly")
set(CPACK_PACKAGE_VENDOR "helton-godoy")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "SHantilly - Modern UI Builder for Shell Scripts")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Helton Godoy <helton@example.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6widgets6, libqt6charts6, libqt6svg6")
set(CPACK_GENERATOR "DEB")

include(CPack)
