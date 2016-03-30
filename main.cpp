#include <QApplication>
#include <QQmlApplicationEngine>

#include <QtQml>

#include "src/httprequest.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //HttpRequest* httpRequest = new HttpRequest();

    //httpRequest->clear();

    //QObject::connect(httpRequest, &HttpRequest::finished, [=](){
    //    qDebug() << httpRequest->getAllResponseHeader();
    //    QJsonDocument dom = QJsonDocument::fromJson(httpRequest->getResponseText().toUtf8());
    //    qDebug() << dom.object();

    //    httpRequest->deleteLater();
    //});

    //QObject::connect(httpRequest, &HttpRequest::timeout, [=](){
    //    qDebug() << " time out";
    //    httpRequest->deleteLater();
    //});

    //QObject::connect(httpRequest, &HttpRequest::error, [=](){
    //    qDebug() << httpRequest->getStatus();
    //    qDebug() << httpRequest->getStatusText();
    //    httpRequest->deleteLater();
    //});

    //QByteArray data("?limit=60&offset=0&s=%E7%88%B1&total=true&type=1");

    //httpRequest->setRequestHeader("Referer", "http://music.163.com/search/");
    //httpRequest->setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    //httpRequest->open("POST", QUrl("http://music.163.com/api/search/get"));

    //httpRequest->setTimeout(300000);             // 30s

    //httpRequest->send(QString(data));

    qmlRegisterType<HttpRequest>("Network",
                                 1, 0,
                                 "HttpRequest");

    qmlRegisterSingletonType<HttpRequestFactory>("Network",
                                                 1, 0,
                                                 "HttpRequestFactory",
                                                 &HttpRequestFactory::singleton);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
