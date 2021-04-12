//
// Created by virialdi on 09.04.21.
//

#include "processor.hpp"
#include "merger.hpp"


Processor::Processor(int indexing_threads, int merging_threads, const std::string &dir_path, int raw_files_queue_size, int dictionaries_queue_size, int max_txt_size):
        read_queue{raw_files_queue_size},
        dict_queue{dictionaries_queue_size}
        {
            idx_thr = indexing_threads;
            path = dir_path;
            mrg_thr = merging_threads;
            max_txt_filesize = max_txt_size;
        }

indexing_result Processor::process() {
    std::vector<std::thread> threads;
    Indexer indexer(&read_queue, &dict_queue, max_txt_filesize);
    Merger merger(&dict_queue);

    auto idx_start = get_current_time_fenced();
    long long reading_time;
    read_queue.add_producer();
    threads.emplace_back(ArchiveReader::read_files_into_queue, std::ref(read_queue), std::ref(path), &reading_time);

    for (int i = 0; i < idx_thr; i++){
        dict_queue.add_producer();
        threads.emplace_back(&Indexer::index, indexer);
    }
    for (int i = 0; i < mrg_thr; i++){
        threads.emplace_back(&Merger::merge, merger);
    }

    for (auto &thr: threads){
        thr.join();
    }
    auto idx_end = get_current_time_fenced();

    indexing_result result{dict_queue.pop(),
                           double(to_us(idx_end - idx_start)) / 1000000,
                           double(reading_time) / 1000000};

    return result;
};