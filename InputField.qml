import QtQuick 2.9
import QtQuick.Controls 2.2
import AppTheme 1.0

// Вспомогательный компонент - кастомное поле ввода для логина/пароля

Column
{
    id: root
    spacing: AppTheme.sp6
    property string label: ""       // Название над полем ввода
    property string placeholder: "" // плейсхолдер
    property string value: ""       // Данные введенные в поле
    property bool password: false   // Является ли полем для ввода пароля
    property string error: ""       // Есть ли ошибки
    property bool enabled: true     // Активно ли поле

    signal accepted()

    //Название формы сверху нее
    Text
    {
        text: root.label
        font.family: AppTheme.fontFamily
        font.pixelSize: AppTheme.fontSm
        font.weight: Font.Medium
        color: AppTheme.textSecondary
        visible: root.label !== ""
    }

    //Сама форма
    Rectangle
    {
        id: form
        property bool showPasswd: false // Хранит отображается ли сейчас пароль или он скрыт

        width: parent.width
        height: 42
        radius: AppTheme.radiusMd
        color: root.enabled ? AppTheme.surface : AppTheme.surfaceAlt
        border.color: tf.activeFocus ? AppTheme.accent : (root.error !== "" ? AppTheme.danger : AppTheme.border)
        border.width: tf.activeFocus ? 2 : 1

        Behavior on border.color { ColorAnimation {duration: AppTheme.durationFast }}

        TextInput
        {
            id: tf
            anchors
            {
                left: parent.left; right: eyeBtn.left
                verticalCenter: parent.verticalCenter
                leftMargin: AppTheme.sp12; rightMargin: AppTheme.sp8
            }
            text: root.value
            echoMode: root.password && !form.showPasswd ? TextInput.Password : TextInput.Normal
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontMd
            color: AppTheme.textPrimary
            enabled: root.enabled
            clip: true
            onTextChanged: root.value = text
            onAccepted: root.accepted()


            Text
            {
                anchors.fill: parent
                text: root.placeholder
                font: tf.font
                color: AppTheme.textHint
                visible: tf.text === "" && !tf.activeFocus
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Кнопка для показа/скрытия пароля
        Text
        {
            id: eyeBtn
            anchors {right: parent.right; rightMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter }
            text: parent.showPasswd ? "S" : "C"
            font.pixelSize: 14
            visible: root.password
            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: parent.parent.showPasswd = !parent.parent.showPasswd
            }
        }
    }

    //для отображения ошибок
    Text
    {
        text: root.error
        color: AppTheme.danger
        font.family: AppTheme.fontFamily
        font.pixelSize: AppTheme.fontSm
        visible: root.error !== ""
    }

    function focusField()
    {
        tf.forceActiveFocus()
    }
}
