include_directories(../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt5 REQUIRED COMPONENTS
    Network
)
add_definitions(
        -DSERVER_KEY_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../key/server.key"
        -DSERVER_CRT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../key/server.crt"
        -DCLIENT_CRT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../key/client.crt"
        -DCLIENT_KEY_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../key/client.key"
)

set(AtCoreServer_SRCS
    serverconfig.cpp
    serverpart.cpp
)

add_library(Server ${AtCoreServer_SRCS})


target_link_libraries(Server AtCore::AtCore Qt5::Core Qt5::Network)





