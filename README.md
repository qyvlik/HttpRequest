# HttpRequest

HttpRequest use Qt Network module that register in qml env.Such as XMLHttpRequest.

`HttpRequest` 使用 Qt 的网络模块编写。通过注册到 QML 中，就可以像使用 `XMLHttpRequest` 那样使用了。

第一种使用方法是，直接实例化 `HttpRequest`。

```
        HttpRequest {
            id: httpRequest
            onReadyStateChanged: {
                console.log(httpRequest.responseText);
                console.log("http code:", httpRequest.status);
            }
        }
            Button {
                text: "test"
                onClicked: {
                    httpRequest.open("GET","https://api.github.com/zen");
                    httpRequest.send();
                }
            }
```

第二种类似于 `XMLHttpRequest`：

```
var httpRequest = HttpRequestFactory.create();

httpRequest.readyStateChanged.connect(function() {
    console.log(httpRequest.responseText);
    showText.text = httpRequest.responseText;
});
httpRequest.open("GET", "https://api.github.com/zen");
httpRequest.send();
```

