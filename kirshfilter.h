#ifndef KIRSHFILTER_H
#define KIRSHFILTER_H
#include "utility.h"

class KirshFilter {
public:
    KirshFilter();

    typedef std::vector<std::vector<int64_t>> Filter;
    typedef std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> CoordType;

    uint64_t filter_size = 3;
    uint64_t last_transform_size = 1;
    Filter H;
\
    std::vector <int> coofs;

    bool cool = false;

    void construct_filters();
    int64_t rotation(int64_t sum, CoordType& coords, const QImage& image, uint64_t x, uint64_t y);
    uint64_t use_H(const QImage& image, uint64_t x, uint64_t y);
    QImage apply_filter(const QImage& image);
};

#endif // KIRSHFILTER_H
