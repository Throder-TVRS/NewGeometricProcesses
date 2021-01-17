#include "embosser.h"

Embosser::Embosser() {
    indent.resize(3);
    extruder.resize(3);
    for(int i = 0; i < 3; i++) {
        indent[i].resize(3);
        extruder[i].resize(3);
    }
    indent[0][1] = indent[1][2] = -1;
    indent[1][0] = indent[2][1] = 1;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            extruder[i][j] = (-1)*indent[i][j];
}

uint64_t Embosser::use_indent(const QImage &image, uint64_t x, uint64_t y) {
    uint64_t sum = 0;
    for(uint64_t i = 0; i < indent.size(); i++)
        for(uint64_t j = 0; j < indent.size(); j++)
            sum += indent[i][j] * image.pixelColor(x + i, y + j).red();
    return sum;
}

uint64_t Embosser::use_extruder(const QImage &image, uint64_t x, uint64_t y) {
    uint64_t sum = 0;
    for(uint64_t i = 0; i < extruder.size(); i++)
        for(uint64_t j = 0; j < extruder.size(); j++)
            sum += extruder[i][j] * image.pixelColor(x + i, y + j).red();
    return sum;
}

QImage Embosser::apply_indention_filter(const QImage &image) {
    QImage exp_image = expand_image(image, 3);
    QImage new_image = QImage(image.size(), QImage::Format_RGB32);
    uint64_t color;
    for(uint64_t x = 0; x + 3 - 1 < uint64_t(exp_image.width()); x++)
        for(uint64_t y = 0; y + 3 - 1 < uint64_t(exp_image.height()); y++) {
            color = use_indent(exp_image, x, y);
            color = std::min(uint64_t(255), color + additional_brightness);
            new_image.setPixelColor(x, y, qRgb(color, color, color));
        }
    last_filter = 1;
    return new_image;
}

QImage Embosser::apply_extrusion_filter(const QImage &image) {
    QImage exp_image = expand_image(image, 3);
    QImage new_image = QImage(image.size(), QImage::Format_RGB32);
    uint64_t color;
    for(uint64_t x = 0; x + 3 - 1 < uint64_t(exp_image.width()); x++)
        for(uint64_t y = 0; y + 3 - 1 < uint64_t(exp_image.height()); y++) {
            color = use_extruder(exp_image, x, y);
            color = std::min(uint64_t(255), color + additional_brightness);
            new_image.setPixelColor(x, y, qRgb(color, color, color));
        }
    last_filter = 2;
    return new_image;
}

