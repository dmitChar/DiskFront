import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import AppTheme 1.0

// Компнонент для отображения файлов из модели и выбора формата отображения (gridView || ListView)
Item
{
    id: root
    property int selectedIndex: -1
    property bool gridView: false
    property int rowIconSize: 23
    property int gridIconSize: 32

    signal openItem(int index, string path, bool isDir)     // сигнал для открытия элемента по дабл клику
    signal contextMenuRequested(int index, real x, real y)  // Сигнал для открытия контекстного меню по нажатию пкм
    signal itemClicked(int index)


    // ───────────── Grid view ─────────────────────────
    GridView
    {
        id: grid
        anchors.fill: parent
        anchors.margins: AppTheme.sp8
        visible: root.gridView
        cellWidth: 110
        cellHeight: 150
        clip: true

        ScrollBar.vertical: ScrollBar {}

        model: ProxyModel
        delegate: FileCard
        {
            iconName: model.iconName
            iconSize: root.rowIconSize
            fileName: model.fileName
            fileSize: model.humanSize
            fileDate: model.updatedAt
            isDir: model.isDir
            isShared: model.isShared
            selected: root.selectedIndex === model.index

            onClicked:
            {
                root.itemClicked(index)
                root.selectedIndex = model.index
            }

            onDoubleClicked: root.openItem(model.index, model.filePath, model.isDir)

            onRightClicked: function(x, y)
            {
                root.selectedIndex = model.index
                root.contextMenuRequested(model.index, x, y)
            }
        }
        MouseArea
        {
            anchors.fill: parent
            z:-1
            onClicked: function(point)
            {
                var item = grid.itemAt(point.x, point.y)
                if (item === null)
                    root.selectedIndex = -1
            }
        }

        add: Transition
        {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
            NumberAnimation { property: "y"; from: y + 20; duration: 200}
        }
        remove: Transition
        {
            NumberAnimation { property: "y"; from: y + 20; duration: 200}
            NumberAnimation { property: "opacity"; to: 0; duration: 200 }
        }
    }

    // ───────────── List view ─────────────────────────
    ListView
    {
        id: list
        anchors.fill: parent
        visible: !root.gridView
        model: ProxyModel

        ScrollBar.vertical: ScrollBar {}

        delegate: FileRow
        {
            iconName: model.iconName
            iconSize: root.rowIconSize
            fileName: model.fileName
            fileSize: model.humanSize
            fileDate: model.updatedAt
            mimeType: model.suffixType
            isDir: model.isDir
            isShared: model.isShared
            selected: root.selectedIndex === model.index
            width: list.width

            onClicked:
            {
                root.itemClicked(index)
                root.selectedIndex = model.index
            }

            onDoubleClicked: root.openItem(model.index, model.filePath, model.isDir)

            onRightClicked: function(x, y)
            {
                root.selectedIndex = model.index
                root.contextMenuRequested(model.index, x, y)
            }
        }
        MouseArea
        {
            anchors.fill: parent
            z:-1
            onClicked: function(point)
            {
                var item = grid.itemAt(point.x, point.y)
                if (item === null)
                    root.selectedIndex = -1
                root.itemClicked(selectedIndex)
            }
        }

        add: Transition
        {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
            NumberAnimation { property: "y"; from: y + 20; duration: 200}
        }
        remove: Transition
        {
            NumberAnimation { property: "y"; from: y + 50; duration: 200}
            NumberAnimation { property: "opacity"; to: 0; duration: 200 }
        }
    }

    // Loading overlay
    Rectangle
    {
        anchors.fill: parent
        visible: FilesModel.loading

        Column
        {
            anchors.centerIn: parent
            spacing: AppTheme.sp16
            BusyIndicator
            {
                anchors.horizontalCenter: parent.horizontalCenter
                running: FilesModel.loading

                Text
                {
                    text: "Загрузка..."
                    font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                    color: AppTheme.textSecondary
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}
