import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtMultimedia 5.5

import Network 1.0

Item {
    id: root
    anchors.fill: parent
    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            TextField {
                id: inputText
                Layout.fillWidth: true
                Layout.rightMargin: 10
                Layout.leftMargin: 10
                onAccepted: {
                    root.search();
                }
            }

            Button {
                text: qsTr("search")
                onClicked: {
                    root.search();
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: view
                anchors.fill: parent
                clip: true
                property var searchsObject: ({});
                header: Rectangle {
                    width: parent.width
                    height: 60
                    Text {
                        anchors.centerIn: parent
                        elide: Text.ElideRight
                        text: try {"搜索结果条数：" + view.searchsObject["result"]["songCount"]}
                              catch(e){ "无搜索结果" }
                    }
                }

                delegate: Rectangle {
                    width: parent.width
                    height: 80
                    border.color: "#ccc"
                    border.width: 1

                    Row {
                        anchors.fill: parent

                        ColumnLayout {
                            width: parent.width * 0.9
                            height: parent.height
                            Text {
                                // 歌名
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                elide: Text.ElideRight
                                text: try {view.searchsObject["result"]["songs"][index]["name"]}
                                      catch(e){ "歌名" }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Text {
                                    // 艺术家/歌手
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    elide: Text.ElideRight
                                    text: try {view.searchsObject["result"]["songs"][index]["artists"][0]["name"]}
                                          catch(e){ "艺术家/歌手" }
                                }
                                Text {
                                    // 专辑
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    elide: Text.ElideRight
                                    text: try { view.searchsObject["result"]["songs"][index]["album"]["name"]}
                                          catch(e){ "专辑" }
                                }
                            }
                        }
                        Rectangle {
                            height: parent.height
                            width: parent.width * 0.1

                            color: "lightblue"
                            border.color: "#ccc"
                            border.width: 1
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    try {
                                        getSongUrl(view.searchsObject["result"]["songs"][index]["album"]["id"],
                                                   function(resultObject){
                                                       console.log(JSON.stringify(resultObject));
                                                       playbar.playUrl = resultObject["songs"][0]["mp3Url"];
                                                   });
                                    } catch(e) {
                                        console.log(e);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        PlayBar {
            id: playbar
            height: 60
            Layout.fillWidth: true
        }

    }

    function search() {
        netEasySearch(inputText.text, 1, 0, "true", 10, function(jsonObj){
            //console.log(JSON.stringify(jsonObj));
            view.searchsObject = jsonObj;
            view.model = jsonObj["result"]["songs"].length;
            try {
                console.log(jsonObj["result"]["songs"][0]["name"]);
            } catch(e) {
                console.log(e)
            }
        });
    }
    // s: 关键词
    // stype: 搜索单曲(1)，歌手(100)，专辑(10)，歌单(1000)，用户(1002)
    function netEasySearch(s, stype, offset, total, limit, callable) {
        stype = stype || 1;
        offset = offset || 0;
        total = total || "true";
        limit = limit || 60;
        var data = {
            "limit": limit,
            "offset": offset,
            "s": s,
            "total": total,
            "type": stype,
        };

        var xhr = HttpRequestFactory.create();
        xhr.finished.connect(function(){
            xhr.finished.disconnect(arguments.callee);
            callable(JSON.parse(xhr.responseText))
        });

        xhr.error.connect(function(){
            xhr.error.disconnect(arguments.callee);
            console.log(xhr.statusText)
        });

        xhr.timeout.connect(function(){
            xhr.timeout.disconnect(arguments.callee);
            console.log("time out")
        });

        xhr.open("POST","https://music.163.com/api/search/get");

        xhr.setTimeout(30 * 1000);      // 3 s
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xhr.setRequestHeader("Referer", "http://music.163.com/search/");

        xhr.send(urlQuery(data));
    }

    function getSongUrl(songid, callable) {
        callable = callable || function(resultObject){
            console.log(JSON.stringify(resultObject));
        }
        var url = "http://music.163.com/api/song/detail/?id=" + songid + "&ids=" + encodeURI("["+songid+"]");

        var xhr = HttpRequestFactory.create();

        xhr.finished.connect(function(){
            xhr.finished.disconnect(arguments.callee);
            callable(JSON.parse(xhr.responseText));
        });

        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xhr.setRequestHeader("Referer", "http://music.163.com/search/");

        xhr.open("GET", url);
        xhr.send();
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
        //console.log("url query:", query);
        return query;
    }
}

