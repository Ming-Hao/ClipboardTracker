cmake_minimum_required(VERSION 3.19.0)

project(ClipboardTracker)

list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    Clipboard/clipboardactionhandler.cpp
    Clipboard/clipboardmanager.cpp
    Clipboard/clipboardmonitor.cpp
    Clipboard/myclipboardlistmodel.cpp
    UI/customtrayicon.cpp
    UI/mainwindow.cpp
    main.cpp)

set(HEADERS
    Clipboard/clipboardactionhandler.h
    Clipboard/clipboardmanager.h
    Clipboard/clipboardmonitor.h
    Clipboard/myclipboardinfo.h
    Clipboard/myclipboardlistmodel.h
    UI/customtrayicon.h
    UI/mainwindow.h)

add_executable(ClipboardTracker ${SOURCES} ${HEADERS})

set_target_properties(
    ClipboardTracker
    PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

target_compile_features(ClipboardTracker PRIVATE cxx_std_17)
target_include_directories(ClipboardTracker PRIVATE Clipboard)
target_include_directories(ClipboardTracker PRIVATE UI)

target_link_libraries(ClipboardTracker Qt5::Widgets Qt5::Core)
target_include_directories(ClipboardTracker PRIVATE ${Qt5Core_INCLUDES} ${Qt5Widgets_INCLUDES})


install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)

find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QTDIR}")
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QTDIR}")

install(CODE "execute_process(COMMAND deploy.bat ${CMAKE_INSTALL_PREFIX}/Release/bin ${WINDEPLOYQT_ENV_SETUP} ${WINDEPLOYQT_EXECUTABLE})")