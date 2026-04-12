import QtQuick 2.9
import AppTheme 1.0

Rectangle
{
    id: root
    property string text: ""                // Текст кнопки
    property string buttonType: "primary"   // primary || secondary || ghost || danger
    property bool busy: false
    property bool enabled: true

    signal clicked()

    implicitHeight: 40
    implicitWidth: 140
    radius: AppTheme.radiusMd

    readonly property color backgroundColor:
    {
        if (!enabled) return AppTheme.border
        switch (buttonType)
        {
            case "secondary": return AppTheme.surfaceAlt
            case "ghost":     return "transparent"
            case "danger":    return AppTheme.dangerLight
            default:          return ma.containsMouse ? AppTheme.accentHover : AppTheme.accent
        }
    }

    readonly property color textColor:
    {
        if (!enabled) return AppTheme.textHint
        switch (buttonType)
        {
            case "secondary": return AppTheme.textPrimary
            case "ghost":     return AppTheme.textSecondaty
            case "danger": return AppTheme.danger
            default: return AppTheme.textOnAccent
        }
    }

    // Внешний вид кнопки
    color: backgroundColor
    border.color: buttonType === "secondary" ? AppTheme.border : "transparent"
    border.width: buttonType === "secondary" ? 1 : 0
    Behavior on color {ColorAnimation { duration: AppTheme.durationFast } }

    // Текст кнопки
    Row
    {
        anchors.centerIn: parent
        spacing: AppTheme.sp8
        visible: !busy

        Text
        {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            text: root.text
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontMd
            font.weight: Font.Medium
            color: root.textColor
        }
    }

    // Анимация бегунка во время ожидания ответа от сервера
    // Круг, по которому крутится бегунок
    Rectangle
    {
        anchors.centerIn: parent
        visible: busy
        width: 18; height: 18; radius: 9
        color: "transparent"
        border.color: root.textColor
        border.width: 2

        // Бегунок
        Rectangle
        {
            width: 8; height: 8; radius: 4
            color: root.textColor
            anchors {top: parent.top; horizontalCenter: parent.horizontalCenter }
            RotationAnimation on rotation
            {
                from: 0; to: 360
                duration: 800
                loops: Animation.Infinite
                running: busy
            }
        }
    }

    // Для считывания нажатия
    MouseArea
    {
        anchors.fill: parent
        id: ma
        hoverEnabled: true
        cursorShape: enabled && !busy ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: if (root.enabled && !root.busy) root.clicked()
    }
}
