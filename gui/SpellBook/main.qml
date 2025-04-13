import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import spellbook

Window {
    id: window
    minimumWidth:600
    minimumHeight: 800
    visible: true
    title: "Test"

    Bridge {
        id: bridge
    }

    ScrollView {
        ScrollBar.vertical.interactive: true
        width:600
        height: 800

        Column {
            padding: 10
            ColumnLayout {
                spacing: 10

                PayloadLocation { }

                MenuSeparator { }

                EncryptionType { }

                MenuSeparator { }

                LoaderType { }

                MenuSeparator { }

                APIHashing { }

                MenuSeparator { }

                DecoyFunction { }

                RowLayout {
                    Button {
                        text: "Cast"
                        onClicked:  bridge.validateInput(payload_location.text)
                    }
                }
            }
        }
    }
}