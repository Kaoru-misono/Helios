
set(ENGINE_ROOT_DIR "${ROOT_DIR}/engine")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/support")
#set(ENGINE_ASSET_DIR "/asset")

set(DEPLOY_CONFIG_DIR "configs/deployment")
set(DEVELOP_CONFIG_DIR "configs/development")


#if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    add_compile_options("/MP")
#    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)
#endif()

add_subdirectory(support)

add_subdirectory(source/runtime)
add_subdirectory(source/editor)
