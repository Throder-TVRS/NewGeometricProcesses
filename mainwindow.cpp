#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent, ImageProcessor *processor)
    : QMainWindow(parent)
    , _processor(processor)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update_image() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    if(!_processor->default_size_status)
        ui->preview_label->setPixmap(QPixmap::fromImage(_processor->_processed_image));
    else
        ui->preview_label->setPixmap(QPixmap::fromImage(scale_image(_processor->_processed_image, _processor->_preview_label_size, _processor->_preview_label_size)));
    ui->hystogram_label->setPixmap(QPixmap::fromImage(_processor->_hystogram));
}

void MainWindow::update_image(const QImage& image) {
    if(_processor->_filepath.isEmpty() ||
      !_processor->source_status)
        return;
    ui->preview_label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_load_image_button_clicked() {
    QString new_filepath = QFileDialog::getOpenFileName(0, "File selection", "", "*.jpg *.png *.bmp");
    if(new_filepath.isEmpty() || _processor->_filepath == new_filepath)
        return;
    _processor->load_image(new_filepath);
    if(_processor->default_size_status)
        _processor->_processed_image = _processor->_color_converter->gray_scale_image(_processor->_default_size_source_image);
    else
        _processor->_processed_image = _processor->_color_converter->gray_scale_image(_processor->_source_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_source_flag_box_stateChanged(int arg1) {
    if(arg1 == Qt::CheckState::Checked)
        _processor->source_status = true;
    if(arg1 == Qt::CheckState::Unchecked)
        _processor->source_status = false;
    if(_processor->source_status)
        update_image(_processor->_source_image);
    update_image();
}

void MainWindow::on_reset_button_clicked() {
    if(_processor->source_status)
        return;
    if(_processor->default_size_status)
        _processor->_processed_image = _processor->_color_converter->gray_scale_image(_processor->_default_size_source_image);
    else
        _processor->_processed_image = _processor->_color_converter->gray_scale_image(_processor->_source_image);
    _processor->calculate_hystogram();
    update_image();
    _processor->_kirsch_filter->cool = false;
    _processor->_embosser->last_filter = 0;
}

void MainWindow::on_mdian_use_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_median_filter->apply_filter(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_median_size_box_valueChanged(int arg1) {
    _processor->_median_filter->filter_size = arg1;
}

void MainWindow::on_gaussian_use_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_gaussian_filter->apply_filter(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_gaussian_size_box_valueChanged(int arg1) {
    _processor->_gaussian_filter->filter_size = arg1;
}

void MainWindow::on_save_image_button_clicked() {
    _processor->save_image();
}

void MainWindow::on_minimize_size_flag_box_stateChanged(int arg1) {
    if(arg1 == Qt::CheckState::Checked) {
        _processor->default_size_status = true;
        if(!_processor->_source_image.isNull())
            _processor->_processed_image = _processor->_color_converter->gray_scale_image(_processor->_source_image);
    }
    if(arg1 == Qt::CheckState::Unchecked) {
        _processor->source_status = false;
        if(!_processor->_default_size_source_image.isNull())
            _processor->_processed_image = _processor->_color_converter->gray_scale_image(_processor->_default_size_source_image);
    }
    update_image();
}

void MainWindow::on_sobel_use_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_sobel_filter->apply_filter(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_sobel_size_box_valueChanged(int arg1) {
    _processor->_sobel_filter->filter_size = arg1;
}

void MainWindow::on_kirsch_use_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_kirsch_filter->apply_filter(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_kirsch_size_box_valueChanged(int arg1) {
    _processor->_kirsch_filter->filter_size = arg1;
}

void MainWindow::on_embosser_brightness_slider_valueChanged(int value) {
    _processor->_embosser->additional_brightness = value;
    ui->embosser_brightness_lcd->display(value);
    if(_processor->_embosser->last_filter == 0)
        return;
    if(_processor->_embosser->last_filter == 1) {
        on_reset_button_clicked();
        on_embosser_indention_use_button_clicked();
    }
    else {
        on_reset_button_clicked();
        on_embosser_extrusion_use_button_clicked();
    }
}

void MainWindow::on_embosser_indention_use_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_embosser->apply_indention_filter(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_embosser_extrusion_use_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_embosser->apply_extrusion_filter(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_binarize_button_clicked() {
    if(_processor->_filepath.isEmpty() ||
       _processor->source_status)
        return;
    _processor->_processed_image = _processor->_claster_binarizator->binarize(_processor->_processed_image);
    _processor->calculate_hystogram();
    update_image();
}

void MainWindow::on_centrate_button_clicked() {
    if(_processor->_filepath.isEmpty())
        return;
    int32_t angle = _processor->get_angle();
    _processor->_processed_image = rotate_image(_processor->_processed_image, angle);
    update_image();
}

void MainWindow::on_rotate_on_180_button_clicked() {
    _processor->_processed_image = rotate_image(_processor->_processed_image, 180);
    update_image();
}

void MainWindow::on_rotate_button_clicked() {
    _processor->_processed_image = rotate_image(_processor->_processed_image, ui->rotate_spinbox->value());
    update_image();
}
