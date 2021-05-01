using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using UnityEngine;
using UnityEngine.Experimental.GlobalIllumination;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Exceptions;
using uPLibrary.Networking.M2Mqtt.Messages;
using uPLibrary.Networking.M2Mqtt.Utility;
public class Mqtt : MonoBehaviour
{
     MqttClient client;
     public Light luz;
     private static bool on;
     private static bool off;

     private bool envia;
     

    // Start is called before the first frame update
    void Start()
    {
        MQTTClass.Connect();
        on = false;
        off = false;

    }

    // Update is called once per frame


    private void Update()
    {
        
        gestionaMsg();
    }

    void gestionaMsg()
    {
        if (MQTTClass.topic == "/light" && MQTTClass.msg == "0")
        {
                   
            luz.enabled = false;
           
         if (!on)
         {MQTTClass.Publish("/fanal", "0");
             @on = true;
             off = false;
         }
        }

        

        if (MQTTClass.topic == "/light" && MQTTClass.msg == "1")
        {
            luz.enabled = true;

            if (!off)
            {
                MQTTClass.Publish("/fanal", "1");
                @off = true;
                @on = false;
            }


        }
         

        if (MQTTClass.topic == "/ControlManualFanal" && MQTTClass.msg == "1")
        {
            MQTTClass.manF = true;
        } if (MQTTClass.topic == "/ControlManualFanal" && MQTTClass.msg == "0")
        {
            MQTTClass.manF = false;
            MQTTClass.manF = false;
        }

    }

   
}
