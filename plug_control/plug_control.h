#pragma once
#include <vector>
#include <array>
#include <nlohmann/json.hpp>
#include <pybind11/embed.h>
#include <unordered_map>
#include <mutex>

namespace NiceNice{

enum class PLUG_STATUS{
    OFF = 0,
    ON = 1,
    UNKNOWN = 2,
};

struct DeviceInfo{
    std::string ip = "";
    std::string token = "";
};

// Attribute visibility prevents warning on class being more visible than python interpreter
class PlugControl{
public:
    PlugControl(const nlohmann::json& configIn);

    // Get status of a vector of named lights.
    std::vector<PLUG_STATUS> GetStatus(const std::vector<std::string>& query_vec);

    // On a vector of named lights. Return nlohmann::json of status when trying to on light.
    std::vector<bool> SetPlugs(const std::vector<std::string>& query_vec, const bool isOn);
    std::vector<bool> TogglePlugs(const std::vector<std::string>& query_vec);
    
    private:
    bool ParseConfig();
    PLUG_STATUS QueryPlug(const std::string& id);
    bool SetPlug(const std::string& id, const bool isOn);
    bool TogglePlug(const std::string& id);

private:
    nlohmann::json mConfig;
    // pybind11::scoped_interpreter mPythonInterpreter;
    pybind11::dict mPythonVariables;
    std::unordered_map<std::string, DeviceInfo> mPlugMap;
};


}