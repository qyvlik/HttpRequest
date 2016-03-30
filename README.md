# HttpRequest

HttpRequest use Qt Network module that register in qml env.Such as XMLHttpRequest.

`HttpRequest` 使用 Qt 的网络模块编写。通过注册到 QML 中，就可以像使用 `XMLHttpRequest` 那样使用了。

第一种使用方法是，直接实例化 `HttpRequest`。

```
HttpRequest { id: httpRequest }
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
        console.log(httpRequest.responseText.length)
        console.log(httpRequest.responseText)
    });

    httpRequest.error.connect(function(){
        httpRequest.error.disconnect(arguments.callee);
        console.log(httpRequest.statusText)
    });

    httpRequest.timeout.connect(function(){
        httpRequest.timeout.disconnect(arguments.callee);
        console.log("time out")
    });

    httpRequest.open("GET", "https://api.github.com/zen")

    httpRequest.setTimeout(3 * 1000);      // 30 s
    httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    httpRequest.send();
}
```

第二种类似于 `XMLHttpRequest`：

```
var xhr = HttpRequestFactory.create();

xhr.finished.connect(function(){
    xhr.finished.disconnect(arguments.callee);
    console.log(httpRequest.responseText.length)
    console.log(httpRequest.responseText)
});

xhr.error.connect(function(){
    xhr.error.disconnect(arguments.callee);
    console.log(xhr.statusText)
});

xhr.timeout.connect(function(){
    xhr.timeout.disconnect(arguments.callee);
    console.log("time out")
});

httpRequest.open("GET", "https://api.github.com/zen")

xhr.setTimeout(3 * 1000);      // 30 s

xhr.send(urlQuery(data));
```

`HttpRequestFactory.create();` 构建的 `HttpRequest` 会在适当时候被自动回收。

在 C++ 中使用：

```
HttpRequest* httpRequest = new HttpRequest();

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

QByteArray data("?limit=60&offset=0&s=%E7%88%B1&total=true&type=1");

httpRequest->setRequestHeader("Referer", "http://music.163.com/search/");
httpRequest->setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

httpRequest->open("POST", QUrl("http://music.163.com/api/search/get"));

httpRequest->setTimeout(300000);             // 30s

httpRequest->send(QString(data));
```

