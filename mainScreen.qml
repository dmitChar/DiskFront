import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

import AppTheme 1.0
import fileModel 1.0

Rectangle
{
    id: root
    color: AppTheme.bg
    property bool gridView: false
    property int rowIconSize: 23
    property int gridIconSize: 32

    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        //---- Sidebar -----
        SideBar
        {
            id: sideBar
        }

        // ----- Main Area -----
        ColumnLayout
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            //Toolbar
            CustomToolBar
            {
                id: toolbar
                Layout.fillWidth: true;
                onSetGridView: function(flag)
                {
                    root.gridView = flag
                }
            }

            // Бар заголовков таблицы при List View + сортировка по нажатию по названию столбца
            Rectangle
            {
                id: titlesBox
                Layout.fillWidth: true
                height: 36
                color:AppTheme.surfaceAlt
                visible: !root.gridView

                property string nameSort: "up"
                property string sizeSort: ""
                property string dataSort: ""

                property int sortState: FileModel.NameSortUp

                RowLayout
                {
                    anchors {fill: parent; leftMargin: 16; rightMargin: 16 }

                    Item {Layout.preferredWidth: root.rowIconSize}
                    Text
                    {
                        id: nameText
                        text: "Имя"
                        Layout.preferredWidth: 300
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.ExtraBold
                        color: AppTheme.textSecondary

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                if (titlesBox.sortState === FileModel.NameSortUp)
                                {
                                    titlesBox.sortState = FileModel.NameSortDown
                                    FileController.sort(titlesBox.sortState)
                                }
                                else
                                {
                                    titlesBox.sortState = FileModel.NameSortUp
                                    FileController.sort(titlesBox.sortState)
                                }
                                nameText.font.weight = Font.ExtraBold
                                sizeText.font.weight = Font.Medium
                                updateText.font.weight = Font.Medium
                            }
                        }
                    }
                    Item {Layout.fillWidth: true}
                    Text
                    {
                        id: sizeText
                        text: "Размер"
                        Layout.preferredWidth: 110
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium
                        color: AppTheme.textSecondary
                        horizontalAlignment: Text.AlignHCenter

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                if (titlesBox.sortState === FileModel.SizeSortUp)
                                {
                                    titlesBox.sortState = FileModel.SizeSortDown
                                    FileController.sort(titlesBox.sortState)
                                }
                                else
                                {
                                    titlesBox.sortState = FileModel.SizeSortUp
                                    FileController.sort(titlesBox.sortState)
                                }
                                nameText.font.weight = Font.Medium
                                sizeText.font.weight = Font.ExtraBold
                                updateText.font.weight = Font.Medium
                            }
                        }
                    }

                    Item {width: 100 }

                    Text
                    {
                        id: updateText
                        text: "Изменен"
                        Layout.preferredWidth: 110
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium
                        color: AppTheme.textSecondary
                        horizontalAlignment: Text.AlignHCenter

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                if (titlesBox.sortState === FileModel.DataSortUp)
                                {
                                    titlesBox.sortState = FileModel.DataSortDown
                                    FileController.sort(titlesBox.sortState)
                                }
                                else
                                {
                                    titlesBox.sortState = FileModel.DataSortUp
                                    FileController.sort(titlesBox.sortState)
                                }
                                nameText.font.weight = Font.Medium
                                sizeText.font.weight = Font.Medium
                                updateText.font.weight = Font.ExtraBold
                            }
                        }
                    }

                    Item {width: 100 }

                    Text
                    {
                        id: typeText
                        text: "Тип"
                        Layout.preferredWidth: 110
                        font.family: AppTheme.fontFamily
                        font.pixelSize: AppTheme.fontSm
                        font.weight: Font.Medium
                        color: AppTheme.textSecondary
                        horizontalAlignment: Text.AlignHCenter

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {

                            }
                        }
                    }

                    Item {width: 80 }
                }

                Rectangle
                {
                    anchors {left: parent.left; right: parent.right; top: parent.bottom}
                    height: 1;
                    color: AppTheme.border
                }
            }

            // Компонент для отображения файлов
            FileView
            {
                id: fileView
                Layout.fillWidth: true
                Layout.fillHeight: true
                gridView: root.gridView
                rowIconSize: parent.rowIconSize
                gridIconSize: root.gridIconSize


                onOpenItem: function(index, path, isDir)
                {
                    if (isDir)
                        FileController.navigateTo(path)
                }
                onContextMenuRequested: function(index, x, y)
                {
                    toolbar.resetFocus()
                    contextMenu.visible_ = true
                    contextMenu.x = x
                    contextMenu.y = y
                }
            }

            // Компонент для отображения загрузки файлов
            TransferPanel
            {
                id: transferPanel

            }
        }
    }
    // ─── Dialogs ───────────────────────────────────────────────────────────
    // Диалог создания новой папки
    NewFolderDialog
    {
        id: newFolderDialog
        currentPath: FileController.currentPath
        onMkdir: function(folderName)
        {
            FileController.mkdir(folderName)
        }
    }

    // Диалог выбора файлов для загрузки
    FileDialog
        {
            id: fileDialog
            title: "Выберите файл для загрузки"
            nameFilters: ["All files (*)", "Images (*.png *.jgp)", "Documents (*.pdf *.txt)"]
            fileMode: FileDialog.OpenFiles
            onAccepted:
            {
                FileController.uploadFiles(selectedFiles)
                console.log("Выбранный файл " + selectedFiles)
            }

        }

    // Контекстное меню
    ContextMenu
    {
        id: contextMenu
    }

    Connections
    {
        target: fileView
        onItemClicked: function(index)
        {
            contextMenu.visible_ = false
            toolbar.resetFocus()

        }
    }

    Connections
    {
        target: sideBar
        onCloseContextMenu: function()
        {
            contextMenu.visible_ = false
            toolbar.resetFocus()
        }
    }

    Connections
    {
        target: transferPanel
        onCloseContextMenu: function()
        {
            contextMenu.visible_ = false
            toolbar.resetFocus()
        }
    }

    Connections
    {
        target: toolbar
        onCloseContextMenu: function()
        {
            contextMenu.visible_ = false
        }
    }

}
