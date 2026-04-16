cmake_minimum_required(VERSION 3.26)
project(GiraffeAdventure)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0135 NEW)

# --- 1. 引入外部庫 PTSD (框架) ---
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd
    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip
    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)



# --- 2. 自動掃描原始碼檔案 ---
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# --- 3. 建立執行檔 ---
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES} "src/Block.cpp" "src/Goomba.cpp" "src/Mushroom.cpp" "src/Star.cpp" "src/Coin.cpp" "src/ScoreEffect.cpp")

# --- 4. 編譯選項設定 ---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Zc:__cplusplus)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# --- 5. 資源路徑定義 ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GA_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE GA_RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

# --- 6. 關鍵修正：Include 路徑設定 ---
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include/Core"  
    "${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include/Util"  
    "${CMAKE_CURRENT_SOURCE_DIR}/PTSD/lib/glew/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/PTSD/lib/glm"
)

# ➡️ 額外建議：確保連結順序
target_link_libraries(${PROJECT_NAME}
    PUBLIC  # 使用 PUBLIC 確保依賴性傳遞
    PTSD
    SDL2::SDL2main
)