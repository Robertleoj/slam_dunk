vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH_RAW
    REPO Robertleoj/slam_dunk
    REF bundle_with_vcpkg
    SHA512 9b4867cb293b76144f912f4539bc665f9ace4105beccd98f3c1fb37799c8e285e05ccc54f2fb7034e2788ed69c3c25e20ab9685f58426a82482d82070b16236a
    HEAD_REF bundle_with_vcpkg
)


vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH_RAW}/slamd"
)

# vcpkg_cmake_configure(
#     SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}"  # this is your real source folder
# )

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "slamd")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH_RAW}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
# file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)