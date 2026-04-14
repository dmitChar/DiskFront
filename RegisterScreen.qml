import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import AppTheme 1.0


Rectangle
{
    id: root
    color: AppTheme.sidebarBg
//    width: parent.width
//    height: parent.height

    signal switchToLogin() // Переход на окно регистрации

    Rectangle
    {
        id: mainRect
        anchors.centerIn: parent
        width: Math.min(420, parent.width - 80)
        height: formColumm.height + 65
        color: AppTheme.surface
        border.color: AppTheme.border
        radius: 12

        Column
        {
            id: formColumm
            anchors {top: parent.top; left: parent.left; right: parent.right; topMargin: AppTheme.sp32; leftMargin: AppTheme.sp32; rightMargin: AppTheme.sp32}
            spacing: AppTheme.sp20

            // Верхний ряд (заголвки + кнопка закрытия)
            RowLayout
            {
                width: parent.width
                // Заголовки
                Column
                {
                    width: parent.width;
                    spacing: AppTheme.sp6
                    Text { text: "Регистрация"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontH; font.weight: Font.Bold; color: AppTheme.textPrimary }
                    Text { text: "Создайте новый аккаунт"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.textSecondary }
                }

                Item { Layout.fillWidth: true }

                // Кнопка закрытия
               Button
               {
                   id: btn
                   implicitHeight: 32
                   implicitWidth: 32

                   contentItem: Text
                   {
                       text: "x"
                       font.pixelSize: 16
                       color: "black"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                   }
                   background: Rectangle
                   {
                       radius: 16
                       color: btn.hovered ? "#C0C0C0" : "transparent"

                       Behavior on color
                       {
                           ColorAnimation { duration: AppTheme.durationNormal }
                       }
                       border.width: hovered ? 3 :0
                   }

                   onClicked:
                   {
                       emit: switchToLogin()
                   }

               }
            }

            // Поле для вывода ошибок
            Rectangle
            {
                width: parent.width
                height: 40
                visible: AuthController.error !== ""
                radius: AppTheme.radiusSm
                color: AppTheme.dangerLight; border.color: AppTheme.danger; border.width: 1
                Text
                {
                    anchors {left: parent.left; leftMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter}
                    text: AuthController.error
                    font.family: AppTheme.fontFamily
                    font.pixelSize: AppTheme.fontSm
                    color: AppTheme.danger
                    elide: Text.ElideRight
                    width: parent.width - 40
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: AuthController.clearError()
                }
            }

            // Поля для ввода логина пароля
            InputField { id: loginField; width: parent.width; label: "Логин"; placeholder: "sobakaRaiz@mail.com"; onAccepted: passwordFied1.focusField() ; onValueChanged: checkFields()}
            InputField { id: passwordFied1; width: parent.width; label: "Пароль"; placeholder: "••••••••"; password: true;
                error: passwordFied1.value !== "" ? "" : passwordFied1.error; onValueChanged: checkFields() }
            InputField { id: passwordFied2; width: parent.width; label: "Подтвердите пароль"; placeholder: "••••••••"; password: true;
                         error: passwordFied2.value !== "" && passwordFied2.value !== passwordFied1.value ? "Пароли не совпадают": ""
                         onAccepted: doRegister()

            }

            // Кнопка подтверждения регистрации
            AppButton
            {
                width: parent.width;
                text: "Зарегистрироваться"
                busy: AuthController.busy
                enabled: passwordFied1.value === passwordFied2.value
                onClicked: doRegister()
            }
        }
    }

    function checkFields()
    {
        var flag = true
        if (loginField.value === "")
        {
            loginField.error = "Поле логина не может быть пустым"
            flag = false
        }
        else loginField.error = ""

        if (passwordFied1.value === "")
        {
            passwordFied1.error = "Поле пароля не может быть пустым"
            flag = false
        }
        else passwordFied1.error = ""
        return flag
    }

    function doRegister()
    {
        if (checkFields())
            AuthController.registerUser(loginField.value, loginField.value, passwordFied1.value)
    }

    // Чтобы увидеть сигнал из AuthController об успешной регистрации
    Connections
    {
        target: AuthController
        onRegisterSuccess:
        {
            root.switchToLogin()
        }
    }
}
