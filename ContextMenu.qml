import QtQuick 2.0

import AppTheme 1.0

Rectangle
{
    id: root
    property string filePath:  ""
    property string fileName:  ""
    property bool   isDir:     false
    property bool   isShared:  false
    property bool   visible_:  false

    signal openFolder()
    signal downloadRequested()
    signal renameRequested()
    signal copyRequested()
    signal deleteRequested()
    signal shareRequested()
    signal unshareRequested()

    visible: visible_
    width: 195
    height: menuCol.height + AppTheme.sp8
    radius: AppTheme.radiusMd
    color: AppTheme.surface
    border.color: AppTheme.border
    border.width: 1
    z:10

    Column
    {
        id: menuCol
        anchors { top: parent.top; left: parent.left; right: parent.right; topMargin: AppTheme.sp4 }

        Repeater
        {
            model:
            {
                var items = []
                items.push({ label: "Открыть",          icon: "", action: "open"})
                items.push({ label: "Скачать",          icon: "", action: "download"})
                items.push({ label: "Переименовать",    icon: "", action: "rename" })
                items.push({ label: "Скопировать",      icon: "", action: "copy" })
                items.push({ label: "Удалить",          icon: "", action: "delete" })
                if (!isShared) items.push({ label: "Сделать публичным",icon: "", action: "share" })
                else items.push({ label: "Сдеалть приватным", icon: "", action: "unshare" })

                return items
            }

            // Один ряд
            Rectangle
            {
                width: parent.width
                height: 36
                color: menuMouseArea.containsMouse ? AppTheme.surfaceAlt : "transparent"

                Row
                {
                    anchors {left: parent.left; verticalCenter: parent.verticalCenter; leftMargin: AppTheme.sp12}
                    spacing: AppTheme.sp8

                    Image {source: modelData.icon; width: 10; height: 10}
                    Text
                    {
                        text: modelData.label
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontMd
                        color: modelData.action === "delete" ? AppTheme.danger : AppTheme.textPrimary
                    }
                }

                MouseArea
                {
                    id: menuMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                            root.visible_ = false
                        switch (modelData.action)
                        {
                        case "open":
                            root.openFolder()
                            break;

                        case "download":
                            root.downloadRequested()
                            break;

                        case "rename":
                            root.copyRequested()
                            break;

                        case "copy":
                            root.copyRequested()
                            break;

                        case "delete":
                            root.deleteRequested()
                            break;

                        case "share":
                            root.shareRequested()
                            break;

                        case "unshare":
                            root.unshareRequested()
                            break;
                        }
                    }
                }

            }
        }
        Item { height: AppTheme.sp4; width: 1 }
    }
}
