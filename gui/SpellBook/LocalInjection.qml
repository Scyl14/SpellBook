import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "Local Injection type"
        }

        ComboBox {
            id: encryption_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Basic Local Injection", 
                "Local Thread Hijacking (Dummy Function)", 
                "Local APC Injection", 
                "Local Mapping Injection"
            ]
        }       
    }
}