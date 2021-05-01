#include <Arduino.h>

#include "SPI.h"

#include "TFT_eSPI.h"

#include "../lvgl/src/lvgl.h"
#include <WiFi.h>
#include "IoT-02_wifiMng.h"
#include <WiFi.h>
#include "IoT-02_wifiMng.h"
#include "IoT-02_mqttCredentials.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>
#define LDR 36
WiFiClient espClient; 
PubSubClient client(espClient);
TFT_eSPI tft = TFT_eSPI();
static bool con;

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one I am using, its 340 ohms across the X plate

static lv_disp_buf_t disp_buf; //LVGL stuff;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;

lv_obj_t * btn1;
lv_obj_t * btn2;
lv_obj_t * btn3;

lv_obj_t *screenMain;
lv_obj_t *label;
void vMissatgeRebut(char* topic, byte* payload, unsigned int length){
  //Amb aquesta funció gestionem els misstages rebuts per MQTT
  String szTopic = String(topic), szPayload = "";
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.println("Missatge: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    szPayload += (char)payload[i];

  }
  lv_label_set_text(label,szPayload.c_str());

  }
void vConnexioMqtt(){
    while (!client.connected()) {
    Serial.print("Connectant al broker ...");
    String clientId = "palau";
    if (client.connect(clientId.c_str(),mqtt_user, mqtt_password)){
      //if (client.connect(clientId.c_str())) {
      Serial.println("connectat");
      /* subscribe topic */
    client.subscribe("/ldr"); // <-------   Subscription to MQTT(S) topic

  
   } else {
      Serial.print("Hi ha hagut un error, codi d'error =");
      Serial.print(client.state());
      Serial.println("Tornant a probar");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}
void vIniciMqtt(){

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(vMissatgeRebut);

}
static void wifi(){
  vSetupWifi();
  
  //szGetMac().toCharArray(sMac, MAC_SIZE);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lv_label_set_text(label,"Connectat");
  vIniciMqtt();
}
static void publica(){
String szLdr = String(analogRead(LDR));
Serial.print("LDR: "); Serial.println(szLdr);
  client.publish( String("/ldr").c_str(), szLdr.c_str());
}
static void event_handler_btn(lv_obj_t * obj, lv_event_t event){
    //lv_label_set_text(label, "Connectant a la Xarxa");

    if(event == LV_EVENT_CLICKED) {
        if (obj == btn1){
           // lv_label_set_text(label, "Connectant a la Xarxa");
           // delay(2000);
            wifi();}
        else if (obj == btn2){
            lv_label_set_text(label, "Desconectat");
            Serial.println(WiFi.disconnect());
        }
        else if(obj == btn3){
          publica();
        }
    }
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}



bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
  
  uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if(!touched)
    {
      return false;
    }

    if(touchX>320 || touchY > 240)
    {
      Serial.println("Y or y outside of expected parameters..");
      Serial.print("y:");
      Serial.print(touchX);
      Serial.print(" x:");
      Serial.print(touchY);
    }
    else
    {

      data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; 
  
      /*Save the state and save the pressed coordinate*/
      //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);
     
      /*Set the coordinates (if released use the last pressed coordinates)*/
      data->point.x = touchX;
      data->point.y = touchY;
  
    

    }


  return false; /*No buffering now so no more data read*/
}

void setup() {
  Serial.begin(115200);

  tft.begin();

  tft.setRotation(1);
  


  lv_init();

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display*/
  
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 320;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_input_read;         /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/


  screenMain = lv_obj_create(NULL, NULL);

  label = lv_label_create(screenMain, NULL);
  lv_label_set_long_mode(label, LV_LABEL_LONG_BREAK);
  lv_label_set_text(label, "Prem un boto");
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_size(label, 240, 40);
  lv_obj_set_pos(label, 20, 15);

  btn1 = lv_btn_create(screenMain, NULL);
  lv_obj_set_event_cb(btn1, event_handler_btn);
  lv_obj_set_width(btn1, 150);
  lv_obj_set_height(btn1, 32);
  lv_obj_set_pos(btn1, 80, 100);

  lv_obj_t * label1 = lv_label_create(btn1, NULL);
  lv_label_set_text(label1, "Connecta");

  btn3 = lv_btn_create(screenMain, NULL);
  lv_obj_set_event_cb(btn3, event_handler_btn);
  lv_obj_set_width(btn3, 150);
  lv_obj_set_height(btn3, 32);
  lv_obj_set_pos(btn3, 80, 150);

  lv_obj_t * label3 = lv_label_create(btn3, NULL);
  lv_label_set_text(label3, "Publica");

  btn2 = lv_btn_create(screenMain, NULL);
  lv_obj_set_event_cb(btn2, event_handler_btn);
  lv_obj_set_width(btn2, 140);
  lv_obj_set_height(btn2, 32);
  lv_obj_set_pos(btn2, 80, 200);

  

  lv_obj_t * label2 = lv_label_create(btn2, NULL);
  lv_label_set_text(label2, "Desconnecta");

  static lv_style_t style1;
  lv_style_init(&style1);
  lv_style_set_text_color(&style1, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_style_set_border_color(&style1, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_border_width(&style1, LV_STATE_DEFAULT, 2);
  lv_style_set_bg_color(&style1, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_style_set_bg_opa(&style1, LV_STATE_DEFAULT, 255);
  lv_style_set_radius(&style1, LV_STATE_DEFAULT, 1);
  

  static lv_style_t style2;
  lv_style_init(&style2);
  lv_style_set_text_color(&style2, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_style_set_border_color(&style2, LV_STATE_DEFAULT, lv_color_hex(0x222222));
  lv_style_set_border_width(&style2, LV_STATE_DEFAULT, 1);
  lv_style_set_bg_color(&style2, LV_STATE_DEFAULT, lv_color_hex(0x222222));
  lv_style_set_bg_opa(&style2, LV_STATE_DEFAULT, 255);
  lv_style_set_radius(&style2, LV_STATE_DEFAULT, 4);
  lv_style_set_shadow_spread(&style2, LV_STATE_DEFAULT, 1);
  lv_style_set_shadow_color(&style2, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_style_set_shadow_opa(&style2, LV_STATE_DEFAULT, 255);
  lv_style_set_shadow_width(&style2, LV_STATE_DEFAULT, 1);

  static lv_style_t bigStyle;
  lv_style_init(&bigStyle);
  lv_style_set_text_font(&bigStyle ,LV_STATE_DEFAULT, &lv_font_montserrat_36);



  lv_obj_add_style(label, LV_LABEL_PART_MAIN, &bigStyle);

  //Uncomment to chabge buutton 1 styling:
  //lv_obj_add_style(btn1, LV_LABEL_PART_MAIN, &style1);

  //Uncomment to chabge buutton 2 styling:
  //lv_obj_add_style(btn2, LV_LABEL_PART_MAIN, &style2);
  

  lv_scr_load(screenMain);
vSetupWifi();
vIniciMqtt();
}

void loop() {

  lv_task_handler();

  delay(1);
if (!client.connected()) {
      vConnexioMqtt();
  }

  client.loop();

  client.loop();
}