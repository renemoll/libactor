
add_executable(05_api_dynamic
	main.cpp
)

target_link_libraries(05_api_dynamic
	PRIVATE
		bob_interface
)

target_compile_options(05_api_dynamic
	PRIVATE
		# CMake hack: I do not want to use RTTI globaly but need it for this experiment.
		# So, to disable the `-fno-rtti` from bob_interface, define it again so "Option De-duplication"
		# does not add the second `-fno-rtti` (from bob_interface).
		# > see https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_OPTIONS.html#option-de-duplication
		-fno-rtti
		-frtti
)
