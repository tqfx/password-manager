add_executable(t_key key.c)
target_link_libraries(t_key manager)
add_test(NAME t_key COMMAND t_key)

add_executable(t_info info.c)
target_link_libraries(t_info manager)
add_test(NAME t_info COMMAND t_info)

add_executable(t_word word.c)
target_link_libraries(t_word manager)
add_test(NAME t_word COMMAND t_word)

add_executable(t_sqlite sqlite.c)
target_link_libraries(t_sqlite manager)
add_test(NAME t_sqlite COMMAND t_sqlite)

add_executable(t_generate generate.c)
target_link_libraries(t_generate manager)
add_test(NAME t_generate COMMAND t_generate)
