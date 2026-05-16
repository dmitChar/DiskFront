import QtQuick
import QtQuick.Controls

import QtMultimedia

// Видеоплеер с авторизацией через LocalMediaProxy.
//
// QMediaPlayer → http://127.0.0.1:<port>/<fileId>
// LocalMediaProxy добавляет JWT и пробрасывает Range-запросы к бэкенду.
//
// Использование:
//   VideoViewer { fileId: "42"; fileName: "movie.mp4" }
Rectangle
{
    id: root
    color: "#000"
    radius: AppTheme.radiusMd
    clip: true

    property string fileId: ""
    property string fileName: ""

    onFileIdChanged: load()

    Component.onCompleted: if (fileId !== "") load()

    function load()
    {
        if (!MediaModel.proxyReady)
            return
        player.source = MediaModel.videoUrl(fileId)
        player.play()
    }

    //----- MediaPlayer --------

    MediaPlayer
    {
        id: player
        videoOutput: videoOutput
        audioOutput: AudioOutput {}

        onErrorOccurred: (err, msg) =>
        {
            errorText.text = "Ошибка: " + msg
            errorOverlay.visible = true
        }

        onPlaybackStateChanged:
        {
            errorOverlay.visible = false
        }
    }

    VideoOutput
    {
        id: videoOutput
        anchors.fill: parent
    }

    // ---- Оверлей управления(виден при наведении) ------
    Rectangle
    {
        id: controls
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: 64
        color: Qt.rgba(0, 0, 0, 0.65)
        //visible: controlsTimer.running || playPauseBtnMa.containsMouse
        visible: true

        // Прогресс бар
        Rectangle
        {
            id: progressBar
            anchors {left: parent.left; right: parent.right; top: parent.top; topMargin: 0}
            height: 3
            color: Qt.rgba(1, 1, 1, 0.2)


            // Загруженная часть (буфер)
            Rectangle
            {
                width: player.bufferedTimeRange && player.duration > 0
                    ? parent.width * player.bufferedTimeRange.latestTime() / player.duration
                    : 0
                height: parent.height
                color: Qt.rgba(1,1,1,0.3)
                Behavior on width { NumberAnimation { duration: 200 } }
            }

            // Воспроизведенная часть
            Rectangle
            {
                id: progressFill
                width: player.duration > 0 ? parent.width * player.position / player.duration : 0
                height: parent.height
                color: AppTheme.accent
            }

            // Кнопка позиционирования
            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: function(mouse)
                {
                    player.position = player.duration * (mouse.x / width)
                }
            }
        }

        Row
        {
            anchors { left: parent.left; leftMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter; verticalCenterOffset: 4 }
            spacing: AppTheme.sp12

            // Play / pause
            Rectangle
            {
                width: 32; height: 32; radius: 16; color: Qt.rgba(1,1,1,0.15)
                Text
                {
                    anchors.centerIn: parent
                    text: player.playbackState === MediaPlayer.PlayingState ? "play" : "stop"
                    font.pixelSize: 12; color: "white"
                }

                MouseArea
                {
                    id: playPauseBtnMa
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        player.playbackState === MediaPlayer.PlayingState ? player.pause() : player.play()
                    }
                }
            }

            // Время
            Text
            {
                anchors.verticalCenter: parent.verticalCenter
                text: root.fileName; font.family: AppTheme.fontFamily
                font.pixelSize: AppTheme.fontSm; color: Qt.rgba(1,1,1, 0.7)
                elide: Text.ElideMiddle; width: 200
            }
        }

        // Громкость + полноэкранный режим
        Row
        {
            anchors { right: parent.right; rightMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter; verticalCenterOffset: 4 }
            spacing: AppTheme.sp8

            Text
            {
                text: player.audioOutput && player.audioOutput.muted ? "🔇" : "🔊"
                font.pixelSize: 16; color: "white"
                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: if (player.audioOutput) player.audioOutput.muted = !player.audioOutput.muted
                }
            }

            Slider
            {
                width: 80
                height: 20
                from: 0; to: 1
                value: player.audioOutput ? player.audioOutput.volume : 1
                onMoved: if (player.audioOutput) player.audioOutput.volume = value
            }
        }
    }

    // Показ управления при движении мыши
    // MouseArea
    // {
    //     anchors.fill: parent
    //     hoverEnabled: true
    //     onPositionChanged: controlsTimer.restart()
    //     onClicked: player.playbackState === MediaPlayer.PlayingState ? player.pause() : player.play()

    // }

    Timer
    {
        id: controlsTimer
        interval: 2000
        running: true
        onTriggered: {}
    }

    // ---- Буферизация --------
    Rectangle
    {
        anchors.centerIn: parent
        visible: player.mediaStatus === MediaPlayer.BufferingMedia || player.mediaStatus === MediaPlayer.LoadingMedia
        width: 60; height: 60
        radius: 30; color: Qt.rgba(0,0,0,0.5)

        Rectangle
        {
            anchors.centerIn: parent
            width: 36; height: 36; radius: 18
            color: "transparent"; border.color: "white"; border.width: 3

            Rectangle
            {
                width: 12; height: 12; radius: 6; color: "white"
                anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: -2 }
                RotationAnimator on rotation
                {
                    from: 0; to: 360; duration: 800; loops: Animation.Infinite
                    running: parent.parent.parent.visible
                }
            }
        }
    }

    // --- Сообщения об ошибках ------
    Rectangle
    {
        id: errorOverlay; anchors.fill: parent
        color: Qt.rgba(0,0,0,0.8)
        visible: false
        Column
        {
            anchors.centerIn: parent
            spacing: AppTheme.sp12
            Text
            {
                id: errorText
                font.family: AppTheme.fontFamily
                font.pixelSize: AppTheme.fontMd
                color: "white"
            }

            Text
            {
                anchors.horizontalCenter: parent.horizontalCenter; text: "Повторить"
                font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.accent
                MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: { errorOverlay.visible = false; root.load() } }
            }
        }
    }

    // Вспомогательные функции
    function pad(n)
    {
        return n < 10 ? "0" + n : "" + n
    }

    function formatTime(ms)
    {
        var s = Math.floor(ms / 1000)
        var m = Math.floor(s / 60); s = s % 60
        var h = Math.floor(m / 60); m = m % 60
        if (h > 0)
            return pad(h) + ":" + pad(m) + ":" + pad(s)
        return pad(m) + ":" + pad(s)
    }
}
