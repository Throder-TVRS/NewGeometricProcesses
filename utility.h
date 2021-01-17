#ifndef UTILITY_H
#define UTILITY_H
#include "mainwindow.h"
#include <algorithm>
#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <iostream>
#include <cmath>
#include <algorithm>

uint64_t uint64_round(uint64_t a, uint64_t b, uint64_t lim);
QImage scale_image(const QImage& image, int width, int height);
QImage expand_image(const QImage& image, int n);
QImage rotate_image(const QImage& image, double angle);

#endif // UTILITY_H
