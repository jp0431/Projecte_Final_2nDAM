import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    width: 400
    height: 400
    property alias btCon: btCon
    property alias tit: tit

    Text {
        id: tit
        x: 49
        y: 25
        width: 302
        height: 39
        color: "#e10909"
        text: qsTr("MQTT Desconnectat")
        font.pixelSize: 30
        horizontalAlignment: Text.AlignHCenter
    }

    Button {
        id: btCon
        x: 142
        y: 205
        text: qsTr("Connecta")
        font.pointSize: 12
    }
}
