foreach(component IN ITEMS pitch value)
    add_executable (${component} "test_${component}.cpp")
    target_link_libraries(${component} lilyparse Mettle rapidcheck Threads::Threads)
    set_target_properties(${component} PROPERTIES PREFIX "ut.")
    add_test(NAME ${component} 
	     COMMAND ${CMAKE_BINARY_DIR}/bin/ut.${component} -o verbose)
    setup_target_for_coverage(
	     NAME "cover.${component}" 
	     EXECUTABLE ${CMAKE_BINARY_DIR}/bin/ut.${component} 
	     DEPENDENCIES ${component})
endforeach()
