#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonValue>

struct KeyValue {
    QString key;
    QJsonValue value;
};

class Network : public QObject
{
    Q_OBJECT
public:
    static Network *globalInstance();

    Q_INVOKABLE QString generateUniqueId() const;
    Q_INVOKABLE bool sendConversation(const QString &ingestId, const QString &conversation);
    Q_INVOKABLE void trackChatEvent(const QString &event, QVariantMap props = QVariantMap());
    Q_INVOKABLE void trackEvent(const QString &event, const QVariantMap &props = QVariantMap());

Q_SIGNALS:
    void healthCheckFailed(int code);
    void requestMixpanel(const QByteArray &json, bool isOptOut = false);

public Q_SLOTS:
    void sendOptOut();
    void sendModelLoaded();
    void sendStartup();

private Q_SLOTS:
    void handleIpifyFinished();
    void handleHealthFinished();
    void handleJsonUploadFinished();
    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void handleMixpanelFinished();
    void handleIsActiveChanged();
    void handleUsageStatsActiveChanged();
    void sendMixpanel(const QByteArray &json, bool isOptOut);

private:
    void sendHealth();
    void sendIpify();
    bool packageAndSendJson(const QString &ingestId, const QString &json);

private:
    bool m_sendUsageStats = false;
    bool m_hasSentOptIn;
    bool m_hasSentOptOut;
    QString m_ipify;
    QString m_uniqueId;
    QString m_sessionId;
    QNetworkAccessManager m_networkManager;
    QVector<QNetworkReply*> m_activeUploads;

private:
    explicit Network();
    ~Network() {}
    friend class MyNetwork;
};

#endif // LLM_H
