cmake_minimum_required(VERSION 3.4.1)

add_library(Tcp
        STATIC
        tcp/TcpSocket.cpp
        tcp/TcpConnection.cpp
        tcp/NativeByteBuffer.cpp
        tcp/ByteStream.cpp
        tcp/ConnectionManager.cpp
        tcp/EventsDispatcher.cpp
        tcp/Request.cpp
        tcp/DataCenter.cpp
        tcp/Timer.cpp)