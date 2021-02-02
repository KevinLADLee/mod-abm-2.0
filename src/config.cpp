/// \author Jian Wen
/// \date 2021/02/01

#include "config.hpp"

#include <fmt/format.h>

PlatformConfig load_platform_config(const std::string &path_to_platform_config)
{
    auto platform_config_yaml = YAML::LoadFile(path_to_platform_config);

    PlatformConfig platform_config;

    platform_config.area_config.lon_min = platform_config_yaml["area_config"]["lon_min"].as<double>();
    platform_config.area_config.lon_max = platform_config_yaml["area_config"]["lon_max"].as<double>();
    platform_config.area_config.lat_min = platform_config_yaml["area_config"]["lat_min"].as<double>();
    platform_config.area_config.lat_max = platform_config_yaml["area_config"]["lat_max"].as<double>();

    platform_config.mod_system_config.fleet_config.fleet_size = platform_config_yaml["mod_system_config"]["fleet_config"]["fleet_size"].as<size_t>();
    platform_config.mod_system_config.fleet_config.veh_capacity = platform_config_yaml["mod_system_config"]["fleet_config"]["veh_capacity"].as<size_t>();

    platform_config.mod_system_config.request_config.walkaway_time_s = platform_config_yaml["mod_system_config"]["request_config"]["walkaway_time_s"].as<double>();

    platform_config.simulation_config.cycle_s = platform_config_yaml["simulation_config"]["cycle_s"].as<double>();
    platform_config.simulation_config.simulation_duration_s = platform_config_yaml["simulation_config"]["simulation_duration_s"].as<double>();
    platform_config.simulation_config.warmup_duration_s = platform_config_yaml["simulation_config"]["warmup_duration_s"].as<double>();
    platform_config.simulation_config.winddown_duration_s = platform_config_yaml["simulation_config"]["winddown_duration_s"].as<double>();

    platform_config.path_to_osrm_data = platform_config_yaml["path_to_osrm_data"].as<std::string>();

    fmt::print("[INFO] Loaded the platform configuration yaml file from {}.\n", path_to_platform_config);

    return platform_config;
}