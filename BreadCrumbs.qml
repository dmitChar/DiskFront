import QtQuick 2.9
import AppTheme 1.0

Row
{
    id: root
    property var parts: []
    signal navigate(string path)

    onPartsChanged: console.log("Parts:" + parts)

    Repeater
    {
        model: root.parts

        // Текст отдельного сегмента
        Text
        {
            id: txt
            text: (modelData  === "/") ? "Главная страница / " : modelData  + " / "                 // отображение имени сегмента
            //text: "text"
            font.family: AppTheme.fontFamily
            font.pixelSize: AppTheme.fontMd
            color: index === root.parts.length - 1 ? AppTheme.textPrimary : AppTheme.accent // Для последнего элемента - другой цвет
            font.weight: index === root.parts.length - 1 ? Font.Medium : Font.Normal        // Для последнего элемента - выделение жирным

            MouseArea
            {
                anchors.fill: parent
                cursorShape: index < root.parts.length - 1 ? Qt.PointingHandCursor : Qt.ArrowCursor // Если последний элемент - на него нельзя нажать
                onClicked:
                {
                    if (index < root.parts.length - 1)          // Если не последний элемент, то
                        root.navigate(parent.crumbPath) // Сообщаем о том, что пользователь хочет перейти в другую директорию по ссылке
                }
                hoverEnabled: true
                onContainsMouseChanged:
                {
                    // Установка цвета - отдельный цвет для hovered элементов, отдельный цвет для последнего элемента
                    parent.color = (containsMouse && index < root.parts.length - 1) ? AppTheme.accentHover : (index === root.parts.length - 1 ? AppTheme.textPrimary : AppTheme.accent)
                }
            }
            // Вычисление пути для нажатой ссылки
            property string crumbPath:
            {
                if (index === 0) return "/"
                var clickedPath = root.parts.slice(1, index + 1) // с позиции 1 до текущего индекса включительно
                return "/" + clickedPath.join("/")              // склеиваем обратно в путь
            }
        }
    }
}
