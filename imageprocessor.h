#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <QColorDialog>
#include <QDialog>
#include <QPushButton>
#include <QPainter>

#include "colorconverter.h"
#include "medianfilter.h"
#include "gaussianfilter.h"
#include "sobelfilter.h"
#include "kirshfilter.h"
#include "embosser.h"
#include "clasterbinarizator.h"

class ColorConverter;
class MedianFilter;
class GaussianFilter;
class SobelFilter;
class KirshFilter;
class Embosser;
class ClasterBinarizator;

class ImageProcessor {
public:
    ImageProcessor(ColorConverter *color_converter,
                   MedianFilter *median_filter,
                   GaussianFilter *gaussian_filter,
                   SobelFilter *sobel_filter,
                   KirshFilter *kirsch_filter,
                   Embosser *embosser,
                   ClasterBinarizator *claster_binarizator);

    QString _filepath;

    QImage _default_size_source_image;
    QImage _source_image;
    QImage _processed_image;

    bool default_size_status = true;
    bool source_status = false;

    QImage _hystogram;
    const uint64_t _preview_label_size = 711;

    ColorConverter *_color_converter;
    MedianFilter *_median_filter;
    GaussianFilter *_gaussian_filter;
    SobelFilter *_sobel_filter;
    KirshFilter *_kirsch_filter;
    Embosser *_embosser;
    ClasterBinarizator *_claster_binarizator;

    void load_image(const QString& filepath);
    void save_image();
    void calculate_hystogram();

    //
    int32_t get_angle();
};

#endif // IMAGEPROCESSOR_H
