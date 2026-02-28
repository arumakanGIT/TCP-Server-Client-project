cmake_minimum_required(VERSION 3.16)

project(Server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Protobuf)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Protobuf)

add_executable(Server
  main.cpp
  app.cpp app.h
  server.cpp server.h
  mainmenu.cpp mainmenu.h
  Message.pb.cc Message.pb.h
  logger.cpp logger.h
  networkmanager.cpp
  networkmanager.h
)
target_link_libraries(Server
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  protobuf::libprotobuf
)

include(GNUInstallDirs)
install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
