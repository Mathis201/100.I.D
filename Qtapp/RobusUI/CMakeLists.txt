cmake_minimum_required(VERSION 3.19)
project(RobusUI LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Bluetooth)

qt_standard_project_setup()

qt_add_executable(RobusUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    devicehandler.h
    devicehandler.cpp
    BoardWidget.h
    BoardWidget.cpp
    playwindow.h playwindow.cpp playwindow.ui


)
target_include_directories(RobusUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(APPLE)
    set_target_properties(RobusUI PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST
        "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )

    target_link_libraries(RobusUI PRIVATE
        Qt6::QDarwinBluetoothPermissionPlugin
    )
endif()

target_link_libraries(RobusUI
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Bluetooth
)

include(GNUInstallDirs)

install(TARGETS RobusUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET RobusUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
