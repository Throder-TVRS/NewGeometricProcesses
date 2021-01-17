#include "colorconverter.h"

ColorConverter::ColorConverter() {

}

QImage ColorConverter::gray_scale_image(const QImage& image) {
    int width = image.width();
    int height = image.height();
    QRgb pixelColor;
    int red, green, blue;
    QImage _gray_scaled_image = image.scaled(width, height);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            pixelColor = image.pixel(x, y);
            red = qRed(pixelColor);
            green = qGreen(pixelColor);
            blue = qBlue(pixelColor);
            _gray_scaled_image.setPixel(x, y, normalize_color(red, green, blue));
        }
    }
    return _gray_scaled_image;
}

QRgb ColorConverter::normalize_color(int red, int green, int blue) {
    uint64_t divisor = _red_percentage + _green_percentage + _blue_percentage;
    if(divisor == 0)
        return qRgb(0, 0, 0);
    uint64_t new_color = _red_percentage * red + _green_percentage * green + _blue_percentage * blue;
    new_color = uint64_round(new_color, divisor, 50);
    new_color = std::min(new_color, uint64_t(255));
    return qRgb(new_color, new_color, new_color);
}
