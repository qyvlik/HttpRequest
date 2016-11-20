import QtQuick 2.0
import QtMultimedia 5.5
import QtQuick.Controls 1.4

Item {
    property url playUrl: ""

    Audio {
        id: playMusic
        source: playUrl
    }

    function play() {
        playMusic.play()
    }

    function pause() {
        playMusic.pause();
    }

    Button {
        text: "play"
        anchors.centerIn: parent
        onClicked: {
            console.log("play url", playMusic.source)
            playMusic.playbackState == MediaPlayer.PlayingState
                    ? playMusic.pause()
                    : playMusic.play()
        }
    }
}

