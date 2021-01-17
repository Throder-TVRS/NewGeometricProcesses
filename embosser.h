#ifndef EMBOSSER_H
#define EMBOSSER_H
#include "utility.h"

class Embosser {
public:
    Embosser();

    typedef std::vector<std::vector<int64_t>> Filter;

    Filter indent, extruder;
    uint64_t additional_brightness = 128;
    int last_filter = 0;

    uint64_t use_indent(const QImage& image, uint64_t x, uint64_t y);
    uint64_t use_extruder(const QImage& image, uint64_t x, uint64_t y);
    QImage apply_indention_filter(const QImage& image);
    QImage apply_extrusion_filter(const QImage& image);
};

#endif // EMBOSSER_H
