
add_executable(03_process_message_queue
	main.cpp
)

target_link_libraries(03_process_message_queue
	PRIVATE
		bob_interface
)
