
add_executable(hello_world_managed main.cpp)
target_link_libraries(
    hello_world_managed PRIVATE 
    
    slamd
)


