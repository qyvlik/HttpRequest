#include "httprequest.h"
#include "httprequest_p.h"

#include <QTimer>
#include <QtDebug>
#include <QtQml>

QNetworkAccessManager HttpRequest::netwrokAccessManager;

HttpRequest::HttpRequest(QObject *parent) :
    QObject(parent),
    d_ptr(new HttpRequestPrivate(&HttpRequest::netwrokAccessManager, this))
{
    qRegisterMetaType<NetworkStatus>("HttpRequest::NetworkStatus");
    qRegisterMetaType<State>("HttpRequest::State");

    connect(d_ptr, &HttpRequestPrivate::responseTextChanged, this, &HttpRequest::responseTextChanged);
    connect(d_ptr, &HttpRequestPrivate::readyStateChanged, this, &HttpRequest::readyStateChanged);
    connect(d_ptr, &HttpRequestPrivate::statusChanged, this, &HttpRequest::statusChanged);
    connect(d_ptr, &HttpRequestPrivate::statusTextChanged, this, &HttpRequest::statusTextChanged);
    connect(d_ptr, &HttpRequestPrivate::error, this, &HttpRequest::error);
}

HttpRequest::HttpRequest(QNetworkAccessManager *networkManager, QObject *parent):
    QObject(parent),
    d_ptr(new HttpRequestPrivate(networkManager, this))
{
    qRegisterMetaType<NetworkStatus>("HttpRequest::NetworkStatus");
    qRegisterMetaType<State>("HttpRequest::State");

    connect(d_ptr, &HttpRequestPrivate::responseTextChanged, this, &HttpRequest::responseTextChanged);
    connect(d_ptr, &HttpRequestPrivate::readyStateChanged, this, &HttpRequest::readyStateChanged);
    connect(d_ptr, &HttpRequestPrivate::statusChanged, this, &HttpRequest::statusChanged);
    connect(d_ptr, &HttpRequestPrivate::statusTextChanged, this, &HttpRequest::statusTextChanged);
    connect(d_ptr, &HttpRequestPrivate::error, this, &HttpRequest::error);
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::clear()
{
    d_ptr->clear();
}

void HttpRequest::setTimeout(qint32 time)
{
    if(this->getReadyState() != Loading ) {
        d_ptr->setTimeout(time);
    }
}

int HttpRequest::getTimeout() const
{
    return d_ptr->getTimeout();
}

void HttpRequest::setRequestHeader(const QByteArray &headerName, const QByteArray &value)
{
    QNetworkRequest request(d_ptr->getRequest());
    request.setRawHeader(headerName, value);
    d_ptr->setRequest(request);
}

QString HttpRequest::getRequestHeader(const QString &headerName) const
{
    return d_ptr->getRequest().rawHeader(headerName.toUtf8());
}

void HttpRequest::open(const QString &method, const QUrl &url, bool async)
{
    Q_UNUSED(async)

    d_ptr->setMethodName(method);
    QNetworkRequest request(d_ptr->getRequest());
    request.setUrl(url);
    d_ptr->setRequest(request);
    d_ptr->setReadyState(HttpRequest::UnStart);
}

void HttpRequest::send(const QString &data)
{
    if(d_ptr->getReadyState() == Loading) {
        return ;
    }
    if(d_ptr->getMethodName() == "GET") {
        QNetworkReply* reply = d_ptr->getManager()->get(d_ptr->getRequest());
        d_ptr->setReply(reply);

    } else if(d_ptr->getMethodName() == "POST") {
        QNetworkReply* reply = d_ptr->getManager()->post(d_ptr->getRequest(), data.toUtf8());
        d_ptr->setReply(reply);

    } else if(d_ptr->getMethodName() == "PUT") {
        QNetworkReply* reply = d_ptr->getManager()->put(d_ptr->getRequest(), data.toUtf8());
        d_ptr->setReply(reply);

    } else if(d_ptr->getMethodName() == "DELETE") {
        QNetworkReply* reply = d_ptr->getManager()->deleteResource(d_ptr->getRequest());
        d_ptr->setReply(reply);


    } else if(d_ptr->getMethodName() == "HEAD") {
        QNetworkReply* reply = d_ptr->getManager()->head(d_ptr->getRequest());
        d_ptr->setReply(reply);
    } else {
        // invaild method
        return ;
    }

    d_ptr->setReadyState(HttpRequest::Loading);

    connect(d_ptr, &HttpRequestPrivate::finished,
            this, &HttpRequest::finished);

    connect(this, &HttpRequest::abort,
            d_ptr->getReply(), &QNetworkReply::abort);

    connect( d_ptr->getReply(), &QNetworkReply::downloadProgress,
             this, &HttpRequest::downloadProgress);

    connect( d_ptr->getReply(), &QNetworkReply::uploadProgress,
             this, &HttpRequest::uploadProgress);

    Q_EMIT started();

    if(d_ptr->getTimeout() > 0) {
        QTimer::singleShot(d_ptr->getTimeout(), [&](){
            if(d_ptr->getReply() && !d_ptr->getReply()->isFinished()) {
                Q_EMIT this->abort();
                Q_EMIT this->timeout();
            }
        });
    }
}

/*
 * QString more better than QByteArray in qml.
*/
QString HttpRequest::getResponseText() const
{
    return QString(d_ptr->getResponseText());
}

QString HttpRequest::getStatusText() const
{
    return d_ptr->getStatusText();
}

HttpRequest::NetworkStatus HttpRequest::getStatus() const
{
    return d_ptr->getStatus();
}

HttpRequest::State HttpRequest::getReadyState() const
{
    return d_ptr->getReadyState();
}

QJsonArray HttpRequest::getAllResponseHeader() const
{
    QList<QNetworkReply::RawHeaderPair> rawHeaderPairs = d_ptr->getRawHeaderPairs();
    QJsonArray responseHeaders;
    if(!rawHeaderPairs.isEmpty()) {
        foreach(auto iter, rawHeaderPairs) {
            QJsonObject header;
            header[iter.first] = QString(iter.second);
            responseHeaders.push_back(QJsonValue(header));
        }
    }
    return responseHeaders;
}

HttpRequestFactory::HttpRequestFactory(QObject *parent):
    QObject(parent)
{}

HttpRequest *HttpRequestFactory::create()
{
    HttpRequest* httpRequest ;
    QQmlEngine* qmlEngine = qobject_cast<QQmlEngine *>(this->parent());
    if(qmlEngine && qmlEngine->networkAccessManager()) {
        httpRequest = new HttpRequest(qmlEngine->networkAccessManager(),
                                      Q_NULLPTR);
    } else {
        httpRequest = new HttpRequest;
    }

    QQmlEngine::setObjectOwnership(httpRequest, QQmlEngine::JavaScriptOwnership);

    //#ifdef QT_DEBUG
    //    connect(httpRequest, &HttpRequest::destroyed, [=](){
    //        qDebug() << "Delete " << httpRequest << " by QML Engine";
    //    });
    //#endif
    return httpRequest;
}

QObject *HttpRequestFactory::singleton(QQmlEngine *engine, QJSEngine *jsEngine)
{
    Q_UNUSED(jsEngine)
    return new HttpRequestFactory(engine);
}
