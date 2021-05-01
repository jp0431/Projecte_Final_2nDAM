import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    width: 400
    height: 400
    property alias conSem: conSem
    property alias semVer: semVer
    property alias semV: semV
    property alias semG: semG

    Button {
        id: semV
        x: 150
        y: 146
        text: qsTr("Vermell")
        font.pointSize: 16

        palette {
            button: "#FE0000"
            buttonText: "#FFFFFF"
        }
    }

    Button {
        id: semG
        x: 150
        y: 205
        text: qsTr("Ambar")
        font.pointSize: 16
        palette {
            button: "#FFC100"
            buttonText: "#FFFFFF"
        }
    }

    Button {
        id: semVer
        x: 150
        y: 265
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
        x: 55
        y: 60
        width: 324
        height: 52
        text: qsTr("Control de Semafors")
        font.pixelSize: 36
    }

    Switch {
        id: conSem
        x: 136
        y: 325
        text: qsTr("Control Automatic Semafors")
    }
}
