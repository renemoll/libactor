
add_executable(02_call_functions
	main.cpp
)

target_link_libraries(02_call_functions
	PRIVATE
		bob_interface
)

target_compile_options(02_call_functions
	PRIVATE
		# CMake hack: I do not want to use RTTI globaly but need it for this experiment.
		# So, to disable the `-fno-rtti` from bob_interface, define it again so "Option De-duplication"
		# does not add the second `-fno-rtti` (from bob_interface).
		# > see https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_OPTIONS.html#option-de-duplication
		-fno-rtti
		-frtti
)
