#include <QtTest>

#include <HttpRequest>
#include <NetworkCookieJar>

class test_in_cpp : public QObject
{
    Q_OBJECT

    QNetworkAccessManager* manager;
public:
    test_in_cpp();
    ~test_in_cpp();

public slots:
    void test_case1();

};

test_in_cpp::test_in_cpp():
    manager(new QNetworkAccessManager(this))
{

}

test_in_cpp::~test_in_cpp()
{

}

void test_in_cpp::test_case1()
{
    HttpRequest* httpRequest = new HttpRequest(manager, this);

    httpRequest->clear();

    QObject::connect(httpRequest, &HttpRequest::finished, [=](){
        qDebug() << httpRequest->getAllResponseHeader();
        QJsonDocument dom = QJsonDocument::fromJson(httpRequest->getResponseText().toUtf8());
        qDebug() << dom.object();

        httpRequest->deleteLater();
    });

    QObject::connect(httpRequest, &HttpRequest::timeout, [=](){
        qDebug() << " time out";
        httpRequest->deleteLater();
    });

    QObject::connect(httpRequest, &HttpRequest::error, [=](){
        qDebug() << httpRequest->getStatus();
        qDebug() << httpRequest->getStatusText();
        httpRequest->deleteLater();
    });

    QObject::connect(httpRequest, &HttpRequest::destroyed, QCoreApplication::quit);

    QByteArray data("?limit=60&offset=0&s=%E7%88%B1&total=true&type=1");

    httpRequest->setRequestHeader("Referer", "http://music.163.com/search/");
    httpRequest->setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    httpRequest->open("POST", QUrl("http://music.163.com/api/search/get"));

    httpRequest->setTimeout(300000);             // 30s

    httpRequest->send(QString(data));
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    test_in_cpp *t = new test_in_cpp();
    t->test_case1();

    return app.exec();
}

//QTEST_MAIN(test_in_cpp)

#include "tst_test_in_cpp.moc"
