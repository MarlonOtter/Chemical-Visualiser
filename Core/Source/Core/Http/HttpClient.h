#pragma once
#include <string>
#include <unordered_map>

namespace Core::Http {
    struct HttpResponse {
        int statusCode;
        std::string body;
    };

    class Client {
    public:
        static HttpResponse Get(const std::string& url,
            const std::unordered_map<std::string, std::string>& headers = {});
    };
}
