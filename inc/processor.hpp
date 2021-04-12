//
// Created by virialdi on 09.04.21.
//

#ifndef AKS_LAB3_PART2_PROCESSOR_HPP
#define AKS_LAB3_PART2_PROCESSOR_HPP

#include <string>
#include <map>
#include "safe_queue.hpp"
#include "indexer.hpp"
#include "read_write.hpp"

struct indexing_result {
    std::map<std::string, int> final_dict;
    double indexing_time;
    double reading_time;
};

class Processor {
private:
    int idx_thr;
    int mrg_thr;
    int max_txt_filesize;
    std::string path;
    SafeQueue<std::string> read_queue;
    SafeQueue<std::map<std::string, int>> dict_queue;
public:
    Processor(int indexing_threads, int merging_threads, const std::string &dir_path, int raw_files_queue_size, int dictionaries_queue_size, int max_txt_filesize);
    indexing_result process();
};

#endif //AKS_LAB3_PART2_PROCESSOR_HPP
