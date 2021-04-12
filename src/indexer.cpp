//
// Created by virialdi on 09.04.21.
//

#include "indexer.hpp"


void Indexer::processText(std::string &text, std::map<std::string, int> &dict) {
    text = boost::locale::normalize(text, boost::locale::norm_nfc, loc);
    text = boost::locale::fold_case(text, loc);
    boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, text.begin(), text.end(), loc);
    map.rule(boost::locale::boundary::word_any);
    for (auto it = map.begin(), e = map.end(); it != e; ++it)
        ++(dict)[*it];
}

void Indexer::index() {
    while (!read_queue->end()){
        auto arch = read_queue->pop();
        auto dict = arch_indexer(arch);
        dict_queue->push(dict);
    }
    dict_queue->remove_producer();
    if (dict_queue->no_more()) {
        std::map<std::string, int> pill{{"a", -1}};
        dict_queue->push(pill);
    }
}

std::map<std::string, int> Indexer::arch_indexer(const std::string &binArch) {
    std::map<std::string, int> dict;
    struct archive_entry *entry;
    struct archive *a = archive_read_new();

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    int r = archive_read_open_memory(a, &binArch[0], binArch.size());

    if (r != ARCHIVE_OK){
        throw std::invalid_argument("Bad archive");
    }

    int filenum = 0;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK && filenum < 1) {
        filenum++;
        auto filesize = archive_entry_size(entry);
        if (filesize >= max_txt_size) {
            continue;
        }
        std::string text(filesize, char{});
        auto read_filesize = archive_read_data(a, &text[0], text.size());
        if (filesize != read_filesize) {
            break;
        }
        processText(text, dict);
    }
    archive_read_free(a);
    return dict;
}
