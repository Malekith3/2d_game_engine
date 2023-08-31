cmake_minimum_required(VERSION 3.22)
set(project_name 2d_game_engine)
project(${project_name})
set(CMAKE_CXX_STANDARD 17)

include_directories(libs)
include_directories(src)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_TTF REQUIRED)
find_package(SDL2_Mixer REQUIRED)
find_package(spdlog REQUIRED)


file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE HEADERS_FILES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)
add_executable(${project_name} ${SRC_FILES} ${HEADERS_FILES})

target_include_directories(${project_name}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)