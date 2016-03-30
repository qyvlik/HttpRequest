# HttpRequestFactory

> Network 1.0

> Inherits: QtObject

## 方法

+ `HttpRequest* create()`

## 详细描述

用于动态创建 `HttpRequest` 的单例工厂。

## 方法文档

> `HttpRequest* create()`

用于动态创建 `HttpRequest`。

```
var xhr = HttpRequestFactory.create();
xhr.finished.connect(function(){
    xhr.finished.disconnect(arguments.callee);
    console.log(xhr.responseText);
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
xhr.send(urlQuery(data));
```
