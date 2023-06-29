#ifndef CWEBSOCKETCLIENT_H
#define CWEBSOCKETCLIENT_H

#include <QObject>
#include <QUrl>
#include <QtWebSockets>

class CWebSocketClient : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param url WebSocket 服务器的 URL
     * @param timeout WebSocket 连接超时时间（单位：毫秒）
     * @param parent 父对象指针
     */
    explicit CWebSocketClient(const QUrl &url, int timeout = 5000, QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~CWebSocketClient();

    /**
     * @brief 发送文本消息
     *
     * @param message 消息内容
     */
    void sendTextMessage(const QString &message);

    /**
     * @brief 发送二进制消息
     *
     * @param message 消息内容
     */
    void sendBinaryMessage(const QByteArray &message);

signals:
    /**
     * @brief 连接成功信号
     */
    void connected();

    /**
     * @brief 文本消息接收信号
     *
     * @param message 接收到的文本消息内容
     */
    void textMessageReceived(const QString &message);

    /**
     * @brief 二进制消息接收信号
     *
     * @param message 接收到的二进制消息内容
     */
    void binaryMessageReceived(const QByteArray &message);

    /**
     * @brief 连接关闭信号
     */
    void disconnected();

    /**
     * @brief 错误信号
     *
     * @param error 连接错误码
     */
    void errorOccurred(QAbstractSocket::SocketError error);

private slots:
    /**
     * @brief WebSocket 连接成功槽函数
     */
    void onConnected();

    /**
     * @brief WebSocket 文本消息接收槽函数
     *
     * @param message 接收到的文本消息内容
     */
    void onTextMessageReceived(const QString &message);

    /**
     * @brief WebSocket 二进制消息接收槽函数
     *
     * @param message 接收到的二进制消息内容
     */
    void onBinaryMessageReceived(const QByteArray &message);

    /**
     * @brief WebSocket 连接关闭槽函数
     */
    void onDisconnected();

    /**
     * @brief WebSocket 错误槽函数
     *
     * @param error 连接错误码
     */
    void onError(QAbstractSocket::SocketError error);

private:
    /**
     * @brief WebSocket 对象指针
     */
    QWebSocket *m_webSocket;

    /**
     * @brief WebSocket 连接超时计时器指针
     */
    QTimer *m_timeoutTimer;

    /**
     * @brief WebSocket 连接超时时间（单位：毫秒）
     */
    int m_timeout;

    /**
     * @brief 连接 WebSocket 服务器
     *
     * @param url WebSocket 服务器的 URL
     */
    void connectToServer(const QUrl &url);

    /**
     * @brief 断开 WebSocket 连接并释放资源
     */
    void disconnectFromServer();

};

#endif // CWEBSOCKETCLIENT_H
