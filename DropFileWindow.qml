import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item
{
    id: root
    anchors.fill: parent

    signal filesDropped(var urls)
    property bool dragging: false
    property var droppedFiles: []

    DropArea
    {
        anchors.fill: parent

        onEntered:
        {
            root.dragging = true
            root.droppedFiles = drag.urls
        }

        onExited:
        {
            root.dragging = false
        }

        onDropped:
        {
            root.dragging = false
            root.filesDropped(drop.urls)
        }
    }
    Rectangle
    {
        anchors.fill: parent
        visible: root.dragging
        color: "#88000000"
        opacity: root.dragging ? 1 : 0
        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 200
            }
        }
        Rectangle
        {
            width: 400
            height: 250

            anchors.centerIn: parent
            radius: 24
            color: "#222"
            border.width: 2
            border.color: "#66AFFF"
            opacity: 0.95

            ColumnLayout
            {
                anchors.centerIn: parent
                spacing: 20

                Text
                {
                    text: "⬆"
                    font.pixelSize: 64
                    color: "#66AFFF"
                    Layout.alignment: Qt.AlignHCenter
                }

                Text
                {
                    text: "Перетащите файлы сюда"
                    font.pixelSize: 24
                    font.bold: true
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                }

                Text
                {
                    text:
                    {
                        if (root.droppedFiles === 0)
                            return ""
                        return "Будет загружено: " + root.droppedFiles.length + " файл(ов)"
                    }
                    color: "#AAA"
                    font.pixelSize: 15
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            SequentialAnimation on scale
            {
                running: root.dragging
                loops: Animation.Infinite

                NumberAnimation
                {
                    from: 1
                    to: 1.03
                    duration: 1000
                }

                NumberAnimation
                {
                    from: 1.03
                    to: 1
                    duration: 1000
                }
            }
        }
    }
}
