// import QtQuick 2.9
// import QtQuick.Controls 2.2
// import QtQuick.Layouts 1.3
// import QtMultimedia 5.9

// import AppTheme 1.0

// // Видеоплеер с авторизацией через LocalMediaProxy.
// //
// // QMediaPlayer → http://127.0.0.1:<port>/<fileId>
// // LocalMediaProxy добавляет JWT и пробрасывает Range-запросы к бэкенду.
// //
// // Использование:
// //   VideoViewer { fileId: "42"; fileName: "movie.mp4" }
// Rectangle
// {
//     id: root
//     color: "#000"
//     radius: AppTheme.radiusMd
//     clip: true

//     property string fileId: ""
//     property string fileName: ""

//     property int reconnectAttempts: 0
//     property int maxReconnectAttempts: 10
//     property int reconnectDelayMs: 1500
//     property bool userPaused: false
//     property bool seekingInProgress: false
//     property real maxBufferProgress: 0.0

//     onFileIdChanged:
//     {
//         load()
//     }

//     Component.onCompleted:
//     {
//         if (fileId !== "")
//             load()
//     }

//     function load()
//     {

//         if (!MediaModel.proxyReady())
//             return
//         player.source = MediaModel.videoUrl(fileId)
//         //player.source = "file:///home/administrator/Desktop/Тест файлы/1.mp4"
//         console.log("Start playing for " + MediaModel.videoUrl(fileId))
//         player.play()
//     }

//     //----- MediaPlayer --------
//     MediaPlayer
//     {
//         id: player
//         autoPlay: false
//         loops: 1


//         notifyInterval: 200


//         onError:
//         {
//             console.error("MediaPlayer error:", error, errorString)
//         }

// //        onStatusChanged:
// //        {
// //            console.log("Статус стрима:", status, "BufferProgress: ", bufferProgress)
// //            switch (status)
// //            {
// //                case MediaPlayer.Loading:
// //                    statusLabel.text = "Подключение к серверу..."
// //                    bufferingIndicator.visible = true
// //                    reconnectAttempts = 0
// //                    break

// //                case MediaPlayer.Buffering:
// //                    if (!seekingInProgress)
// //                    {
// //                        statusLabel.text = "Буферизация "+ (bufferProgress * 100).toFixed(0) + "%"
// //                        bufferingIndicator.visible = true
// //                    }
// //                    break

// //                case MediaPlayer.Loaded:
// //                    console.log("Медиа загружена, длительность: ", duration, "мс")
// //                    statusLabel.text = "Готово к воспроизведению"
// //                    bufferingIndicator.visible = false
// //                    maxBufferProgress = 1.0

// //                    if (!userPaused)
// //                        player.play()
// //                    break;

// //                case MediaPlayer.Stalled:
// //                    console.warn("Воспроизведение остановлено: недостаточно данных")
// //                    statusLabel.text = "Ожидание данных"
// //                    bufferingIndicator.visible = true
// //                    break
// //            }
// //        }
//     }

//     VideoOutput
//     {
//         id: videoOutput
//         anchors.fill: parent
//         fillMode: videoOutput.PreserveAspectFit
//         source: player

//         Rectangle
//         {
//             id: bufferingIndicator
//             anchors.centerIn: parent
//             width: 160
//             height: 80
//             radius: 10
//             color: Qt.rgba(0,0,0,0.8)
//             visible: false

//             ColumnLayout
//             {
//                 anchors.centerIn: parent
//                 spacing: 8

//                 BusyIndicator
//                 {
//                     Layout.alignment: Qt.AlignHCenter
//                     running: bufferingIndicator.visible
//                     width: 40
//                     height: 40
//                 }

//                 Label
//                 {
//                     id: statusLabel
//                     Layout.alignment: Qt.AlignHCenter
//                     text: "Загрузка..."
//                     color: "white"
//                     font.pixelSize: 12
//                     horizontalAlignment: Text.AlignHCenter
//                 }
//             }
//         }
//     }

//     // ---- Оверлей управления(виден при наведении) ------
//     Rectangle
//     {
//         id: controls
//         anchors { left: parent.left; right: parent.right; bottom: parent.bottom; bottomMargin: 30}
//         height: 64
//         color: Qt.rgba(0, 0, 0, 0.65)
//         visible: true //controlsTimer.running || playPauseBtnMa.containsMouse

//         ColumnLayout
//         {
//             anchors.fill: parent
//             anchors.margins: 10
//             spacing: 5

