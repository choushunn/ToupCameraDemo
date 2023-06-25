#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    appInit = new AppInit(ui);
    appEvent = new AppEvent(this);
    m_timer = new QTimer();
}

// 创建FrameProcessing视频流处理对象
//    frameProcessing = new FrameProcessing(this);

//连接 MainWindow的sendFrame信号和VideoProcessing的processFrame槽函数
//    connect(this, &MainWindow::sendFrame, frameProcessing, &FrameProcessing::processFrame);


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
        //        appInit->m_cthread->start();
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
* @brief 显示QImage
* @param image    接收到的QImage
*/
void MainWindow::showFrame(cv::Mat frame) {
    cv::resize(frame, frame, cv::Size(640, 480));
    QSize size = ui->m_lbl_display1->size();
    QImage qimage1(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    QPixmap pixmap1 = QPixmap::fromImage(qimage1);
    pixmap1 = pixmap1.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display1->setPixmap(pixmap1);
    cv::Mat output_image = frame.clone();
    appEvent->processFrame(output_image);
            // 初始化输出图像


            cv::resize(output_image, output_image, cv::Size(640, 480));
            QImage qimage(output_image.data, output_image.cols, output_image.rows, QImage::Format_RGB888);
            QPixmap pixmap = QPixmap::fromImage(qimage);
            pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);
            ui->m_lbl_display2->setPixmap(pixmap);

}


/**
 * @brief 信号槽，灰度化
 * @param
 */
void MainWindow::on_m_btn_graypro_clicked(bool checked) {
    qDebug() << ui->m_btn_Bchannel->isChecked();
    if (ui->m_btn_Bchannel->isChecked()) {
        ui->m_btn_Bchannel->setChecked(false);
        if (appEvent->m_eventQueue.contains(BEvent)) {
            appEvent->m_eventQueue.removeAll((BEvent));
            ui->m_btn_Bchannel->setChecked(false);
        }
        qDebug() << "ischecked";
        return;
    }
    if (checked) {
        if (!appEvent->m_eventQueue.contains(GrayEvent)) {
            appEvent->m_eventQueue.append(GrayEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(GrayEvent)) {
            appEvent->m_eventQueue.removeAll(GrayEvent);
        }
    }
}


/**
 * @brief 信号槽，水平翻转
 * @param
 */
void MainWindow::on_m_btn_hflip_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(FlipEvent)) {
            appEvent->m_eventQueue.append(FlipEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(FlipEvent)) {
            appEvent->m_eventQueue.removeAll(FlipEvent);
        }
    }
}

/**
 * @brief 信号槽，灰度直方图
 * @param
 */
void MainWindow::on_m_btn_Hist_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(HistEvent)) {
            appEvent->m_eventQueue.append(HistEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(HistEvent)) {
            appEvent->m_eventQueue.removeAll(HistEvent);
        }
    }
}


/**
 * @brief 信号槽，rgb直方图
 * @param
 */
void MainWindow::on_m_btn_rgbHist_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(rgbHistEvent)) {
            appEvent->m_eventQueue.append(rgbHistEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(rgbHistEvent)) {
            appEvent->m_eventQueue.removeAll(rgbHistEvent);
        }
    }
}


/**
 * @brief 信号槽，自适应维纳滤波
 * @param
 */
//void MainWindow::on_m_btn_AWF_clicked(bool checked)
//{
//    if (checked) {
//        if(!appEvent->m_eventQueue.contains(AWFHistEvent)){
//            appEvent->m_eventQueue.append(AWFHistEvent);
//        }
//    } else {
//        if(appEvent->m_eventQueue.contains(AWFHistEvent)){
//            appEvent->m_eventQueue.removeAll(AWFHistEvent);
//        }
//    }
//}

/**
 * @brief 信号槽，HSV转换
 * @param
 */
void MainWindow::on_m_btn_hsvpro_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(HSVEvent)) {
            appEvent->m_eventQueue.append(HSVEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(HSVEvent)) {
            appEvent->m_eventQueue.removeAll(HSVEvent);
        }
    }
}

/**
 * @brief 信号槽，蓝色通道显示
 * @param
 */
void MainWindow::on_m_btn_Bchannel_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(BEvent)) {
            appEvent->m_eventQueue.append(BEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(BEvent)) {
            appEvent->m_eventQueue.removeAll(BEvent);
        }
    }
}

/**
 * @brief 信号槽，绿色通道显示
 * @param
 */
void MainWindow::on_m_btn_Gchannel_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(GEvent)) {
            appEvent->m_eventQueue.append(GEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(GEvent)) {
            appEvent->m_eventQueue.removeAll(GEvent);
        }
    }
}


/**
 * @brief 信号槽，红色通道显示
 * @param
 */
void MainWindow::on_m_btn_Rchannel_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(REvent)) {
            appEvent->m_eventQueue.append(REvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(REvent)) {
            appEvent->m_eventQueue.removeAll(REvent);
        }
    }
}


/**
 * @brief 信号槽，二值化阈值处理
 * @param
 */
