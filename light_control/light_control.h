#pragma once
#include <vector>
#include <array>
#include <nlohmann/json.hpp>
#include <pybind11/embed.h>
#include <unordered_map>
#include <mutex>

namespace NiceNice{

enum class LIGHT_STATUS{
    OFF = 0,
    ON = 1,
    UNKNOWN = 2,
};

// Attribute visibility prevents warning on class being more visible than python interpreter
class LightControl{
public:
    LightControl(const nlohmann::json& configIn);

    // Get status of a vector of named lights.
    std::vector<LIGHT_STATUS> GetStatus(const std::vector<std::string>& query_vec);

    // On a vector of named lights. Return nlohmann::json of status when trying to on light.
    std::vector<bool> OnLights(const std::vector<std::string>& query_vec);
    std::vector<bool> OffLights(const std::vector<std::string>& query_vec);

    // On a vector of named lights. Return nlohmann::json of status when trying to do the on action when toggling light.
    std::vector<bool> ToggleLights(const std::vector<std::string>& query_vec);
    
    
    private:
    bool ParseConfig();
    std::vector<LIGHT_STATUS> QuerySwitch(const std::string& id, const std::vector<int>& channel);
    std::vector<bool> ActuateSwitches(const std::vector<std::string>& query_vec, const std::string& cmd);
    bool ActuateSwitch(const std::string& id, const int channel, const std::string& action);

private:
    nlohmann::json mConfig;
    std::string mHubIP;
    std::string mHubToken;
    // pybind11::scoped_interpreter mPythonInterpreter;
    pybind11::dict mPythonVariables;
    std::unordered_map<std::string, std::string> mNameToSwitchIDMap;
    std::unordered_map<std::string, int> mNameToChannelMap;
    std::unordered_map<std::string, std::vector<std::string>> mSwitchIDToNameMap;
    std::mutex mQueryMapBufferMtx;
    std::unordered_map<std::string, std::array<int, 3>> mQueryMapBuffer;
};


}