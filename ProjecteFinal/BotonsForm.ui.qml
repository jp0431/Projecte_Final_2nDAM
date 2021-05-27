import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    id: item1
    width: 450
    height: 700
    property alias btCon: btCon
    property alias tit: tit

    Text {
        id: tit
        x: 79
        y: 28
        width: 302
        height: 39
        color: "#e10909"
        text: qsTr("MQTT Desconnectat")
        font.pixelSize: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Image {
        id: img
        x: 46
        y: 142
        width: 291
        height: 303
        source: "imatges/mosquitto-logo.png"
    }

    Button {
        id: btCon
        x: 109
        y: 550
        width: 183
        height: 47
        text: qsTr("Connecta")
        anchors.horizontalCenterOffset: -6
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 12
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}
}
##^##*/

