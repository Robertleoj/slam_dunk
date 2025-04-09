vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH REPO_ROOT
    REPO Robertleoj/slam_dunk
    REF bundle_with_vcpkg
    SHA512 b3c656c18f354ba24dde47e6268e33b330df52db5a6d47ee787178de42af22d7bc6006909fcc732a27e04bba859dc4c60330a789f406754603a121abae109582
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