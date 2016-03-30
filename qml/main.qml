import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Network 1.0

ApplicationWindow {

    id: window
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    Item {
        anchors.fill: parent

        //    QObject::connect(httpRequest, &HttpRequest::finished, [=](){
        //        qDebug() << httpRequest->getAllResponseHeader();
        //        QJsonDocument dom = QJsonDocument::fromJson(httpRequest->getResponseText());
        //        qDebug() << dom.object();

        //        httpRequest->deleteLater();
        //    });

        //    QObject::connect(httpRequest, &HttpRequest::timeout, [=](){
        //        qDebug() << " time out";
        //        httpRequest->deleteLater();
        //    });

        //    QObject::connect(httpRequest, &HttpRequest::error, [=](){
        //        qDebug() << httpRequest->getStatus();
        //        qDebug() << httpRequest->getStatusText();
        //        httpRequest->deleteLater();
        //    });

        //    httpRequest->open("POST", QUrl("http://music.163.com/api/search/get"));

        //    QByteArray data("?limit=60&offset=0&s=%E7%88%B1&total=true&type=1");
        //    httpRequest->setRequestHeader("Referer", "http://music.163.com/search/");
        //    httpRequest->setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

        //    httpRequest->setTimeout(300000);             // 30s

        //    httpRequest->send(QString(data));

        HttpRequest { id: httpRequest }

        RowLayout {
            anchors.fill: parent

            Text {
                id: showText
                text:"show text"
            }

            Button {
                text: "test"
                onClicked: {
                    search("爱")
                }
            }
            Button {
                text: "HttpRequestFactory"
                onClicked: {
                    search2("爱");
                }
            }
            Button {
                text: "testGC"
                onClicked: {
                    testGC();
                }
            }
        }
    }

    function testGC() {
        for(var i = 0; i< 500; i++) {
            HttpRequestFactory.create();
        }
    }

    function search2(text) {
        var data = {
            limit: 60,
            offset: 0,
            s: text,
            total: true,
            type: 1,
        };

        var xhr = HttpRequestFactory.create();
        xhr.finished.connect(function(){
            xhr.finished.disconnect(arguments.callee);
            console.log(xhr.readyState)
            console.log("http status:", xhr.status);
            console.log("http status text:", xhr.statusText);
            var jsonObj = JSON.parse(xhr.responseText);
            for(var iter in jsonObj) {
                console.log(iter, jsonObj[iter]);
            }
        });

        xhr.error.connect(function(){
            xhr.error.disconnect(arguments.callee);
            console.log(xhr.statusText)
        });

        xhr.timeout.connect(function(){
            xhr.timeout.disconnect(arguments.callee);
            console.log("time out")
        });

        //httpRequest.open("GET", "https://api.github.com/zen")
        xhr.open("POST","https://music.163.com/api/search/get");

        xhr.setTimeout(3 * 1000);      // 30 s
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xhr.setRequestHeader("Referer", "http://music.163.com/search/");

        xhr.send(urlQuery(data));
    }

    function search(text) {
        var data = {
            limit: 60,
            offset: 0,
            s: text,
            total: true,
            type: 1,
        };

        httpRequest.clear();

        httpRequest.finished.connect(function(){
            httpRequest.finished.disconnect(arguments.callee);
            console.log(httpRequest.readyState)
            console.log("http status:", httpRequest.status);
            console.log("http status text:", httpRequest.statusText);
            var jsonObj = JSON.parse(httpRequest.responseText);
            for(var iter in jsonObj) {
                console.log(iter, jsonObj[iter]);
            }
        });

        httpRequest.error.connect(function(){
            httpRequest.error.disconnect(arguments.callee);
            console.log(httpRequest.statusText)
        });

        httpRequest.timeout.connect(function(){
            httpRequest.timeout.disconnect(arguments.callee);
            console.log("time out")
        });

        //httpRequest.open("GET", "https://api.github.com/zen")
        httpRequest.open("POST","https://music.163.com/api/search/get");

        httpRequest.setTimeout(3 * 1000);      // 30 s
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        httpRequest.setRequestHeader("Referer", "http://music.163.com/search/");

        httpRequest.send(urlQuery(data));
    }

    function urlQuery(jsonObject) {
        var query = "";
        var i = 0;
        for(var iter in jsonObject) {

            if(i>0) {
                query += "&";
            }
            query += iter +"=" + encodeURI(jsonObject[iter]);
            i++;
        }
        console.log("url query:", query);
        return query;
    }
}
