PROJECT(cmus-qt-front-end)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)
SET(CMAKE_PROJECT_NAME "Qtmus")
FIND_PACKAGE(Qt4 REQUIRED)

# CMAKE_INSTALL_PREFIX="/opt"

IF(NOT DEBUG)
    ADD_DEFINITIONS(-DNDEBUG -O2 -DQT_NO_DEBUG)
ELSE(NOT DEBUG)
    ADD_DEFINITIONS(-g)
ENDIF(NOT DEBUG)

SET(QT_USE_QTGUI TRUE)
SET(QT_USE_QTCORE TRUE)
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${QT_INCLUDES})

SET(qtmus_source	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp"
# 					"${CMAKE_CURRENT_SOURCE_DIR}/mainwindow2.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/qcmusremote.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/qcmussong.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/qcmusconnection.cpp"
                    )

SET(qtmus_moc		"${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h"
#					"${CMAKE_CURRENT_SOURCE_DIR}/mainwindow2.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/qcmusremote.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/qcmussong.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/qcmusconnection.h"
                    )

QT4_WRAP_CPP(qtmus_source ${qtmus_moc})

# QT4_ADD_RESOURCES(qtmus_source "${CMAKE_CURRENT_SOURCE_DIR}/icons.qrc")

SET(qtmus_uis    	"${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui"
# 					"${CMAKE_CURRENT_SOURCE_DIR}/mainwindow2.ui"
                    )

QT4_WRAP_UI(qtmus_source ${qtmus_uis})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(qtmus ${qtmus_source})
TARGET_LINK_LIBRARIES(qtmus ${QT_LIBRARIES})
# INSTALL(TARGETS qtmus DESTINATION ...)
