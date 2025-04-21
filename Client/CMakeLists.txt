cmake_minimum_required(VERSION 3.16)

project(Client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appClient
    main.cpp
)

qt_add_qml_module(appClient
    URI Client
    VERSION 1.0
    SOURCES client.h client.cpp
    QML_FILES Main.qml
    QML_FILES Authorization.qml
    QML_FILES Chat.qml
    QML_FILES Start.qml
)

target_link_libraries(appClient
    PRIVATE Qt6::Quick Qt6::Network Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
