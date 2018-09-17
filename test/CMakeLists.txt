foreach(component IN ITEMS value pitch lilypond_writer lilypond_reader)
    add_executable (${component} "test_${component}.cpp")
    target_link_libraries(${component} stan libmettle rapidcheck Threads::Threads)
    set_target_properties(${component} PROPERTIES PREFIX "ut.")
    add_test(NAME ${component} 
	     COMMAND ${CMAKE_BINARY_DIR}/bin/ut.${component} -o verbose)
endforeach()
