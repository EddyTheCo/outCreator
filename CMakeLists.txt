cmake_minimum_required(VERSION 3.16)
project(IOTA VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

qt6_add_qml_module(outCreator
    URI  outCreator
    VERSION 1.0
    SOURCES
    src/outcreator.cpp include/outcreator.hpp 
    RESOURCE_PREFIX
   "/esterVtech.com/imports"
    OUTPUT_TARGETS out_targets_var
    NO_IMPORT_SCAN
    OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/outCreator
)

target_link_libraries(outCreator PUBLIC Qt6::Gui Qt6::Quick)

target_include_directories(outCreator PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        "$<INSTALL_INTERFACE:include>")

include(FetchContent)

FetchContent_Declare(
    qaddr_bundle
    GIT_REPOSITORY git@github.com:EddyTheCo/QAddrBundle.git
    GIT_TAG main
)
FetchContent_MakeAvailable(qaddr_bundle)

target_link_libraries(outCreator PUBLIC qaddr_bundle)

target_compile_definitions(outCreator
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

install(TARGETS outCreator ${out_targets_var} EXPORT outCreatorTargets DESTINATION bin )
install(DIRECTORY include/ DESTINATION include/)

install(EXPORT outCreatorTargets
    FILE outCreatorTargets.cmake
    DESTINATION lib/cmake/outCreator
)
include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/outCreatorConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/outCreator"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/outCreatorConfigVersion.cmake"
    VERSION "0.1.1.0"
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/outCreatorConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/outCreatorConfigVersion.cmake
    DESTINATION lib/cmake/outCreator
)
export(EXPORT outCreatorTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/outCreatorTargets.cmake"
)
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}/)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}  CACHE STRING "" FORCE)

