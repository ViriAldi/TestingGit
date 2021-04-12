//
// Created by virialdi on 09.04.21.
//

#ifndef AKS_LAB3_PART2_TIME_MEASURE_HPP
#define AKS_LAB3_PART2_TIME_MEASURE_HPP

#include <chrono>
#include <atomic>

std::chrono::high_resolution_clock::time_point get_current_time_fenced();

template<class D>
long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

#endif //AKS_LAB3_PART2_TIME_MEASURE_HPP
