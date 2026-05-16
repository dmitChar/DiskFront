import QtQuick
import QtQuick.Controls
import AppTheme 1.0

// Просмотр текстовых файлов.
// Загружает файл через Media.fetchText(fileId) — результат кешируется.
//
// Использование:
//   TextViewer { fileId: "42"; fileName: "readme.md" }

Rectangle
{
    id: root
    color: AppTheme.surface
    radius: AppTheme.radiusMd

    property string fileId: ""
    property string fileName: ""

    function reload()
    {
        Media.invalidate(fileId)
        load()
    }

    // --- Состояния ------
    states:
    [
        State { name: "loading" },
        State { name: "ready" },
        State { name: "error" }
    ]
    state: "loading"
    onFileIdChanged: function()
    {
        if (fileId !== "")
            load()
    }

    Component.onCompleted: function()
    {
        if (fileId !== "")
            load()


    }

    function load()
    {
        state = "loading"
        MediaModel.fetchText(fileId)
    }

    // ------ Подключение к MediaController -------
    Connections
    {
        target: MediaModel

        onTextReady: function(id, text)
        {
            //console.log("on Text ready")
            if (id !== root.fileId)
                return
            textArea.text = text
            root.state = "ready"
        }

        onTextError: function(id, error)
        {
            if (id !== root.fileId)
                return
            erorrText.text = "Ошибка загрузки:\n" + error
            root.state = "error"
        }
    }

    // -- Содержимое ----------
    Item
    {
        anchors.fill: parent
        // Загрузка
        Column
        {
            anchors.centerIn: parent
            visible: root.state === "loading"
            spacing: AppTheme.sp12
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: "Загрузка..."; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.textHint }
        }

        // Ошибка
        Column
        {
            anchors.centerIn: parent
            visible: root.state === "error"
            spacing: AppTheme.sp12
            Text { id: errorText; anchors.horizontalCenter: parent.horizontalCenter; verticalAlignment: Text.AlignVCenter; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.textSecondary;}
            Text
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Повторить"
                font.family: AppTheme.fontFamily;
                font.pixelSize: AppTheme.fontMd
                color: AppTheme.accent
                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.reload()
                }
            }
        }

        // Текст
        Flickable
        {
            anchors.fill: parent
            anchors.margins: AppTheme.sp16
            visible: root.state === "ready"
            contentWidth: textArea.implicitWidth
            contentHeight: textArea.implicitHeight
            clip: true

            ScrollBar.vertical:   ScrollBar {}
            ScrollBar.horizontal: ScrollBar {}

            TextEdit
            {
                id: textArea
                width: parent.width
                font.family: "Courier New, Courier, monospace"
                font.pixelSize: AppTheme.fontMd
                color: AppTheme.textPrimary
                wrapMode: TextEdit.Wrap
                readOnly: true
                selectByMouse: true
                textFormat: TextEdit.PlainText
            }
        }
    }


}
