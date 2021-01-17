#include "clasterbinarizator.h"

ClasterBinarizator::ClasterBinarizator() {

}

QImage ClasterBinarizator::binarize(const QImage &image) {
    std::vector <uint64_t> intense;
    for(int x = 0; x < image.width(); x++)
        for(int y = 0; y < image.height(); y++)
            intense.push_back(image.pixelColor(x, y).red());

    double first = 64, second = 192, fl = 0, sl = 255;
    std::vector<uint64_t> white, black;
    while(std::abs(first - fl) > 1 && std::abs(second - sl) > 1) {
        white.clear(), black.clear();
        for(const auto& x : intense) {
            if(std::abs(x - first) <= std::abs(x - second))
                black.push_back(x);
            else
                white.push_back(x);
        }
        fl = first, sl = second;
        first = second = 0;
        for(const auto& x : black)
            first += x;
        first /= black.size();
        for(const auto& x : white)
            second += x;
        second /= white.size();
    }
    std::sort(black.begin(), black.end());
    std::sort(white.begin(), white.end());
    if(black.size() != 0)
        first = black.back();
    else
        first = white.front() - 1;
    QImage new_image = image;
    for(uint64_t x = 0; x < new_image.width(); x++)
        for(uint64_t y = 0; y < new_image.height(); y++) {
            if(image.pixelColor(x, y).red() <= first)
                new_image.setPixelColor(x, y, qRgb(0, 0, 0));
            else
                new_image.setPixelColor(x, y, qRgb(255, 255, 255));
        }
    return new_image;
}
