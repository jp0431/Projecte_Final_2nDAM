import desDel.rerefons 1.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2

ApplicationWindow {
    id: aw
    width: 400
    height: 400
    visible: true
    title: qsTr("Projecte final Jaume Palau Seijas")
    function actualitzaFrontal(nEstatConnexioMqtt){
    switch(nEstatConnexioMqtt){
          case 0:
             p1.tit.text = qsTr("MQTT Desconnectat")
              p1.tit.color = "#ee2222"
              fan.titF.text=qsTr("Broker Desconnectat")
              fan.titF.color="#ee2222"

              break;
          case 1:
             p1.tit.text = qsTr("MQTT Connectant")
             p1.tit.color = "#2222ee"
              //eMqtt.btConnecta.enabled = false

              break;
          case 2:
             p1.tit.text = qsTr("MQTT connectat")
             p1.tit.color = "#22ee22"
             p1.btCon.text =qsTr("Des&connecta")
             fan.titF.text=qsTr("Broker connectat")
             fan.titF.color="#22ee22"
             break;
          }
    }
    Rerefons{
        id: be
        onSenyalEstatConnexioMqtt:    {
            console.log(nEstatConnexioMqtt)
            aw.actualitzaFrontal(nEstatConnexioMqtt)
        }

    }
    SwipeView{
        id: sw
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
    Botons{
        id: p1
         btCon.onClicked:  {
             be.vBotoConnectaDesconnectaMqtt()
         }
        }
    Fanals{
        id:fan
        fanOn.onClicked: {
            be.vPublica("/light","1")
        }

        fanOff.onClicked: {
            be.vPublica("/light","0")

        }
        manC.onClicked:
        {
             be.vPublica("/ControlManualFanal", manC.position?"0":"1")
            if(manC.position==1){
            fanOn.enabled=false
            fanOff.enabled=false}
            else{
                fanOn.enabled=true
                fanOff.enabled=true
            }
        }
    }
    Semaf{
        id:sf
        semV.onClicked: {
            be.vPublica("/Vermell", "1")
        }
        semG.onClicked: {
            be.vPublica("/Groc", "1")
        }
        semVer.onClicked: {
            be.vPublica("/Verd", "1")
        }
        conSem.onClicked:{
          be.vPublica("/ControlManualSemafors", conSem.position?"0":"1")
            if(conSem.position==1){
            semV.enabled=false
            semV.palette.button="#A02323"
            semVer.enabled=false
            semVer.palette.button="#179300"
            semG.enabled=false
            semG.palette.button="#BB8F05"
            semG.


            console.log("Switch premut")
            }
            else{
                semV.enabled=true
                semV.palette.button="#FE0000"

                semVer.enabled=true
                semVer.palette.button="#27FF00"
                semG.enabled=true
                semG.palette.button="#FFC100"
                console.log("Switch premut")

            }

        }
    }

    }
}
