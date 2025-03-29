import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import spellbook

Window {
    id: window
    minimumWidth:300
    minimumHeight: 350
    visible: true
    title: "Test"

    Bridge {
        id: bridge
    }

    Column {
        padding: 10
        ColumnLayout {
            spacing: 10

            PayloadLocation { }

            RowLayout {
                Button {
                    text: "Next"
                    onClicked:  bridge.validateInput(payload_location.text)
                }

                Button {
                    text: "Previous"
                }
            }
        }
    }
}