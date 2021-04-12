#include "read_write.hpp"
#include "safe_queue.hpp"


void ArchiveReader::read_files_into_queue(SafeQueue<std::string> &queue, const std::string &dir_path, long long* time) {
    auto read_start = get_current_time_fenced();
    for (auto &p: std::filesystem::recursive_directory_iterator(dir_path)) {
        if (p.path().extension() == ".zip") {
            queue.push(read_file(p.path().c_str()));
        }
    }
    queue.remove_producer();
    auto read_end = get_current_time_fenced();
    *time = to_us(read_end - read_start);
}

std::string ArchiveReader::read_file(const std::string &filePath) {
    std::ifstream raw_file(filePath, std::ios::binary);
    std::ostringstream buffer_ss;
    buffer_ss << raw_file.rdbuf();
    std::string buffer{buffer_ss.str()};
    return buffer;
}

configuration_t read_conf(std::istream &cf) {
    std::ios::fmtflags flags(cf.flags());
    cf.exceptions(std::ifstream::failbit);
    configuration_t res;
    std::string temp;

    try {
        cf >> res.indir;
        getline(cf, temp);
        cf >> res.out_by_a;
        getline(cf, temp);
        cf >> res.out_by_n;
        getline(cf, temp);
        cf >> res.indexing_threads;
        getline(cf, temp);
        cf >> res.merging_threads;
        getline(cf, temp);
        cf >> res.raw_files_queue_size;
        getline(cf, temp);
        cf >> res.dictionaries_queue_size;
        getline(cf, temp);
        cf >> res.max_txt_filesize;
    } catch (std::ios_base::failure &fail) {
        cf.flags(flags);
        throw;
    }
    cf.flags(flags);
    if (res.indexing_threads <= 0) {
        throw std::runtime_error("The number of indexing threads should be >= 0");
    }
    return res;
}

void DictWriter::write_sorted_by_key(const std::map<std::string, int> &dict, const std::string &path) {
    std::ofstream out_by_a(path, std::ios::trunc);
    for (const auto &x: dict) {
        out_by_a << x.first << " " << x.second << std::endl;
    }
}

void DictWriter::write_sorted_by_val(const std::map<std::string, int> &dict, const std::string &path) {
    std::vector<std::pair<std::string, int>> vec(dict.begin(), dict.end());
    std::sort(vec.begin(), vec.end(), DictWriter::cmp);
    std::ofstream out_by_n(path, std::ios::trunc);
    for (const auto &x: vec) {
        out_by_n << x.first << " " << x.second << std::endl;
    }
}

bool DictWriter::cmp(std::pair<std::string, int> &a, std::pair<std::string, int> &b) {
    return a.second > b.second;
}