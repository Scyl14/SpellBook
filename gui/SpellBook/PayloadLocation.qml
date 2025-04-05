import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ColumnLayout {

    ColumnLayout {
        ColumnLayout {

            Text {
                font.pointSize: 15
                text: "Payload location"
            }

            RadioButton {
                text: "URL"
                checked: true        
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
                    payload_url.text = ""
                }
            }
        }

        ColumnLayout {
            id: payload_url_form
            visible: true
            Text {
                font.pointSize: 12
                text: "Payload URL"
            }
            TextField {
                id: payload_url
                implicitWidth: 400
                text: "http://localhost"
            }
        }

        ColumnLayout {
            id: payload_path_form
            visible: false
            Text {
                font.pointSize: 12
                text: "Payload path"
            }

            Text {
                id: payload_path
                font.pointSize: 10
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