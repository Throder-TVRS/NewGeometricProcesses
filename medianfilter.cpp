#include "medianfilter.h"

MedianFilter::MedianFilter() {

}

uint64_t MedianFilter::use_H(const QImage &image, uint64_t x, uint64_t y) {
    sorting_vector.clear();
    sorting_vector.reserve(filter_size * filter_size);
    for(uint64_t i = 0; i < filter_size; i++)
        for(uint64_t j = 0; j < filter_size; j++)
            sorting_vector.push_back(image.pixelColor(x + i, y + i).red());
    std::nth_element(sorting_vector.begin(), sorting_vector.begin() + (sorting_vector.size() / 2), sorting_vector.end());
    return sorting_vector[sorting_vector.size() / 2];
}

QImage MedianFilter::apply_filter(const QImage &image) {
    QImage exp_image = expand_image(image, filter_size);
    QImage new_image = QImage(image.size(), QImage::Format_RGB32);
    uint64_t color;
    for(uint64_t x = 0; x + filter_size - 1 < uint64_t(exp_image.width()); x++)
        for(uint64_t y = 0; y + filter_size - 1 < uint64_t(exp_image.height()); y++) {
            color = use_H(exp_image, x, y);
            new_image.setPixelColor(x, y, qRgb(color, color, color));
        }
    return new_image;
}
