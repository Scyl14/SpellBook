import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ColumnLayout {
    spacing: 10

    ColumnLayout {
        ColumnLayout {
            Text {
                text: "Payload location"
            }

            RadioButton {
                text: "URL"        
                onClicked: {
                    payload_url_form.visible = true
                    payload_path_form.visible = false
                    payload_path.text = ""
                }
            }

            RadioButton {
                text: "Local"
                onClicked: {
                    payload_url_form.visible = false
                    payload_path_form.visible = true
                }
            }
        }

        ColumnLayout {
            id: payload_url_form
            visible: false
            Text {
                text: "Payload URL"
            }
            TextField {
                id: payload_url
            }
        }

        ColumnLayout {
            id: payload_path_form
            visible: false
            Text {
                text: "Payload path"
            }

            Text {
                id: payload_path
                text: ""
            }

            Button {
                text: "Select"
                onClicked: payload_file_dialog.open()
            }
        }
    }

    FileDialog {
        id: payload_file_dialog
        onAccepted: payload_path.text = selectedFile.toString().replace("file://", "")
    }
}