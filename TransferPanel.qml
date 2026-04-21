import QtQuick 2.9
import QtQuick.Layouts 1.3

import AppTheme 1.0

Rectangle
{
    id: root
    //height: TransferModel.activeCount > 0 ? 180 : 0
    height: 180
    Layout.fillWidth: true
    visible: TransferModel.activeCount >= 0
    color: AppTheme.surface
    border.color: AppTheme.border;
    border.width: 1

    signal closeContextMenu()

    Behavior on height
    {
        NumberAnimation
        {
            duration: AppTheme.durationNormal
            easing.type: Easing.OutCubic
        }
    }

    ColumnLayout
    {
        anchors { fill: parent; margins: 12 }
        spacing: AppTheme.sp8

        // Заголовок
        Row
        {
            Layout.fillWidth: true
            spacing: AppTheme.sp8

            // Количество загружаемых файлов
            Text
            {
                text: "Передача файлов: " + TransferModel.activeCount
                font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                font.weight: Font.Medium; color: AppTheme.textPrimary
                anchors.verticalCenter: parent.verticalCenter
            }

            Item {Layout.fillWidth: true; height: 1 }

            // Кнопка "очистить"
            Text
            {
                text: "Очистить"
                font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd
                color: AppTheme.accent
                anchors.verticalCenter: parent.verticalCenter

                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        TransferModel.clearCompleted()
                        root.closeContextMenu()
                    }

                }
            }
        }

        ListView
        {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: TransferModel
            clip: true
            spacing: AppTheme.sp4

            delegate: Rectangle
            {
                width: listView.width; height: 36
                color: AppTheme.surfaceAlt
                radius: AppTheme.radiusSm

                RowLayout
                {
                    anchors { fill: parent; leftMargin: 10; rightMargin: 10 }
                    spacing: AppTheme.sp8

                    // Иконка загрузки/выгрузки
                    Text
                    {
                        text: model.isUpload ? "⬆" : "⬇"; font.pixelSize: 13
                        color: model.isUpload ? AppTheme.accent : AppTheme.success
                    }

                    Text
                    {
                        Layout.fillWidth: true
                        text: model.transferName
                        elide: Text.ElideRight
                        font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontSm
                        color: AppTheme.textPrimary
                    }

                    Rectangle
                    {
                        width: 100; height: 6; radius: 3; color: AppTheme.border
                        Rectangle
                        {
                            width: parent.width * model.transferProgress / 100
                            height: parent.height; radius: parent.radius
                            color: model.transferState === "failed" ? AppTheme.danger : model.transferState === "done" ? AppTheme.success: AppTheme.accent

                        }
                    }
                    Text
                    {
                        text: transferProgress + "%"
                        font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontSm
                        color: AppTheme.textSecondary
                        Layout.preferredWidth: 35; horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
    MouseArea
    {
        anchors.fill: parent
        z: -1
        onClicked:
        {
            root.closeContextMenu()
        }
    }
}
