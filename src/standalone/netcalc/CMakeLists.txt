
cmake_minimum_required(VERSION 4.3)
project(netcalc LANGUAGES C)


set(REPO_ROOT_SRC "${CMAKE_CURRENT_LIST_DIR}/../..")
set(NETCALC_INCLUDES 
    "${CMAKE_CURRENT_LIST_DIR}"
    "${REPO_ROOT_SRC}"
)
set(NETCALC_TARGET_PROPS 
    C_STANDARD 23
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

add_executable(netcalc)
target_sources(netcalc PRIVATE
    main.c
    netcalc.c
    "${REPO_ROOT_SRC}/endian.c"
    "${REPO_ROOT_SRC}/mini_network_stack/shared.c"
)
target_include_directories(netcalc PRIVATE ${NETCALC_INCLUDES})
set_target_properties(netcalc PROPERTIES ${NETCALC_TARGET_PROPS})





add_executable(TEST_TARGET_given_ipv4_show_network_profile)
target_sources(TEST_TARGET_given_ipv4_show_network_profile PRIVATE
    "test/given_ip_show_network_profile.c"
    netcalc.c
    "${REPO_ROOT_SRC}/endian.c"
    "${REPO_ROOT_SRC}/mini_network_stack/shared.c"
)
add_dependencies(TEST_TARGET_given_ipv4_show_network_profile netcalc)
target_include_directories(TEST_TARGET_given_ipv4_show_network_profile PRIVATE 
    ${NETCALC_INCLUDES}
)
set_target_properties(netcalc PROPERTIES ${NETCALC_TARGET_PROPS})




add_executable(TEST_TARGET_show_network_profile_edge_cases)
target_sources(TEST_TARGET_show_network_profile_edge_cases PRIVATE
    "test/show_network_profile_edge_cases.c"
    netcalc.c
    "${REPO_ROOT_SRC}/endian.c"
    "${REPO_ROOT_SRC}/mini_network_stack/shared.c"
)
add_dependencies(TEST_TARGET_show_network_profile_edge_cases netcalc)
target_include_directories(TEST_TARGET_show_network_profile_edge_cases PRIVATE 
    ${NETCALC_INCLUDES}
)
set_target_properties(TEST_TARGET_show_network_profile_edge_cases PROPERTIES ${NETCALC_TARGET_PROPS})




add_test(NAME TEST_given_ipv4_show_network_profile COMMAND TEST_TARGET_given_ipv4_show_network_profile)
add_test(NAME TEST_show_network_profile_edge_cases COMMAND TEST_TARGET_show_network_profile_edge_cases)