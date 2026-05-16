import QtQuick
import AppTheme 1.0

// Изображение с авторизацией. Замена стандартному Image.
//
// Использование:
//   AuthImage { fileId: "42"; width: 300; height: 200 }
//
// Внутри использует source: "image://media/<fileId>"
// MediaImageProvider добавляет JWT, кеширует результат

Item
{
    id: root
    property string fileId: ""
    property bool fillMode: false
    property real radius: 0

    // Перезагрузка изображения
    function reload()
    {
        // Cache-buster: добавление timestamp → QML считает это новым URL
        img.source = "image://media/" + root.fileId + "?t=" + Date.now()
    }

    // Фоновый прямоугольник (во время загрузки)
    Rectangle
    {
        anchors.fill: parent
        color: AppTheme.surfaceAlt
        radius: root.radius
        visible: img.status !== Image.Ready

        // placeholder
        Rectangle
        {
            anchors.centerIn: parent
            width: 32
            height: 32
            radius: 16
            color: "transparent"
            border.color: AppTheme.border;
            border.width: 2

            RotationAnimation on rotation
            {
                from: 0
                to: 360
                duration: 1000
                loops: Animation.Infinite
                running: img.status === Image.Loading
            }
        }
    }

    Image
    {
        id: img
        anchors.fill: parent

        source: root.fileId !== "" ? ("image://media/" + root.fileId) : ""
        fillMode: root.fillMode ? Image.PreserveAspectCrop : Image.PreserveAspectFit

        asynchronous: true
        opacity: status === Image.Ready ? 1 : 0
        Behavior on opacity {NumberAnimation { duration: AppTheme.durationNormal }}

        // Скругление
        layer.enabled: root.radius > 0
        layer.effect: null
        cache: false
    }

    // Иконка ошибки
    Column
    {
        anchors.centerIn: parent
        visible: img.status === Image.Error
        spacing: AppTheme.sp8

        Text { anchors.horizontalCenter: parent.horizontalCenter; text: "⚠"; font.pixelSize: 28; color: AppTheme.textHint }
        Text { anchors.horizontalCenter: parent.horizontalCenter; text: "Не удалось загрузить"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontSm; color: AppTheme.textHint }
    }
}
