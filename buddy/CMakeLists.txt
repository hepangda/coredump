cmake_minimum_required(VERSION 3.12)
project(buddy_system)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -lreadline)

add_executable(buddy_system main.cpp buddy_allocator.cc buddy_allocator.h buddy_types.h memory_zone_list.cc memory_zone_list.h buddy_proxy.cc buddy_proxy.h clrprintf.h)