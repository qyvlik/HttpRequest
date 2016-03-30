#ifndef HTTPREQUEST_P_H
#define HTTPREQUEST_P_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "httprequest.h"

class HttpRequestPrivate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray responseText READ getResponseText WRITE setResponseText NOTIFY responseTextChanged)
    Q_PROPERTY(HttpRequest::State readyState READ getReadyState WRITE setReadyState NOTIFY readyStateChanged)
    Q_PROPERTY(HttpRequest::NetworkStatus status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString statusText READ getStatusText WRITE setStatusText NOTIFY statusTextChanged)

public:
    HttpRequestPrivate(QObject* parent = Q_NULLPTR):
        QObject(parent),
        timeout(300000),                            // 30 s
        reply(Q_NULLPTR),
        readyState(HttpRequest::UnStart),
        status(HttpRequest::NoError)
    {
    }

    ~HttpRequestPrivate()
    {
        //        if(reply) {
        //            reply->deleteLater();
        //        }
    }

    QString getMethodName() const
    { return methodName; }

    void setMethodName(const QString &value)
    { methodName = value; }

    HttpRequest::State getReadyState() const
    { return readyState; }

    void setReadyState(const HttpRequest::State &value)
    {
        if(readyState != value) {
            readyState = value;
            emit readyStateChanged();
        }
    }

    HttpRequest::NetworkStatus getStatus() const
    { return status; }

    void setStatus(const HttpRequest::NetworkStatus &value)
    {
        if(status != value ) {
            status = value;
            emit statusChanged();
        }
    }

    QNetworkRequest getRequest() const
    { return request; }

    void setRequest(const QNetworkRequest &value)
    { request = value; }

    QNetworkReply *getReply() const
    { return reply; }

    void setReply(QNetworkReply *value)
    {
        if(reply != value) {
            if(reply) {
                reply->disconnect();
            }
            reply = value;
            if(reply) {
                connect(reply, &QNetworkReply::finished,
                        this, &HttpRequestPrivate::onFinished);
            }
        }
    }

    QString getStatusText() const
    { return statusText; }

    void setStatusText(const QString &value)
    {
        if(statusText != value) {
            statusText = value;
            emit statusTextChanged();
        }
    }

    QByteArray getResponseText() const
    { return responseText; }

    void setResponseText(const QByteArray &value)
    {
        if(responseText != value) {
            responseText = value;
            emit responseTextChanged();
        }
    }

    QList<QNetworkReply::RawHeaderPair> getRawHeaderPairs() const
    { return rawHeaderPairs; }

    void clear() {
        rawHeaderPairs.clear();
        this->setRequest(QNetworkRequest());
        this->setReadyState(HttpRequest::UnStart);
        this->setMethodName("");
    }


    int getTimeout() const
    { return timeout; }

    void setTimeout(int value)
    { timeout = value; }

Q_SIGNALS:
    void responseTextChanged();
    void readyStateChanged();
    void statusChanged();
    void statusTextChanged();

    void finished();
    void timeoutChanged();

    void error();


private Q_SLOTS:
    void onFinished() {
        if(reply) {

            this->setResponseText(reply->readAll());
            QNetworkReply::NetworkError e = reply->error();
            this->setStatus((HttpRequest::NetworkStatus)e);
            rawHeaderPairs = reply->rawHeaderPairs();

            if(e != QNetworkReply::NoError) {
                this->setReadyState(HttpRequest::Error);
                this->setStatusText(reply->errorString());
                emit error();
                // error Not finished
                return ;
            } else {
                this->setReadyState(HttpRequest::Finished);
                this->setStatusText("");
            }

            emit finished();
        }
    }


private:
    int timeout;

    QNetworkRequest request;
    QNetworkReply* reply;

    QString methodName;

    HttpRequest::State readyState;
    HttpRequest::NetworkStatus status;
    QString statusText;

    QList<QNetworkReply::RawHeaderPair> rawHeaderPairs;
    QByteArray responseText;
};

#endif // HTTPREQUEST_P_H

