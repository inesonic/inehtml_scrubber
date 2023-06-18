##-*-makefile-*-########################################################################################################
# Copyright 2022 - 2023 Inesonic, LLC.
#
# GNU Public License, Version 3:
#   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
#   version.
#   
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#   
#   You should have received a copy of the GNU General Public License along with this program.  If not, see
#   <https://www.gnu.org/licenses/>.
########################################################################################################################

TEMPLATE = lib

########################################################################################################################
# Basic build characteristics
#

QT += core
CONFIG += static c++14

win32 {
    # Windows generates a warning about potential overflow during operations on integer values.  While the warning may
    # make sense in some cases, it rapidly becomes excessive and is meaningless for this module.  We turn that warning
    # off here.

    QMAKE_CXXFLAGS_WARN_ON += -wd4307
}

########################################################################################################################
# Public includes
#

INCLUDEPATH += include
HEADERS = include/html_scrubber_parser.h \
          include/html_scrubber_engine.h \
          include/html_scrubber_scrubber.h \
          include/html_scrubber_hasher.h \

########################################################################################################################
# Source files
#

SOURCES = source/html_scrubber_engine.cpp \
          source/html_scrubber_scrubber.cpp \
          source/html_scrubber_hasher.cpp \

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = inehtml_scrubber

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
