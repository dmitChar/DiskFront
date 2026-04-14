import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

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
                        //onClicked: Открытие окна загрузки файла
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

                Rectangle
                {
                    Layout.fillWidth: true; height: 56
                    color: AppTheme.sidebarSel

                    Row
                    {
                        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; leftMargin: 16; rightMargin: 8 }
                        spacing: AppTheme.sp10

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
                        Text
                        {
                            text: User.username
                            font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                            color: AppTheme.sidebarText; anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                            width: parent.width  - 65 - AppTheme.sp10 * 2
                        }
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
    }
}
