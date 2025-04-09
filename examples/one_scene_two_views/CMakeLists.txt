add_executable(
    one_scene_two_views
    main.cpp
)

target_link_libraries(
    one_scene_two_views 
    
    PRIVATE

    slamd::slamd
)