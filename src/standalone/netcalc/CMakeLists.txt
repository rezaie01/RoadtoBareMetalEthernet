
cmake_minimum_required(VERSION 4.3)
project(netcalc LANGUAGES C)


set(REPO_ROOT_SRC "${CMAKE_CURRENT_LIST_DIR}/../..")
set(NETCALC_INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}"
    "${REPO_ROOT_SRC}"
)
set(NETCALC_TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test")
set(NETCALC_TARGET_PROPS
    C_STANDARD 23
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

add_library(netcalc_core STATIC
    netcalc.c
    "${REPO_ROOT_SRC}/endian.c"
    "${REPO_ROOT_SRC}/mini_network_stack/shared.c"
)
target_include_directories(netcalc_core PUBLIC ${NETCALC_INCLUDES})
set_target_properties(netcalc_core PROPERTIES ${NETCALC_TARGET_PROPS})

add_executable(netcalc main.c)
target_link_libraries(netcalc PRIVATE netcalc_core)
set_target_properties(netcalc PROPERTIES ${NETCALC_TARGET_PROPS})

function(add_netcalc_test target_name test_name)
    add_executable(${target_name} ${ARGN})
    target_link_libraries(${target_name} PRIVATE netcalc_core)
    target_include_directories(${target_name} PRIVATE "${NETCALC_TEST_DIR}")
    set_target_properties(${target_name} PROPERTIES ${NETCALC_TARGET_PROPS})
    add_test(NAME ${test_name} COMMAND ${target_name})
endfunction()

add_netcalc_test(TEST_TARGET_given_ipv4_show_network_profile
    TEST_given_ipv4_show_network_profile
    "${NETCALC_TEST_DIR}/given_ip_show_network_profile.c"
)
add_netcalc_test(TEST_TARGET_show_network_profile_edge_cases
    TEST_show_network_profile_edge_cases
    "${NETCALC_TEST_DIR}/show_network_profile_edge_cases.c"
)
add_netcalc_test(TEST_TARGET_subnets_correctly
    TEST_subnets_correctly
    "${NETCALC_TEST_DIR}/subnets_correctly.c"
    "${NETCALC_TEST_DIR}/subnets_correctly_test_support.c"
)
add_netcalc_test(TEST_TARGET_subnets_correctly_edge_cases
    TEST_subnets_correctly_edge_cases
    "${NETCALC_TEST_DIR}/subnets_correctly_edge_cases.c"
    "${NETCALC_TEST_DIR}/subnets_correctly_test_support.c"
)