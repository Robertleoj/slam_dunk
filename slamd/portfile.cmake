vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH REPO_ROOT
    REPO Robertleoj/slam_dunk
    REF bundle_with_vcpkg
    SHA512 89435f685d8092c2dd54ddd1223da20858330dc18861026afa0c72f01995bc44a65e1701208ecbeba3f2bbf6b96dc3229fc4e3a93534a1c81e661342120d7105
    HEAD_REF bundle_with_vcpkg
)

vcpkg_cmake_configure(
    SOURCE_PATH "${REPO_ROOT}/slamd"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "slamd")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${REPO_ROOT}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
# file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)