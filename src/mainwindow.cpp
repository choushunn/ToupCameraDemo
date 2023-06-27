#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    appInit = new AppInit(ui);
    appEvent = new AppEvent(this);
    m_timer = new QTimer();

}

/**
 * @brief 析构函数，结束程序时
 * @param
 */
MainWindow::~MainWindow() {
    delete ui;
    delete appInit;
    delete appEvent;
}


//读取图像槽函数
void MainWindow::readFrame() {
    cv::Mat image;
    appInit->camera->read(image);
    if (!image.empty()) {
        this->showFrame(image);
    }
}

/**
 * @brief 打开相机信号槽
 * @param
 */
void MainWindow::on_m_btn_open_camera_clicked(bool checked) {
    if (checked) {
        ui->m_btn_open_camera->setText("关闭");
        m_timer->setInterval(int(1000 / fps));
        appInit->camera->open();
        m_timer->start();
        connect(m_timer, &QTimer::timeout, this, &MainWindow::readFrame);
        ui->m_cbx_camera_list->setDisabled(true);
        ui->m_cbx_camera_type->setDisabled(true);

    } else {
        appInit->camera->close();
        ui->m_btn_open_camera->setText("打开");
        ui->m_lbl_display1->clear();
        ui->m_lbl_display2->clear();
        ui->m_cbx_camera_list->setDisabled(false);
        ui->m_cbx_camera_type->setDisabled(false);
    }
}

/**
* @brief 显示frame
* @param frame
*/
void MainWindow::showFrame(cv::Mat frame) {
    cv::resize(frame, frame, cv::Size(640, 480));
    QSize size = ui->m_lbl_display1->size();
    QImage qimage1(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    QPixmap pixmap1 = QPixmap::fromImage(qimage1);
    pixmap1 = pixmap1.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display1->setPixmap(pixmap1);
    cv::Mat output_image = frame.clone();

    // 初始化输出图像
    processor.process(output_image);

    cv::resize(output_image, output_image, cv::Size(640, 480));
    QImage qimage(output_image.data, output_image.cols, output_image.rows, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimage);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display2->setPixmap(pixmap);
}


void MainWindow::on_exit_action_triggered() {
    int ret = QMessageBox::warning(this, "退出", "是否退出程序", QMessageBox::Ok, QMessageBox::Cancel);
    switch (ret) {
        case QMessageBox::Ok:
            qDebug() << "退出程序";
            QApplication::quit();
            break;
        case QMessageBox::Cancel:
            qDebug() << "取消退出程序";
            break;
        default:
            break;
    }

}

/**
 * @brief 打开新窗口
 * @param
 */
void MainWindow::on_pushButton_clicked() {
    //    创建一个窗口对象
    Dialog dialog(this);
    //    调用显示窗口的函数
    dialog.exec();
}


void MainWindow::on_action_2_triggered() {
    QString path = QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this, "选择一个文件", path, "图像文件 (*.jpg *.png *.bmp)");

    // 如果选择了文件，则加载图像文件并显示在标签控件中
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        ui->m_lbl_display1->setPixmap(pixmap);
    }
}


void MainWindow::on_action_7_triggered() {
    QMessageBox::about(this, "关于", "作者: Spring.");
}


void MainWindow::on_pushButton_5_clicked(bool checked)
{
    if (checked) {
        processor.addHandler(thr);
    } else {
        processor.removeHandler(thr);
    }
}


void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
    if(thr){
        thr->setRotation(value);
    }else{
    }
}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1) {
        processor.addHandler(hflip);
    } else {
        processor.removeHandler(hflip);
    }
}


void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if (arg1) {
        processor.addHandler(vflip);
    } else {
        processor.removeHandler(vflip);
    }
}


void MainWindow::on_pushButton_6_clicked(bool checked)
{
    if (checked) {
        processor.addHandler(bgr2rgb);
    } else {
        processor.removeHandler(bgr2rgb);
    }
}


void MainWindow::on_pushButton_7_clicked(bool checked)
{
    if (checked) {
        processor.addHandler(cah);
    } else {
        processor.removeHandler(cah);
    }
}


void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    if(cah){
        cah->setHue(value);
        qDebug() << value;
    }
}


void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    if(cah){
        cah->setSaturation(value);
    }
}


void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    if(cah){
        cah->setContrast(value);
    }
}


void MainWindow::on_pushButton_7_clicked()
{

}

