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

#ifndef HTML_SCRUBBER_H
#define HTML_SCRUBBER_H

#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QCryptographicHash>

#include <cstdint>

#include "html_scrubber_engine.h"

namespace HtmlScrubber {
    /**
     * Class that can be used to generate hashes from scrubbed HTML, removing tags, whitespace, and other elements that
     * are not visible.
     */
    class Hasher:private Engine, public QCryptographicHash {
        public:
            /**
             * Constructor
             *
             * \param[in] rawData       The raw data to be scrubbed and hashed.
             *
             * \param[in] hashAlgorithm The hashing algorithm to be used.
             */
            Hasher(const QByteArray& rawData, Algorithm hashAlgorithm);

            ~Hasher();

            /**
             * Method you can call to scrub HTML.
             */
            void scrubAndHash();

            /**
             * Functor
             *
             * \param[in] rawData       The raw data instance to be scrubbed.
             *
             * \param[in] hashAlgorithm The hashing algorithm to be used.
             *
             * \return Returns the resulting cryptographic hash.
             */
            static QByteArray scrubAndHash(const QByteArray& rawData, Algorithm hashAlgorithm);

        protected:
            /**
             * Method you can overload to modify how the scrubber operates on supplied raw data.
             *
             * \param[in] inputPointer The pointer to the data to be extracted or processed.
             *
             * \param[in] charsToCopy  The number of characters to be copied.
             */
            void update(const char* inputPointer, unsigned long charsToCopy) override;
    };
};
#endif
