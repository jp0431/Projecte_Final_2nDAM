#ifndef REREFONS_H
#define REREFONS_H

#include <QObject>
#include <QMqttClient>

class Rerefons : public QObject
{
    Q_OBJECT
public:
    explicit Rerefons(QObject *parent = nullptr);
    ~Rerefons();

signals:
   void senyalEstatConnexioMqtt(int nEstatConnexioMqtt);

public slots:
   void vBotoConnectaDesconnectaMqtt();
   void  vPublica(QString szTopic, QString szMis);

private slots:
   void updateLogStateChange();
   void brokerDisconnected();
 //  void vGestionaMissatgeRebut(const QByteArray &message);




private:
   int m_nEstat;
    QMqttClient *m_client;


};

#endif // REREFONS_H
