add_library(dll SHARED 
	main.cc
	memory.cc
	il2cpp.cc
	pch.h
)
target_precompile_headers(dll PRIVATE [[pch.h]])
target_link_libraries(dll PRIVATE -lpsapi minhook)

