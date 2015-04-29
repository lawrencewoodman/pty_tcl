cmake_minimum_required(VERSION 2.6)
project(CanAce)


add_library(pty-tcl SHARED pty.c)

find_package(TCL)

#target_link_libraries(canace tkwin ${SDL_LIBRARY} ${TCL_LIBRARY} ${TK_LIBRARY} m)
