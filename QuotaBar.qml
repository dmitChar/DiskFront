import QtQuick 2.2
import QtQuick.Layouts 1.3
import AppTheme 1.0

Column
{
    spacing: AppTheme.sp6

    RowLayout
    {
        width: parent.width
        Text
        {
            text: User.humanUsed + " из " + User.humanQuota
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            color: AppTheme.sidebarText
        }
        Item { Layout.fillWidth: true; height: 1}
        Text
        {
            text: User.usedPercent + "%"
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontSm
            color: User.usedPercent > 85 ? AppTheme.danger : AppTheme.sidebarHint
        }
    }

    Rectangle
    {
        width: parent.width
        height: 5
        radius: 3
        color: AppTheme.sidebarSel

        Rectangle
        {
            width: Math.max(parent.width * User.usedPercent / 100, 4)
            height: parent.height
            radius: parent.radius
            color: User.usedPercent > 85 ? AppTheme.danger : User.usedPercent > 60 ? AppTheme.warning : AppTheme.accent

            Behavior on width { NumberAnimation {duration: AppTheme.durationSlow} }
        }
    }
}
