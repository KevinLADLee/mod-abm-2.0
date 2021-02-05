/// \author Jian Wen
/// \date 2021/02/01

#pragma once

#include "config.hpp"
#include "types.hpp"

#include <fstream>
#include <queue>

/// \brief The agent-based modeling platform that simulates the mobility-on-demand systems.
template <typename RouterFunc, typename DemandGeneratorFunc>
class Platform
{
public:
    /// \brief Constructor.
    explicit Platform(
        PlatformConfig _platform_config, 
        RouterFunc _router_func, 
        DemandGeneratorFunc _demand_generator_func,
        std::string path_to_datalog_file = "");

    /// \brief Destructor.
    ~Platform();

    /// \brief Delete the other constructors. Rule of five. 
    Platform(const Platform& other) = delete;
    Platform(Platform&& other) = delete;
    Platform& operator=(const Platform& other) = delete;
    Platform& operator=(Platform&& other) = delete;

    void run_simulation();

    // void invoke_demand_generator()
    // {
    //     // Pos origin{113.93593149478123, 22.312648328005512};
    //     // Pos destination{114.13602296340699, 22.28328541732128};

    //     // auto ret = router_func_(origin, destination);
    // }

private:
    void run_cycle();

    void dispatch();

    void write_to_datalog();

    /// \brief The set of config parameters for the simulation platform.
    PlatformConfig platform_config_;

    /// \brief The router functor which finds the shortest route for an O/D pair on request.
    RouterFunc router_func_;

    /// \brief The demand generator functor which generates trip requests in each cycle.
    DemandGeneratorFunc demand_generator_func_;

    /// \brief The system time starting from 0.0.
    double system_time_s_ = 0.0;

    /// \brief The vector of trips created during the entire simulation process.
    std::vector<Trip> trips_ = {};

    /// \brief The vector of trips created during the entire simulation process.
    std::vector<Vehicle> vehicles_ = {};

    /// \brief The queue of iterators pointing to trips to be dispatched.
    std::queue<size_t> pending_trip_ids_ = {};

    /// \brief The boolean indicating if we output datalog.
    bool if_output_datalog = false;

    /// \brief The ofstream that outputs to the datalog.
    std::ofstream fout_datalog;
};

// Implementation is put in a separate file for clarity and maintainability.
#include "platform_impl.hpp"