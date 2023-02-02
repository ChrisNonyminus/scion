# Build static lib only
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)


include(FetchContent)

FetchContent_Declare(glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "0.9.9.8"
)

FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)

set(GLM_INCLUDE_DIRS ${glm_SOURCE_DIR} ${glm_BINARY_DIR} PARENT_SCOPE)
set(GLM_LIBRARIES glm::glm PARENT_SCOPE)
