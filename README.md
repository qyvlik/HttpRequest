# HttpRequest

`HttpRequest` use Qt **Network module** that register in qml env. Such as `XMLHttpRequest`.

`HttpRequest` 使用 Qt 的**网络模块**编写。通过注册到 QML 中，就可以像使用 `XMLHttpRequest` 那样使用了。

## Use in QML

第一种使用方法是，直接实例化 `HttpRequest`。

```
HttpRequest { id: httpRequest }
function doGet() {

    httpRequest.clear();

    httpRequest.finished.connect(function(){
        httpRequest.finished.disconnect(arguments.callee);
        console.log(httpRequest.responseText.length);
        console.log(httpRequest.responseText);
        // 如果 httpRequest.responseText 内容太长，console.log() 就不会再控制台打印
    });

    httpRequest.error.connect(function(){
        httpRequest.error.disconnect(arguments.callee);
        console.log(httpRequest.statusText);
    });

    httpRequest.timeout.connect(function(){
        httpRequest.timeout.disconnect(arguments.callee);
        console.log("time out");
    });

    httpRequest.open("GET", "https://api.github.com/zen");

    httpRequest.setTimeout(3 * 1000);      // 3 s
    httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    httpRequest.send();
}
```

> **注意**：实例化的 `HttpRequest` 可以使用多次，但是每次重新使用前，务必使用 `clear()` 函数，这个函数会取消上一次还未完成的请求和请求设置。

第二种类似于 `XMLHttpRequest`：

```
var xhr = HttpRequestFactory.create();

xhr.finished.connect(function(){
    xhr.finished.disconnect(arguments.callee);
    console.log(httpRequest.responseText.length);
    console.log(httpRequest.responseText);
    // 如果 httpRequest.responseText 内容太长，console.log() 就不会再控制台打印
});

xhr.error.connect(function(){
    xhr.error.disconnect(arguments.callee);
    console.log(xhr.statusText);
});

xhr.timeout.connect(function(){
    xhr.timeout.disconnect(arguments.callee);
    console.log("time out");
});

httpRequest.open("GET", "https://api.github.com/zen");

xhr.setTimeout(3 * 1000);      // 3 s

xhr.send();
```

`HttpRequestFactory.create();` 构建的 `HttpRequest` 会在适当时候被自动回收。

## Use In C++

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

httpRequest->setTimeout(30 * 1000);             // 30s

httpRequest->send(QString(data));
```

