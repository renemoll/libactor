
add_executable(05_api_dynamic
	main.cpp
)

target_link_libraries(05_api_dynamic
	PRIVATE
		bob_interface
)
