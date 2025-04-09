
add_executable(hello_world main.cpp)
target_link_libraries(
    hello_world PRIVATE 
    
    slamd::slamd
)


