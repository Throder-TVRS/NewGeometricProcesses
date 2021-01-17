#include "sobelfilter.h"

SobelFilter::SobelFilter() {

}

void SobelFilter::construct_filters() {
    if(last_transform_size == filter_size)
        return;
    H1.clear();
    H2.clear();
    H1.resize(filter_size);
    H2.resize(filter_size);
    for(uint64_t i = 0; i < filter_size; i++) {
        H1[i].resize(filter_size);
        H2[i].resize(filter_size);
    }
    for(int i = 0; i < filter_size; i++) {
        int i_shift = i - filter_size / 2;
        for(int j = 0; j < filter_size; j++) {
            int j_shift = j - filter_size / 2;
            if(i == filter_size / 2 && j == filter_size / 2) {
                H1[i][j] = 0;
                H2[i][j] = 0;
                continue;
            }
            H1[i][j] = double(i_shift) / ((i_shift) * (i_shift) + (j_shift) * (j_shift));
            H2[i][j] = double(j_shift) / ((i_shift) * (i_shift) + (j_shift) * (j_shift));
        }
    }
    last_transform_size = filter_size;
}

uint64_t SobelFilter::use_H(const QImage &image, uint64_t x, uint64_t y, Filter H) {
    double sum = 0;
    for(uint64_t i = 0; i < H.size(); i++)
        for(uint64_t j = 0; j < H.size(); j++)
            sum += H[i][j] * image.pixelColor(x + i, y + j).red();
    sum = std::abs(sum);
    return uint64_t(sum);
}

QImage SobelFilter::apply_filter(const QImage &image) {
    construct_filters();
    QImage exp_image = expand_image(image, filter_size);
    QImage new_image = QImage(image.size(), QImage::Format_RGB32);
    uint64_t color1, color2;
    for(uint64_t x = 0; x + filter_size - 1 < uint64_t(exp_image.width()); x++)
        for(uint64_t y = 0; y + filter_size - 1 < uint64_t(exp_image.height()); y++) {
            color1 = use_H(exp_image, x, y, H1);
            color2 = use_H(exp_image, x, y, H2);
            color1 = std::max(color1, color2);
            color1 = std::min(color1, uint64_t(255));
            new_image.setPixelColor(x, y, qRgb(color1, color1, color1));
        }
    return new_image;
}
