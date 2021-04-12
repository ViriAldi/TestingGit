//
// Created by virialdi on 09.04.21.
//

#include "merger.hpp"

Merger::Merger(SafeQueue<std::map<std::string, int>> *dict_queue) {
    this->dict_queue = dict_queue;
}

void Merger::merge() {
    while (true){
        auto dct1 = dict_queue->pop();
        if (dct1.find("a") != dct1.end() && dct1["a"] == -1){
            dict_queue->push(dct1);
            continue;
        }
        auto dct2 = dict_queue->pop();
        if (dct2.find("a") != dct2.end() && dct2["a"] == -1){
            if (dict_queue->empty()) {
                dict_queue->push(dct1);
                dict_queue->push(dct2);
                break;
            }
            else{
                dict_queue->push(dct1);
                dict_queue->push(dct2);
                continue;
            }
        }
        for (auto [k, v]: dct2){
            dct1[k] += v;
        }
        dict_queue->push(dct1);
    }
};