void MainWindow::on_m_btn_binarythre_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(BinthEvent)) {
            appEvent->m_eventQueue.append(BinthEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(BinthEvent)) {
            appEvent->m_eventQueue.removeAll(BinthEvent);
        }
    }
}

void MainWindow::on_horizontalSlider_binarythre_valueChanged(int value) {
    appEvent->m_val = value;
}


/**
 * @brief 信号槽，反二值化阈值处理
 * @param
 */
void MainWindow::on_m_btn_binarythre_inv_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(BinthinvEvent)) {
            appEvent->m_eventQueue.append(BinthinvEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(BinthinvEvent)) {
            appEvent->m_eventQueue.removeAll(BinthinvEvent);
        }
    }
}

void MainWindow::on_horizontalSlider_binarythre_inv_valueChanged(int value) {
    appEvent->m_val = value;
}

/**
 * @brief 信号槽，截断阈值化处理
 * @param
 */
void MainWindow::on_m_btn_thtr_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(ThtrEvent)) {
            appEvent->m_eventQueue.append(ThtrEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(ThtrEvent)) {
            appEvent->m_eventQueue.removeAll(ThtrEvent);
        }
    }
}

void MainWindow::on_horizontalSlider_thtr_valueChanged(int value) {
    appEvent->m_val = value;
}

/**
 * @brief 信号槽，超阈值零处理
 * @param
 */
void MainWindow::on_m_btn_thtoinv_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(ThtoinvEvent)) {
            appEvent->m_eventQueue.append(ThtoinvEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(ThtoinvEvent)) {
            appEvent->m_eventQueue.removeAll(ThtoinvEvent);
        }
    }
}

void MainWindow::on_horizontalSlider_thtoinv_valueChanged(int value) {
    appEvent->m_val = value;
}

/**
 * @brief 信号槽，低阈值零处理
 * @param
 */
void MainWindow::on_m_btn_thto_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(ThtoEvent)) {
            appEvent->m_eventQueue.append(ThtoEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(ThtoEvent)) {
            appEvent->m_eventQueue.removeAll(ThtoEvent);
        }
    }
}

void MainWindow::on_horizontalSlider_thto_valueChanged(int value) {
    appEvent->m_val = value;
}


/**
 * @brief 信号槽，自适应阈值处理(二值)
 * @param
 */
void MainWindow::on_m_btn_adTh_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(adThEvent)) {
            appEvent->m_eventQueue.append(adThEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(adThEvent)) {
            appEvent->m_eventQueue.removeAll(adThEvent);
        }
    }
}


/**
 * @brief 信号槽，均值滤波
 * @param
 */
void MainWindow::on_m_btn_avblur_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(avblEvent)) {
            appEvent->m_eventQueue.append(avblEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(avblEvent)) {
            appEvent->m_eventQueue.removeAll(avblEvent);
        }
    }
}

/**
 * @brief 信号槽，方框滤波
 * @param
 */
void MainWindow::on_m_btn_bofil_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(bofilEvent)) {
            appEvent->m_eventQueue.append(bofilEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(bofilEvent)) {
            appEvent->m_eventQueue.removeAll(bofilEvent);
        }
    }
}

/**
 * @brief 信号槽，高斯滤波
 * @param
 */
void MainWindow::on_m_btn_gabl_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(gablEvent)) {
            appEvent->m_eventQueue.append(gablEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(gablEvent)) {
            appEvent->m_eventQueue.removeAll(gablEvent);
        }
    }
}


/**
 * @brief 信号槽，中值滤波
 * @param
 */
void MainWindow::on_m_btn_mebl_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(meblEvent)) {
            appEvent->m_eventQueue.append(meblEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(meblEvent)) {
            appEvent->m_eventQueue.removeAll(meblEvent);
        }
    }
}


/**
 * @brief 信号槽，双边滤波
 * @param
 */
void MainWindow::on_m_btn_bifi_clicked(bool checked) {
    if (checked) {
        if (!appEvent->m_eventQueue.contains(bifiEvent)) {
            appEvent->m_eventQueue.append(bifiEvent);
        }
    } else {
        if (appEvent->m_eventQueue.contains(bifiEvent)) {
            appEvent->m_eventQueue.removeAll(bifiEvent);
        }
    }
}

void MainWindow::on_horizontalSlider_bifi_valueChanged(int value) {
    appEvent->m_val = value;
}


/**
 * @brief 信号槽，2D卷积
 * @param
 */
void MainWindow::on_m_btn_2Dfi_clicked(bool checked) {
    appEvent->m_kernel = QString();
    if (checked) {
        if (!appEvent->m_eventQueue.contains(tDfiEvent)) {
            appEvent->m_eventQueue.append(tDfiEvent);
        }
        //        if(ui->com_juanjisuanzi->text()=="lapulasi"){
        appEvent->m_kernel = "lapulasi";
        //        }
    } else {
        if (appEvent->m_eventQueue.contains(tDfiEvent)) {
            appEvent->m_eventQueue.removeAll(tDfiEvent);
        }
    }
}


/**
 * @brief 析构函数，结束程序时
 * @param
 */
MainWindow::~MainWindow() {
    delete ui;
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

