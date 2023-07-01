#include "ctoupcamera.h"

//CToupCamera& CToupCamera::getInstance()
//{
//    static CToupCamera instance;

//    return instance;

//}

CToupCamera::CToupCamera()
{
    Toupcam_EnumV2(m_arr);
}

CToupCamera::~CToupCamera(){
    close();
}

bool CToupCamera::isOpened() const {
    // 检查是否已经打开
    if (m_hcam) {
        return true;
    } else {
        return false;
    }
}

int CToupCamera::open(){
    //TODO:未实现
    return 0;
}

int CToupCamera::open(int index) {
    // 打开
    if (!isOpened()) {
        m_hcam = Toupcam_Open(m_arr[index].id);
        if (m_hcam) {
            Toupcam_get_eSize(m_hcam, (unsigned *) &m_res);
            m_imgWidth = m_arr[index].model->res[m_res].width;
            m_imgHeight = m_arr[index].model->res[m_res].height;
            //初始化Toup设置
            //1:BGR,2:RGB,Qimage use RGB byte order
            Toupcam_put_Option(m_hcam, TOUPCAM_OPTION_BYTEORDER, 0);
            //自动曝光 0:不启用,1:连续模式,2:单次模式
            Toupcam_put_AutoExpoEnable(m_hcam, 1);
            if (m_pData) {
                delete[] m_pData;
                m_pData = nullptr;
            }
            //启动相机
            if (SUCCEEDED(Toupcam_StartPullModeWithCallback(m_hcam, eventCallBack, this))) {
                qDebug() << "CToupCam:打开成功.";
                m_pData = new unsigned char[TDIBWIDTHBYTES(m_imgWidth * 24) * m_imgHeight];
                pInfo = new ToupcamFrameInfoV2();
                return 200;
            } else {
                this->close();
                qDebug() << "CToupCam:打开失败，拉取图像失败。";
                return 404;
            }
        }
    }
    return isOpened();
}

void CToupCamera::close() {
    // 关闭
    if (isOpened()) {
        Toupcam_Close(m_hcam);
        m_hcam = nullptr;
        if (m_pData) {
            delete[] m_pData;
            m_pData = nullptr;
        }
        qDebug() << "CToupCam:关闭相机成功.";
    }
}

bool CToupCamera::read(cv::Mat &frame) {
    // 读取帧
    if (isOpened()) {
        HRESULT hr = Toupcam_PullImageV2(m_hcam, m_pData, 24, pInfo);
        if (SUCCEEDED(hr)) {
//            qDebug() << "CToupCam:读取图像成功。" << pInfo->width << "x" << pInfo->height;
            // 将图像数据和大小信息存储到 Mat 对象中
            cv::Mat image(m_imgHeight, m_imgWidth, CV_8UC3, m_pData);
            frame = image.clone();
            // 将图像数据和大小信息存储到 ImageData 对象中
            // image.data = m_pData;
            // image.width = m_imgWidth;
            // image.height = m_imgHeight;
            return true;
        }
//        qDebug() << "CToupCam:读取图像失败。" << FAILED(hr);
    }
    return false;
}

void CToupCamera::getCameraList(std::vector<std::string> &camera_list)
{
    int toupCamCount = Toupcam_EnumV2(m_arr);
    for (int i = 0; i < toupCamCount; ++i) {
    qDebug() << m_arr[i].id << m_arr[i].displayname;
#ifdef _WIN32
        camera_list.push_back(QString::fromWCharArray(m_arr[i].displayname).toStdString());
#else
        camera_list.push_back(m_arr[i].displayname);
#endif
    }
}

void CToupCamera::saveImage()
{
    // 保存图像
    std::vector<uchar> vec(TDIBWIDTHBYTES(m_imgWidth * 24) * m_imgHeight);
    if (SUCCEEDED(Toupcam_PullStillImageV2(m_hcam, &vec[0], 24, pInfo)))
    {
        cv::Mat image(m_imgHeight, m_imgWidth, CV_8UC3, m_pData);
        cv::imwrite("save.bmp", image);
        qDebug() << "save image ";
//        image.save(QString::asprintf("toupcam_%u.jpg", ++m_count));
    }
    qDebug() << "save image ffffff";
}


/**
 * @brief 回调函数
 * @param
 */
void __stdcall CToupCamera::eventCallBack(unsigned nEvent, void *pCallbackCtx) {
    if (TOUPCAM_EVENT_IMAGE == nEvent) {
//        qDebug() << "CToupCam:handleEvent:pull image ok" << nEvent;
    } else {
//        qDebug() << "CToupCam:handleEvent" << nEvent;
    }
}

