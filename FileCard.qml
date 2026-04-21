import QtQuick 2.9
import QtQuick.Controls 2.2
import AppTheme 1.0

// Кастомный виджет для отобржаения файла в GridView
Rectangle
{
    id: root
    property string iconName: "file"
    property int iconSize: 64
    property string fileName: "unknown"
    property string fileSize: "0"
    property string fileDate: "value"
    property bool isDir: false
    property bool isShared: false
    property bool selected: false

    signal clicked()                        // Просто лкм
    signal doubleClicked()                  // Двойное нажатия -> открытие файла
    signal rightClicked(real x, real y)     // Пкм -> открытие  contextMenu

    width: 100
    height: 145
    radius: AppTheme.radiusSm
    z:999

    color: selected ? AppTheme.accentLight : (cardMouseArea.containsMouse ? AppTheme.surfaceAlt : AppTheme.surface)
    border.color: selected ? AppTheme.accent : AppTheme.border
    border.width: selected ? 2 : 1

    Behavior on color
    {
        ColorAnimation {duration: AppTheme.durationFast}
    }

    Column
    {
        anchors
        {
            top: parent.top; left: parent.left; right: parent.right
            topMargin: AppTheme.sp16; leftMargin: AppTheme.sp16; rightMargin: AppTheme.sp16
        }
        spacing: AppTheme.sp8

        // Иконка файла
        Rectangle
        {
            width: 64;
            height: 64
            anchors.horizontalCenter: parent.horizontalCenter
            radius: AppTheme.radiusMd
            color: root.isDir ? AppTheme.warningLight : AppTheme.accentLight

            Image
            {
                anchors.centerIn: parent
                source: AppTheme.iconForType(root.iconName)
                height: root.iconSize
                width: root.iconSize
                fillMode: Image.PreserveAspectFit
            }

            // Share badge
            Rectangle
            {
                visible: root.isShared
                anchors { right: parent.right; top: parent.top; margins: -3 }
                width: 16; height: 16; radius: 8
                color: AppTheme.success
                Text
                {
                    anchors.centerIn: parent
                    text: "⊕"
                    font.pixelSize: 10
                    color: "white"
                }
            }
        }

        // Название файла
        Text
        {
            width: parent.width
            text: root.fileName
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            font.weight: Font.Medium
            color: AppTheme.textPrimary
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideMiddle
            maximumLineCount: 2
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        // Размер файла под иконкой
        Text
        {
            width: parent.width
            anchors {topMargin: 5; }
            text: root.isDir ? "" : root.fileSize
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            color: AppTheme.textHint
            horizontalAlignment: Text.AlignHCenter
        }
    }

    MouseArea
    {
        id: cardMouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: function(mouse)
        {
            if (mouse.button === Qt.RightButton)
                root.rightClicked(mouse.x + root.x, mouse.y + root.y)
            else root.clicked()
        }
        onDoubleClicked: root.doubleClicked()
    }

}
