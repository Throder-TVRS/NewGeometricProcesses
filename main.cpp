#include "mainwindow.h"
#include "imageprocessor.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ColorConverter converter;
    MedianFilter median_filter;
    GaussianFilter gaussian_filter;
    SobelFilter sobel_filter;
    KirshFilter kirsch_filter;
    Embosser embosser;
    ClasterBinarizator claster_binarizator;
    ImageProcessor processor(&converter,
                             &median_filter,
                             &gaussian_filter,
                             &sobel_filter,
                             &kirsch_filter,
                             &embosser,
                             &claster_binarizator);
    MainWindow w(nullptr, &processor);
    w.setFixedSize(w.size());
    w.setWindowTitle("Image processor");
    w.show();
    return a.exec();
}
