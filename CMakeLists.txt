cmake_minimum_required(VERSION 3.16)
project(PI_lab C)

# standard C 11
set(CMAKE_C_STANDARD 11)
link_libraries(m)

# fix debuggera
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

#add_executable(sum_N lab1/sum_N.c)
#add_executable(random lab1/random.c)
#add_executable(vec lab1/vec-template.c)
#add_executable(stack lab2/stack_template.c)
#add_executable(perm lab2/perm_template.c)
#add_executable(stat lab3/stat_template.c)
#add_executable(ids lab4/ids_template.c)
#add_executable(str lab4/str_template.c)
#add_executable(ids lab4/ids_template.c)
#add_executable(mat lab5/mat_template.c)
#add_executable(int lab6/int_template.c)
#add_executable(rel lab7/rel_template.c)
#add_executable(mat lab8/mat_template.c)
#add_executable(sort lab9/sort_template.c)
#add_executable(vect lab10/vect_template.c)
#add_executable(list lab11/list_template.c)
#add_executable(dll lab12/dll_template.c)
add_executable(ht lab13/ht_template.c)