import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtMultimedia 5.5

import Network 1.0

import "Component"

ApplicationWindow {

    id: window
    title: qsTr("Hello World")
    width: 360
    height: 640
    visible: true

    // HttpRequest {}

    NetEasyMusicSearchPage {
        anchors.fill: parent
    }
}
