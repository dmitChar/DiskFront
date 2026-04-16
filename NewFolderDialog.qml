import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import AppTheme 1.0

Dialog
{
    id: dialog
    property string currentPath: ""
    signal mkdir(string path)

    modal: true
    focus: true
    width: 500
    height: 300
    padding: 5

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    //standardButtons:

    background: Rectangle
    {
        radius: 14
        color: "white"
        border.color: "#dfe4ea"
        border.width: 1
    }

    // Заголовок
    header: Rectangle
    {
        height:64
        radius: 14
        color: AppTheme.accent
        implicitHeight: 40

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            Rectangle
            {
                height: 32
                width: 32
                radius: 16
                color: "transparent"
                border.color: "white"
                border.width: 2

                Text
                {
                    text: "?"
                    font.pixelSize: 22
                    color: "white"
                    anchors.centerIn: parent
                }
            }
            Text
            {
                text: "Добавление папки"
                font.pixelSize: 18
                font.bold: true
                color: "white"
                anchors.centerIn: parent
            }
        }
    }

    // Содержимое
    contentItem: Rectangle
    {
        height: 100
        radius: AppTheme.radiusMd
        color: AppTheme.surfaceAlt
        border.color: textInput.activeFocus ? AppTheme.accent : AppTheme.border
        border.width: 1

        TextInput
        {
            id: textInput
            width: parent.width - 20 - AppTheme.sp8
            anchors.verticalCenter: parent.verticalCenter
            font.family: AppTheme.fontFamily;
            font.pixelSize: AppTheme.fontMd
            color: AppTheme.textPrimary
            onAccepted: acceptBtn.clicked()

            Text
            {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                font: textInput.font
                color: AppTheme.textHint
                text: "Введите имя новой папки..."
                visible: textInput.text === "" && !textInput.activeFocus
            }
        }
    }

    footer: ColumnLayout
    {
        RowLayout
        {
            spacing: 12
            Layout.fillWidth: true
            Layout.bottomMargin: 8

            Button
            {
                text: "Отмена"
                Layout.fillWidth: true
                implicitHeight: 45
                Layout.margins: {left: 8; right: 8}
                contentItem: Text
                {
                    text: parent.text
                    font.pixelSize: 14
                    font.bold: true
                    color: "#7f8c8d"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle
                {
                    radius: 8
                    color: parent.hoverd ? "#ecf0f1" : "transparent"
                    border.color: "#dfe4ea"
                    border.width: 2
                    Behavior on color
                    {
                        ColorAnimation { duration: 150}
                    }
                }
                onClicked:
                {
                    textInput.text = ""
                    dialog.close()
                }
            }

            Button
            {
                id: acceptBtn
                text: "Добавить"
                implicitHeight: 45
                Layout.fillWidth: true
                Layout.margins: {left: 8; right: 8}
                enabled: textInput.text !== ""


                contentItem: Text
                {
                    text: parent.text
                    font.pixelSize: 14
                    font.bold: true
                    color: parent.enabled ? "white" : "#95a5a6"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle
                {
                    radius: 8
                    color:
                    {
                        if (!parent.enabled) return "#bdc3c7"
                        if (parent.pressed) return "#aa0000"
                        if (parent.hoverd)  return "#DC143C"
                        return "#aa0000"
                    }
                    Behavior on color
                    {
                        ColorAnimation { duration: 150}
                    }
                }

                onClicked:
                {
                    dialog.mkdir(textInput.text)
                    textInput.text = ""
                    dialog.close()
                }
            }
        }
    }
}
