import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    width: 400
    height: 750
    property alias semVer: semVer
    property alias semV: semV
    property alias semG: semG

    Button {
        id: semV
        x: 112
        y: 146
        width: 139
        height: 61
        text: qsTr("Vermell")
        font.pointSize: 16

        palette {
            button: "#FE0000"
            buttonText: "#FFFFFF"
        }
    }

    Button {
        id: semG
        x: 112
        y: 252
        width: 139
        height: 61
        text: qsTr("Ambar")
        font.pointSize: 16
        palette {
            button: "#FFC100"
            buttonText: "#FFFFFF"
        }
    }

    Button {
        id: semVer
        x: 112
        y: 368
        width: 139
        height: 61
        text: qsTr("Verd")
        font.pointSize: 16
        palette {
            button: "#27FF00"
            buttonText: "#FFFFFF"
        }

        layer.format: ShaderEffectSource.Alpha
    }

    Text {
        id: text1
        x: 63
        y: 54
        text: qsTr("Control de Semàfors")
        font.pixelSize: 30
        font.pointSize: 30
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/

