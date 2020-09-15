include_directories(../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt5 REQUIRED COMPONENTS
    Network
)


set(AtCoreServer_SRCS
    atcorehostauthorization.cpp
    serverpart.cpp
)

add_library(Server ${AtCoreServer_SRCS})


target_link_libraries(Server AtCore::AtCore Qt5::Core Qt5::Network)





