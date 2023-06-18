/*-*-c++-*-*************************************************************************************************************
* Copyright 2021 - 2023 Inesonic, LLC.
*
* GNU Public License, Version 3:
*   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program.  If not, see
*   <https://www.gnu.org/licenses/>.
********************************************************************************************************************//**
* \file
*
* This file implements a trivial HTML scrubber function.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QByteArray>
#include <QString>

#include <cstdint>
#include <iostream>

#include "html_scrubber_engine.h"
#include "html_scrubber_scrubber.h"

namespace HtmlScrubber {
    Scrubber::Scrubber(const QByteArray& rawData):Engine(rawData) {}


    Scrubber::~Scrubber() {}


    void Scrubber::scrub() {
        outputData.clear();
        Engine::scrub();
    }


    QByteArray Scrubber::scrub(const QByteArray& rawData) {
        Scrubber scrubber(rawData);
        scrubber.scrub();
        return scrubber.outputData;
    }


    const QByteArray& Scrubber::output() const {
        return outputData;
    }


    void Scrubber::update(const char* inputPointer, unsigned long charsToCopy) {
        outputData.append(inputPointer, charsToCopy);
    }
}
