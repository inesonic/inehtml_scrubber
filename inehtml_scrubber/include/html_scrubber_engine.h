/*-*-c++-*-*************************************************************************************************************
* Copyright 2022 - 2023 Inesonic, LLC.
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
* This header provides access to the HTML Scrubber function.
***********************************************************************************************************************/

/* .. sphinx-project inehtmlscrubber */

#ifndef HTML_SCRUBBER_ENGINE_H
#define HTML_SCRUBBER_ENGINE_H

#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QCryptographicHash>

#include <cstdint>

#include "html_scrubber_parser.h"

namespace HtmlScrubber {
    /**
     * Class that can be used to process HTML, removing tags, whitespace, and other elements that are not visible.
     *
     * You can use this class several distinct ways:
     * - You can use the supplied functor to simply scrub an HTML payload, in place.
     * - You can derive from this class, overloading the update method.  You can then call the supplied functor to
     *   do comparisons or other tasks on scrubbed HTML.
     *
     * \note
     * To generate the base class, use the command:
     *
     *     ./ypg --yaml parser.yaml                \
     *           -m INEHTML_SCRUBBER_PARSER        \
     *           -s inehtml_parser                 \
     *           -c Parser                         \
     *           -n HtmlScrubber                   \
     *           -o include/html_scrubber_parser.h
     */
    class Engine:private Parser {
        public:
            /**
             * Value indicating the start of a source attribute.
             */
            static const char beginSrcAttribute = 0x18;

            /**
             * Value indicating the end of a source attribute.
             */
            static const char finishSrcAttribute = 0x19;

            /**
             * Value indicating the start of an href attribute.
             */
            static const char beginHrefAttribute = 0x1A;

            /**
             * Value indicating the end of an href attribute.
             */
            static const char finishHrefAttribute = 0x1B;

            /**
             * Value indicating the start of a cite attribute.
             */
            static const char beginCiteAttribute = 0x1C;

            /**
             * Value indicating the end of a cite attribute.
             */
            static const char finishCiteAttribute = 0x1D;

            /**
             * Constructor
             *
             * \param[in] rawData The raw data to be scrubbed.  Data will be modified, in place when scrubbed.
             */
            Engine(const QByteArray& rawData);

            ~Engine();

            /**
             * Method you can call to scrub HTML.
             *
             * \return Returns true on success, returns false on error.
             */
            void scrub();

            /**
             * Method you can use to obtain the current raw input data instance.
             *
             * \return Returns a reference to the raw input data instance.
             */
            const QByteArray& input() const;

        protected:
            /**
             * Method you can use to obtain the current raw data instance.
             *
             * \return Returns a reference to the raw data instance, with modifications.
             */
            QByteArray& input();

            /**
             * Pure virtual method you can overload to modify how the scrubber operates on supplied raw data.
             *
             * \param[in] inputPointer The pointer to the data to be extracted or processed.
             *
             * \param[in] charsToCopy  The number of characters to be copied.
             */
            virtual void update(const char* inputPointer, unsigned long charsToCopy) = 0;

        private:
            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TEXT_SPACE -> IN_TAG_START c == '<'
             * * IN_TEXT_MULTIPLE_SPACE -> IN_TAG_START c == '<'
             * * IN_TEXT -> IN_TAG_START c == '<'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void startTag(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TEXT_SPACE -> IN_TEXT_MULTIPLE_SPACE std::isspace(c)
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void multipleWhiteSpace(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TEXT -> IN_TEXT_SPACE std::isspace(c)
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            virtual void whitespace(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TEXT_SPACE -> IN_TEXT c != '<' && !std::isspace(c)
             * * IN_TEXT_MULTIPLE_SPACE -> IN_TEXT c != '<' && !std::isspace(c)
             * * IN_TAG_SPACE_S -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_SR -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_SRC -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_SRC_EQUALS -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_H -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_HR -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_HRE -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_HREF -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_HREF_EQUALS -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_C -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_CI -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_CIT -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_CITE -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE_CITE_EQUALS -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_S -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SC -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCR -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRI -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIP -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIPT -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIPT_SPACE -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIPT_SPACE_S -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIPT_SPACE_SR -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIPT_SPACE_SRC -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SCRIPT_SPACE_SRC_EQUALS -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_ST -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_STY -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_STYL -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_STYLE_SLASH -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_STYLE_SEARCH -> IN_TEXT_SPACE c == '>'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void resumeText(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SEARCH -> IN_TEXT_SPACE c == '>'
             * * IN_TAG_SPACE -> IN_TEXT_SPACE c == '>'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void endTag(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SPACE_SRC_EQUALS -> IN_TAG_SPACE_SRC_EQUALS_QUOTE c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void startSrcAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SPACE_SRC_EQUALS_QUOTE -> IN_TAG_SEARCH c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void endSrcAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SPACE_HREF_EQUALS -> IN_TAG_SPACE_HREF_EQUALS_QUOTE c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void startHrefAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SPACE_HREF_EQUALS_QUOTE -> IN_TAG_SEARCH c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void endHrefAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SPACE_CITE_EQUALS -> IN_TAG_SPACE_CITE_EQUALS_QUOTE c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void startCiteAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SPACE_CITE_EQUALS_QUOTE -> IN_TAG_SEARCH c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void endCiteAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SCRIPT_SPACE_SRC_EQUALS -> IN_TAG_SCRIPT_SPACE_SRC_EQUALS_QUOTE c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void startScriptSrcAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_SCRIPT_SPACE_SRC_EQUALS_QUOTE -> IN_TEXT_SPACE c == '"'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void endScriptSrcAttribute(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_TAG_STYLE -> IN_STYLE_START c == '>'
             * * IN_STYLE_START -> IN_STYLE
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void startStyle(States oldState, States newState, char& c) final;

            /**
             * Virtual method triggered on the following transitions:
             *
             * * IN_STYLE -> IN_TAG_START c == '<'
             *
             * \param[in] oldState Our current state.
             *
             * \param[in] newState The new state.
             *
             * \param[in] c        The value that triggered this transition.
             */
            void endStyle(States oldState, States newState, char& c) final;

        private:
            /**
             * The supported data capture modes.
             */
            enum class CaptureMode {
                /**
                 * Indicates the content should be ignored.
                 */
                IGNORE,

                /**
                 * Indicates that we're in normal text content.
                 */
                IN_TEXT,

                /**
                 * Indicates that we're in JavaScript content.
                 */
                IN_SCRIPT,

                /**
                 * Indicates that we're in CSS style data.
                 */
                IN_STYLE,

                /**
                 * Indicates we're in a URL, either a src. cite, or href attribute within a tag.
                 */
                IN_URL
            };

            /**
             * The current data capture mode.
             */
            CaptureMode captureMode;

            /**
             * The raw data to be scrubbed.
             */
            QByteArray inputData;
    };
};
#endif
