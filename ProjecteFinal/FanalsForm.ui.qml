import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    id: item1
    width: 450
    height: 700
    property alias fanal: fanal
    property alias titF: titF
    property alias fanOff: fanOff
    property alias fanOn: fanOn

    Button {
        id: fanOn
        x: 49
        y: 563
        width: 100
        height: 32
        text: "Encén Fanal"
        font.pointSize: 12
    }

    Button {
        id: fanOff
        x: 206
        y: 563
        width: 111
        height: 32
        text: "Apaga Fanal"
        font.pointSize: 12
    }

    Text {
        id: titF
        x: 57
        y: 22
        width: 278
        height: 48
        color: "#dc0606"
        text: qsTr("Broker desconnectat")
        font.pixelSize: 30
        anchors.horizontalCenterOffset: 24
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: fanal
        x: 135
        y: 155
        width: 118
        height: 114
        color: "#CC8400"
        radius: 100
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/

