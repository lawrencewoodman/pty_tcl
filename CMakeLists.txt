cmake_minimum_required(VERSION 2.8)
project(pty-tcl)

find_package(TCL)
add_library(pty-tcl SHARED pty.c)
