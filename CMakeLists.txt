cmake_minimum_required(VERSION 3.24)
project(MSNWhatsAppXP VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Svg)

qt_standard_project_setup()

qt_add_executable(msn-whatsapp-xp
    native/src/main.cpp
    native/src/mainwindow.cpp
    native/src/mainwindow.h
    native/src/skinmanager.cpp
    native/src/skinmanager.h
    native/src/useravatar.cpp
    native/src/useravatar.h
    native/src/appstorage.cpp
    native/src/appstorage.h
    native/resources/resources.qrc
)

target_link_libraries(msn-whatsapp-xp PRIVATE Qt6::Widgets Qt6::Svg)

target_compile_definitions(msn-whatsapp-xp PRIVATE
    MSN_WA_XP_VERSION="${PROJECT_VERSION}"
)

set_target_properties(msn-whatsapp-xp PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    OUTPUT_NAME "MSN-WhatsApp-XP"
)
