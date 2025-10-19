#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
#endif

#include <cpr/cpr.h>
#include "HttpClient.h"

namespace Core::Http {

    HttpResponse Client::Get(const std::string& url,
        const std::unordered_map<std::string, std::string>& headers) {
        cpr::Header cprHeaders;
        for (const auto& [key, value] : headers) {
            cprHeaders[key] = value;
        }

        auto response = cpr::Get(cpr::Url{ url }, cprHeaders);
        return HttpResponse{ response.status_code, response.text };
    }
}