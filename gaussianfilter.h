#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H
#include "utility.h"

class GaussianFilter {
public:
    GaussianFilter();

    struct Filter {
        std::vector<std::vector<int>> matrix;
        int denumerator;
    };
    uint64_t filter_size = 3;
    uint64_t last_transform_size = 1;
    Filter H;

    void construct_filter();
    uint64_t use_H(const QImage& image, uint64_t x, uint64_t y);
    QImage apply_filter(const QImage& image);
};

#endif // GAUSSIANFILTER_H
