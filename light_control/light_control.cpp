#include "light_control.h"
#include <pybind11/stl.h>
#include <iostream>
#include <exception>
#include <string>

namespace NiceNice{

LightControl::LightControl(const nlohmann::json& configIn)
:mConfig(configIn)
{
    ParseConfig();
    mPythonVariables["ip"] = mHubIP;
    mPythonVariables["token"] = mHubToken;
    pybind11::exec(R"(
        from miio import DeviceFactory
        import threading
        dev = DeviceFactory.create(ip, token)
    )", pybind11::globals(), mPythonVariables);
    std::cout << "[Light Control] - Imported miio." << std::endl;
}

bool LightControl::ParseConfig(){
    nlohmann::json conf;
    if(mConfig.contains("hub")){
        conf = mConfig["hub"];
        if(conf.contains("ip") && conf.contains("token")){
            mHubIP = conf["ip"].get<std::string>();
            mHubToken = conf["token"].get<std::string>();
            if(conf.contains("hub_switches")){
                conf = conf["hub_switches"];
                for(const auto& item : conf.items()){
                    if(item.value().contains("id") && item.value().contains("channel_0")){
                        std::string id = item.value()["id"].get<std::string>();
                        std::string name = item.value()["channel_0"].get<std::string>();
                        mNameToSwitchIDMap[name] = id;
                        mNameToChannelMap[name] = 0;
                        mSwitchIDToNameMap[id] = {name};
                        if(item.value().contains("channel_1")){
                            name = item.value()["channel_1"].get<std::string>();
                            mNameToSwitchIDMap[name] = id;
                            mNameToChannelMap[name] = 1;
                            mSwitchIDToNameMap[id].push_back(name);
                        }
                        if(item.value().contains("channel_2")){
                            name = item.value()["channel_2"].get<std::string>();
                            mNameToSwitchIDMap[name] = id;
                            mNameToChannelMap[name] = 2;
                            mSwitchIDToNameMap[id].push_back(name);
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool LightControl::ActuateSwitch(const std::string& id, const int channel, const std::string& action){
    mPythonVariables["id"] = id;
    mPythonVariables["chnl"] = "channel_" + std::to_string(channel);
    mPythonVariables["action"] = action;

    pybind11::exec(R"(
        reply = dev.send("toggle_ctrl_neutral", [chnl, action], extra_parameters={"sid": id})[0]
    )", pybind11::globals(), mPythonVariables);
    try{
        return mPythonVariables["reply"].cast<std::string>() == "ok";
    }catch(std::exception& ex){
        std::cout << "[Activate Switch] - Expcetion caught: " << ex.what() << std::endl;
    }
    return false;
}

std::vector<bool> LightControl::ActuateSwitches(const std::vector<std::string>& query_vec, const std::string& cmd){
    std::vector<bool> results;
    results.resize(query_vec.size());
    for(size_t i = 0; i < query_vec.size(); i++){
        const auto& name = query_vec[i];
        const auto& id = mNameToSwitchIDMap[name];
        const auto& chnl = mNameToChannelMap[name];
        results[i] = ActuateSwitch(id, chnl, cmd);
    }
    return results;
}

std::vector<bool> LightControl::OnLights(const std::vector<std::string>& query_vec){
    return ActuateSwitches(query_vec, "on");
}

std::vector<bool> LightControl::OffLights(const std::vector<std::string>& query_vec){
    return ActuateSwitches(query_vec, "off");
}

std::vector<bool> LightControl::ToggleLights(const std::vector<std::string>& query_vec){
    ActuateSwitches(query_vec, "off");
    return ActuateSwitches(query_vec, "on");
}

std::vector<LIGHT_STATUS> LightControl::QuerySwitch(const std::string& id, const std::vector<int>& channel){
    mPythonVariables["id"] = id;
    std::vector<LIGHT_STATUS> response{};
    pybind11::exec(R"(
        reply = dev.send("get_device_prop", [id, "neutral_0", "neutral_1", "neutral_2"])
        neu0 = reply[0]
        neu1 = reply[1]
        neu2 = reply[2]
    )", pybind11::globals(), mPythonVariables);
    try{
        for(const auto& chnl : channel){
            if(chnl == 0){
                response.push_back(mPythonVariables["neu0"].cast<std::string>() == "on"? LIGHT_STATUS::ON : LIGHT_STATUS::OFF);
            }else if(chnl == 1){
                response.push_back(mPythonVariables["neu1"].cast<std::string>() == "on"? LIGHT_STATUS::ON : LIGHT_STATUS::OFF);
            }else if(chnl == 2){
                response.push_back(mPythonVariables["neu2"].cast<std::string>() == "on"? LIGHT_STATUS::ON : LIGHT_STATUS::OFF);
            }
        }
    }catch(std::exception& ex){
        std::cout << "[Query Switch] - Expcetion caught: " << ex.what() << std::endl;
    }
    return response;
}

std::vector<LIGHT_STATUS> LightControl::GetStatus(const std::vector<std::string>& query_vec){
    std::lock_guard<std::mutex> lock(mQueryMapBufferMtx); // Ensure only 1 query at a time.
    std::vector<LIGHT_STATUS> response;
    response.resize(query_vec.size());

    // Trial 1 - Naive approach: 2216ms to query whole house
    // std::vector<int> buffer;
    // buffer.resize(1);
    // for(size_t i=0; i<query_vec.size(); i++){
    //     auto& name = query_vec[i];
    //     buffer[0] = mNameToChannelMap[name];
    //     response.push_back(QuerySwitch(mNameToSwitchIDMap[name], buffer)[0]);
    // }

    // Trial 2 - Construct a Map to multiquery if possible: 1497ms without parallelise
    // Tried to parallelise in python for Trial 3. But miIO library is NOT thread safe.
    // Construct a Map to multiquery if possible: 1497ms without parallelise
    mQueryMapBuffer.clear();
    for(size_t i=0; i<query_vec.size(); i++){
        auto& name = query_vec[i];
        auto& id = mNameToSwitchIDMap[name];
        if(mQueryMapBuffer.find(id) != mQueryMapBuffer.end()){
            mQueryMapBuffer[id] = {-1, -1, -1};
        }
        mQueryMapBuffer[id][mNameToChannelMap[name]] = i;
    }
    for(const auto& iter : mQueryMapBuffer){
        auto& id = iter.first;
        std::vector<int> chnls;
        chnls.reserve(3);
        for(int j=0; j<3; j++){
            if(iter.second[j] != -1){
                chnls.push_back(j);
            }
        }
        auto reply = QuerySwitch(id, chnls);
        int checker = 0;
        for(int j=0; j<3; j++){
            if(iter.second[j] != -1){
                response[iter.second[j]] = reply[checker];
                checker++;
            }
        }
    }

    return response;
}



}