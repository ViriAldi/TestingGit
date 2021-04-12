#include <iostream>
#include <deque>
#include <thread>
#include "processor.hpp"


int main(int argc, char *argv[]) {

    if (argc != 2){
        std::cout << "Invalid number of arguments" << std::endl;
        return 1;
    }
    std::string cfg_path = argv[1];

    std::ifstream config_stream(cfg_path);
    if (!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file" << std::endl;
        return 2;
    }
    configuration_t config;
    try {
        config = read_conf(config_stream);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }

    auto total_start = get_current_time_fenced();
    Processor processor(config.indexing_threads, config.merging_threads, config.indir,
                        config.raw_files_queue_size, config.dictionaries_queue_size, config.max_txt_filesize);
    auto result = processor.process();

    DictWriter::write_sorted_by_key(result.final_dict, config.out_by_a);
    DictWriter::write_sorted_by_val(result.final_dict, config.out_by_n);

    auto total_end = get_current_time_fenced();

    std::cout << "Indexing: " << result.indexing_time << " sec" << std::endl;
    std::cout << "Reading: " << result.reading_time << " sec" << std::endl;
    std::cout << "Total: " << double(to_us(total_end - total_start)) / 1000000 << " sec" << std::endl;

    return 0;
}