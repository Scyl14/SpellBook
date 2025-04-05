import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        ColumnLayout {
            Text {
                font.pointSize: 15
                text: "Loader type"
            }

            ComboBox {
                id: encryption_type
                implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
                model: ["Local Injection", "Remote Process Injection"]
            }            
        }

        
    }
}