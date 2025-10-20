#pragma once

#include <string>
#include "Chemical.h"

#include <future>

namespace ChemVis::PubChem
{
	std::string Get3D(std::string name);
	std::string Get2D(std::string name);
	std::string GetAutoComplete(std::string inp);
	std::vector<std::string> _ParseAutoComplete(std::string data);

    // singleton as it doesn't store any data
	class Async
    {
    public:
        static std::future <Chemical> GetChemical(std::string name) {
        return std::async(std::launch::async, [name]() -> Chemical {
            auto future3D = std::async(std::launch::async, Get3D, name);
            auto future2D = std::async(std::launch::async, Get2D, name);

            std::string result3D = future3D.get();
            std::string result2D = future2D.get();

            auto optionalChem = Chemical::Parse(Merge2Dand3D(result2D, result3D));
            if (optionalChem.has_value())
            {
                return optionalChem.value();
            }
            return {};
            });
    }

    static std::future<std::vector<std::string>> GetAutoComplete(std::string input) {
        return std::async(std::launch::async, [input]() -> std::vector<std::string> {
            auto futureAuto = std::async(std::launch::async, PubChem::GetAutoComplete, input);
            std::string autoResult = futureAuto.get();
            return _ParseAutoComplete(autoResult);
            });
    }

    template <typename T>
    static bool isFutureReady(std::future<T>& future) {
        return future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
    }
};
}