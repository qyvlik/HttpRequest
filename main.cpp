#include <QApplication>
#include <QQmlApplicationEngine>

#include <QtQml>
#include "httprequest.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // QCoreApplication app(argc, argv);

    // HttpRequest httpRequest;

    // httpRequest.connect(&httpRequest, &HttpRequest::readyStateChanged, [&](){
    //     qDebug() << httpRequest.responseText();
    // });

    // httpRequest.open("GET", QUrl("https://api.github.com/zen"));
    // httpRequest.send();

    qmlRegisterType<HttpRequest>("Network",
                                 1, 0,
                                 "HttpRequest");

    qmlRegisterSingletonType<HttpRequestFactory>("Network",
                                                 1, 0,
                                                 "HttpRequestFactory",
                                                 &HttpRequestFactory::singleton);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
