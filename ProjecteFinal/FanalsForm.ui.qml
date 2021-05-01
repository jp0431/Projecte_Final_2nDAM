import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    width: 400
    height: 400
    property alias manC: manC
    property alias titF: titF
    property alias fanOff: fanOff
    property alias fanOn: fanOn

    Button {
        id: fanOn
        x: 71
        y: 309
        width: 100
        height: 32
        text: "Encén Fanal"
        font.pointSize: 12
    }

    Button {
        id: fanOff
        x: 221
        y: 309
        width: 111
        height: 32
        text: "Apaga Fanal"
        font.pointSize: 12
    }

    Text {
        id: titF
        x: 32
        y: 53
        width: 336
        height: 48
        color: "#dc0606"
        text: qsTr("Broker desconnectat")
        font.pixelSize: 36
    }

    Switch {
        id: manC
        x: 129
        y: 210
        text: qsTr("Control Automatic")
    }
}
