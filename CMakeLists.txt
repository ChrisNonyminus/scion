cmake_minimum_required(VERSION 3.24)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(EXECUTABLE_NAME scion)
project(${EXECUTABLE_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_executable(scion_SimCity4 src/app/sc4/cSC4App.cpp)

set(USING_SDL2 ON)
set(USING_OPENGL ON)

if (USING_SDL2)
    add_subdirectory("third_party/SDL2")
    target_link_libraries(scion_SimCity4 ${SDL2_LIBRARIES})
    target_compile_definitions(scion_SimCity4 PUBLIC USING_SDL2)
    target_sources(scion_SimCity4 PUBLIC
            "src/platform/sdl2/sc4.cpp"
            )
endif ()

if (USING_OPENGL)
    find_package(OpenGL REQUIRED)
    target_link_libraries(scion_SimCity4 OpenGL::GL)
    target_compile_definitions(scion_SimCity4 PUBLIC USING_OPENGL)
endif ()

if (USING_SDL2)
    target_include_directories(scion_SimCity4 PUBLIC ${SDL2_INCLUDE_DIRS})

    target_compile_definitions(scion_SimCity4 PUBLIC
            "PLATFORM=PLATFORM_SDL2"
            "PLATFORM_SDL2"
            )
endif ()

target_include_directories(scion_SimCity4 PUBLIC "src/framework/include")
target_include_directories(scion_SimCity4 PUBLIC "src/app/sc4")
FILE(GLOB GZ_Framework_Sources "src/framework/src/*.cpp")
FILE(GLOB SC4_Sources "src/app/sc4/*.cpp")
target_sources(scion_SimCity4 PUBLIC
        ${GZ_Framework_Sources}
        )

target_sources(scion_SimCity4 PUBLIC
        ${SC4_Sources}
        )

target_compile_definitions(scion_SimCity4 PUBLIC
        "SZ_DEFAULT_APP_TITLE=\"SimCity 4\"" ,
        "SZ_APP_MUTEX=\"SimCity 4 Mutex\"" ,
        "APP_IS_SC4=1"
        )


