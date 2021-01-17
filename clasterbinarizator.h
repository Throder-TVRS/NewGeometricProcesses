#ifndef CLASTERBINARIZATOR_H
#define CLASTERBINARIZATOR_H
#include "utility.h"

class ClasterBinarizator {
public:
    ClasterBinarizator();

    QImage binarize(const QImage& image);
};

#endif // CLASTERBINARIZATOR_H
