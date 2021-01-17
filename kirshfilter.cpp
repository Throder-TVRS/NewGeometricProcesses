#include "kirshfilter.h"

KirshFilter::KirshFilter() {

}

void KirshFilter::construct_filters() {
    if(last_transform_size == filter_size)
        return;
    H.clear();
    coofs.clear();
    int center = filter_size / 2;
    H.resize(filter_size);
    for(uint64_t i = 0; i < filter_size; i++)
        H[i].resize(filter_size);
    coofs.resize(center + 1);

    coofs[0] = 0;
    coofs[1] = 5;
    for(uint64_t i = 2; i < coofs.size(); i++)
        coofs[i] = coofs[i - 1] + 4;

    for(int i = 1; i <= center; i++) {
        for(int x = center - i; x <= center + i; x++) {
            H[center - i][x] = coofs[i];
            H[center + i][x] = coofs[i] - 8 * i;
        }
        for(int y = center - i + 1; y < center; y++)
            H[y][center - i] = H[y][center + i] = coofs[i];
        for(int y = center; y < center + i; y++)
            H[y][center - i] = H[y][center + i] = coofs[i] - 8 * i;
    }

    last_transform_size = filter_size;
}

int64_t KirshFilter::rotation(int64_t sum, CoordType& coords, const QImage& image, uint64_t x, uint64_t y) {
    int center = filter_size / 2;
    for(int i = 1; i < coords.size(); i++) {
        int _x = coords[i].first.second;
        int _y = coords[i].first.first;
        int k = i;
        while(k) {
            k--;
            sum -= H[_x][_y] * image.pixelColor(x + _x, y + _y).red();
            H[_x][_y] = coofs[i] - 8*i;
            sum += H[_x][_y] * image.pixelColor(x + _x, y + _y).red();
            if(_x < center + i && _y == center - i)
                _x++;
            else if(_x == center + i && _y < center + i)
                _y++;
            else if(_x > center - i && _y == center + i)
                _x--;
            else if(_x == center - i && _y > center - i)
                _y--;
        }
        coords[i].first.second = _x;
        coords[i].first.first = _y;
        _x = coords[i].second.second;
        _y = coords[i].second.first;
        k = i;
        while(k) {
            k--;
            if(_x < center + i && _y == center - i)
                _x++;
            else if(_x == center + i && _y < center + i)
                _y++;
            else if(_x > center - i && _y == center + i)
                _x--;
            else if(_x == center - i && _y > center - i)
                _y--;
            sum -= H[_x][_y] * image.pixelColor(x + _x, y + _y).red();
            H[_x][_y] = coofs[i];
            sum += H[_x][_y] * image.pixelColor(x + _x, y + _y).red();
        }
        coords[i].second.second = _x;
        coords[i].second.first = _y;
    }
    return sum;
}

uint64_t KirshFilter::use_H(const QImage &image, uint64_t x, uint64_t y) {
    CoordType coords;
    int center = filter_size / 2;
    coords.resize(center + 1);
    for(uint64_t i = 1; i < coords.size(); i++) {
        coords[i].first = {center - 1, center - i};
        coords[i].second = {center - 1, center + i};
    }

    int64_t sum = 0;
    for(uint64_t i = 0; i < H.size(); i++)
        for(uint64_t j = 0; j < H.size(); j++)
            sum += H[i][j] * image.pixelColor(x + i, y + j).red();
    int64_t buf_sum = sum;
    sum = std::abs(sum);
    for(int i = 0; i < 8; i++) {
        buf_sum = rotation(buf_sum, coords, image, x, y);
        sum = std::max(sum, std::abs(buf_sum));
    }
    cool = true;
    coords.clear();
    return uint64_t(std::min(std::abs(sum), int64_t(255)));
}

QImage KirshFilter::apply_filter(const QImage &image) {
    construct_filters();
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
