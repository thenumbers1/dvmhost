// SPDX-License-Identifier: BSL-1.0
/*
 * Digital Voice Modem - Common Library
 * BSL-1.0 Open Source. Use is subject to license terms.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 *  Copyright (c) 2003-2013 Christopher M. Kohlhoff
 *  Copyright (C) 2023-2025 Bryan Biedenkapp, N2PLL
 *
 */
/**
 * @file SIPClient.h
 * @ingroup sip
 */
#if !defined(__SIP__SIP_HEADERS_H__)
#define __SIP__SIP_HEADERS_H__

#include "common/Defines.h"
#include "common/Log.h"
#include "common/Utils.h"

#include <string>
#include <vector>

namespace network
{
    namespace sip
    {
        // ---------------------------------------------------------------------------
        //  Class Prototypes
        // ---------------------------------------------------------------------------

        struct SIPPayload;

        // ---------------------------------------------------------------------------
        //  Structure Declaration
        // ---------------------------------------------------------------------------

        /**
         * @brief Represents SIP headers.
         * @ingroup sip
         */
        struct SIPHeaders {
            /**
             * @brief Structure representing an individual SIP header.
             * @ingroup sip
             */
            struct Header
            {
                /**
                 * @brief Header name.
                 */
                std::string name;
                /**
                 * @brief Header value.
                 */
                std::string value;

                /**
                 * @brief Initializes a new instance of the Header class.
                 */
                Header() : name{}, value{} { /* stub */}
                /**
                 * @brief Initializes a new instance of the Header class
                 * @param n Header name.
                 * @param v Header value.
                 */
                Header(std::string n, std::string v) : name{n}, value{v} { /* stub */ }
            };

            /**
             * @brief Gets the list of SIP headers.
             * @returns std::vector<Header> List of SIP headers.
             */
            std::vector<Header> headers() const { return m_headers; }
            /**
             * @brief Returns true if the headers are empty.
             * @returns bool True, if no SIP headers are present, otherwise false.
             */
            bool empty() const { return m_headers.empty(); }
            /**
             * @brief Returns the number of headers.
             * @returns std::size_t Number of headers.
             */
            std::size_t size() const { return m_headers.size(); }
            /**
             * @brief Clears the list of SIP headers.
             */
            void clearHeaders() { m_headers = std::vector<Header>(); }
            /**
             * @brief Helper to add a SIP header.
             * @param name Header name.
             * @param value Header value.
             */
            void add(const std::string& name, const std::string& value)
            {
                //::LogDebugEx(LOG_SIP, "SIPHeaders::add()", "header = %s, value = %s", name.c_str(), value.c_str());
                for (auto& header : m_headers) {
                    if (::strtolower(header.name) == ::strtolower(name)) {
                        header.value = value;
                        return;
                    }
                }

                m_headers.push_back(Header(name, value));
                //for (auto header : m_headers)
                //    ::LogDebugEx(LOG_SIP, "SIPHeaders::add()", "m_headers.header = %s, m_headers.value = %s", header.name.c_str(), header.value.c_str());
            }
            /**
             * @brief Helper to remove a SIP header.
             * @param headerName Header name.
             */
            void remove(const std::string headerName)
            {
                auto header = std::find_if(m_headers.begin(), m_headers.end(), [&](const Header& h) {
                    return ::strtolower(h.name) == ::strtolower(headerName);
                });

                if (header != m_headers.end()) {
                    m_headers.erase(header);
                }
            }
            /**
             * @brief Helper to find the named SIP header.
             * @param headerName Header name.
             * @returns std::string Value of named header (if any).
             */
            std::string find(const std::string headerName) const
            {
                auto header = std::find_if(m_headers.begin(), m_headers.end(), [&](const Header& h) {
                    return ::strtolower(h.name) == ::strtolower(headerName);
                });

                if (header != m_headers.end()) {
                    return header->value;
                }
                else {
                    return "";
                }
            }

        private:
            friend struct SIPPayload;
            std::vector<Header> m_headers;
        };
    } // namespace sip
} // namespace network

#endif // __SIP__SIP_HEADERS_H__
