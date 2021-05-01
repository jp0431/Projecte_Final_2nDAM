#include "rerefons.h"
#define BROKER_NAME "formacio.things.cat"
#define BROKER_PORT 1883
#define BROKER_USER "ecat"
#define BROKER_PASSWORD "clotClot"
Rerefons::Rerefons(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setHostname(BROKER_NAME);
    m_client->setPort(BROKER_PORT);

    connect(m_client, &QMqttClient::stateChanged, this, &Rerefons::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &Rerefons::brokerDisconnected);
    m_nEstat =1;
}
void Rerefons::vBotoConnectaDesconnectaMqtt(){
    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->setUsername(BROKER_USER);
        m_client->setPassword(BROKER_PASSWORD);

        m_client->connectToHost();
    } else {
        m_client->disconnectFromHost();
    }
}
void Rerefons::updateLogStateChange(){
   const QString content = QDateTime::currentDateTime().toString()
                   + QLatin1String(": State Change")
                   + QString::number(m_client->state())
                   + QLatin1Char('\n');
   qDebug() << content;
   emit senyalEstatConnexioMqtt(m_nEstat = m_client->state());

   if(m_nEstat == 2){

       qDebug()<<"Connectat correctament";
   }
}
void Rerefons::vPublica(QString szTopic, QString szMis){
    qDebug() << "Tema: " << szTopic << ", Missatge: " << szMis;
    if (m_client->publish(szTopic, szMis.toUtf8()) == -1)
        qDebug() << "Error. No es pot publicar el missatge";
}

void Rerefons::brokerDisconnected(){
   qDebug() << "Broker desconnectat!";
}
Rerefons::~Rerefons(){
   if (m_client->state() == QMqttClient::Connected || m_client->state() == QMqttClient::Connecting) {
       m_client->disconnectFromHost();
   }}
