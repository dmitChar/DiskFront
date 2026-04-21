import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import AppTheme 1.0

// Toolbar
Rectangle
{
    id: root
    height: 56
    color: AppTheme.surface
    border.color: AppTheme.border; border.width: 1

    property bool isGridView: false
    signal setGridView(bool flag)
    signal closeContextMenu()


    RowLayout
    {
        anchors { fill: parent; rightMargin: 20; leftMargin: 20 }
        spacing: AppTheme.sp12

        // Кнопка возврата назад
        Button
        {
            id: goUpButton
            implicitHeight: 32
            implicitWidth: 32
            enabled: FileController.currentPath !== "/"
            ToolTip.visible: hovered
            ToolTip.text: "Подняться на каталог вверх"
            ToolTip.delay: 150

            contentItem: Text
            {
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "←"
                font.pixelSize: 18
                color: AppTheme.textSecondary
            }

            background: Rectangle
            {
                radius: AppTheme.radiusSm
                color: parent.hovered ? AppTheme.surfaceAlt : "transparent"
                Behavior on color
                {
                    ColorAnimation {duration: 150}
                }
            }

            onClicked:
            {
                root.closeContextMenu()
                FileController.navigateUp()
            }
        }

        // BreadCrumbs
        BreadCrumbs
        {
            Layout.fillWidth: true
            parts: FileController.breadcrumbs
            onNavigate: function(path)
            {
                root.closeContextMenu()
                FileController.navigateTo(path)
            }
        }

        // Search
        Rectangle
        {
            width: 220
            height: 34
            radius: AppTheme.radiusMd
            color: AppTheme.surfaceAlt
            border.color:searchInput.activeFocus ? AppTheme.accent : AppTheme.border
            border.width: 1

            Row
            {
                anchors { fill: parent; leftMargin: 10; rightMargin: 8 }
                spacing: AppTheme.sp8
                Text { text: ""; font.pixelSize: 13; anchors.verticalCenter: parent.verticalCenter} // TO DO добавить иконку поиска

                // Поле поиска
                TextInput
                {
                    id: searchInput
                    width: parent.width - 20 - AppTheme.sp8
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: AppTheme.fontFamily;
                    font.pixelSize: AppTheme.fontMd
                    color: AppTheme.textPrimary
                    onTextChanged: searchTimer.restart()

                    Text
                    {
                        verticalAlignment: Text.AlignVCenter
                        text: "Поиск файлов..."; font: searchInput.font
                        color: AppTheme.textHint
                        visible: searchInput.text === "" && !searchInput.activeFocus
                    }
                    onAccepted:
                    {
                        ProxyModel.search = searchInput.text.trim()
                        parent.focus = false
                    }
                }
                Button
                {
                    visible: searchInput.text !== ""
                    anchors.verticalCenter: parent.verticalCenter
                    anchors {right: parent.right}
                    width: 15
                    height: 15
                    background: hovered ? AppTheme.textPrimary : "transparent"
                    Text
                    {
                        anchors.centerIn: parent
                        text: "x"
                        color: "black"
                    }
                    onClicked:
                    {
                        searchInput.text = ""
                        ProxyModel.search = searchInput.text.trim()
                        root.closeContextMenu()
                    }
                }
            }

            // Таймер для задержки поиска после ввода символа
            Timer
            {
                id: searchTimer; interval: 400
                onTriggered:
                {
                    ProxyModel.search = searchInput.text.trim()
                    searchInput.focus = false
                }
            }
        }

        // Кнопка Смена режима отобржаения
        Rectangle
        {
            id: changeViewBtn
            width: 34
            height: 34
            radius: AppTheme.radiusSm
            color: changeViewMouseArea.containsMouse ? AppTheme.surfaceAlt : "transparent"

            Text
            {
                anchors.centerIn: parent
                text: root.isGridView ? "#" : "="
                font.pixelSize: 14
                color: AppTheme.textSecondary
            }

            MouseArea
            {
                id: changeViewMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    root.isGridView = !root.isGridView
                    root.setGridView(isGridView)
                }
            }
        }

        // New Folder button
        Rectangle
        {
            width: 34; height: 34; radius: AppTheme.radiusSm
            color: mkDirMouseArea.containsMouse ? AppTheme.surfaceAlt : "transparent"
            Text
            {
                anchors.centerIn: parent
                text: "+"
                font.pixelSize: 14;
                color: AppTheme.textSecondary
            }

            MouseArea
            {
                id: mkDirMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: newFolderDialog.open()
            }
        }

        // Кнопка обновления страницы
        Rectangle
        {
            width: 34; height: 34; radius: AppTheme.radiusSm
            color: refreshMouseArea.containsMouse ? AppTheme.surfaceAlt : "transparent"
            Text
            {
                anchors.centerIn: parent;
                text:"↺";
                font.pixelSize: 18
                color: AppTheme.textSecondary
            }

            MouseArea
            {
                id: refreshMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    controller.refreshQuota()
                    FileController.refresh()
                }
            }
        }

        // Нижняя линия(граница) header'a
        Rectangle
        {
            anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
            height: 1; color: AppTheme.border
        }
    }

    function resetFocus()
    {
        searchInput.focus = false
    }
}
