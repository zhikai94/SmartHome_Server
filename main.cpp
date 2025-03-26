
#include <iostream>
#include <fstream>
#include <memory>
#include <App.h>
#include <nlohmann/json.hpp>
#include "file_paths.h"
#include "light_control.h"
#include <chrono>

int main(void){
    // Read Config
    std::ifstream config_file(get_config_path());
    nlohmann::json config = nlohmann::json::parse(config_file);
    if( !(config.contains("lights")&&config.contains("curtains")) ){
        std::cout << "Config file is corrupted! Please check." << std::endl;
        return 0;
    }

    // Spawn up Light Controller
    auto light_ptr = std::make_shared<NiceNice::LightControl>( config["lights"] );
    std::vector<std::string> all_lights = {"Balcony Right", "Storeroom", "Master Bedroom Toilet", "Wet Utility", "Balcony Left", "Guest Toilet", "Master Bedroom Balcony", "Guest Bedroom",
                                           "Shoe Cabinet", "Entrance", "Lazy Corner", "Display Shelf", "Living Room", "Study", "Kitchen Pendant", "Kitchen Spotlight", "Dining Spotlight", "Dining Pendant",
                                           "Corridor", "Master Bedroom Spotlight", "Master Bedroom Pendant", "Master Bedroom Study"};

    // light_ptr->OffLights({"Display Shelf", "Study"});
    // light_ptr->OnLights({"Study"});
    // auto start = std::chrono::steady_clock::now();
    // std::vector<NiceNice::LIGHT_STATUS> res = light_ptr->GetStatus({"Balcony Right", "Storeroom", "Master Bedroom Toilet", "Wet Utility", "Balcony Left", "Guest Toilet", "Master Bedroom Balcony", "Drying Rack", "Guest Bedroom",
    //                                 "Shoe Cabinet", "Entrance", "Lazy Corner", "Display Shelf", "Living Room", "Study", "Kitchen Pendant", "Kitchen Spotlight", "Dining Spotlight", "Dining Pendant",
    //                                 "Corridor", "Master Bedroom Spotlight", "Master Bedroom Pendant", "Master Bedroom Study"});
    
    // std::cout << "Query took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << " ms" << std::endl;
    
    // Rest Server
    std::ifstream ifs(get_webpage_folder() + "main.html");
    std::string sample_html( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );

    int port = 3000;
    uWS::SocketContextOptions ssl_options = {};
    ssl_options.passphrase = "5965";
    ssl_options.cert_file_name = "/home/zhikai/Code/cert/domain.crt";
    ssl_options.key_file_name = "/home/zhikai/Code/cert/domain.key";
    ssl_options.dh_params_file_name = "/home/zhikai/Code/cert/dhparam.pem";

    // Either serve over HTTP or HTTPS
    uWS::SocketContextOptions empty_ssl_options = {};
    {
        // HTTPS
        uWS::SSLApp(ssl_options).get("/hello", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /hello call over HTTPS." << std::endl;
            res->end(sample_html);
        }).get("/open_day_study", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_day_study call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.82 --token cacb555f28ff0a6dd18aae2d03584840 set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_day_study", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_day_study call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.82 --token cacb555f28ff0a6dd18aae2d03584840 set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_study", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_study call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.88 --token b87d282bf2d9e3c42659aa26e4ae3be0 set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_study", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_study call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.88 --token b87d282bf2d9e3c42659aa26e4ae3be0 set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_balcony", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_balcony call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.74 --token af6f8f42ef6d1f4316236d1ac8594ed6 set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_balcony", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_balcony call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.74 --token af6f8f42ef6d1f4316236d1ac8594ed6 set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_day_balcony", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_day_balcony call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.185 --token 23269f72e84d810a0cf3deeb3e176150 set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_day_balcony", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_day_balcony call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.185 --token 23269f72e84d810a0cf3deeb3e176150 set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_master", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_master call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.144 --token 6f384c7d6af596d31d5c50f30a0b965f set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_master", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_master call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.144 --token 6f384c7d6af596d31d5c50f30a0b965f set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_day_master", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_day_master call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.206 --token b4b0d7b033c9b413ba21c2fe49da401f set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_day_master", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_day_master call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.206 --token b4b0d7b033c9b413ba21c2fe49da401f set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_guest", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_guest call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.86 --token 0fbff2c74768f124477f867f008cc54f set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_guest", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_guest call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.86 --token 0fbff2c74768f124477f867f008cc54f set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/open_day_guest", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /open_day_guest call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.69 --token 4d36a186d058106d1687dbdc6dadd1b7 set curtain_target_position_2_5 100");
            res->end(sample_html);
        }).get("/close_day_guest", [sample_html](auto *res, auto *req) {
            std::cout << "Responding to a /close_day_guest call over HTTPS." << std::endl;
            system("miiocli genericmiot --ip 192.168.50.69 --token 4d36a186d058106d1687dbdc6dadd1b7 set curtain_target_position_2_5 0");
            res->end(sample_html);
        }).get("/on_study", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_study call over HTTPS." << std::endl;
            light_ptr->OnLights({"Study"});
            res->end(sample_html);
        }).get("/off_study", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_study call over HTTPS." << std::endl;
            light_ptr->OffLights({"Study"});
            res->end(sample_html);
        }).get("/toggle_study", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_study call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Study"});
            res->end(sample_html);
        }).get("/on_living_room", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_living_room call over HTTPS." << std::endl;
            light_ptr->OnLights({"Living Room"});
            res->end(sample_html);
        }).get("/off_living_room", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_living_room call over HTTPS." << std::endl;
            light_ptr->OffLights({"Living Room"});
            res->end(sample_html);
        }).get("/toggle_living_room", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_living_room call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Living Room"});
            res->end(sample_html);
        }).get("/on_shoe_cabinet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_shoe_cabinet call over HTTPS." << std::endl;
            light_ptr->OnLights({"Shoe Cabinet"});
            res->end(sample_html);
        }).get("/off_shoe_cabinet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_shoe_cabinet call over HTTPS." << std::endl;
            light_ptr->OffLights({"Shoe Cabinet"});
            res->end(sample_html);
        }).get("/toggle_shoe_cabinet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_shoe_cabinet call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Shoe Cabinet"});
            res->end(sample_html);
        }).get("/on_entrance", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_entrance call over HTTPS." << std::endl;
            light_ptr->OnLights({"Entrance"});
            res->end(sample_html);
        }).get("/off_entrance", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_entrance call over HTTPS." << std::endl;
            light_ptr->OffLights({"Entrance"});
            res->end(sample_html);
        }).get("/toggle_entrance", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_entrance call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Entrance"});
            res->end(sample_html);
        }).get("/on_lazy_corner", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_lazy_corner call over HTTPS." << std::endl;
            light_ptr->OnLights({"Lazy Corner"});
            res->end(sample_html);
        }).get("/off_lazy_corner", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_lazy_corner call over HTTPS." << std::endl;
            light_ptr->OffLights({"Lazy Corner"});
            res->end(sample_html);
        }).get("/toggle_lazy_corner", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_lazy_corner call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Lazy Corner"});
            res->end(sample_html);
        }).get("/on_display_shelf", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_display_shelf call over HTTPS." << std::endl;
            light_ptr->OnLights({"Display Shelf"});
            res->end(sample_html);
        }).get("/off_display_shelf", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_display_shelf call over HTTPS." << std::endl;
            light_ptr->OffLights({"Display Shelf"});
            res->end(sample_html);
        }).get("/toggle_display_shelf", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_display_shelf call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Display Shelf"});
            res->end(sample_html);
        }).get("/on_master_bedroom_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_master_bedroom_spotlight call over HTTPS." << std::endl;
            light_ptr->OnLights({"Master Bedroom Spotlight"});
            res->end(sample_html);
        }).get("/off_master_bedroom_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_master_bedroom_spotlight call over HTTPS." << std::endl;
            light_ptr->OffLights({"Master Bedroom Spotlight"});
            res->end(sample_html);
        }).get("/toggle_master_bedroom_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_master_bedroom_spotlight call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Master Bedroom Spotlight"});
            res->end(sample_html);
        }).get("/on_master_bedroom_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_master_bedroom_pendant call over HTTPS." << std::endl;
            light_ptr->OnLights({"Master Bedroom Pendant"});
            res->end(sample_html);
        }).get("/off_master_bedroom_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_master_bedroom_pendant call over HTTPS." << std::endl;
            light_ptr->OffLights({"Master Bedroom Pendant"});
            res->end(sample_html);
        }).get("/toggle_master_bedroom_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_master_bedroom_pendant call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Master Bedroom Pendant"});
            res->end(sample_html);
        }).get("/on_master_bedroom_study", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_master_bedroom_study call over HTTPS." << std::endl;
            light_ptr->OnLights({"Master Bedroom Study"});
            res->end(sample_html);
        }).get("/off_master_bedroom_study", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_master_bedroom_study call over HTTPS." << std::endl;
            light_ptr->OffLights({"Master Bedroom Study"});
            res->end(sample_html);
        }).get("/toggle_master_bedroom_study", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_master_bedroom_study call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Master Bedroom Study"});
            res->end(sample_html);
        }).get("/on_master_bedroom_toilet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_master_bedroom_toilet call over HTTPS." << std::endl;
            light_ptr->OnLights({"Master Bedroom Toilet"});
            res->end(sample_html);
        }).get("/off_master_bedroom_toilet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_master_bedroom_toilet call over HTTPS." << std::endl;
            light_ptr->OffLights({"Master Bedroom Toilet"});
            res->end(sample_html);
        }).get("/toggle_master_bedroom_toilet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_master_bedroom_toilet call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Master Bedroom Toilet"});
            res->end(sample_html);
        }).get("/on_master_bedroom_balcony", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_master_bedroom_balcony call over HTTPS." << std::endl;
            light_ptr->OnLights({"Master Bedroom Balcony"});
            res->end(sample_html);
        }).get("/off_master_bedroom_balcony", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_master_bedroom_balcony call over HTTPS." << std::endl;
            light_ptr->OffLights({"Master Bedroom Balcony"});
            res->end(sample_html);
        }).get("/toggle_master_bedroom_balcony", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_master_bedroom_balcony call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Master Bedroom Balcony"});
            res->end(sample_html);
        }).get("/on_kitchen_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_kitchen_pendant call over HTTPS." << std::endl;
            light_ptr->OnLights({"Kitchen Pendant"});
            res->end(sample_html);
        }).get("/off_kitchen_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_kitchen_pendant call over HTTPS." << std::endl;
            light_ptr->OffLights({"Kitchen Pendant"});
            res->end(sample_html);
        }).get("/toggle_kitchen_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_kitchen_pendant call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Kitchen Pendant"});
            res->end(sample_html);
        }).get("/on_kitchen_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_kitchen_spotlight call over HTTPS." << std::endl;
            light_ptr->OnLights({"Kitchen Spotlight"});
            res->end(sample_html);
        }).get("/off_kitchen_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_kitchen_spotlight call over HTTPS." << std::endl;
            light_ptr->OffLights({"Kitchen Spotlight"});
            res->end(sample_html);
        }).get("/toggle_kitchen_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_kitchen_spotlight call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Kitchen Spotlight"});
            res->end(sample_html);
        }).get("/on_dining_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_dining_spotlight call over HTTPS." << std::endl;
            light_ptr->OnLights({"Dining Spotlight"});
            res->end(sample_html);
        }).get("/off_dining_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_dining_spotlight call over HTTPS." << std::endl;
            light_ptr->OffLights({"Dining Spotlight"});
            res->end(sample_html);
        }).get("/toggle_dining_spotlight", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_dining_spotlight call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Dining Spotlight"});
            res->end(sample_html);
        }).get("/on_dining_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_dining_pendant call over HTTPS." << std::endl;
            light_ptr->OnLights({"Dining Pendant"});
            res->end(sample_html);
        }).get("/off_dining_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_dining_pendant call over HTTPS." << std::endl;
            light_ptr->OffLights({"Dining Pendant"});
            res->end(sample_html);
        }).get("/toggle_dining_pendant", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_dining_pendant call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Dining Pendant"});
            res->end(sample_html);
        }).get("/on_corridor", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_corridor call over HTTPS." << std::endl;
            light_ptr->OnLights({"Corridor"});
            res->end(sample_html);
        }).get("/off_corridor", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_corridor call over HTTPS." << std::endl;
            light_ptr->OffLights({"Corridor"});
            res->end(sample_html);
        }).get("/toggle_corridor", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_corridor call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Corridor"});
            res->end(sample_html);
        }).get("/on_balcony_right", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_balcony_right call over HTTPS." << std::endl;
            light_ptr->OnLights({"Balcony Right"});
            res->end(sample_html);
        }).get("/off_balcony_right", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_balcony_right call over HTTPS." << std::endl;
            light_ptr->OffLights({"Balcony Right"});
            res->end(sample_html);
        }).get("/toggle_balcony_right", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_balcony_right call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Balcony Right"});
            res->end(sample_html);
        }).get("/on_balcony_left", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_balcony_left call over HTTPS." << std::endl;
            light_ptr->OnLights({"Balcony Left"});
            res->end(sample_html);
        }).get("/off_balcony_left", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_balcony_left call over HTTPS." << std::endl;
            light_ptr->OffLights({"Balcony Left"});
            res->end(sample_html);
        }).get("/toggle_balcony_left", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_balcony_left call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Balcony Left"});
            res->end(sample_html);
        }).get("/on_storeroom", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_storeroom call over HTTPS." << std::endl;
            light_ptr->OnLights({"Storeroom"});
            res->end(sample_html);
        }).get("/off_storeroom", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_storeroom call over HTTPS." << std::endl;
            light_ptr->OffLights({"Storeroom"});
            res->end(sample_html);
        }).get("/toggle_storeroom", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_storeroom call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Storeroom"});
            res->end(sample_html);
        }).get("/on_wet_utility", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_wet_utility call over HTTPS." << std::endl;
            light_ptr->OnLights({"Wet Utility"});
            res->end(sample_html);
        }).get("/off_wet_utility", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_wet_utility call over HTTPS." << std::endl;
            light_ptr->OffLights({"Wet Utility"});
            res->end(sample_html);
        }).get("/toggle_wet_utility", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_wet_utility call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Wet Utility"});
            res->end(sample_html);
        }).get("/on_guest_bedroom", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_guest_bedroom call over HTTPS." << std::endl;
            light_ptr->OnLights({"Guest Bedroom"});
            res->end(sample_html);
        }).get("/off_guest_bedroom", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_guest_bedroom call over HTTPS." << std::endl;
            light_ptr->OffLights({"Guest Bedroom"});
            res->end(sample_html);
        }).get("/toggle_guest_bedroom", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_guest_bedroom call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Guest Bedroom"});
            res->end(sample_html);
        }).get("/on_guest_toilet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /on_guest_toilet call over HTTPS." << std::endl;
            light_ptr->OnLights({"Guest Toilet"});
            res->end(sample_html);
        }).get("/off_guest_toilet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /off_guest_toilet call over HTTPS." << std::endl;
            light_ptr->OffLights({"Guest Toilet"});
            res->end(sample_html);
        }).get("/toggle_guest_toilet", [sample_html, light_ptr](auto *res, auto *req) {
            std::cout << "Responding to a /toggle_guest_toilet call over HTTPS." << std::endl;
            light_ptr->ToggleLights({"Guest Toilet"});
            res->end(sample_html);
        }).get("/off_all_lights", [sample_html, light_ptr, &all_lights](auto *res, auto *req) {
            std::cout << "Responding to a /off_all_lights call over HTTPS." << std::endl;
            light_ptr->OffLights(all_lights);
            res->end(sample_html);
        }).listen(port, [port](auto *token) {
            if (token) {
                std::cout << "Serving over HTTPS at " << port << std::endl;
            }
        }).run();
    }

    std::cout << "Failed to listen to port " << port << std::endl;

    std::cout << "--X--" << std::endl;
    return 0;
}