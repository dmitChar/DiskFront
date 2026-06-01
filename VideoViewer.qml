import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
    property bool controlVisible: true

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
        audioOutput: AudioOutput {
            id: audioOutput
            volume: volumeSlider.value
        }

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
        fillMode: VideoOutput.PreserveAspectFit
    }

    //----------------------------------------
    // CLICK TO PLAY/PAUSE
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged:
        {
            root.controlVisible = true
            hideTimer.restart()
        }
        onClicked:
        {
            if (player.playbackState === MediaPlayer.PlayingState)
                player.pause()
            else player.play()
        }
    }

    //----------------------------------------
    // AUTO HIDE CONTROLS
    Timer
    {
        id: hideTimer
        interval: 2000
        onTriggered:
        {
            controlVisible = false
        }
    }

    //----------------------------------------
    // CONTROLS PANEL
    Rectangle
    {
        id: controlsPanel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 120
        color: "#66000000"

        visible: controlsVisible
        opacity: controlsVisible ? 1 : 0
        Behavior on opacity
        {
            NumberAnimation { duration: 180 }
        }

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 16

            // Seek bar
            Slider
            {
                id: seekBar
                Layout.fillWidth: true
                from: 0
                to: player.duration
                value: player.position

                onMoved:
                {
                    player.position = value
                }

                background: Rectangle
                {
                    implicitHeight: 6
                    radius: 3
                    color: "#33FFFFFF"

                    Rectangle
                    {
                        width: seekBar.visualPosition * parent.width
                        height: parent.height
                        radius: 3
                        color: "#00A3FF"
                    }
                }

                handle: Rectangle
                {
                    width: 18
                    height: 18
                    radius: 9
                    color: "white"
                    y: -6
                }
            }

            //----------------------------------------
            // BOTTOM ROW
            RowLayout
            {
                Layout.fillWidth: true
                spacing: 18
                implicitHeight: 40

                //Play Button
                RoundButton
                {
                    width: 46
                    height: 46
                    text: player.playbackState === MediaPlayer.PlayingState ? "❚❚" : "▶"
                    font.pixelSize: 20

                    onClicked:
                    {
                        if (player.playbackState === MediaPlayer.PlayingState)
                            player.pause()
                        else player.play()
                    }
                }
                // Time
                Text
                {
                    color: "white"
                    text: formatTime(player.position) + " / " + formatTime(player.duration)
                    font.pixelSize: 15
                }
                Item
                {
                    Layout.fillWidth: true
                }

                // Volume
                Text
                {
                    text: audioOutput.muted ? "🔇" : "🔊"
                    color: "white"
                    font.pixelSize: 18
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            audioOutput.muted = !audioOutput.muted
                            if (volumeSlider.value === 0)
                                volumeSlider.value = volumeSlider.lastValue
                            else volumeSlider.value = 0
                        }
                    }
                }
                Slider
                {
                    property real lastValue: 0.5
                    id: volumeSlider
                    width: 100
                    from: 0
                    to: 1
                    value: 0.5

                    onValueChanged:
                    {
                        if (value !== 0)
                            lastValue = value
                    }

                }

                // Fullscreen
                RoundButton
                {
                    text: "⛶"
                    font.pixelSize: 20
                    onClicked:
                    {
                        if (root.visibility === Window.FullScreen)
                            root.showNormal()
                        else
                            root.showFullScreen()
                    }
                }
            }
        }

        Rectangle
        {
            anchors.fill: parent
            visible: player.mediaStatus === MediaPlayer.LoadingMedia || player.mediaStatus === MediaPlayer.BufferingMedia
            color: "#55000000"
            BusyIndicator
            {
                anchors.centerIn: parent
                running: true
            }
        }

        Shortcut
        {
            sequence: "Space"

            onActivated:
            {
                if (player.playbackState === MediaPlayer.PlayingState)
                    player.pause()
                else
                    player.play()
            }
        }

        Shortcut
        {
            sequence: "Left"

            onActivated:
            {
                player.position -= 5000
            }
        }

        Shortcut
        {
            sequence: "Right"

            onActivated:
            {
                player.position += 5000
            }
        }
    }


    //-----------------------------------------

    // ---- Оверлей управления(виден при наведении) ------
    // Rectangle
    // {
    //     id: controls
    //     anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
    //     height: 64
    //     color: Qt.rgba(0, 0, 0, 0.65)
    //     //visible: controlsTimer.running || playPauseBtnMa.containsMouse
    //     visible: true

    //     // Прогресс бар
    //     Rectangle
    //     {
    //         id: progressBar
    //         anchors {left: parent.left; right: parent.right; top: parent.top; topMargin: 0}
    //         height: 3
    //         color: Qt.rgba(1, 1, 1, 0.2)


    //         // Загруженная часть (буфер)
    //         Rectangle
    //         {
    //             width: player.bufferedTimeRange && player.duration > 0
    //                 ? parent.width * player.bufferedTimeRange.latestTime() / player.duration
    //                 : 0
    //             height: parent.height
    //             color: Qt.rgba(1,1,1,0.3)
    //             Behavior on width { NumberAnimation { duration: 200 } }
    //         }

    //         // Воспроизведенная часть
    //         Rectangle
    //         {
    //             id: progressFill
    //             width: player.duration > 0 ? parent.width * player.position / player.duration : 0
    //             height: parent.height
    //             color: AppTheme.accent
    //         }

    //         // Кнопка позиционирования
    //         MouseArea
    //         {
    //             anchors.fill: parent
    //             cursorShape: Qt.PointingHandCursor
    //             onClicked: function(mouse)
    //             {
    //                 player.position = player.duration * (mouse.x / width)
    //             }
    //         }
    //     }

    //     Row
    //     {
    //         anchors { left: parent.left; leftMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter; verticalCenterOffset: 4 }
    //         spacing: AppTheme.sp12

    //         // Play / pause
    //         Rectangle
    //         {
    //             width: 32; height: 32; radius: 16; color: Qt.rgba(1,1,1,0.15)
    //             Text
    //             {
    //                 anchors.centerIn: parent
    //                 text: player.playbackState === MediaPlayer.PlayingState ? "play" : "stop"
    //                 font.pixelSize: 12; color: "white"
    //             }

    //             MouseArea
    //             {
    //                 id: playPauseBtnMa
    //                 anchors.fill: parent
    //                 hoverEnabled: true
    //                 cursorShape: Qt.PointingHandCursor
    //                 onClicked:
    //                 {
    //                     player.playbackState === MediaPlayer.PlayingState ? player.pause() : player.play()
    //                 }
    //             }
    //         }

    //         // Время
    //         Text
    //         {
    //             anchors.verticalCenter: parent.verticalCenter
    //             text: root.fileName; font.family: AppTheme.fontFamily
    //             font.pixelSize: AppTheme.fontSm; color: Qt.rgba(1,1,1, 0.7)
    //             elide: Text.ElideMiddle; width: 200
    //         }
    //     }

    //     // Громкость + полноэкранный режим
    //     Row
    //     {
    //         anchors { right: parent.right; rightMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter; verticalCenterOffset: 4 }
    //         spacing: AppTheme.sp8

    //         Text
    //         {
    //             text: player.audioOutput && player.audioOutput.muted ? "🔇" : "🔊"
    //             font.pixelSize: 16; color: "white"
    //             MouseArea
    //             {
    //                 anchors.fill: parent
    //                 cursorShape: Qt.PointingHandCursor
    //                 onClicked: if (player.audioOutput) player.audioOutput.muted = !player.audioOutput.muted
    //             }
    //         }

    //         Slider
    //         {
    //             width: 80
    //             height: 20
    //             from: 0; to: 1
    //             value: player.audioOutput ? player.audioOutput.volume : 1
    //             onMoved: if (player.audioOutput) player.audioOutput.volume = value
    //         }
    //     }
    // }

    // // Показ управления при движении мыши
    // // MouseArea
    // // {
    // //     anchors.fill: parent
    // //     hoverEnabled: true
    // //     onPositionChanged: controlsTimer.restart()
    // //     onClicked: player.playbackState === MediaPlayer.PlayingState ? player.pause() : player.play()

    // // }

    // Timer
    // {
    //     id: controlsTimer
    //     interval: 2000
    //     running: true
    //     onTriggered: {}
    // }

    // // ---- Буферизация --------
    // Rectangle
    // {
    //     anchors.centerIn: parent
    //     visible: player.mediaStatus === MediaPlayer.BufferingMedia || player.mediaStatus === MediaPlayer.LoadingMedia
    //     width: 60; height: 60
    //     radius: 30; color: Qt.rgba(0,0,0,0.5)

    //     Rectangle
    //     {
    //         anchors.centerIn: parent
    //         width: 36; height: 36; radius: 18
    //         color: "transparent"; border.color: "white"; border.width: 3

    //         Rectangle
    //         {
    //             width: 12; height: 12; radius: 6; color: "white"
    //             anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: -2 }
    //             RotationAnimator on rotation
    //             {
    //                 from: 0; to: 360; duration: 800; loops: Animation.Infinite
    //                 running: parent.parent.parent.visible
    //             }
    //         }
    //     }
    // }

    // // --- Сообщения об ошибках ------
    // Rectangle
    // {
    //     id: errorOverlay; anchors.fill: parent
    //     color: Qt.rgba(0,0,0,0.8)
    //     visible: false
    //     Column
    //     {
    //         anchors.centerIn: parent
    //         spacing: AppTheme.sp12
    //         Text
    //         {
    //             id: errorText
    //             font.family: AppTheme.fontFamily
    //             font.pixelSize: AppTheme.fontMd
    //             color: "white"
    //         }

    //         Text
    //         {
    //             anchors.horizontalCenter: parent.horizontalCenter; text: "Повторить"
    //             font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.accent
    //             MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: { errorOverlay.visible = false; root.load() } }
    //         }
    //     }
    // }

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
