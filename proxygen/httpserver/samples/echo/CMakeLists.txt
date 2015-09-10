#
# Copyright (C) 2015, Yeolar
#

add_executable(proxygen_httpserver_sample_echo
    EchoHandler.cpp  EchoServer.cpp)
target_link_libraries(proxygen_httpserver_sample_echo proxygen_static)

