using System;
using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
public static class MQTTClass 
{
    public static MqttClient _mqttClient;
    public static string brokerHostname = "formacio.things.cat";
    public static int brokerPort = 1883;
    public static string userName = "ecat";
    public static string password = "clotClot";
    public static string msg;
    public static string topic;
    public static bool manF;

    public static void Connect()
    {
        Debug.Log("about to connect on '" + brokerHostname + "'");

        _mqttClient = new MqttClient(brokerHostname);
        string clientId = Guid.NewGuid().ToString();
        Debug.Log("About to connect using '" + userName + "' / '" + password + "'");
        try
        {
            _mqttClient.Connect(clientId, userName, password);
        }
        catch (Exception e)
        {
            Debug.LogError("Connection error: " + e);
        }
        _mqttClient.MqttMsgPublishReceived += client_MqttMsgPublishReceived;
        byte[] qosLevels = { MqttMsgBase.QOS_LEVEL_AT_LEAST_ONCE };
        _mqttClient.Subscribe(new string[] { "/Vermell" }, qosLevels);
        _mqttClient.Subscribe(new string[] { "/Verd" }, qosLevels);
        _mqttClient.Subscribe(new string[] { "/Groc" }, qosLevels);
        _mqttClient.Subscribe(new string[] { "/light" }, qosLevels);
        _mqttClient.Subscribe(new string[] { "/ControlManualFanal" }, qosLevels);
        _mqttClient.Subscribe(new string[] { "/ControlManualFanal" }, qosLevels);

        


    } 
    static void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
    {
        msg = System.Text.Encoding.UTF8.GetString(e.Message);
        Debug.Log ("Received message from " + e.Topic + " : " + msg);
        topic = e.Topic;

    }
    public static void Publish(string topic,string msg1)
    {
       
        // string _topic = "/light";
        _mqttClient.Publish(
            topic, Encoding.UTF8.GetBytes(msg1),
            MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
    }
}
