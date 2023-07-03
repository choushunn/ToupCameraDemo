#include "ctoupcamera.h"

#include <QImage>

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
    m_index = index;
    if (!isOpened()) {
        m_curDev = m_arr[m_index];
        m_hcam = Toupcam_Open(m_curDev.id);
        if (m_hcam) {
            Toupcam_get_eSize(m_hcam, (unsigned *) &m_res);
            m_imgWidth = m_curDev.model->res[m_res].width;
            m_imgHeight = m_curDev.model->res[m_res].height;
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
            cv::Mat image(pInfo->height, pInfo->width, CV_8UC3, m_pData);
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

}

void CToupCamera::setAutoExpo(int state)
{
    if(this->isOpened()){
        if(Toupcam_put_AutoExpoEnable(this->m_hcam, state) < 0){
            return;
        }
    }
}

void CToupCamera::setExpoTime(int value){
    if(this->isOpened()){
        Toupcam_put_ExpoTime(m_hcam, value);
    }
}

void CToupCamera::setExpoTarget(int value)
{
    if(this->isOpened()){
        Toupcam_put_ExpoAGain(m_hcam, value);
    }
}

void CToupCamera::setExpoGain(int value){
    if(this->isOpened()){
        Toupcam_put_ExpoAGain(m_hcam, value);
    }
}


void CToupCamera::getContext(Context &ctx)
{
    if(this->isOpened()){
        Toupcam_get_ExpTimeRange(m_hcam, &ctx.uimin, &ctx.uimax, &ctx.uidef);
        Toupcam_get_ExpoAGainRange(m_hcam, &ctx.usmin, &ctx.usmax, &ctx.usdef);
        qDebug() << "读取Toup相机参数成功";
    }
}


void CToupCamera::getResolution(std::vector<std::string>& res){
    if(this->isOpened()){
        for(unsigned i=0; i< m_curDev.model->preview;++i){
            std::string str = std::to_string(m_curDev.model->res[i].width) + "*" + std::to_string(m_curDev.model->res[i].height);
            res.push_back(str);
        }
    }
}

void CToupCamera::setResolution(int index)
{
    if(this->isOpened()){
        m_res = index;
        m_imgWidth = m_curDev.model->res[m_res].width;
        m_imgHeight = m_curDev.model->res[m_res].height;
        Toupcam_put_eSize(m_hcam, static_cast<unsigned>(m_res));
    }
}

void CToupCamera::getSnap()
{
    Toupcam_Snap(m_hcam, m_res);
}


void CToupCamera::handleImageStill(){
    qDebug() << "CToupCam:开始捕获" << g_totalstill;
    unsigned width = 0, height = 0;
    if (SUCCEEDED(Toupcam_PullStillImage(m_hcam, nullptr, 24, &width, &height))) // peek
    {
        std::vector<uchar> vec(TDIBWIDTHBYTES(width * 24) * height);
        if (SUCCEEDED(Toupcam_PullStillImage(m_hcam, &vec[0], 24, &width, &height)))
        {
            std::string file_name = "snaps/ctoupcam_" + std::to_string(++g_totalstill) + ".bmp";
            QImage image(&vec[0], width, height, QImage::Format_RGB888);
            image.save(QString::fromStdString(file_name));
        }
    }
};


/**
 * @brief 回调函数
 * @param
 */
void CToupCamera::eventCallBack(unsigned nEvent, void *pCallbackCtx) {
    CToupCamera* pThis = static_cast<CToupCamera*>(pCallbackCtx);

    if (TOUPCAM_EVENT_IMAGE == nEvent) {
        //        qDebug() << "CToupCam:handleEvent:pull image ok" << nEvent;
    } else if(TOUPCAM_EVENT_DISCONNECTED == nEvent){
        qDebug() << "CToupCam:断开连接" << nEvent;
    }else if(TOUPCAM_EVENT_STILLIMAGE == nEvent){
        pThis->handleImageStill();
    }
}

