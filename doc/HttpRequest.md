# HttpRequest

> Network 1.0

> Inherits: QtObject

## 属性

+ `responseText` : `string`

+ `readyState` : `HttpRequest::State`

+ `status` : `HttpRequest::NetworkStatus`

+ `statusText` : `string`

## 信号

+ `void started();`

+ `void downloadProgress(qint64 bytesReceived, qint64 bytesTotal)`

+ `void uploadProgress(qint64 bytesSent, qint64 bytesTotal)`

+ `void finished()`

+ `void error()`

+ `void timeout()`

## 方法

+ `void abort()`

+ `void clear()`

+ `void setTimeout(qint32 time)`

+ `int getTimeout() const`

+ `void setRequestHeader(const QJsonObject& headers)`

+ `void setRequestHeader(const QByteArray &headerName, const QByteArray &value)`

+ `void setRequestCookies(const QJsonObject &cookies)`

+ `QString getRequestHeader(const QString& headerName) const`

+ `QJsonArray getAllResponseHeader() const`

+ `void open(const QString& method, const QUrl& url, bool async = true)`

+ `void send(const QString& data = QString(""))`

## 详细描述

`HttpRequest` 使用 Qt 的**网络模块**编写。通过注册到 QML 中，就可以像使用 `XMLHttpRequest` 那样使用了。

## 属性文档

> ### `responseText` : `string`

进行网络请求后获得的内容。

> ### `readyState` : `HttpRequest::State`

| 枚举值 | 描述 |
|:---:|:--:|
| UnStart | 未开始请求的状态，以及在调用 `clear()` 后所处的状态 |
| Loading | 正在请求远端数据 |
| Error | 请求远端数据错误，错误提示查看 `statusText` |
| Finished | 请求结束后的状态 |
| TimeOut | 请求超时 |

> ### `status` : `HttpRequest::NetworkStatus`

对比查看 [`QNetworkReply::NetworkError`](http://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum)。

> ### `statusText` : `string`

对比查看 [`QNetworkReply::errorString()`](http://doc.qt.io/qt-5/qiodevice.html#errorString)。

## 信号文档

> ### `void started();`

在进行网络请求时触发。

> ### `void downloadProgress(qint64 bytesReceived, qint64 bytesTotal)`

在拉取数据时触发。详情查看 [`QNetworkReply::downloadProgress`](http://doc.qt.io/qt-5/qnetworkreply.html#downloadProgress)。

> ### `void uploadProgress(qint64 bytesSent, qint64 bytesTotal)`

在上传数据时触发。详情查看 [`QNetworkReply::uploadProgress`](http://doc.qt.io/qt-5/qnetworkreply.html#uploadProgress)。

> ### `void finished()`

请求完成时触发。此信号触发时，`error()` 以及 `timeout()` 不会再触发。

```
xhr.finished.connect(function(){
    xhr.finished.disconnect(arguments.callee);
    console.log(xhr.readyState)
    console.log("http status:", xhr.status);
    console.log(xhr.responseText);
});

xhr.open("GET","http://www.baidu.com");
xhr.send();
```

> ### `void error()`

请求错误时触发。此信号触发时，`finished()` 以及 `timeout()` 不会再触发。

```
...

xhr.error.connect(function(){
    xhr.error.disconnect(arguments.callee);
    console.log(xhr.statusText)
});

...
```

> ### `void timeout()`

响应超时时触发。此信号触发时，`error()` 以及 `finished()` 不会再触发。

```
...
xhr.timeout.connect(function(){
    xhr.timeout.disconnect(arguments.callee);
    console.log("time out")
});
...
```

查看 ` setTimeout(time)` 和 `getTimeout()`。

## 方法文档

> ### `void abort()`

取消网络请求。

> ### `void clear()`

清空请求设置，例如请求网址，请求头等。

> ### `void setTimeout(qint32 time)`

设置超时时间。仅当 `readyState` 不处于 `HttpRequest.Loading` 时设置有效。就是在 `send()` 之前设置有效。单位为毫秒。

```
xhr.open("GET","http://www.baidu.com");
xhr.setTimeout(3 * 1000);      // 30 s
xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
xhr.send();
```

> ### `int getTimeout() const`

获取最近一次设置的超时时间。

> ### `void setRequestHeader(const QJsonObject& headers)`

一次性设置多个请求头。只有在 `readyState` 不处于 `Loading` 时，设置有效。

```
var headers = {
    'Accept': '*/*',
    'Accept-Encoding': 'gzip,deflate,sdch',
    'Accept-Language': 'zh-CN,zh;q=0.8,gl;q=0.6,zh-TW;q=0.4',
    'Connection': 'keep-alive',
    'Content-Type': 'application/x-www-form-urlencoded',
    'Host': 'music.163.com',
    'Referer': 'http://music.163.com/search/',
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.152 Safari/537.36'
};

xhr.setRequestHeader(headers);
```

> ### `void setRequestHeader(const QByteArray &headerName, const QByteArray &value)`

设置请求头。只有在 `readyState` 不处于 `Loading` 时，设置有效。

```
xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
```

> ### `void setRequestCookies(const QJsonObject &cookies)`

为请求头设置 `Cookie`。

> **注意**：如果通过 `setRequestHeader` 或者 `setRequestCookies` 为请求头设置了 `Cookie`，就不会使用上一次网页返回的默认的 `Cookie`。

```
var cookies = {
    'appver': '1.5.2',
};

xhr.setRequestCookies(cookies);
```

> ### `QString getRequestHeader(const QString& headerName) const`

获取对应的请求头。

> ### `QJsonArray getAllResponseHeader() const`

获取所有响应头。格式如下：

```
[
  {"header":"value"},
  {"header":"value"},
]
```

> ### `void open(const QString& method, const QUrl& url, bool async = true)`

准备一次网络请求，设置请求方法，请求地址，默认也只支持异步请求。

> **注意**：如果方法为 `POST` 时，必须设置 `Content-Type` 请求头。

```
xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
```

支持的请求方式为 `GET`，`POST`，`DELETE`，`PUT`，`HEAD`。

> ### `void send(const QString& data = QString(""))`

发送网络请求，如果请求方法为 `POST`，`PUT` 等方法，给 `send` 传入数据。

```
var url = "";
xhr.open("POST",url);
xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
xhr.send("data");
```
