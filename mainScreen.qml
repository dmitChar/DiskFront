import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import AppTheme 1.0

Rectangle
{
    id: root
    color: AppTheme.bg

    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        //---- Sidebar -----
        Rectangle
        {
            Layout.preferredWidth: 220
            Layout.fillHeight: true
            color: AppTheme.sidebarBg

            ColumnLayout
            {
                anchors {fill: parent; margins: 0}
                spacing: 0

                // App title
                Rectangle
                {
                    Layout.fillWidth: true;
                    height: 60
                    color: "transparent"
                    Row
                    {
                        anchors {left: parent.left; verticalCenter: parent.verticalCenter; leftMargin: 20}
                        spacing: 10
                        Text { text: ""; font.pixelSize: 24 }
                        Text
                        {
                            text: "Cloud Disk"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontLg
                            font.weight: Font.Medium; color: "#FFFFFF"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                // Кнопка загрузки новых файлов
                Rectangle
                {
                    Layout.fillWidth: true; height: 44
                    Layout.leftMargin: 12; Layout.rightMargin: 12
                    Layout.bottomMargin: 12
                    radius: AppTheme.radiusMd; color: AppTheme.accent

                    Row
                    {
                        anchors.centerIn: parent; spacing: AppTheme.sp8
                        Text { text: "+"; font.pixelSize: 20; color: "white"; anchors.verticalCenter: parent.verticalCenter }
                        Text
                        {
                            text: "Загрузить файл"
                            font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                            color: "white"; anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    MouseArea
                    {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: fileDialog.open()
                    }
                }

                // Элементы навигации на боковой панели
                Repeater
                {
                    model: [
                        {icon: "", label: "Мой диск", path: "/"},
                        {icon: "", label: "Последние", path: "__recent" },
                        {icon: "", label: "Публичные", path: "__shared" }
                    ]
                    Rectangle
                    {
                        Layout.fillWidth: true; height: 40
                        Layout.leftMargin: 8; Layout.rightMargin: 8
                        radius: AppTheme.radiusSm
                        color: (modelData.path === "/" && FileController.currentPath === "/") || (modelData.path !== "/" && modelData.path !== "__recent" && modelData.path !== "__shared" && FileController.currentPath.startsWith(modelData.path))
                            ? AppTheme.sidebarSel : "transparent"

                        Row
                        {
                            anchors { left: parent.left; verticalCenter: parent.verticalCenter; leftMargin:  12}
                            spacing: AppTheme.sp10
                            Text { text: modelData.icon; font.pixelSize: 16; anchors.verticalCenter: parent.verticalCenter }
                            Text
                            {
                                text: modelData.label
                                font.family:  AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                                color: AppTheme.sidebarText; anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                if (modelData.path === "__recent" || modelData.path === "__shared")
                                    return
                                FileController.navigateTo(modelData.path)
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                // QuotaBar
                ColumnLayout
                {
                    Layout.fillWidth: true
                    Layout.leftMargin: 16; Layout.rightMargin: 16; Layout.bottomMargin: 20
                    spacing: AppTheme.sp8

                    Text
                    {
                        text: "Хранилище"
                        font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium; color: AppTheme.sidebarText
                    }
                    QuotaBar {Layout.fillWidth: true}
                }

                // Иконка пользователя + имя пользователя + кнопка логаута
                Rectangle
                {
                    Layout.fillWidth: true; height: 56
                    color: AppTheme.sidebarSel

                    Row
                    {
                        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; leftMargin: 16; rightMargin: 8 }
                        spacing: AppTheme.sp10

                        // Иконка
                        Rectangle
                        {
                            width: 32; height: 32; radius: 16
                            color: AppTheme.accent
                            Text
                            {
                                anchors.centerIn: parent
                                text: User.username.length > 0 ? User.username[0].toUpperCase() : "?"
                                font.pixelSize: 14; font.weight: Font.Medium; color: "white"
                            }

                        }
                        // Имя пользователя
                        Text
                        {
                            text: User.username
                            font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                            color: AppTheme.sidebarText; anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                            width: parent.width  - 65 - AppTheme.sp10 * 2
                        }
                        // Кнопка логаута
                        Text
                        {
                            text: "⏏"; font.pixelSize: 16; color: AppTheme.sidebarHint
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea
                            {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: AuthController.logout()
                            }
                        }
                    }
                }
            }
        }

        // ----- Main Area -----
        ColumnLayout
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            // Toolbar
            Rectangle
            {
                Layout.fillWidth: true;
                height: 56
                color: AppTheme.surface
                border.color: AppTheme.border; border.width: 1

                RowLayout
                {
                    anchors { fill: parent; rightMargin: 20; leftMargin: 20 }
                    spacing: AppTheme.sp12

                    // Back button
                    Rectangle
                    {
                        width: 32; height: 32; radius: AppTheme.radiusSm
                        color: backMouseArea.containsMouse ? AppTheme.surfaceAlt : "transparent"
                        Text
                        {
                            anchors.centerIn: parent
                            text: "←"
                            font.pixelSize: 18
                            color: AppTheme.textSecondary
                        }
                        MouseArea
                        {
                            id: backMouseArea
                            anchors.fill: parent
                            enabled: FileController.currentPath !== "/"
                            hoverEnabled: true
                            cursorShape: (FileController.currentPath === "/") ? Qt.ArrowCursor : Qt.PointingHandCursor
                            onClicked: FileController.navigateUp()
                        }
                    }

                    // BreadCrumbs
                    BreadCrumbs
                    {
                        Layout.fillWidth: true
                        parts: FileController.breadcrumbs
                        onNavigate: function(path)
                        {
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
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    text: "Поиск файлов..."; font: searchInput.font
                                    color: AppTheme.textHint
                                    visible: searchInput.text === "" && !searchInput.activeFocus
                                }
                            }
                        }

                        // Таймер для задержки поиска после ввода символа
                        Timer
                        {
                            id: searchTimer; interval: 400
                            onTriggered:
                            {
                                if (searchInput.text.trim().length >= 2)
                                    FileController.search(searchInput.text.trim())
                                else if (searchInput.text === "")
                                    FileController.refresh()
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
            }

            // Бар заголовков таблицы + сортировка по нажатию по названию столбца
            Rectangle
            {
                Layout.fillWidth: true
                height: 36
                color:AppTheme.surfaceAlt
                RowLayout
                {
                    anchors {fill: parent; leftMargin: 16; rightMargin: 16 }
                    Text
                    {
                        text: "Имя"
                        Layout.preferredWidth: 300
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium
                        color: AppTheme.textSecondary

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                // TO DO сортировка
                            }
                        }
                    }
                    Item {Layout.fillWidth: true}
                    Text
                    {
                        text: "Размер"
                        Layout.preferredWidth: 80
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium
                        color: AppTheme.textSecondary
                        horizontalAlignment: Text.AlignRight

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                // TO DO сортировка
                            }
                        }
                    }

                    Text
                    {
                        text: "Изменен"
                        Layout.preferredWidth: 110
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium
                        color: AppTheme.textSecondary
                        horizontalAlignment: Text.AlignRight

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                // TO DO сортировка
                            }
                        }
                    }

                    Item {width: 80 }
                }

                Rectangle
                {
                    anchors {left: parent.left; right: parent.right; top: parent.bottom}
                    height: 1;
                    color: AppTheme.border
                }
            }

            Item
            {
                Layout.fillWidth: true
                Layout.fillHeight: true


            }
        }
    }

    // ─── Dialogs ───────────────────────────────────────────────────────────
    // Создание новой папки
    NewFolderDialog
    {
        id: newFolderDialog
        currentPath: FileController.currentPath
        onMkdir: function(folderName)
        {
            FileController.mkdir(folderName)
        }
    }

    FileDialog
    {
        id: fileDialog
        title: "Выберите файл для загрузки"
        nameFilters: ["All files (*)", "Images (*.png *.jgp)", "Documents (*.pdf *.txt)"]
        fileMode: FileDialog.OpenFiles
        onAccepted:
        {
            FileController.uploadFiles(selectedFiles)
            console.log("Выбранный файл " + selectedFiles)
        }

    }

}
