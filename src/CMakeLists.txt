add_library(lilyparse "")

include(notation/CMakeLists.txt)

target_link_libraries(lilyparse PUBLIC type_safe fmt)
set_property(TARGET lilyparse PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY}
	"-checks=modernize-*,readability-*,performance-*,boost-*,clang-analyzer-*")
