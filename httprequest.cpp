#include "httprequest.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <QtQml>
#include <QDebug>
#include <QByteArray>


HttpRequest::HttpRequest(QObject *parent)
    : QObject(parent),
      m_manager(new QNetworkAccessManager(this)),
      m_readyState(Unset)
{
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

HttpRequest::~HttpRequest()
{
#ifdef QT_DEBUG
    qDebug() << "destroy: " << this;
#endif
}

void HttpRequest::open(const QString &method, const QUrl &url)
{
    m_method = method;
    m_url = url;
}


void HttpRequest::setRequestHeader(const QString &header, const QString &value)
{
    m_headers.append(QPair<QString, QString>(header, value));
}


void HttpRequest::clearRequestHeaders()
{
    m_headers.clear();
}


void HttpRequest::send(const QString &data)
{

    QNetworkRequest request(m_url);
    for(int i=0; i<this->m_headers.size(); i++) {
        request.setRawHeader(this->m_headers[i].first.toUtf8(), this->m_headers[i].second.toUtf8());
    }

#ifdef QT_DEBUG
    qDebug() << "method: " << this->m_method;
    qDebug() << "url: " << request.url();
    qDebug() << "data: " << data;
#endif

    if(this->m_method == "GET") {
        QNetworkReply* reply = m_manager->get(request);

        connect(this, SIGNAL(abort()), reply, SLOT(abort()));

        connect( reply, SIGNAL(downloadProgress(qint64,qint64)),
                 this, SLOT(replyDownloadProgress(qint64,qint64)));

    } else if(this->m_method == "POST") {
        QNetworkReply* reply = m_manager->post(request, data.toUtf8());

        connect(this, SIGNAL(abort()), reply, SLOT(abort()));

        connect( reply, SIGNAL(downloadProgress(qint64,qint64)),
                 this, SLOT(replyDownloadProgress(qint64,qint64)));

    } else if(this->m_method == "PUT") {
        QNetworkReply* reply = m_manager->put(request, data.toUtf8());

        connect(this, SIGNAL(abort()), reply, SLOT(abort()));

        connect( reply, SIGNAL(downloadProgress(qint64,qint64)),
                 this, SLOT(replyDownloadProgress(qint64,qint64)));

    } else if(this->m_method == "DELETE") {
        QNetworkReply* reply = m_manager->deleteResource(request);

        connect(this, SIGNAL(abort()), reply, SLOT(abort()));

        connect( reply, SIGNAL(downloadProgress(qint64,qint64)),
                 this, SLOT(replyDownloadProgress(qint64,qint64)));

    } else if(this->m_method == "HEAD") {
        QNetworkReply* reply = m_manager->head(request);

        connect(this, SIGNAL(abort()), reply, SLOT(abort()));

        connect( reply, SIGNAL(downloadProgress(qint64,qint64)),
                 this, SLOT(replyDownloadProgress(qint64,qint64)));
    }
}


HttpRequest::State HttpRequest::readyState() const
{
    return m_readyState;
}


void HttpRequest::setReadyState(const State &readyState)
{
    m_readyState = readyState;
    emit readyStateChanged();
}


void HttpRequest::replyFinished(QNetworkReply *reply)
{
    this->setStatus(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    if(reply->error()!= QNetworkReply::NoError) {

        this->setResponseText(reply->readAll());
        this->setReadyState(HttpRequest::Error);
        this->setErrorString(reply->errorString());

    } else if(reply->isFinished()) {
        this->setResponseText(reply->readAll());
        this->setReadyState(HttpRequest::Finished);
    } else {
        this->setResponseText(reply->readAll());
        this->setReadyState(HttpRequest::Running);
    }
}


void HttpRequest::replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(bytesTotal != 0) {
        this->setProgress(bytesReceived/bytesTotal);
    }
}


double HttpRequest::progress() const
{
    return m_progress;
}


void HttpRequest::setProgress(const double progress)
{
    if(m_progress != progress) {
        m_progress = progress;
        emit progressChanged(m_progress);
    }
}


int HttpRequest::status() const
{
    return m_status;
}


void HttpRequest::setStatus(int status)
{
    if(m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}


QString HttpRequest::responseText() const
{
    return m_responseText;
}


void HttpRequest::setResponseText(const QString &responseText)
{
    if(m_responseText != responseText) {
        m_responseText = responseText;
        emit responseTextChanged();
    }
}


QString HttpRequest::errorString() const
{
    return m_errorString;
}


void HttpRequest::setErrorString(const QString &errorString)
{
    if(m_errorString!= errorString) {
        m_errorString = errorString;
        emit errorStringChanged();
    }
}


HttpRequestFactory::HttpRequestFactory(QObject *parent):
    QObject(parent)
{

}


HttpRequest *HttpRequestFactory::create()
{
    HttpRequest* httpRequest = new HttpRequest();
    QQmlEngine::setObjectOwnership(httpRequest, QQmlEngine::JavaScriptOwnership);
    return httpRequest;
}


QObject *HttpRequestFactory::singleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(jsEngine)
    static HttpRequestFactory* factory = new HttpRequestFactory(qmlEngine);
    return factory;
}
