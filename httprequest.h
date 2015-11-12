#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QUrl>
#include <QList>
#include <QMap>
#include <QVariant>

class QNetworkReply;
class QJSEngine;
class QQmlEngine;
class QNetworkAccessManager;


/*!
  使用 HttpRequestFactory::create() 产生的
  HttpRequest，其网络处理明显慢于 XMLHttpRequest
  在 QML 文档中直接实例化 HttpRequest 网络处理速度与 XMLHttpRequest 相近
  但是明显慢那么几毫秒。
*/
class HttpRequest : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(State readyState READ readyState NOTIFY readyStateChanged)
    Q_PROPERTY(QString errorString READ errorString  NOTIFY errorStringChanged)
    Q_PROPERTY(QString responseText READ responseText NOTIFY responseTextChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)

public:
    enum State {
        Unset = 0,
        Finished,
        Running,
        Error
    };

    explicit HttpRequest(QObject *parent = 0);
    ~HttpRequest();

    Q_INVOKABLE void open(const QString& method, const QUrl& url);
    Q_INVOKABLE void setRequestHeader(const QString& header, const QString& value);
    Q_INVOKABLE void clearRequestHeaders();
    Q_INVOKABLE void send(const QString& data = QString());

public:
    State readyState() const;
    QString errorString() const;
    QString responseText() const;
    int status() const;
    double progress() const;

private:
    void setReadyState(const State &readyState);
    void setErrorString(const QString &errorString);
    void setResponseText(const QString &responseText);
    void setStatus(int status);
    void setProgress(const double progress);

signals:
    void readyStateChanged();
    void errorStringChanged();
    void responseTextChanged();
    void statusChanged();
    void progressChanged(double progress);

signals:
    // connect reply slot abort
    void abort();

private slots:
    void replyFinished(QNetworkReply* reply);
    void replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager* m_manager;

    // request
    QString m_method;
    QUrl m_url;
    QList<QPair<QString, QString> > m_headers;

    // response
    State m_readyState;
    QString m_responseText;
    int m_status;
    QString m_errorString;
    double m_progress;
};


class HttpRequestFactory : public QObject
{
    Q_OBJECT
    explicit HttpRequestFactory(QObject* parent = 0);
public:
    Q_INVOKABLE HttpRequest* create();
    static QObject* singleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
};


#endif // HTTPREQUEST_H
