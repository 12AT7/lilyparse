add_library(lilyparse SHARED pitch.cpp)

target_link_libraries(lilyparse type_safe fmt)
