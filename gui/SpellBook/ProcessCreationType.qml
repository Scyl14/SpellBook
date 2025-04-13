import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "Process creation type"
        }

        ComboBox {
            id: process_creation_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Create Suspended Process",
                "Create Debugged Process",
                "Create Suspended (spoofing explorer.exe)",
                "Create Debugged (spoofing explorer.exe)"
            ]
        }               
    }
}