// SPDX-License-Identifier: BSL-1.0
/*
 * Digital Voice Modem - Common Library
 * BSL-1.0 Open Source. Use is subject to license terms.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 *  Copyright (c) 2003-2013 Christopher M. Kohlhoff
 *  Copyright (C) 2023 Bryan Biedenkapp, N2PLL
 *
 */
#include "Defines.h"
#include "network/rest/http/HTTPPayload.h"
#include "Log.h"
#include "Utils.h"

using namespace network::rest::http;

#include <string>

namespace status_strings {
    const std::string ok = "HTTP/1.0 200 OK\r\n";
    const std::string created = "HTTP/1.0 201 Created\r\n";
    const std::string accepted = "HTTP/1.0 202 Accepted\r\n";
    const std::string no_content = "HTTP/1.0 204 No Content\r\n";
    const std::string multiple_choices = "HTTP/1.0 300 Multiple Choices\r\n";
    const std::string moved_permanently = "HTTP/1.0 301 Moved Permanently\r\n";
    const std::string moved_temporarily = "HTTP/1.0 302 Moved Temporarily\r\n";
    const std::string not_modified = "HTTP/1.0 304 Not Modified\r\n";
    const std::string bad_request = "HTTP/1.0 400 Bad Request\r\n";
    const std::string unauthorized = "HTTP/1.0 401 Unauthorized\r\n";
    const std::string forbidden = "HTTP/1.0 403 Forbidden\r\n";
    const std::string not_found = "HTTP/1.0 404 Not Found\r\n";
    const std::string internal_server_error = "HTTP/1.0 500 Internal Server Error\r\n";
    const std::string not_implemented = "HTTP/1.0 501 Not Implemented\r\n";
    const std::string bad_gateway = "HTTP/1.0 502 Bad Gateway\r\n";
    const std::string service_unavailable = "HTTP/1.0 503 Service Unavailable\r\n";

    asio::const_buffer toBuffer(HTTPPayload::StatusType status)
    {
        switch (status)
        {
        case HTTPPayload::OK:
            return asio::buffer(ok);
        case HTTPPayload::CREATED:
            return asio::buffer(created);
        case HTTPPayload::ACCEPTED:
            return asio::buffer(accepted);
        case HTTPPayload::NO_CONTENT:
            return asio::buffer(no_content);
        case HTTPPayload::MULTIPLE_CHOICES:
            return asio::buffer(multiple_choices);
        case HTTPPayload::MOVED_PERMANENTLY:
            return asio::buffer(moved_permanently);
        case HTTPPayload::MOVED_TEMPORARILY:
            return asio::buffer(moved_temporarily);
        case HTTPPayload::NOT_MODIFIED:
            return asio::buffer(not_modified);
        case HTTPPayload::BAD_REQUEST:
            return asio::buffer(bad_request);
        case HTTPPayload::UNAUTHORIZED:
            return asio::buffer(unauthorized);
        case HTTPPayload::FORBIDDEN:
            return asio::buffer(forbidden);
        case HTTPPayload::NOT_FOUND:
            return asio::buffer(not_found);
        case HTTPPayload::INTERNAL_SERVER_ERROR:
            return asio::buffer(internal_server_error);
        case HTTPPayload::NOT_IMPLEMENTED:
            return asio::buffer(not_implemented);
        case HTTPPayload::BAD_GATEWAY:
            return asio::buffer(bad_gateway);
        case HTTPPayload::SERVICE_UNAVAILABLE:
            return asio::buffer(service_unavailable);
        default:
            return asio::buffer(internal_server_error);
        }
    }
} // namespace status_strings

namespace misc_strings {
    const char name_value_separator[] = { ':', ' ' };
    const char request_method_separator[] = { ' ' };
    const char crlf[] = { '\r', '\n' };

    const char http_default_version[] = { 'H', 'T', 'T', 'P', '/', '1', '.', '0' };
} // namespace misc_strings

