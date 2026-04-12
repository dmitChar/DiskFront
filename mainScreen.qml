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
                        Text { text: "☁"; font.pixelSize: 24 }
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
                        //color: (modelData.path === "/" &&)


                    }

                }
            }
        }
    }
}
