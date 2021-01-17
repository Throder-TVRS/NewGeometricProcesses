#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H
#include "utility.h"

class MedianFilter {
public:
    MedianFilter();

    uint64_t filter_size = 3;
    std::vector<uint64_t> sorting_vector;

    uint64_t use_H(const QImage& image, uint64_t x, uint64_t y);
    QImage apply_filter(const QImage& image);
};

#endif // MEDIANFILTER_H
