import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import AppTheme 1.0

Rectangle
{
    id: root
    color: AppTheme.bg
    property bool isLoginForm: true

    signal switchToRegister()

    RegisterScreen
    {
        id: regScreen
        width: parent.width * 0.5
        height: parent.height
        onSwitchToLogin: root.isLoginForm = true
    }

    // Прямоугольник для левой части окна(обложка программы)
    Rectangle
    {
        id: movingRect
        width: parent.width * 0.5
        height: parent.height
        color: AppTheme.sidebarBg
        z: 100
        x: root.isLoginForm ? 0 : loginRect.x

        Behavior on x
        {

            NumberAnimation
            {
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }

        // Основаня колонка для элементов
        Column
        {
            anchors.centerIn: parent
            spacing: AppTheme.sp24

            //Иконка
            Rectangle
            {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 72
                height: 72
                radius: 18
                color: AppTheme.accent
                Text
                {
                    anchors.centerIn: parent
                    text: "Y"
                    font.pixelSize: 40
                    font.weight: Font.Bold
                    color: "white"
                }
            }

            //Надпись под иконкой
            Column
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: AppTheme.sp8
                Text
                {
                    anchors.horizontalCenter: parent.horizontalCenter;
                    text:"Cloud Disk"
                    font.family: AppTheme.fontFamily
                    font.pixelSize: 32
                    font.weight: Font.Bold
                    color: "#FFFFFF"
                }

                Text {anchors.horizontalCenter: parent.horizontalCenter; text: "Ваше облачное хранилище"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.sidebarHint;}
            }

            // Галочки
            Column
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: AppTheme.sp12
                Repeater
                {
                    model: ["Храните файлы в облаке","Делитесь ссылками мгновенно","Доступ с любого устройства"]
                    Row
                    {
                        spacing: AppTheme.sp4
                        Text { text: "✓"; font.pixelSize: 25; color: AppTheme.success; anchors.verticalCenter: parent.verticalCenter}
                        Text { text: modelData; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.sidebarText; anchors.verticalCenter: parent.verticalCenter}
                    }
                }
            }
        }
        MouseArea
        {
            anchors.fill: parent
        }
    }

    // Прямоугольник для правой части окна - Форма регистрации
    Rectangle
    {
        id: loginRect
        anchors {left: parent.left; leftMargin: parent.width * 0.5; right: parent.right; top: parent.top; bottom: parent.bottom }
        color: AppTheme.surface

        Column
        {
            anchors.centerIn: parent
            width: Math.min(360, parent.width - 80)
            spacing: AppTheme.sp24

            Column
            {
                width: parent.width; spacing: AppTheme.sp6
                Text { text: "Добро пожаловать"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontH; font.weight: Font.Bold; color: AppTheme.textPrimary }
                Text { text: "Войдите в свой аккаунт"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.textSecondary}
            }

            // Поле отображения ошибок
            Rectangle
            {
                width: parent.width; height: 40
                visible: AuthController.error !== ""
                radius: AppTheme.radiusSm
                color: AppTheme.dangerLight; border.color: AppTheme.danger; border.width: 1
                Text
                {
                    anchors {left: parent.left; leftMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter}
                    text: AuthController.error
                    font.family: AppTheme.fontFamily;
                    font.pixelSize: AppTheme.fontSm
                    color: AppTheme.danger
                }

                Text
                {
                    anchors {right: parent.right; rightMargin: AppTheme.sp12; verticalCenter: parent.verticalCenter}
                    text: "×"
                    color: AppTheme.danger;
                    font.pixelSize: 16
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: AuthController.clearError()
                }
            }

            // Поля для ввода логина и пароля
            Column
            {
                width: parent.width
                spacing: AppTheme.sp16
                InputField {id: loginField; width: parent.width; label: "Логин или email"; placeholder: "@raiz@mail.com"; onAccepted: passField.focusField()}
                InputField {id: passField; width: parent.width; label: "Пароль"; placeholder: "••••••••"; password: true; onAccepted: doLogin() }
            }

            // Кнопка подтверждения входа
            AppButton
            {
                width: parent.width; text: "Войти"; busy: AuthController.busy
                onClicked: doLogin()
            }

            Row
            {
                anchors.horizontalCenter: parent.horizontalCenter; spacing: AppTheme.sp6
                Text { text: "Нет аккаунта?"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.textSecondary }
                Text
                {
                    text: "Зарегистрироваться"; font.family: AppTheme.fontFamily; font.pixelSize: AppTheme.fontMd; color: AppTheme.accent
                    MouseArea
                    {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            AuthController.clearError();
                            console.log("clicked to register screen")
                            root.isLoginForm = false
                        }
                    }
                }
            }
        }

    }

    // Очистка поля при переходе в новое состояние
    Connections
    {
        target: AuthController
        onAuthStateChanged:
        {
            loginField.value = ""
            passField.value = ""
        }
    }

    function doLogin()
    {
        AuthController.login(loginField.value, passField.value)
    }

}
