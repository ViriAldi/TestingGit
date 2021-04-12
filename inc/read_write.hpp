//
// Created by virialdi on 09.04.21.
//

#ifndef AKS_LAB3_PART2_READ_WRITE_HPP
#define AKS_LAB3_PART2_READ_WRITE_HPP

#include <map>
#include <algorithm>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include "time_measure.hpp"
#include "safe_queue.hpp"

struct configuration_t {
    std::string indir;
    std::string out_by_a;
    std::string out_by_n;
    int indexing_threads;
    int merging_threads;
    int max_txt_filesize;
    int raw_files_queue_size;
    int dictionaries_queue_size;
};
configuration_t read_conf(std::istream &cf);

class ArchiveReader{
public:
    static void read_files_into_queue(SafeQueue<std::string> &queue, const std::string &dir_path, long long* time);
    static std::string read_file(const std::string &filePath);
};

class DictWriter{
private:
    static bool cmp(std::pair<std::string, int>& a, std::pair<std::string, int>& b);
public:
    static void write_sorted_by_key(const std::map<std::string, int> &dict, const std::string &path);
    static void write_sorted_by_val(const std::map<std::string, int> &dict, const std::string &path);
};

#endif //AKS_LAB3_PART2_READ_WRITE_HPP