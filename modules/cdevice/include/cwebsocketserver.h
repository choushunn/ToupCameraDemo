#ifndef CWEBSOCKETSERVER_H
#define CWEBSOCKETSERVER_H

#include <QObject>
#include <QtWebSockets>

class CWebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit CWebSocketServer(quint16 port, QObject *parent = nullptr);

    CWebSocketServer(QObject *parent = nullptr);

    ~CWebSocketServer();

    /**
     * @brief 启动 WebSocket 服务器
     *
     * @param port 监听的端口号
     * @return true 启动成功；false 启动失败
     */
    bool start(int port);

    /**
     * @brief 停止 WebSocket 服务器
     */
    void stop();
    bool isListening();
    /**
     * @brief 获得当前设备所有 IP 地址
     *
     * @return 当前设备所有 IP 地址
     */
    QList<QHostAddress> getAllIpAddresses() const;

    QList<QWebSocket*> findClientSockets(const QString& ip) const;

signals:
    void clientConnected(QWebSocket *clientSocket);
    void clientDisconnected(QWebSocket *clientSocket);
    void textMessageReceived(QWebSocket *clientSocket, const QString &message);
    void binaryMessageReceived(QWebSocket *clientSocket, const QByteArray &message);
public slots:
    void onNewConnection();
    void onClientDisconnected();
    void onTextMessageReceived(const QString &message);
    void onBinaryMessageReceived(const QByteArray &message);

    void sendTextMessage(QWebSocket *clientSocket, const QString &message);
    void sendBinaryMessage(QWebSocket *clientSocket, const QByteArray &message);
    void closeClientConnection(QWebSocket *clientSocket);
    void closeClientConnection(const QHostAddress &clientAddress);

private:
    QWebSocketServer *m_server;
    QList<QWebSocket *> m_clients;
};

#endif // CWEBSOCKETSERVER_H
