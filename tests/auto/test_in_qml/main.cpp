#include <QApplication>
#include <QQmlApplicationEngine>

#include <QtQml>

#include <QStandardPaths>
#include <QtDebug>
#include <QQmlNetworkAccessManagerFactory>

#include <HttpRequest>
#include <NetworkCookieJar>

class MyNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    virtual QNetworkAccessManager *create(QObject *parent);
};

QNetworkAccessManager *MyNetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);

    nam->setCookieJar(new NetworkCookieJar(nam));

    return nam;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<HttpRequest>("Network",
                                 1, 0,
                                 "HttpRequest");

    qmlRegisterSingletonType<HttpRequestFactory>("Network",
                                                 1, 0,
                                                 "HttpRequestFactory",
                                                 &HttpRequestFactory::singleton);

    QQmlApplicationEngine engine;

    engine.setNetworkAccessManagerFactory(new MyNetworkAccessManagerFactory());

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
