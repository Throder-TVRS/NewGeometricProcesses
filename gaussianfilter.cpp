#include "gaussianfilter.h"

GaussianFilter::GaussianFilter() {

}

void GaussianFilter::construct_filter() {
    if(last_transform_size == filter_size)
        return;
    H.denumerator = uint64_t(std::pow(2, filter_size - 1));
    H.denumerator *= H.denumerator;
    std::vector<int> first(1, 1);
    std::vector<int> second;
    for(uint64_t i = 1; i < filter_size; i++) {
        second.resize(first.size() + 1);
        second[0] = 1;
        second[i] = 1;
        for(uint64_t j = 1; j < i; j++)
            second[j] = first[j] + first[j - 1];
        first = second;
    }
    H.matrix.resize(second.size());
    for(auto& x : H.matrix)
        x.resize(second.size());
    for(uint64_t i = 0; i < second.size(); i++)
        for(uint64_t j = 0; j < second.size(); j++)
            H.matrix[i][j] = second[i] * second[j];
    last_transform_size = filter_size;
}

uint64_t GaussianFilter::use_H(const QImage &image, uint64_t x, uint64_t y) {
    uint64_t sum = 0;
    for(uint64_t i = 0; i < H.matrix.size(); i++)
        for(uint64_t j = 0; j < H.matrix.size(); j++)
            sum += H.matrix[i][j] * image.pixelColor(x + i, y + j).red();
    sum = std::min(uint64_round(sum , H.denumerator, 127), uint64_t(255));
    return sum;
}

QImage GaussianFilter::apply_filter(const QImage &image) {
    construct_filter();
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
