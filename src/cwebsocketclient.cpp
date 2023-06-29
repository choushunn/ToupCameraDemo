#include "cwebsocketclient.h"

CWebSocketClient::CWebSocketClient(const QUrl &url, int timeout, QObject *parent)
    : QObject(parent), m_timeout(timeout)
{
    // 创建 WebSocket 对象
    m_webSocket = new QWebSocket();

    // 连接 WebSocket 连接成功信号
    connect(m_webSocket, &QWebSocket::connected, this, &CWebSocketClient::onConnected);
    // 文本消息接收信号
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &CWebSocketClient::onTextMessageReceived);
    // 二进制消息接收信号
    connect(m_webSocket, &QWebSocket::binaryMessageReceived, this, &CWebSocketClient::onBinaryMessageReceived);

    // 连接 WebSocket 连接关闭信号和错误信号，以便在连接关闭或连接出错时执行相应的槽函数
    connect(m_webSocket, &QWebSocket::disconnected, this, &CWebSocketClient::onDisconnected);
    connect(m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &CWebSocketClient::onError);

    // 连接 WebSocket 服务器
    connectToServer(url);
}


CWebSocketClient::~CWebSocketClient()
{
    // 断开 WebSocket 连接并释放资源
    disconnectFromServer();

    // 删除 WebSocket 对象
    m_webSocket->deleteLater();
}


void CWebSocketClient::sendTextMessage(const QString &message)
{
    // 发送文本消息
    m_webSocket->sendTextMessage(message);
}

void CWebSocketClient::sendBinaryMessage(const QByteArray &message)
{
    // 发送二进制消息
    m_webSocket->sendBinaryMessage(message);
}

void CWebSocketClient::onConnected()
{
    // 发出连接成功信号
    emit connected();

    // 停止 WebSocket 连接超时计时器
    m_timeoutTimer->stop();
}


void CWebSocketClient::onTextMessageReceived(const QString &message)
{
    // 发出文本消息接收信号
    emit textMessageReceived(message);
}


void CWebSocketClient::onBinaryMessageReceived(const QByteArray &message)
{
    // 发出二进制消息接收信号
    emit binaryMessageReceived(message);
}

void CWebSocketClient::onDisconnected()
{
    // 发出连接关闭信号
    emit disconnected();

    // 停止 WebSocket 连接超时计时器
    m_timeoutTimer->stop();
}

void CWebSocketClient::onError(QAbstractSocket::SocketError error)
{
    // 发出错误信号
    emit errorOccurred(error);

    // 断开 WebSocket 连接并释放资源
    disconnectFromServer();
}

void CWebSocketClient::connectToServer(const QUrl &url)
{
    // 开启 WebSocket 连接超时计时器
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, [this]() {
        if (m_webSocket->state() == QAbstractSocket::ConnectingState) {
            // 如果连接超时，发出错误信号并断开 WebSocket 连接
            emit errorOccurred(QAbstractSocket::SocketTimeoutError);
            disconnectFromServer();
        }
    });
    m_timeoutTimer->start(m_timeout);

    // 连接 WebSocket 服务器
    m_webSocket->open(url);
}

void CWebSocketClient::disconnectFromServer()
{
    // 停止 WebSocket 连接超时计时器
    m_timeoutTimer->stop();

    // 断开 WebSocket 连接并释放资源
    if (m_webSocket->state() != QAbstractSocket::UnconnectedState) {
        m_webSocket->close();
    }
}
