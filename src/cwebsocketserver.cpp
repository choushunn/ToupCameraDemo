#include "cwebsocketserver.h"

/**
 * @brief 构造函数
 *
 * @param port 监听的端口
 * @param parent 父对象
 */
CWebSocketServer::CWebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
{
    // 创建 WebSocket 服务器对象
    m_server = new QWebSocketServer("WebSocket Server", QWebSocketServer::NonSecureMode, this);

    // 监听指定的端口
    if (m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "CWebSocketServer:WebSocket server listening on port" << port;
    }
    else {
        qCritical() << "CWebSocketServer:Failed to listen on port" << port << ":" << m_server->errorString();
    }

    // 连接新连接信号，以便在新客户端连接时执行相应的槽函数
    connect(m_server, &QWebSocketServer::newConnection, this, &CWebSocketServer::onNewConnection);
}

/**
 * @brief 构造函数
 *
 * @param parent 父对象
 */
CWebSocketServer::CWebSocketServer(QObject *parent)
{
    // 创建 WebSocket 服务器对象
    m_server = new QWebSocketServer("WebSocket Server", QWebSocketServer::NonSecureMode, this);

    // 连接新的 WebSocket 连接信号，以便在有新的 WebSocket 连接时执行相应的槽函数
    connect(m_server, &QWebSocketServer::newConnection, this, &CWebSocketServer::onNewConnection);
}

/**
 * @brief 析构函数
 */
CWebSocketServer::~CWebSocketServer()
{
    // 停止 WebSocket 服务器
    stop();
}

/**
 * @brief 启动 WebSocket 服务器
 */
bool CWebSocketServer::start(int port)
{
    // 启动 WebSocket 服务器并监听指定的端口
    if (m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "CWebSocketServer:WebSocket server started";
        return true;
    } else {
        qCritical() << "CWebSocketServer:Failed to start WebSocket server:" << m_server->errorString();
        return false;
    }
}

/**
 * @brief 停止 WebSocket 服务器
 */
void CWebSocketServer::stop()
{
    if (m_server->isListening()) {
        qDebug() << "CWebSocketServer:Stopping WebSocket server";
        m_server->close();
        qDeleteAll(m_clients);
    }
}

bool CWebSocketServer::isListening()
{
    return m_server->isListening();
}


QList<QHostAddress> CWebSocketServer::getAllIpAddresses() const
{
    QList<QHostAddress> ipList;

    // 获得当前设备所有网络接口
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();

    // 遍历所有网络接口，获得每个接口的 IP 地址列表
    for (int i = 0; i < interfaceList.size(); i++) {
        const QNetworkInterface &interface = interfaceList.at(i);
        if ((interface.flags() & QNetworkInterface::IsUp) && (interface.flags() & QNetworkInterface::IsRunning) && !(interface.flags() & QNetworkInterface::IsLoopBack)) {
            QList<QNetworkAddressEntry> entryList = interface.addressEntries();
            for (int j = 0; j < entryList.size(); j++) {
                const QNetworkAddressEntry &entry = entryList.at(j);
                QHostAddress ip = entry.ip();
                if (!ip.isNull() && ip != QHostAddress::LocalHost && ip.protocol() == QAbstractSocket::IPv4Protocol) {
                    ipList.append(ip);
                }
            }
        }
    }

    return ipList;
}

QList<QWebSocket *> CWebSocketServer::findClientSockets(const QString &ipPort) const
{
    QList<QWebSocket*> clientSockets;

    for (QWebSocket* clientSocket : m_clients) {
        QString i_ipPort = QString("%1:%2").arg(clientSocket->peerAddress().toString()).arg(clientSocket->peerPort());
        if (i_ipPort == ipPort) {
            qDebug() << "CWebSocketServer:查找客户端成功";
            clientSockets.append(clientSocket);
        }
    }
    return clientSockets;
}

/**
 * @brief 处理新客户端连接
 */
void CWebSocketServer::onNewConnection()
{
    QWebSocket *clientSocket = m_server->nextPendingConnection();

    // 连接客户端断开信号、文本消息接收信号和二进制消息接收信号，以便在客户端断开、收到文本消息或收到二进制消息时执行相应的槽函数
    connect(clientSocket, &QWebSocket::disconnected, this, &CWebSocketServer::onClientDisconnected);
    connect(clientSocket, &QWebSocket::textMessageReceived, this, &CWebSocketServer::onTextMessageReceived);
    connect(clientSocket, &QWebSocket::binaryMessageReceived, this, &CWebSocketServer::onBinaryMessageReceived);

    // 将客户端连接添加到连接列表中，并发出客户端连接信号
    m_clients.append(clientSocket);
    emit clientConnected(clientSocket);
}

/**
 * @brief 处理客户端断开连接
 */
void CWebSocketServer::onClientDisconnected()
{
    QWebSocket *clientSocket = qobject_cast<QWebSocket *>(sender());
    if (clientSocket) {
        // 从连接列表中移除客户端连接，并发出客户端断开信号
        m_clients.removeAll(clientSocket);
        emit clientDisconnected(clientSocket);
        clientSocket->deleteLater();
    }
}

/**
 * @brief 处理文本消息接收
 *
 * @param message 接收到的文本消息
 */
void CWebSocketServer::onTextMessageReceived(const QString &message)
{
    QWebSocket *clientSocket = qobject_cast<QWebSocket *>(sender());
    if (clientSocket) {
        // 发出文本消息接收信号
        emit textMessageReceived(clientSocket, message);
    }
}

/**
 * @brief 处理二进制消息接收
 *
 * @param message 接收到的二进制消息
 */
void CWebSocketServer::onBinaryMessageReceived(const QByteArray &message)
{
    QWebSocket *clientSocket = qobject_cast<QWebSocket *>(sender());
    if (clientSocket) {
        // 发出二进制消息接收信号
        emit binaryMessageReceived(clientSocket, message);
    }
}

/**
 * @brief 发送文本消息给指定客户端连接
 *
 * @param clientSocket 指定客户端连接
 * @param message 要发送的文本消息
 */
void CWebSocketServer::sendTextMessage(QWebSocket *clientSocket, const QString &message)
{
    // 发送文本消息给指定客户端连接
    clientSocket->sendTextMessage(message);
}

/**
 * @brief 发送二进制消息给指定客户端连接
 *
 * @param clientSocket 指定客户端连接
 * @param message 要发送的二进制消息
 */
void CWebSocketServer::sendBinaryMessage(QWebSocket *clientSocket, const QByteArray &message)
{
    // 发送二进制消息给指定客户端连接
    clientSocket->sendBinaryMessage(message);
}

/**
 * @brief 关闭指定客户端连接
 *
 * @param clientSocket 指定客户端连接
 */
void CWebSocketServer::closeClientConnection(QWebSocket *clientSocket)
{
    // 如果指定客户端连接有效，则关闭该连接
    if (clientSocket && clientSocket->isValid()) {
        clientSocket->close();
    }
}

/**
 * @brief 关闭指定地址的客户端连接
 *
 * @param clientAddress 要关闭的客户端连接地址
 */
void CWebSocketServer::closeClientConnection(const QHostAddress &clientAddress)
{
    // 遍历连接列表，找到指定地址的客户端连接，并关闭该连接
    for (QWebSocket *clientSocket : m_clients) {
        if (clientSocket->peerAddress() == clientAddress) {
            closeClientConnection(clientSocket);
            return;
        }
    }
}
