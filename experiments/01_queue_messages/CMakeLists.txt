
add_executable(01_queue_messages
	main.cpp
)

target_link_libraries(01_queue_messages
	PRIVATE
		bob_interface
)
