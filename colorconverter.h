#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H
#include "utility.h"

class ColorConverter {
public:
    ColorConverter();

    uint64_t _red_percentage = 30;
    uint64_t _green_percentage = 59;
    uint64_t _blue_percentage = 11;

    QImage gray_scale_image(const QImage& image);
    QRgb normalize_color(int red, int green, int blue);
};

#endif // COLORCONVERTER_H
