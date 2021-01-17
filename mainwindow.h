#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QPainter>
#include "imageprocessor.h"
class ImageProcessor;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    ImageProcessor *_processor;

public:
    MainWindow(QWidget *parent = nullptr, ImageProcessor *processor = nullptr);
    ~MainWindow();

    void update_image();
    void update_image(const QImage& image);
private slots:
    void on_load_image_button_clicked();
    void on_source_flag_box_stateChanged(int arg1);
    void on_reset_button_clicked();
    void on_mdian_use_button_clicked();
    void on_median_size_box_valueChanged(int arg1);
    void on_gaussian_use_button_clicked();
    void on_gaussian_size_box_valueChanged(int arg1);
    void on_save_image_button_clicked();
    void on_minimize_size_flag_box_stateChanged(int arg1);
    void on_sobel_use_button_clicked();
    void on_sobel_size_box_valueChanged(int arg1);
    void on_kirsch_use_button_clicked();
    void on_kirsch_size_box_valueChanged(int arg1);
    void on_embosser_brightness_slider_valueChanged(int value);
    void on_embosser_indention_use_button_clicked();

    void on_embosser_extrusion_use_button_clicked();

    void on_binarize_button_clicked();

    void on_centrate_button_clicked();

    void on_rotate_on_180_button_clicked();

    void on_rotate_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
