import QtQuick 2.0
import QtQuick.Layouts 1.3

import AppTheme 1.0

Rectangle
{
    id: root
    property string iconName: "file"
    property int iconSize: 23
    property string fileName: "unknown"
    property string fileSize: "0"
    property string fileDate: "value"
    property string mimeType: ""
    property bool isDir: false
    property bool isShared: false
    property bool selected: false

    signal clicked()
    signal doubleClicked()
    signal rightClicked(real x, real y)

    Layout.fillWidth: true
    implicitHeight: 33
    radius: AppTheme.radiusSm

    color: selected ? AppTheme.accentLight : (rowMouseArea.containsMouse ? AppTheme.surfaceAlt : AppTheme.surface)
    border.color: selected ? AppTheme.accent : AppTheme.border
    border.width: selected ? 2 : 1

    Behavior on color
    {
        ColorAnimation { duration: AppTheme.durationFast}
    }

    RowLayout
    {
        anchors
        {
            fill: parent; leftMargin: 15; rightMargin: 15
        }

        //Иконка
        Image
        {
            //anchors.centerIn: parent
            source: AppTheme.iconForType(root.iconName)
            Layout.preferredHeight: root.iconSize
            Layout.preferredWidth: root.iconSize
            fillMode: Image.PreserveAspectFit
        }


        //Название
        Text
        {
            text: root.fileName
            Layout.preferredWidth: 300
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            font.weight: Font.Medium
            color: AppTheme.textPrimary
        }

        // Для подгонки размеров
        Item {Layout.fillWidth: true}

        // Размер
        Text
        {
            text: root.fileSize
            Layout.preferredWidth: 110
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            font.weight: Font.Medium
            color: AppTheme.textPrimary
            horizontalAlignment: Text.AlignHCenter
        }

        Item {width: 100 }

        // Дата
        Text
        {
            text: root.fileDate
            Layout.preferredWidth: 110
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            font.weight: Font.Medium
            color: AppTheme.textPrimary
            horizontalAlignment: Text.AlignRight
        }

        Item {width: 100 }

        // Тип файла
        Text
        {
            text: root.mimeType
            Layout.preferredWidth: 110
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            font.weight: Font.Medium
            color: AppTheme.textPrimary
            horizontalAlignment: Text.AlignHCenter
        }

        Item {width: 80}
    }

    MouseArea
    {
        id: rowMouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        cursorShape: Qt.PointingHandCursor

        onClicked: function(mouse)
        {
            if (mouse.button === Qt.RightButton)
                root.rightClicked(mouse.x + root.x, mouse.y + root.y)
            else root.clicked()
        }

        onDoubleClicked: root.doubleClicked()
    }
}
