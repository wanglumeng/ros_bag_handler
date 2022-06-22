#pragma once

#include <glog/logging.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <string>

#include "common/toml.hpp"

class BagHandler {
   public:
    static BagHandler &GetInstance() {
        static BagHandler instance;
        return instance;
    }

    bool init(const toml::table &tbl);
    bool process();

   private:
    void addTopic(std::string topic) { topics_.push_back(topic); }
    void addFile(std::string file) { files_.push_back(file); }

    std::string main_topic_;
    std::vector<std::string> topics_;
    std::vector<std::string> files_;
    std::map<std::string, std::shared_ptr<rosbag::View>> main_topic_map_;
    std::map<std::string, std::shared_ptr<rosbag::View>> topic_map_;

    BagHandler() = default;
    ~BagHandler() = default;
    BagHandler &operator=(const BagHandler &);
};
