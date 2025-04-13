import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "Enumeration type"
        }

        ComboBox {
            id: enumeration_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Snapshot",
                "NtQuerySystemInformation"
            ]
        }               
    }
}