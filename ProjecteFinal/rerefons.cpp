#include "rerefons.h"
#define BROKER_NAME "palauseijas.duckdns.org"
#define BROKER_PORT 1883
#define BROKER_USER "palau"
#define BROKER_PASSWORD "1234"
Rerefons::Rerefons(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setHostname(BROKER_NAME);
    m_client->setPort(BROKER_PORT);

    connect(m_client, &QMqttClient::stateChanged, this, &Rerefons::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &Rerefons::brokerDisconnected);
    connect(m_client, &QMqttClient::messageReceived, this, &Rerefons::vGestionaMissatgeRebut);

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
       QString szSubscriptionTopic1 = QString("/") + "fanal";
       QString vermell = QString("/") + "Vermell_esp";
       QString ambar = QString("/") + "Groc_esp";
       QString verd = QString("/") + "Verd_esp";

       auto subscription1 = m_client->subscribe(szSubscriptionTopic1);
       auto sVerm = m_client->subscribe(vermell);
       auto sGroc = m_client->subscribe(ambar);
       auto sVerd = m_client->subscribe(verd);

       if (!subscription1||!sVerm||!sGroc||!sVerd) {
           qDebug() << "Error: No m'hi puc subscriure. Hi ha una connexió vàlida?";
           return;
       }else
           qDebug() << "Subscrit a " << szSubscriptionTopic1;
   }
}
void Rerefons::vPublica(QString szTopic, QString szMis){
    qDebug() << "Tema: " << szTopic << ", Missatge: " << szMis;
    if (m_client->publish(szTopic, szMis.toUtf8()) == -1)
        qDebug() << "Error. No es pot publicar el missatge";
}
void Rerefons::vGestionaMissatgeRebut(const QByteArray &message, const QMqttTopicName &topic){

   const QString content = QDateTime::currentDateTime().toString()
               + QLatin1String(" Received Topic: ")
               + topic.name()
               + QLatin1String(" Message: ")
               + message
               + QLatin1Char('\n');
   qDebug() << content;
   if(topic.name()=="/fanal"){

        emit estatFanal(message);
    }
   if(topic.name()=="/Vermell_esp"){
       qDebug()<<"Vermell";
        emit estatVermell(message);
   }
   if(topic.name()=="/Groc_esp"){
       emit estatGroc(message);

   }
   if(topic.name()=="/Verd_esp"){
       emit estatVerd(message);

   }
}

void Rerefons::brokerDisconnected(){
   qDebug() << "Broker desconnectat!";
}
Rerefons::~Rerefons(){
   if (m_client->state() == QMqttClient::Connected || m_client->state() == QMqttClient::Connecting) {
       m_client->disconnectFromHost();
   }}
