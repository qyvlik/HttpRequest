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


        HttpRequest {
            id: httpRequest
            onReadyStateChanged: {
                console.log(httpRequest.responseText);
                console.log("http code:", httpRequest.status);
            }
        }

        RowLayout {
            anchors.fill: parent

            Text {
                id: showText
                text:"show text"
            }


            Button {
                text: "test"
                onClicked: {
                    httpRequest.open("GET","https://api.github.com/zen");
                    httpRequest.send();
                }
            }

            Button {
                text:"httpRequest"
                onClicked: {
                    try {
                        var httpRequest = HttpRequestFactory.create();

                        httpRequest.readyStateChanged.connect(function() {
                            console.log(httpRequest.responseText);
                            showText.text = httpRequest.responseText;
                        });
                        httpRequest.open("GET", "https://api.github.com/zen");
                        httpRequest.send();

                    } catch(e) {
                        console.log(e);
                    }
                }
            }

            Button {
                text:"XMLHttpRequest"
                onClicked: {
                    var xhr = new XMLHttpRequest;
                    xhr.onreadystatechange = function() {
                        if(xhr.readyState == xhr.DONE) {
                            console.log(xhr.responseText);
                        }
                    }
                    xhr.open("GET", "https://api.github.com/zen");
                    xhr.send();
                }
            }
        }
    }
}
