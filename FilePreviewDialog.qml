import QtQuick
import QtQuick.Controls
import AppTheme 1.0
import QtQuick.Layouts


// Универсальный диалог предпросмотра файла.
// Автоматически выбирает нужный компонент по mimeType.
//
// Использование:
//   FilePreviewDialog { id: preview }
//   preview.show("42", "photo.jpg", "image/jpeg")

Rectangle
{
    id: root
    anchors.fill: parent
    color: Qt.rgba(0, 0, 0, 0.75)
    visible: false
    z:100

    property string fileId: ""
    property string fileName: ""
    property string mimeType: ""

    function show(id, name, mime)
    {
        fileId   = id
        fileName = name
        mimeType = mime
        visible  = true
    }

    function hide()
    {
        visible = false
        //unloadViewers()
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: root.hide()
    }

    // --- Контейнер --------
    Rectangle
    {
        anchors.centerIn: parent
        width: Math.min(root.width * 0.88, 1000)
        height: Math.min(root.height * 0.88, 700)
        radius: AppTheme.radiusLg
        color: AppTheme.surface

        MouseArea
        {
            anchors.fill: parent // Для поглощения кликов
        }

        Column
        {
            anchors.fill: parent

            // Заголовок
            Rectangle
            {
                width: parent.width;
                height: 50
                color: AppTheme.surfaceAlt;
                radius: AppTheme.radiusLg

                Rectangle
                {
                    anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
                    height: parent.radius;
                    color: parent.color
                }

                Row
                {
                    anchors { left: parent.left; leftMargin: AppTheme.sp16; verticalCenter: parent.verticalCenter }
                    spacing:  AppTheme.sp10
                    //Text {text: iconForMime(root.mimeType); font.pixelSize: 18}
                    Text { text: root.fileName; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; font.weight: Font.Medium; color: AppTheme.textPrimary }
                }

                Text
                {
                    anchors { right: parent.right; rightMargin: AppTheme.sp16; verticalCenter: parent.verticalCenter }
                    text: "x"; font.pixelSize: 22; color: AppTheme.textHint
                    MouseArea
                    {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.hide()
                    }
                }
            }

            // Область контента
            Item
            {
                width: parent.width
                height: parent.height - 50

                // Изображение
                ImagePreview
                {
                    id: imgPreview
                    anchors { fill: parent; margins: AppTheme.sp8 }
                    fileId: root.visible && isImage(root.mimeType) ? root.fileId : ""
                    visible: isImage(root.mimeType)
                }

                // Текстовые файлы
                TextViewer
                {
                    id: textViewer
                    anchors { fill: parent; margins: AppTheme.sp8 }
                    fileId: root.visible && isText(root.mimeType) ? root.fileId : ""
                    fileName: root.fileName
                    visible: isText(root.mimeType)
                }

                // Видео
                VideoViewer
                {
                    id: videoViewer
                    anchors { fill: parent; margins: AppTheme.sp8 }
                    fileId: root.visible && isVideo(root.mimeType) ? root.fileId : ""
                    fileName: root.fileName
                    visible: isVideo(root.mimeType)
                }

                // Неподдерживаемый формат
                ColumnLayout
                {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    visible: !isImage(root.mimeType) && !isText(root.mimeType) && !isVideo(root.mimeType)
                    spacing: AppTheme.sp16

                    Text { text: "Предпросмотр недоступен"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontLg; color: AppTheme.textSecondary }
                }
            }
        }
    }


    function isImage(mime)
    {
        return mime.startsWith("image/")
    }

    function isText(mime)
    {
        return (mime.startsWith("text/") || mime === "application/json" || mime === "application/xml" || mime === "application/javascript")
    }

    function isVideo(mime)
    {
        return mime.startsWith("video/")
    }


    Keys.onEscapePressed: function()
    {
        root.hide()
    }

    focus: visible

}
