add_library(stan "")

include(notation/CMakeLists.txt)
include(driver/lilypond/CMakeLists.txt)
include(driver/debug/CMakeLists.txt)

target_link_libraries(stan PUBLIC type_safe fmt)
set_property(TARGET stan PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY}
	"-checks=modernize-*,readability-*,performance-*,boost-*,clang-analyzer-*")
