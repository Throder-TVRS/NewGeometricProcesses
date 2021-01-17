#include "imageprocessor.h"

ImageProcessor::ImageProcessor(ColorConverter *color_converter,
                               MedianFilter *median_filter,
                               GaussianFilter *gaussian_filter,
                               SobelFilter *sobel_filter,
                               KirshFilter *kirsch_filter,
                               Embosser *embosser,
                               ClasterBinarizator *claster_binarizator)
    : _color_converter(color_converter)
    , _median_filter(median_filter)
    , _gaussian_filter(gaussian_filter)
    , _sobel_filter(sobel_filter)
    , _kirsch_filter(kirsch_filter)
    , _embosser(embosser)
    , _claster_binarizator(claster_binarizator) {

    _hystogram = QImage(256, 280, QImage::Format_RGB32);
}

void ImageProcessor::load_image(const QString& filepath) {
    _filepath = filepath;
    _default_size_source_image = _source_image = _processed_image = QImage(_filepath);
    _source_image = scale_image(_source_image, _preview_label_size, _preview_label_size);
}

void ImageProcessor::save_image() {
    QString fileName = QFileDialog::getSaveFileName();
    if (fileName.isEmpty())
        return;
    _processed_image.save(fileName);
}

void ImageProcessor::calculate_hystogram() {
    uint64_t h_max = 0, k;
    std::vector<uint64_t> h(256, 0);
    for(int y = 0; y < _processed_image.height(); y++) {
        for(int x = 0; x < _processed_image.width(); x++) {
            k = _processed_image.pixelColor(x, y).red();
            h[k]++;
        }
    }

    for(const auto& x : h)
        h_max = std::max(h_max, x);
    for(auto& x : h) {
        x *= 280;
        x += h_max - 1;
        x /= h_max;
    }
    for(uint64_t x = 0; x < 256; x++) {
        for(uint64_t y = 0; y < 280 - h[x]; y++)
            _hystogram.setPixel(x, y, qRgb(255, 0, 0));
        for(int y = 280 - h[x]; y < 280; y++)
            _hystogram.setPixel(x, y, qRgb(x, x, x));
    }
}


double get_metric(const QImage& image) {
    uint64_t whites = image.height();
    for(uint64_t y = 0; y < image.height(); y++) {
        for(uint64_t x = 0; x < image.width(); x++) {
            if(image.pixelColor(x, y) != Qt::GlobalColor::white) {
                whites--;
                break;
            }
        }
    }
    return double(whites);
}

int32_t ImageProcessor::get_angle() {
    double best_metric = 0, buf_metric = 0, angle = 0;
    QImage image = _claster_binarizator->binarize(_processed_image);
    for(int ang = -90; ang <= 90; ang++) {
        buf_metric = get_metric(rotate_image(image, ang));
        if(buf_metric > best_metric) {
            best_metric = buf_metric;
            angle = ang;
        }
    }
    return angle;
}


