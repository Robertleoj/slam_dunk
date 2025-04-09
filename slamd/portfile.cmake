vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH REPO_ROOT
    REPO Robertleoj/slam_dunk
    REF bundle_with_vcpkg
    SHA512 71b6bf6f7cf49337a5026933447a2a290e654589ec2e697d113b58c258ca5d7d6777427d37279ecac57732b51d3fd996c8aa9fbbbb9d3c8aaf8ccaabc2441570
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