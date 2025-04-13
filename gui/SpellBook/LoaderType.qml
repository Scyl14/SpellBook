import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10
    ColumnLayout {
        Text {
            font.pointSize: 15
            text: "Loader type"
        }

        ComboBox {
            property var components: [local_injection, remote_process_injection]
            property int prev_index: 0

            id: encryption_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: ["Local Injection", "Remote Process Injection"]
            onCurrentIndexChanged: {
                components[prev_index].visible = false
                prev_index = currentIndex
                components[currentIndex].visible = true
            }
        }

        LocalInjection { id: local_injection }

        RemoteProcessInjection {
            visible: false
            id: remote_process_injection
        }
    }
}