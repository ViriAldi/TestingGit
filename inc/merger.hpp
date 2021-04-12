//
// Created by virialdi on 10.04.21.
//

#ifndef AKS_LAB3_PART2_MERGER_HPP
#define AKS_LAB3_PART2_MERGER_HPP

#include <string>
#include <map>
#include "safe_queue.hpp"

class Merger {
private:
    SafeQueue<std::map<std::string, int>> *dict_queue;

public:
    explicit Merger(SafeQueue<std::map<std::string, int>> *dict_queue);
    void merge();
};

#endif //AKS_LAB3_PART2_MERGER_HPP
