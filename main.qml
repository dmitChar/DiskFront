import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2


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
        onActiveCountChanged:
        {
            console.log(TransferModel.activeCount)
        }
    }

    //----- Переходы окон из loginForm -------
    Connections
    {
        target: stack.currentItem
        onSwitchToRegister:
        {
            stack.push("RegisterScreen.qml")
        }

        onSwitchToLogin:
        {
            stack.pop()
        }
    }

    //----- Переходы окон в mainScreen -------
    Connections
    {
        target: AuthController
        onLoginSuccess:
        {
            stack.push("mainScreen.qml")
        }

        onLogoutSuccess:
        {
            stack.pop()
        }
    }

}
