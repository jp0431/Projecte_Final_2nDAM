using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

public class MqttSemafor : MonoBehaviour
{
    public Light red;
    public Light ambar;
    public Light verd;
    

    // Start is called before the first frame update
    void Start()
    {
        MQTTClass.Connect();
        red.enabled = false;
        ambar.enabled = false;
        verd.enabled = false;

       

    }
    // Update is called once per frame
    void Update()
    {
        if (MQTTClass.topic == "/Vermell" && MQTTClass.msg == "1")
        {
            red.enabled = true;
            ambar.enabled = false;
            verd.enabled = false;
            MQTTClass.Publish("/Vermell_esp", "1");       
            MQTTClass.Publish("/Verd_esp", "0");
            MQTTClass.Publish("/Groc_esp", "0");

            MQTTClass.topic = "";
        }



        if (MQTTClass.topic == "/Groc" &&  MQTTClass.msg == "1")
        {
            red.enabled = false;
            ambar.enabled = true;
            verd.enabled = false;
            MQTTClass.Publish("/Vermell_esp", "0");       
            MQTTClass.Publish("/Verd_esp", "0");
            MQTTClass.Publish("/Groc_esp", "1");

            MQTTClass.topic = "";            
        }


        if (MQTTClass.topic == "/Verd" &&  MQTTClass.msg == "1")
        {
            red.enabled = false;
            ambar.enabled = false;
            verd.enabled = true;
            MQTTClass.Publish("/Vermell_esp", "0");       
            MQTTClass.Publish("/Verd_esp", "1");
            MQTTClass.Publish("/Groc_esp", "0");

            MQTTClass.topic = "";      }
    
    }
    
}
