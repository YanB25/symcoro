add_executable(main main.cpp)
target_link_libraries(main PRIVATE symcoro_obj ${Boost_LIBRARIES})
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})