//             // Ползунок перемотки
//             RowLayout
//             {
//                 Layout.fillWidth: true
//                 spacing: 5

//                 // Текущее время
//                 Label
//                 {
//                     text: formatTime(player.position)
//                     color: "white"
//                     font.pixelSize: AppTheme.sp12
//                 }

//                 // Ползунок прогресса с индикатором буфера
//                 Slider
//                 {
//                     id: positionSlider
//                     Layout.fillWidth: true
//                     from: 0
//                     to: player.duration > 0 ? player.duration : 1
//                     value: player.position
//                     enabled: player.seekable

//                     // Фон слайдера для буферизованной части
//                     background: Rectangle
//                     {
//                         x: positionSlider.leftPadding
//                         y: positionSlider.topPadding + positionSlider.availableHeight / 2 - height / 2
//                         implicitWidth: 200
//                         implicitHeight: 6
//                         width: positionSlider.availableWidth
//                         height: implicitHeight
//                         radius: 3
//                         color: "#333333"

//                         // Прогресс буфера
//                         Rectangle
//                         {
//                             width: positionSlider.visualPosition * parent.width
//                             height: parent.height
//                             color: "#555555"
//                             radius: 3
//                         }

//                         // Прогресс воспроизведения
//                         Rectangle
//                         {
//                             width: (player.bufferProgress > 0 ? player.bufferProgress : 0) * parent.width
//                             height: parent.height
//                             color: "#228833"
//                             radius: 3
//                         }
//                     }

//                     handle: Rectangle
//                     {
//                         x: positionSlider.leftPadding + positionSlider.visualPosition * (positionSlider.availableWidth - width)
//                         y: positionSlider.topPadding + positionSlider.availableHeight / 2 - height / 2
//                         implicitHeight: 14
//                         implicitWidth: 14
//                         radius: 7
//                         color: positionSlider.pressed ? "#f0f0f0" : "#ffffff"
//                         border.color: "#bdbebf"
//                     }
//                     onMoved: player.seek(value)
//                 }

//                 // Общая длительность
//                 Label
//                 {
//                     text: formatTime(player.duration)
//                     color: "white"
//                     font.pixelSize: 12
//                 }
//             }

//             // Нижний ряд - кнопки управления
//             RowLayout
//             {
//                 spacing: 10

//                 // Play / pause
//                 Rectangle
//                 {
//                     width: 32; height: 32; radius: 16; color: Qt.rgba(1,1,1, 0.15)
//                     Text
//                     {
//                         anchors.centerIn: parent
//                         text: player.playbackState === MediaPlayer.PlayingState ? "STOP" : "▶"
//                         font.pixelSize: 14; color: "white"
//                     }
//                     MouseArea
//                     {
//                         id: playPauseBtnMa
//                         anchors.fill: parent
//                         hoverEnabled: true
//                         cursorShape: Qt.PointingHandCursor
//                         onClicked:
//                         {
//                             player.playbackState === MediaPlayer.PlayingState ? player.pause() : player.play()
//                         }
//                     }
//                 }

//                 // Регулятор громкости
//                 RowLayout
//                 {
//                     Layout.fillWidth: true
//                     Layout.alignment: Qt.AlignCenter

//                     //Иконки
//                     Text
//                     {

//                     }

//                     Slider
//                     {
//                         id: volumeSlider
//                         from: 0.0
//                         to: 1
//                         value: player.volume
//                         width: 100
//                         onValueChanged: player.volume = value
//                     }
//                 }
//             }
//         }
//     }

// //    MouseArea
// //    {
// //        anchors.fill: parent
// //        hoverEnabled: true
// //        onPositionChanged: controlsTimer.restart()
// //        onClicked: player.playbackState === MediaPlayer.PlayingState ? player.pause() : player.play()
// //    }

//     Timer
//     {
//         id: controlsTimer
//         interval: 2000
//         running: true
//         onTriggered: {} // controls скрывается через visible
//     }


//     // Вспомогательные функции
//     function pad(n)
//     {
//         return n < 10 ? "0" + n : "" + n
//     }

//     function formatTime(ms)
//     {
//         var s = Math.floor(ms / 1000)
//         var m = Math.floor(s / 60); s = s % 60
//         var h = Math.floor(m / 60); m = m % 60
//         if (h > 0)
//             return pad(h) + ":" + pad(m) + ":" + pad(s)
//         return pad(m) + ":" + pad(s)
//     }
// }