namespace stock_replies {
    const char ok[] = "";
    const char json_ok[] = "{status:200,message:\"ok\"}";
    const char created[] =
        "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created</h1></body>"
        "</html>";
    const char json_created[] = "{status:201,message:\"created\"}";
    const char accepted[] =
        "<html>"
        "<head><title>Accepted</title></head>"
        "<body><h1>202 Accepted</h1></body>"
        "</html>";
    const char json_accepted[] = "{status:202,message:\"accepted\"}";
    const char no_content[] =
        "<html>"
        "<head><title>No Content</title></head>"
        "<body><h1>204 Content</h1></body>"
        "</html>";
    const char json_no_content[] = "{status:204,message:\"no content\"}";
    const char multiple_choices[] =
        "<html>"
        "<head><title>Multiple Choices</title></head>"
        "<body><h1>300 Multiple Choices</h1></body>"
        "</html>";
    const char json_multiple_choices[] = "{status:300,message:\"multiple choices\"}";
    const char moved_permanently[] =
        "<html>"
        "<head><title>Moved Permanently</title></head>"
        "<body><h1>301 Moved Permanently</h1></body>"
        "</html>";
    const char json_moved_permanently[] = "{status:301,message:\"moved permanently\"}";
    const char moved_temporarily[] =
        "<html>"
        "<head><title>Moved Temporarily</title></head>"
        "<body><h1>302 Moved Temporarily</h1></body>"
        "</html>";
    const char json_moved_temporarily[] = "{status:302,message:\"moved temporarily\"}";
    const char not_modified[] =
        "<html>"
        "<head><title>Not Modified</title></head>"
        "<body><h1>304 Not Modified</h1></body>"
        "</html>";
    const char json_not_modified[] = "{status:304,message:\"not modified\"}";
    const char bad_request[] =
        "<html>"
        "<head><title>Bad Request</title></head>"
        "<body><h1>400 Bad Request</h1></body>"
        "</html>";
    const char json_bad_request[] = "{status:400,message:\"bad request\"}";
    const char unauthorized[] =
        "<html>"
        "<head><title>Unauthorized</title></head>"
        "<body><h1>401 Unauthorized</h1></body>"
        "</html>";
    const char json_unauthorized[] = "{status:401,message:\"unauthorized\"}";
    const char forbidden[] =
        "<html>"
        "<head><title>Forbidden</title></head>"
        "<body><h1>403 Forbidden</h1></body>"
        "</html>";
    const char json_forbidden[] = "{status:403,message:\"forbidden\"}";
    const char not_found[] =
        "<html>"
        "<head><title>Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
        "</html>";
    const char json_not_found[] = "{status:404,message:\"not found\"}";
    const char internal_server_error[] =
        "<html>"
        "<head><title>Internal Server Error</title></head>"
        "<body><h1>500 Internal Server Error</h1></body>"
        "</html>";
    const char json_internal_server_error[] = "{status:500,message:\"internal server error\"}";
    const char not_implemented[] =
        "<html>"
        "<head><title>Not Implemented</title></head>"
        "<body><h1>501 Not Implemented</h1></body>"
        "</html>";
    const char json_not_implemented[] = "{status:501,message:\"not implemented\"}";
    const char bad_gateway[] =
        "<html>"
        "<head><title>Bad Gateway</title></head>"
        "<body><h1>502 Bad Gateway</h1></body>"
        "</html>";
    const char json_bad_gateway[] = "{status:502,message:\"bad gateway\"}";
    const char service_unavailable[] =
        "<html>"
        "<head><title>Service Unavailable</title></head>"
        "<body><h1>503 Service Unavailable</h1></body>"
        "</html>";
    const char json_service_unavailable[] = "{status:503,message:\"service unavailable\"}";

