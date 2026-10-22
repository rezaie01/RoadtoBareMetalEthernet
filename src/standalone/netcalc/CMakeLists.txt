cmake_minimum_required(VERSION 4.3)
project(netcalc LANGUAGES C)


set(REPO_ROOT_SRC "${CMAKE_CURRENT_LIST_DIR}/../..")

add_executable(netcalc)

target_sources(netcalc PRIVATE
    main.c
    netcalc.c
    "${REPO_ROOT_SRC}/endian.c"
    "${REPO_ROOT_SRC}/mini_network_stack/shared.c"
)

target_include_directories(netcalc PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}"
    "${REPO_ROOT_SRC}"
)

set_target_properties(netcalc PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)
