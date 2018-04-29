foreach(component IN ITEMS pitch)
    add_executable (${component} "test_${component}.cpp")
    target_link_libraries(${component} lilyparse Mettle rapidcheck Threads::Threads)
    set_target_properties(${component} PROPERTIES PREFIX "ut.")
endforeach()


