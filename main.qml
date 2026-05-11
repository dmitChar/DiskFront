import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls


import AppTheme 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 720
    x: Screen.width /2 - width / 2
    y: Screen.height / 2 - height/2

    StackView
    {
        id:stack
        anchors.fill: parent
        initialItem: !AuthController.loggedIn ? "loginForm.qml" : "loginForm.qml"
        //Component {id: loginScreen; loginForm {}}

    }

    Connections
    {
        target: TransferModel
        function onActiveCountChanged()
        {
            console.log(TransferModel.activeCount)
        }
    }

    //----- Переходы окон из loginForm -------
    Connections
    {
        target: stack.currentItem
        function onSwitchToRegister()
        {
            stack.push("RegisterScreen.qml")
        }

        function onSwitchToLogin()
        {
            stack.pop()
        }
    }

    //----- Переходы окон в mainScreen -------
    Connections
    {
        target: AuthController
        function onLoginSuccess()
        {
            stack.push("mainScreen.qml")
        }

        function onLogoutSuccess()
        {
            stack.pop()
        }
    }

}
