import QtQuick
import QtQuick.Dialogs

// Диалог выбора места сохранения скачиваемого файла.
// Автоматически подставляет имя файла и подбирает фильтр по расширению.
//
// Использование:
//   DownloadFileDialog
//   {
//       id: saveDlg
//       onPathChosen: (localPath) => FileCtrl.downloadFile(remotePath)
//   }
//   saveDlg.prepareFor("/photos/vacation.zip")
//   saveDlg.open()

Item
{
    id: root

    property string serverPath: ""  // Путь на сервере
    property string fileName: ""    // Имя файла

    signal pathChosen(string localPath)
    signal canceled()


    // function prepareForqt5(remotePath)
    // {
    //     root.serverPath = remotePath
    //     root.fileName = remotePath.split("/").pop()
    //     dlg.currentFile = "file:///home/administrator/Downloads"
    //     dlg.nameFilters = filtersForName(root.fileName)
    //     dlg.open()
    // }

    // ДЛЯ QT6
    // Подготовка диалога для файла
    function prepareFor(path)
    {
        root.serverPath = path
        root.fileName = path.split("/").pop() // Последний сегмент пути
        dlg.selectedFile = defaultSaveUrl(root.fileName)
        dlg.nameFilters = filtersForName(root.fileName)
        dlg.open()
    }

    function open()
    {
        dlg.selectedFile = defaultSaveUrl(root.fileName)
        dlg.nameFilters = filtersForName(root.fileName)
        dlg.open()
    }

    // ДЛЯ QT6
    //URL вида "file:///home/user/Downloads/photo.jpg"
    function defaultSaveUrl(name)
    {
        var downloads = StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
        return downloads + "/" + name
    }

    // Подбираем фильтр по расширению файла
    function filtersForName(name)
    {
        var ext = name.split(".").pop().toLowerCase()
        var specific = extToFilter(ext)
        // Всегда добавляем "Все файлы" запасным вариантом
        if (specific) return [specific, "Все файлы (*)"]
        return ["Все файлы (*)"]
    }

    function extToFilter(ext)
    {
        var imageExts  = ["jpg","jpeg","png","gif","bmp","webp","svg","tiff"]
        var videoExts  = ["mp4","avi","mkv","mov","wmv","flv","webm"]
        var audioExts  = ["mp3","wav","flac","aac","ogg","m4a","wma"]
        var docExts    = ["pdf","doc","docx","xls","xlsx","ppt","pptx","odt","rtf"]
        var archExts   = ["zip","tar","gz","bz2","7z","rar"]
        var codeExts   = ["cpp","h","py","js","ts","java","json","xml","yaml"]

        if (imageExts.indexOf(ext) >= 0) return "Изображения (*.jpg *.jpeg *.png *.gif *.bmp *.webp *.svg)"
        if (videoExts.indexOf(ext) >= 0) return "Видео (*.mp4 *.avi *.mkv *.mov *.wmv *.webm)"
        if (audioExts.indexOf(ext) >= 0) return "Аудио (*.mp3 *.wav *.flac *.aac *.ogg *.m4a)"
        if (docExts.indexOf(ext)   >= 0) return "Документы (*.pdf *.doc *.docx *.xls *.xlsx *.ppt *.pptx)"
        if (archExts.indexOf(ext)  >= 0) return "Архивы (*.zip *.tar *.gz *.bz2 *.7z *.rar)"
        if (codeExts.indexOf(ext)  >= 0) return "Код (*.cpp *.h *.py *.js *.ts *.json *.xml)"
        return ""   // неизвестное расширение — только "Все файлы"
    }


    FileDialog
    {
        id: dlg
        title: "Сохранить файл"
        fileMode: FileDialog.SaveFile

        onAccepted:
        {
            var raw = selectedFile.toString() // Для qt6
            var localPath = Qt.platform.os === "windows" ? raw.replace(/^file:\/\/\//, "") : raw.replace(/^file:\/\//, "")
            root.pathChosen(localPath)
        }
        onRejected: root.canceled()

    }

}
