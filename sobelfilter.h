#ifndef SOBELFILTER_H
#define SOBELFILTER_H
#include "utility.h"

class SobelFilter {
public:
    SobelFilter();

    typedef std::vector<std::vector<double>> Filter;

    uint64_t filter_size = 3;
    uint64_t last_transform_size = 0;
    Filter H1;
    Filter H2;

    void construct_filters();
    uint64_t use_H(const QImage& image, uint64_t x, uint64_t y, Filter H);
    QImage apply_filter(const QImage& image);
};

#endif // SOBELFILTER_H