    std::string to_string(HTTPPayload::StatusType status, std::string contentType)
    {
        std::transform(contentType.begin(), contentType.end(), contentType.begin(), ::tolower);
        if (contentType == "application/json") {
            switch (status)
            {
            case HTTPPayload::OK:
                return json_ok;
            case HTTPPayload::CREATED:
                return json_created;
            case HTTPPayload::ACCEPTED:
                return json_accepted;
            case HTTPPayload::NO_CONTENT:
                return json_no_content;
            case HTTPPayload::MULTIPLE_CHOICES:
                return json_multiple_choices;
            case HTTPPayload::MOVED_PERMANENTLY:
                return json_moved_permanently;
            case HTTPPayload::MOVED_TEMPORARILY:
                return json_moved_temporarily;
            case HTTPPayload::NOT_MODIFIED:
                return json_not_modified;
            case HTTPPayload::BAD_REQUEST:
                return json_bad_request;
            case HTTPPayload::UNAUTHORIZED:
                return json_unauthorized;
            case HTTPPayload::FORBIDDEN:
                return json_forbidden;
            case HTTPPayload::NOT_FOUND:
                return json_not_found;
            case HTTPPayload::INTERNAL_SERVER_ERROR:
                return json_internal_server_error;
            case HTTPPayload::NOT_IMPLEMENTED:
                return json_not_implemented;
            case HTTPPayload::BAD_GATEWAY:
                return json_bad_gateway;
            case HTTPPayload::SERVICE_UNAVAILABLE:
                return json_service_unavailable;
            default:
                return json_internal_server_error;
            }
        }
        else {
            switch (status)
            {
            case HTTPPayload::OK:
                return ok;
            case HTTPPayload::CREATED:
                return created;
            case HTTPPayload::ACCEPTED:
                return accepted;
            case HTTPPayload::NO_CONTENT:
                return no_content;
            case HTTPPayload::MULTIPLE_CHOICES:
                return multiple_choices;
            case HTTPPayload::MOVED_PERMANENTLY:
                return moved_permanently;
            case HTTPPayload::MOVED_TEMPORARILY:
                return moved_temporarily;
            case HTTPPayload::NOT_MODIFIED:
                return not_modified;
            case HTTPPayload::BAD_REQUEST:
                return bad_request;
            case HTTPPayload::UNAUTHORIZED:
                return unauthorized;
            case HTTPPayload::FORBIDDEN:
                return forbidden;
            case HTTPPayload::NOT_FOUND:
                return not_found;
            case HTTPPayload::INTERNAL_SERVER_ERROR:
                return internal_server_error;
            case HTTPPayload::NOT_IMPLEMENTED:
                return not_implemented;
            case HTTPPayload::BAD_GATEWAY:
                return bad_gateway;
            case HTTPPayload::SERVICE_UNAVAILABLE:
                return service_unavailable;
            default:
                return internal_server_error;
            }
        }
    }
} // namespace stock_replies

// ---------------------------------------------------------------------------
//  Public Class Members
// ---------------------------------------------------------------------------

/* Convert the reply into a vector of buffers. The buffers do not own the underlying memory blocks, therefore the reply object must remain valid and not be changed until the write operation has completed. */

