import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "API Hashing mode"
        }

        ComboBox {
            id: encryption_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Plain IAT",
                "IAT Hiding (No Hashing)",
                "Hashing (Rotr32)"
            ]
        }       
    }
}