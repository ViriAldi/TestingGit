//
// Created by virialdi on 09.04.21.
//

#ifndef AKS_LAB3_PART2_INDEXER_HPP
#define AKS_LAB3_PART2_INDEXER_HPP

#include <map>
#include <string>
#include <mutex>
#include <boost/locale.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <thread>
#include "read_write.hpp"
#include "time_measure.hpp"
#include "safe_queue.hpp"

class Indexer {
private:
    int max_txt_size;
    std::locale loc = boost::locale::generator().generate("en_US.UTF-8");
    SafeQueue<std::string> *read_queue;
    SafeQueue<std::map<std::string, int>> *dict_queue;

public:
    Indexer(SafeQueue<std::string> *read_queue, SafeQueue<std::map<std::string, int>> *dict_queue, int max_txt_filesize){
        this->read_queue = read_queue;
        this->dict_queue = dict_queue;
        this->max_txt_size = max_txt_filesize;
    }
    void index();
    std::map<std::string, int> arch_indexer(const std::string &binArch);
    void processText(std::string &text, std::map<std::string, int> &dict);

};

#endif //AKS_LAB3_PART2_INDEXER_HPP
