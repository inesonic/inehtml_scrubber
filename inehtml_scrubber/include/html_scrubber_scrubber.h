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

#ifndef HTML_SCRUBBER_SCRUBBER_H
#define HTML_SCRUBBER_SCRUBBER_H

#include <QtGlobal>
#include <QByteArray>
#include <QString>

#include <cstdint>

#include "html_scrubber_engine.h"

namespace HtmlScrubber {
    /**
     * Class that can be used to generate scrubbed HTML, removing tags, whitespace, and other elements that are not
     * visible.
     */
    class Scrubber:private Engine {
        public:
            /**
             * Constructor
             *
             * \param[in] rawData The raw data to be scrubbed.  Data will be modified, in place when scrubbed.
             */
            Scrubber(const QByteArray& rawData);

            ~Scrubber();

            /**
             * Method you can call to scrub HTML.
             *
             * \return Returns true on success, returns false on error.
             */
            void scrub();

            /**
             * Functor
             *
             * \param[in] rawData The raw data instance to be scrubbed.
             *
             * \return Returns the resulting scrubbed data.
             */
            static QByteArray scrub(const QByteArray& rawData);

            /**
             * Method you can use to obtain the current output data instance.
             *
             * \return Returns a reference to the raw output data instance.
             */
            const QByteArray& output() const;

        protected:
            /**
             * Method you can overload to modify how the scrubber operates on supplied raw data.
             *
             * \param[in] inputPointer The pointer to the data to be extracted or processed.
             *
             * \param[in] charsToCopy  The number of characters to be copied.
             */
            void update(const char* inputPointer, unsigned long charsToCopy) override;

        private:
            /**
             * The resulting output data.
             */
            QByteArray outputData;
    };
};
#endif
