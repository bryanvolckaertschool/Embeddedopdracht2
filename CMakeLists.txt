cmake_minimum_required (VERSION 2.6)
project (ENV-CGI)
add_executable(ENV-CGI env-cgi.c)
set(EXECUTABLE_OUTPUT_PATH "/usr/lib/cgi-bin")

