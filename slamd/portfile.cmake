vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

# vcpkg_from_github(
#     OUT_SOURCE_PATH SOURCE_PATH
#     REPO Robertleoj/slam_dunk
#     REF master
#     SHA512 6e4961a8ddcefda1e4f09cba48d1e7520305931ce67283b0e00be4cc4dbb4897f0bff17752790e60b456e0e111f35c321d485d32223dfdb9226e71b461943737
#     HEAD_REF master
# )


# vcpkg_cmake_configure(
#     SOURCE_PATH "${SOURCE_PATH}"
# )

vcpkg_cmake_configure(
    SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}"  # this is your real source folder
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "slamd")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

# file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)