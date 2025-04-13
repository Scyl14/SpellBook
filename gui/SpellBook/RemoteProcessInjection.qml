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
            id: remote_process_injection_type
            implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted
            model: [
                "Remote Process Injection - (CreateRemoteThread)",
                "Remote Thread Hijacking",
                "Early Bird APC Injection",
                "Remote Mapping Injection (Not working with mingw)"
            ]

            onCurrentIndexChanged: {
                if(currentIndex == 0 || currentIndex == 3) {
                    process_handle.visible = true
                }
                else {
                    process_handle.visible = false
                }

                if(currentIndex == 2) {
                    process_creation_type. visible = true
                }
                else {
                    process_creation_type.visible = false
                }
            }
        }

        ProcessHandle  {
            id: process_handle 
            visible: false 
        }

        ProcessCreationType {
            id: process_creation_type
            visible: false
        }

        ColumnLayout {
            id: process_name_form
            visible: true
            Text {
                font.pointSize: 12
                text: "Process name"
            }
            TextField {
                id: process_name
                implicitWidth: 400
                text: "notepad.exe"
            }
        }
    }
}