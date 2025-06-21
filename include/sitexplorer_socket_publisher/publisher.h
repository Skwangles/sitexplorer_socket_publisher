#pragma once

#include "campus_virtual_socket_publisher/data_serializer.h"
#include "campus_virtual_socket_publisher/socket_client.h"

#include <mutex>
#include <memory>

namespace stella_vslam {

class config;
class system;

namespace publish {
class frame_publisher;
class map_publisher;
} // namespace publish

} // namespace stella_vslam

namespace campus_virtual_socket_publisher {

class publisher {
public:
    publisher(const YAML::Node& yaml_node,
              const std::shared_ptr<stella_vslam::system>& system,
              const std::shared_ptr<stella_vslam::publish::frame_publisher>& frame_publisher,
              const std::shared_ptr<stella_vslam::publish::map_publisher>& map_publisher);

    void run();

    /* thread controls */
    void request_pause();
    bool is_paused();
    void resume();
    void request_terminate();
    bool is_terminated();
    
    void reset_map();
    void is_reset_map();

private:
    const std::shared_ptr<stella_vslam::system> system_;
    const unsigned int emitting_interval_;
    const unsigned int image_quality_;

    std::unique_ptr<socket_client> client_;
    std::unique_ptr<data_serializer> data_serializer_;

    void callback(const std::string& message);

    /* thread controls */
    bool pause_if_requested();

    bool terminate_is_requested();
    void terminate();

    std::mutex mtx_terminate_;
    bool terminate_is_requested_ = false;
    bool is_terminated_ = true;

    std::mutex mtx_pause_;
    bool pause_is_requested_ = false;
    bool is_paused_ = true;
    bool is_resend_requested_ = false;
};

} // namespace socket_publisher
