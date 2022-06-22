#include "bag_handler.h"

namespace trunk {
namespace perception {
namespace lidar {

/**
 * @brief  BagHandler init
 * @note   read toml file(topic + bagfile)
 *         open bag, query topic, add view to topic_map
 * @retval true for success
 */
bool BagHandler::init(const toml::table& tbl) {
    //// read toml
    toml::node_view topics = tbl["local_file"]["topic"];
    toml::node_view files = tbl["local_file"]["bag"];

    main_topic_ = tbl["local_file"]["main_topic"].value<std::string>().value();  // main topic init
    std::shared_ptr<rosbag::View> ptr_bag = std::make_shared<rosbag::View>();
    main_topic_map_[main_topic_] = ptr_bag;  // main_topic_map init

    for (int j = 0; j < topics.as_array()->size(); ++j) {  // for each topic
        auto topic = topics[j].value<std::string>().value();
        LOG(INFO) << "topic: " << topic;
        addTopic(topic);
        std::shared_ptr<rosbag::View> ptr_bag = std::make_shared<rosbag::View>();
        topic_map_[topic] = ptr_bag;  // topic_map_ init
    }

    for (int j = 0; j < files.as_array()->size(); ++j) {  // for each file
        auto file = files[j].value<std::string>().value();
        LOG(INFO) << "file: " << file;
        addFile(file);
    }

    //// open bag, check topic, add to view
    rosbag::Bag bag;
    for (auto file = files_.begin(); file != files_.end(); file++) {
        bag.open(file->data(), rosbag::bagmode::Read);
        if (bag.isOpen()) {
            main_topic_map_.at(main_topic_)->addQuery(bag, rosbag::TopicQuery(main_topic_));  // main_topic addQuery
            LOG(INFO) << "main topic: " << main_topic_ << "\t file: " << file->data();
            for (auto topic = topics_.begin(); topic != topics_.end(); topic++) {  // other topic addQuery
                topic_map_.at(topic->data())->addQuery(bag, rosbag::TopicQuery(topic->data()));
                LOG(INFO) << "topic: " << topic->data() << "\t file: " << file->data();
            }
        } else {
            LOG(ERROR) << "open bag error: " << file->data();
        }
        bag.close();
    }

    for (auto& man_msg : main_topic_map_.at(main_topic)) {
    }


    return true;
}

/**
 * @brief Traversal main_topic, find other topic msg
 *
 * @return true for success
 */
bool BagHandler::process() {
    // topic_map_.at().for (auto& msg : view) {
    //     msg.getTopic();
    //     msg.getTime();

    //     if (at128.size() > min_num) {
    //         continue;
    //     }

    //     // tf
    //     tf_bag = topics_map_["tf"];
    //     frame_data.tf = tf_bag->query(time, data);

    //     // region
    //     region_bag = topics_map_["region"];
    //     frame_data.region = region_bag->query(time, data);

    //     // lidar_perception->set_frame(frame_data) lidar_perception->process()
    // }
    LOG(INFO) << "bag process ";


    return true;
}
}  // namespace lidar
}  // namespace perception
}  // namespace trunk