std::vector<asio::const_buffer> HTTPPayload::toBuffers()
{
    std::vector<asio::const_buffer> buffers;
    if (isClientPayload) {
        // copy method and erase zero terminator
        method.erase(std::find(method.begin(), method.end(), '\0'), method.end());

        // copy URI and erase zero terminator
        uri.erase(std::find(uri.begin(), uri.end(), '\0'), uri.end());
#if DEBUG_HTTP_PAYLOAD
        ::LogDebugEx(LOG_REST, "HTTPPayload::toBuffers()", "method = %s, uri = %s", method.c_str(), uri.c_str());
#endif
        buffers.push_back(asio::buffer(method));
        buffers.push_back(asio::buffer(misc_strings::request_method_separator));
        buffers.push_back(asio::buffer(uri));
        buffers.push_back(asio::buffer(misc_strings::request_method_separator));
        buffers.push_back(asio::buffer(misc_strings::http_default_version));
        buffers.push_back(asio::buffer(misc_strings::crlf));
    }
    else {
        buffers.push_back(status_strings::toBuffer(status));
    }

    for (std::size_t i = 0; i < headers.size(); ++i) {
        HTTPHeaders::Header& h = headers.m_headers[i];
#if DEBUG_HTTP_PAYLOAD
        ::LogDebugEx(LOG_REST, "HTTPPayload::toBuffers()", "header = %s, value = %s", h.name.c_str(), h.value.c_str());
#endif

        buffers.push_back(asio::buffer(h.name));
        buffers.push_back(asio::buffer(misc_strings::name_value_separator));
        buffers.push_back(asio::buffer(h.value));
        buffers.push_back(asio::buffer(misc_strings::crlf));
    }

    buffers.push_back(asio::buffer(misc_strings::crlf));
    if (content.size() > 0)
        buffers.push_back(asio::buffer(content));

#if DEBUG_HTTP_PAYLOAD
    ::LogDebugEx(LOG_REST, "HTTPPayload::toBuffers()", "content = %s", content.c_str());
    for (auto buffer : buffers)
        Utils::dump("[HTTPPayload::toBuffers()] buffer[]", (uint8_t*)buffer.data(), buffer.size());
#endif

    return buffers;
}

/* Prepares payload for transmission by finalizing status and content type. */

void HTTPPayload::payload(json::object& obj, HTTPPayload::StatusType s)
{
    json::value v = json::value(obj);
    std::string json = std::string(v.serialize());
    payload(json, s, "application/json");
}

/* Prepares payload for transmission by finalizing status and content type. */

void HTTPPayload::payload(std::string& c, HTTPPayload::StatusType s, const std::string& contentType)
{
    content = c;
    status = s;
    ensureDefaultHeaders(contentType);
}

// ---------------------------------------------------------------------------
//  Static Members
// ---------------------------------------------------------------------------

/* Get a status payload. */

HTTPPayload HTTPPayload::requestPayload(std::string method, std::string uri)
{
    HTTPPayload rep;
    rep.isClientPayload = true;
    rep.method = ::strtoupper(method);
    rep.uri = std::string(uri);
    return rep;
}

/* Get a status payload. */

HTTPPayload HTTPPayload::statusPayload(HTTPPayload::StatusType status, const std::string& contentType)
{
    HTTPPayload rep;
    rep.isClientPayload = false;
    rep.status = status;

    if (status != HTTPPayload::NO_CONTENT) {
        rep.content = stock_replies::to_string(status, contentType);
        rep.ensureDefaultHeaders(contentType);
    }

    return rep;
}


/* Helper to attach a host TCP stream reader. */

void HTTPPayload::attachHostHeader(const asio::ip::tcp::endpoint remoteEndpoint)
{
    headers.add("Host", std::string(remoteEndpoint.address().to_string() + ":" + std::to_string(remoteEndpoint.port())));
}

// ---------------------------------------------------------------------------
//  Private Members
// ---------------------------------------------------------------------------

/* Internal helper to ensure the headers are of a default for the given content type. */

void HTTPPayload::ensureDefaultHeaders(const std::string& contentType)
{
    if (!isClientPayload) {
        headers.add("Content-Type", std::string(contentType));
        headers.add("Content-Length", std::to_string(content.size()));
        headers.add("Server", std::string(("DVM/" __VER__)));
    }
    else {
        headers.add("User-Agent", std::string(("DVM/" __VER__)));
        headers.add("Accept", "*/*");
        if (::strtoupper(method) != HTTP_GET) {
            headers.add("Content-Type", std::string(contentType));
            headers.add("Content-Length", std::to_string(content.size()));
        }
    }
}
