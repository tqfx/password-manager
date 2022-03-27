add_executable(t_key key.c)
target_link_libraries(t_key ${PROJECT_NAME})
add_test(NAME t_key COMMAND t_key)

add_executable(t_info info.c)
target_link_libraries(t_info ${PROJECT_NAME})
add_test(NAME t_info COMMAND t_info)

add_executable(t_word word.c)
target_link_libraries(t_word ${PROJECT_NAME})
add_test(NAME t_word COMMAND t_word)

add_executable(t_sqlite sqlite.c)
target_link_libraries(t_sqlite ${PROJECT_NAME})
add_test(NAME t_sqlite COMMAND t_sqlite)

add_executable(t_generate generate.c)
target_link_libraries(t_generate ${PROJECT_NAME})
add_test(NAME t_generate COMMAND t_generate)

add_executable(t_json json.c)
target_link_libraries(t_json ${PROJECT_NAME})
add_test(NAME t_json COMMAND t_json)
