cmake_minimum_required ( VERSION 3.5 )

set ( APP_NAME "MyQtProject" )

#https://stackoverflow.com/a/53234995/12177714
set ( CXX_FLAGS
    "-fno-sized-deallocation"
    )

project ( ${APP_NAME} LANGUAGES CXX )

set ( CMAKE_INCLUDE_CURRENT_DIR ON )
set ( CMAKE_AUTOUIC ON )
set ( CMAKE_AUTOMOC ON )
set ( CMAKE_AUTORCC ON )
set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_FLAGS ${CXX_FLAGS} )

find_package ( QT NAMES Qt6 Qt5 COMPONENTS
    Core5Compat Core Gui Svg LinguistTools
    REQUIRED
    )
find_package ( Qt${QT_VERSION_MAJOR} COMPONENTS
    Core5Compat Core Gui Svg LinguistTools
    REQUIRED
    )

set ( TS_FILES
    ${APP_NAME}_en_EN.ts
    )

set ( RES_FILES
    resources.qrc
    )

set ( PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    config.cpp
    config.h
    properties.cpp
    properties.h
    helpers/helper_common.cpp
    helpers/helper_common.h
    helpers/widgethelper.cpp
    helpers/widgethelper.h
    helpers/helpergraphics.cpp
    helpers/helpergraphics.h
    interface/loggingcategories.cpp
    interface/loggingcategories.h
    interface/lineeditselectfile.cpp
    interface/lineeditselectfile.h
    interface/separators.cpp
    interface/separators.h
    interface/edittextwidget.cpp
    interface/edittextwidget.h
    dialogs/dialogbody.cpp
    dialogs/dialogbody.h
    dialogs/dialogvalueslist.cpp
    dialogs/dialogvalueslist.h
    dialogs/dialoginfocontent.cpp
    dialogs/dialoginfocontent.h
    dialogs/splashscreen.cpp
    dialogs/splashscreen.h
    dialogs/dialogtextedit.cpp
    dialogs/dialogtextedit.h
    ${RES_FILES}
    ${TS_FILES}
    )

# for logging
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    QT_MESSAGELOGCONTEXT
)

if ( WIN32 )
    set ( ADDEXEC_KEY "WIN32" ) # disabling terminal window
else ()
    set ( ADDEXEC_KEY "" )
endif ()

if ( ${QT_VERSION_MAJOR} GREATER_EQUAL 6 )
    qt_add_executable( ${APP_NAME} ${ADDEXEC_KEY} MANUAL_FINALIZATION ${PROJECT_SOURCES} )
    qt_create_translation( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )

else ()
    add_executable( ${APP_NAME} ${ADDEXEC_KEY} ${PROJECT_SOURCES} )
    qt5_create_translation( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )

endif ()

target_link_libraries ( ${APP_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core5Compat
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Svg
    )

if ( QT_VERSION_MAJOR EQUAL 6 )
    qt_finalize_executable( ${APP_NAME} ${ADDEXEC_KEY} )
endif ()
