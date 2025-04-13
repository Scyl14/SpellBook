import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "Process Handle retrieve"
        }

        ComboBox {
            id: proc_handle_method
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Create process",
                "Enumerate process"
            ]

            onCurrentIndexChanged: {
                if(currentIndex == 1) {
                    enumeration_type.visible = true
                }
                else {
                    enumeration_type.visible = false
                }
            }
        }

        EnumerationType {
            id: enumeration_type
            visible: false
        }               
    }
}