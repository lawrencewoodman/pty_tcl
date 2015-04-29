cmake_minimum_required(VERSION 2.6)
project(CanAce)

find_package(TCL)
add_library(pty-tcl SHARED pty.c)
