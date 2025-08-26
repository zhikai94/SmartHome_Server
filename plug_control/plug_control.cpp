#include "plug_control.h"
#include <pybind11/stl.h>
#include <iostream>
#include <exception>
#include <string>

namespace NiceNice{

PlugControl::PlugControl(const nlohmann::json& configIn)
:mConfig(configIn)
{
    ParseConfig();
    pybind11::exec(R"(
        from miio import DeviceFactory
        plugs = dict()
    )", pybind11::globals(), mPythonVariables);

    for(const auto& plug : mPlugMap){
        mPythonVariables["plug_ip"] = plug.second.ip;
        mPythonVariables["plug_token"] = plug.second.token;
        mPythonVariables["plug_name"] = plug.first;
        pybind11::exec(R"(
            plugs[plug_name] = DeviceFactory.create(plug_ip, plug_token)
        )", pybind11::globals(), mPythonVariables);
    }
    std::cout << "[Plug Control] - Imported miio and created devices." << std::endl;
}

bool PlugControl::ParseConfig(){
    for(const auto& item : mConfig.items()){
        if(item.value().contains("ip") && item.value().contains("token")){
            DeviceInfo curr_device(item.value().at("ip").get<std::string>(), item.value().at("token").get<std::string>());
            mPlugMap[item.key()] = curr_device;
        }
    }
    return true;
}

PLUG_STATUS PlugControl::QueryPlug(const std::string& id){
    mPythonVariables["queried_plug"] = id;
    pybind11::exec(R"(
        plug_state = plugs[queried_plug].get_property_by(2, 1)[0]['value']
    )", pybind11::globals(), mPythonVariables);
    if(mPythonVariables["plug_state"].cast<bool>()){
        return PLUG_STATUS::ON;
    }
    return PLUG_STATUS::OFF;
}

std::vector<PLUG_STATUS> PlugControl::GetStatus(const std::vector<std::string>& query_vec){
    std::vector<PLUG_STATUS> statusOut{};
    statusOut.reserve(query_vec.size());
    for(const auto& query : query_vec){
        statusOut.push_back(QueryPlug(query));
    }
    return statusOut;
}

bool PlugControl::SetPlug(const std::string& id, const bool isOn){
    mPythonVariables["queried_plug"] = id;
    mPythonVariables["set_plug"] = isOn;
    pybind11::exec(R"(
        plugs[queried_plug].set_property_by(2, 1, set_plug)
    )", pybind11::globals(), mPythonVariables);
    return true;
}

std::vector<bool> PlugControl::SetPlugs(const std::vector<std::string>& query_vec, const bool isOn){
    std::vector<bool> resultOut{};
    resultOut.reserve(query_vec.size());
    for(const auto& query : query_vec){
        resultOut.push_back(SetPlug(query, isOn));
    }
    return resultOut;
}

bool PlugControl::TogglePlug(const std::string& id){
    mPythonVariables["queried_plug"] = id;
    pybind11::exec(R"(
        plugs[queried_plug].call_action_by(2, 1)
    )", pybind11::globals(), mPythonVariables);
    return true;
}

std::vector<bool> PlugControl::TogglePlugs(const std::vector<std::string>& query_vec){
    std::vector<bool> resultOut{};
    resultOut.reserve(query_vec.size());
    for(const auto& query : query_vec){
        resultOut.push_back(TogglePlug(query));
    }
    return resultOut;
}

}