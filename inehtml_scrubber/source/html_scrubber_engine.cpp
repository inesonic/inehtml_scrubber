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

#include "html_scrubber_parser.h"
#include "html_scrubber_engine.h"

namespace HtmlScrubber {
    Engine::Engine(const QByteArray& rawData):inputData(rawData) {}


    Engine::~Engine() {}


    void Engine::scrub() {
        inputData.append('\0');
        inputData.append('\0');
        inputData.append('\0');
        inputData.append('\0');

        char*         basePointer     = inputData.data();
        unsigned long inputBase       = 0;
        unsigned long inputIndex      = 0;
        unsigned long inputLength     = static_cast<unsigned long>(inputData.size());
        unsigned long outputLength    = 0;
        CaptureMode   lastCaptureMode = CaptureMode::IN_TEXT;
        captureMode  = CaptureMode::IN_TEXT;


        reset();

        while (inputIndex < inputLength) {
            char& c = basePointer[inputIndex];

            if ((c & 0x80) == 0x00) {
                lastCaptureMode = captureMode;

                parse(c);

                if (captureMode == CaptureMode::IGNORE) {
                    if (lastCaptureMode != CaptureMode::IGNORE) {
                        update(basePointer + inputBase, outputLength);
                        outputLength = 0;
                    }
                } else {
                    ++outputLength;

                    if (lastCaptureMode == CaptureMode::IGNORE) {
                        inputBase = inputIndex;
                    }
                }

                ++inputIndex;
            } else if ((c & 0xE0) == 0xC0) {
                if (captureMode != CaptureMode::IGNORE) {
                    outputLength += 2;
                }

                inputIndex += 2;
            } else if ((c & 0xF0) == 0xE0) {
                if (captureMode != CaptureMode::IGNORE) {
                    outputLength += 3;
                }

                inputIndex += 3;
            } else if ((c & 0xF8) == 0xF0) {
                if (captureMode != CaptureMode::IGNORE) {
                    outputLength += 4;
                }

                inputIndex += 4;
            } else {
                if (captureMode != CaptureMode::IGNORE) {
                    ++outputLength;
                }

                ++inputIndex;
            }
        }

        if (captureMode != CaptureMode::IGNORE) {
            update(basePointer + inputBase, outputLength);
        }
    }


    const QByteArray& Engine::input() const {
        return inputData;
    }


    QByteArray& Engine::input() {
        return inputData;
    }


    void Engine::startTag(Engine::States /* oldState */, Engine::States /* newState */, char& /* c */)  {
        captureMode = CaptureMode::IGNORE;
    }


    void Engine::multipleWhiteSpace(
            Engine::States /* oldState */,
            Engine::States /* newState */,
            char&          /* c */
        )  {
        captureMode = CaptureMode::IGNORE;
    }


    void Engine::whitespace(Engine::States /* oldState */, Engine::States /* newState */, char& c) {
        c = ' ';
    }


    void Engine::resumeText(Engine::States /* oldState */, Engine::States /* newState */, char& /* c */)  {
        captureMode = CaptureMode::IN_TEXT;
    }


    void Engine::endTag(Engine::States /* oldState */, Engine::States /* newState */, char& /* c */)  {}


    void Engine::startSrcAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = beginSrcAttribute;
        captureMode = CaptureMode::IN_URL;
    }


    void Engine::endSrcAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = finishSrcAttribute;
        captureMode = CaptureMode::IGNORE;
    }


    void Engine::startHrefAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = beginHrefAttribute;
        captureMode = CaptureMode::IN_URL;
    }


    void Engine::endHrefAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = finishHrefAttribute;
        captureMode = CaptureMode::IGNORE;
    }


    void Engine::startCiteAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = beginCiteAttribute;
        captureMode = CaptureMode::IN_URL;
    }


    void Engine::endCiteAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = finishCiteAttribute;
        captureMode = CaptureMode::IGNORE;
    }


    void Engine::startScriptSrcAttribute(
            Engine::States /* oldState */,
            Engine::States /* newState */,
            char&          c
        )  {
        c           = beginSrcAttribute;
        captureMode = CaptureMode::IN_URL;
    }


    void Engine::endScriptSrcAttribute(Engine::States /* oldState */, Engine::States /* newState */, char& c)  {
        c           = finishSrcAttribute;
        captureMode = CaptureMode::IGNORE;
    }


    void Engine::startStyle(Engine::States /* oldState */, Engine::States /* newState */, char& /* c */)  {
        captureMode = CaptureMode::IN_STYLE;
    }


    void Engine::endStyle(Engine::States /* oldState */, Engine::States /* newState */, char& /* c */)  {
        captureMode = CaptureMode::IGNORE;
    }
}
