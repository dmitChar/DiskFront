pragma Singleton
import QtQuick 2.9

QtObject {
    // ── Palette ───────────────────────────────────────────────────────────────
    readonly property color bg:          "#F7F7F5"
    readonly property color surface:     "#FFFFFF"
    readonly property color surfaceAlt:  "#F0EFEB"
    readonly property color border:      "#E0DFD8"
    readonly property color borderStrong:"#C8C7C0"

    readonly property color accent:      "#5B4CE8"
    readonly property color accentHover: "#0030ff"
    readonly property color accentLight: "#EEEDFE"

    readonly property color textPrimary:   "#1A1A18"
    readonly property color textSecondary: "#6B6B64"
    readonly property color textHint:      "#A8A8A0"
    readonly property color textOnAccent:  "#FFFFFF"

    readonly property color success:     "#1D9E75"
    readonly property color successLight:"#E1F5EE"
    readonly property color danger:      "#E24B4A"
    readonly property color dangerLight: "#FCEBEB"
    readonly property color warning:     "#BA7517"
    readonly property color warningLight:"#FAEEDA"

    // ── Sidebar ───────────────────────────────────────────────────────────────
    readonly property color sidebarBg:   "#1E1D2E"
    readonly property color sidebarText: "#C8C7D8"
    readonly property color sidebarHint: "#7A7990"
    readonly property color sidebarSel:  "#2E2D42"

    // ── Typography ────────────────────────────────────────────────────────────
    readonly property int   fontSm:  12
    readonly property int   fontMd:  14
    readonly property int   fontLg:  16
    readonly property int   fontXl:  20
    readonly property int   fontH:   24

    readonly property string fontFamily: Qt.platform.os === "windows"
        ? "Segoe UI" : (Qt.platform.os === "osx" ? "SF Pro Display" : "Ubuntu")

    // ── Spacing ───────────────────────────────────────────────────────────────
    readonly property int sp2:  2
    readonly property int sp4:  4
    readonly property int sp6:  6
    readonly property int sp8:  8
    readonly property int sp10:  10
    readonly property int sp12: 12
    readonly property int sp16: 16
    readonly property int sp20: 20
    readonly property int sp24: 24
    readonly property int sp32: 32

    // ── Radius ────────────────────────────────────────────────────────────────
    readonly property int radiusSm: 6
    readonly property int radiusMd: 10
    readonly property int radiusLg: 16

    // ── Animation ─────────────────────────────────────────────────────────────
    readonly property int durationFast:   120
    readonly property int durationNormal: 200
    readonly property int durationSlow:   350

    // ── Icon names → unicode symbols ─────────────────────────────────────────
    function iconForType(iconName) {
        var map = {
            "folder":       "qrc:/icons/icons/icon_folder.png",
            "image":        "",
            "video":        "",
            "audio":        "",
            "pdf":          "",
            "archive":      "",
            "document":     "",
            "spreadsheet":  "",
            "presentation": "",
            "text":         "qrc:/icons/icons/icon_textFile1.png",
            "file":         "qrc:/icons/icons/icon_textFile2.png"
        }
        return map[iconName] || "qrc:/icons/icons/icon_textFile2.png"
    }
}
