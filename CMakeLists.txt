cmake_minimum_required(VERSION 2.8)
project(pty-tcl)

find_package(TCL)
add_library(pty-tcl SHARED pty.c)

add_test(COMMAND tclsh tests/pty.test.tcl)
enable_testing()

install(TARGETS pty-tcl
        LIBRARY DESTINATION lib/pty-tcl0.1)
install(FILES pkgIndex.tcl
        DESTINATION lib/pty-tcl0.1)
