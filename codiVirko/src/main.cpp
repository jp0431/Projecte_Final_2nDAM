#include <Arduino.h>
#include <SoftwareSerial.h>                                // Remove if using HardwareSerial or Arduino package without SoftwareSerial support

#include "SPI.h"
#include "TFT_eSPI.h"
#include "touchScreenPoint.h"
#include "../lvgl/src/lvgl.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include "credentials.h"
#include "topics.h"

bool bAutoCal_CO2 = false,bAutoCalibrationEnabled;
#define MINIM_TEMPS_CALIBRACIO 60000

#define RX_PIN 35 /*10 */                                         // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 19 /*11 */ 
#define HOR_RES 320
#define VER_RES 240
#define LDRP 36
#include <WiFi.h>
 #define USING_MH_Z19

#ifdef USING_MH_Z19
#define MEASUREMENT_INTERVAL 2000
#endif
#ifdef USING_MH_Z19
  #include "MHZ19.h"
#endif

#ifdef USING_MH_Z19
  MHZ19 myMHZ19;                                             // Constructor for library
#endif
int nCO2=400;
int8_t nTemp;

unsigned long getDataTimer = 0;
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial
static bool enllestit=false;
TFT_eSPI tft = TFT_eSPI();
WiFiClient espClient;
PubSubClient client(espClient);
TouchScrPoint oldPoint;
 char* ssid ="MIWIFI_xGmM";
const char* pwd ="gQMG7nRU";
static lv_disp_buf_t disp_buf; //LVGL stuff;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
 char  wifi[]="";
 char buff[32];
 static bool con=false;
lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;
static bool scan = false;
static lv_obj_t *screenMain;
static lv_obj_t * kb; /* Teclat */
static lv_obj_t * ta; /* Àrea de text */
static lv_obj_t * lvglpage;/*pagina principal*/
static lv_obj_t * lvgPageTec;
static lv_obj_t * lvgPageWifi;
static lv_obj_t *label;
static lv_obj_t *ledR;
static lv_obj_t *ledY;
static lv_obj_t *ledG;
static lv_obj_t *CO2;
static lv_obj_t *Temperatura;
static lv_obj_t *LDR;
static lv_obj_t *temp;
static lv_obj_t *co2;
static lv_obj_t *ldr;
static lv_obj_t *hora;

static bool bSoconnectat =false;
void getCO2();
static void event_handler(lv_obj_t * obj,lv_event_t event){
  if (event == LV_EVENT_CLICKED) {
    WiFi.disconnect(WIFI_OFF);
    lv_obj_set_hidden(lvgPageWifi, true);
    lv_obj_set_hidden(lvglpage, false);

  }

}
void  createInfoWifi(){

  static lv_style_t style1;
  lv_style_init(&style1);
  static lv_style_t style2;
  lv_style_init(&style2);
  static lv_style_t styleY;
  lv_style_init(&styleY);
  static lv_style_t styleG;
  lv_style_init(&styleG);

  lv_style_set_text_color(&style1, LV_STATE_DEFAULT, LV_COLOR_RED );  
  lv_style_set_bg_color(&style2, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_shadow_color(&style2, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_bg_color(&styleY, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_style_set_shadow_color(&styleY, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_style_set_bg_color(&styleG, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_style_set_shadow_color(&styleG, LV_STATE_DEFAULT, LV_COLOR_GREEN);

  lvgPageWifi = lv_cont_create( lv_scr_act(), NULL );
  lv_obj_set_width( lvgPageWifi, lv_disp_get_hor_res( NULL )  );  // Horizontal resolution 
  lv_obj_set_height( lvgPageWifi, lv_disp_get_ver_res( NULL ) );


  lv_obj_t *Titol =lv_label_create(lvgPageWifi,NULL);
  lv_label_set_text(Titol,"DADES DE LA CIUTAT UNITY");
  lv_obj_align(Titol, NULL, LV_ALIGN_CENTER, 0, -100);
  lv_obj_add_style(Titol, LV_LABEL_PART_MAIN,&style1);
  Temperatura =lv_label_create(lvgPageWifi,NULL);
  lv_label_set_text(Temperatura,"TEMPERATURA: ");
  lv_obj_set_pos(Temperatura, 15, 40);

  temp=lv_label_create(lvgPageWifi,NULL);
  lv_obj_set_pos(temp, 150, 40);


  CO2 =lv_label_create(lvgPageWifi,NULL);
  lv_label_set_text(CO2,"NIVELL DE CO2: ");
  lv_obj_set_pos(CO2, 15, 60);

  co2=lv_label_create(lvgPageWifi,NULL);
  lv_obj_set_pos(co2, 150, 60);

  LDR =lv_label_create(lvgPageWifi,NULL);
  lv_label_set_text(LDR,"NIVELL DE LLUM: ");
  lv_obj_set_pos(LDR, 15, 80);

  ldr=lv_label_create(lvgPageWifi, NULL);
  lv_obj_set_pos(ldr, 150, 80);

  lv_obj_t *temps =lv_label_create(lvgPageWifi,NULL);
  lv_label_set_text(temps,"HORA REAL: ");
  lv_obj_set_pos(temps, 15, 100);

  hora=lv_label_create(lvgPageWifi, NULL);
  lv_obj_set_pos(hora, 150, 100);


  ledR  = lv_led_create(lvgPageWifi, NULL);
 lv_obj_align(ledR, NULL, LV_ALIGN_CENTER, -100, 30);
 lv_obj_add_style(ledR, LV_LABEL_PART_MAIN,&style2);
 lv_led_on(ledR);

  ledY  = lv_led_create(lvgPageWifi, NULL);
 lv_obj_align(ledY, NULL, LV_ALIGN_CENTER, 0, 30);
 lv_obj_add_style(ledY, LV_LABEL_PART_MAIN,&styleY);
 lv_led_off(ledY);
 
   ledG = lv_led_create(lvgPageWifi, NULL);
 lv_obj_align(ledG, NULL, LV_ALIGN_CENTER, 100, 30);
 lv_obj_add_style(ledG, LV_LABEL_PART_MAIN,&styleG);
 lv_led_off(ledG);
 
 

  lv_obj_t * btn1 = lv_btn_create(lvgPageWifi, NULL);
  lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 90);
  lv_obj_t *BTL = lv_label_create(btn1, NULL);
  lv_label_set_text(BTL, "DESCONNECTA");
   

  getCO2();
  enllestit=true;

}
void vMissatgeRebut(char* topic, byte* payload, unsigned int length) {
  //Amb aquesta funció gestionem els misstages rebuts per MQTT
  String szTopic = String(topic), szPayload = "";
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.println("Missatge: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    szPayload += (char)payload[i];
  }

//Serial.println(szPayload);
  if (szTopic == String(SEMAFOR_VERMELL).c_str()) {
    if ((char)payload[0] == '1') {
     
      lv_led_on(ledR);
      lv_led_off(ledY);
      lv_led_off(ledG);


    } 
  }
  if (szTopic == String(SEMAFOR_GROC).c_str()) {
    if ((char)payload[0] == '1') {
      lv_led_on(ledY);
      lv_led_off(ledR);
      lv_led_off(ledG);

    } 
  }
  if (szTopic == String(SEMAFOR_VERD).c_str()) {
    if ((char)payload[0] == '1') {
      lv_led_on(ledG);
      lv_led_off(ledY);
      lv_led_off(ledR);

    } 
  }
 
  if(szTopic==String(TOPIC_HORA).c_str()){
    lv_label_set_text(hora,szPayload.c_str());
  }
}
void vConnexioMqtt() {
  while (!client.connected()) {
    Serial.print("Connectant al broker ...");
    String clientId = "virko2";
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      //if (client.connect(clientId.c_str())) {
      Serial.println("connectat");
      // digitalWrite(LED_BUILTIN,HIGH);
      /* subscribe topic 
      //      client.subscribe(TOPIC_REQUEST_MAC); //*/// <-------   Subscription to MQTT(S) topic
      client.subscribe(String(SEMAFOR_VERMELL).c_str());
      client.subscribe( String(SEMAFOR_GROC).c_str());
      client.subscribe( String(SEMAFOR_VERD).c_str());
      client.subscribe(String(TOPIC_HORA).c_str());
            




//      client.subscribe( String(TOPIC_ENDEVANT).c_str());



      // client.subscribe( String(TOPIC_LDR).c_str());

    } else {
      Serial.print("Hi ha hagut un error, codi d'error =");
      Serial.print(client.state());
      Serial.println("Tornant a probar");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}
void vIniciMqtt() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(vMissatgeRebut);

}
void getCO2(){
    unsigned long int ulNow = millis();
 if (millis() - getDataTimer >= 2000)
    {


        /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even 
        if below background CO2 levels or above range (useful to validate sensor). You can use the 
        usual documented command with getCO2(false) */

        nCO2 = myMHZ19.getCO2();                             // Request CO2 (as ppm)
        
                   
        String szCo2=String(nCO2)+" ppm";                     
        lv_label_set_text(co2, szCo2.c_str());
        nTemp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
                     
        String szTemp =String(nTemp)+" (C)";
        lv_label_set_text(temp, szTemp.c_str());
        
        String llum=String(analogRead(LDRP));
        lv_label_set_text(ldr,llum.c_str());
        getDataTimer = millis();
  int lightVal = analogRead(LDRP);    
   client.publish(TOPIC_LDR, String(lightVal).c_str());
   client.publish(TOPIC_CO2, String(nCO2).c_str());  
   client.publish(TOPIC_TEMP, String(nTemp).c_str());
      if (bAutoCal_CO2) {
      bAutoCal_CO2 = false;
      if(ulNow > MINIM_TEMPS_CALIBRACIO){
        Serial.println("Fent l'autocalibració del CO2");
        myMHZ19.calibrateZero();
      }else{
        Serial.println("Per recalibrar cal esperar un mínim de 5 minuts des del darrer reset");
      }
    }
    }
  
}

static void kb_event_cb(lv_obj_t * keyboard, lv_event_t e){
  lv_keyboard_def_event_cb(kb, e);
  if(e == LV_EVENT_CANCEL) {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_del(kb);
    kb = NULL;
    lv_obj_set_hidden(lvgPageTec, true);
    lv_obj_set_hidden(lvglpage, false);
    

  }
  if(e == LV_EVENT_APPLY){ 
    String szTextEscrit = lv_textarea_get_text(ta);
    Serial.println(szTextEscrit);
    
    
    String s=String(buff);
    String b="";
    for(int i=0;i<strlen(s.c_str());i++){
      Serial.println(buff[i]);
        b = b + buff[i];
      if(buff[i+1]==','){
        break;
      }
    }
    Serial.println(b);
   
  
    Serial.print("Xarxa: ");
    Serial.println(s);
    Serial.print("Contrasenya: ");
    Serial.println(szTextEscrit.c_str());
     pwd=szTextEscrit.c_str();
     //pwd="50293d12";
      Serial.printf("Connecting to %s ", buff);
     
      WiFi.begin(b.c_str(), pwd);
      while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      bSoconnectat=true;
  }
  Serial.println(" CONNECTED");
  vIniciMqtt();
  vConnexioMqtt();
  

  //lv_label_set_text(label, "CONNECTAT");
    lv_obj_set_hidden(lvgPageTec, true);
        createInfoWifi();  
  }
}

static void kb_create(void){
  kb = lv_keyboard_create(lvgPageTec, NULL);
  lv_keyboard_set_cursor_manage(kb, true);
  lv_obj_set_event_cb(kb, kb_event_cb);
  lv_keyboard_set_textarea(kb, ta);
}


static void ta_event_cb(lv_obj_t * ta_local, lv_event_t e){
  if(e == LV_EVENT_CLICKED && kb == NULL) {
    kb_create();
  }
}


void lv_ex_keyboard_1(void){
  /*Create a text area. The keyboard will write here*/

  lvgPageTec = lv_cont_create( lv_scr_act(), NULL );
  lv_obj_set_width( lvgPageTec, lv_disp_get_hor_res( NULL )  );  // Horizontal resolution 
  lv_obj_set_height( lvgPageTec, lv_disp_get_ver_res( NULL ) );
  ta  = lv_textarea_create(lvgPageTec, NULL);
  //ta  = lv_textarea_create(scr, NULL);
  lv_obj_align(ta, NULL, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 16);
  lv_obj_set_event_cb(ta, ta_event_cb);
  lv_textarea_set_text(ta, "");
  lv_coord_t max_h = LV_VER_RES / 2 - LV_DPI / 8;
  if(lv_obj_get_height(ta) > max_h) lv_obj_set_height(ta, max_h);

  kb_create();
}



static void wifi_sel(lv_obj_t * obj,lv_event_t event){
  if (event == LV_EVENT_CLICKED) {
      lv_obj_set_hidden(lvglpage, true);
      /*lv_obj_set_hidden(lvgPageTec, true);*/
      lv_roller_get_selected_str(obj, buff, sizeof(buf));
      Serial.println(sizeof(buff));
      
      printf("Xarxa seleccionada: %s\n", buff);
      lv_ex_keyboard_1();
  }

}


void  createMain(){
   lvglpage = lv_cont_create( lv_scr_act(), NULL );
  lv_obj_set_width( lvglpage, lv_disp_get_hor_res( NULL )  );  // Horizontal resolution 
  lv_obj_set_height( lvglpage, lv_disp_get_ver_res( NULL ) );

    lv_obj_t *roller1 = lv_roller_create(lvglpage, NULL);
    lv_roller_set_options(roller1,
                      wifi,
                        LV_ROLLER_MODE_NORMAL);

    lv_roller_set_visible_row_count(roller1, 4);
    lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(roller1, wifi_sel);
    label=lv_label_create(lvglpage,NULL);
    lv_label_set_text(label,"Escull xarxa sense fils");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_obj_set_size(label,60,24);

/*
  lv_obj_t * btn1 = lv_btn_create(lvglpage, NULL);
  lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 40);
  lv_obj_t *label = lv_label_create(btn1, NULL);
  lv_label_set_text(label, "Ves al teclat");*/


}

TouchScrPoint tftTouchGetPoint(){
  uint16_t x, y;
  TouchScrPoint tsp(-1,-1);

  if (tft.getTouch(&x, &y)) {
    tsp = TouchScrPoint(x,y);
  }

  return tsp;
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
  uint16_t x, y;

  if (tft.getTouch(&x, &y)){
    data->state = LV_INDEV_STATE_PR;
    data->point.x = oldPoint.x = x;
    data->point.y = oldPoint.y = y;
    // String sz = "x: " + String(x,DEC) + " , y: " + String(y,DEC);
    // Serial.println(sz);   
  }else{
    data->state = LV_INDEV_STATE_REL;
    data->point.x = oldPoint.x;
    data->point.y = oldPoint.y;    
  }

  return false; 
}

void scanWifi(){
  
  if(!scan) {
   Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
          String wifis=String(WiFi.SSID(i))+" "+ String(WiFi.RSSI(i))+"\n";
            strcat(wifi, String(String(WiFi.SSID(i))+", "+ String(WiFi.RSSI(i))+"\n").c_str());
        /*    // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            //Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            //delay(10);*/
             Serial.print(".");
        }
                   // Serial.println(wifi);
    
    }
    Serial.println("");
    scan=true;
    }
}
void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  oldPoint = tftTouchGetPoint();
  lv_init();
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  /*Initialize the display*/
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = HOR_RES;
  disp_drv.ver_res = VER_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_input_read;         /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

  screenMain = lv_obj_create(NULL, NULL);
  lv_scr_load(screenMain); // Per a poder fer servir lv_scr_act() (Pantalla activa)
  scanWifi();
  createMain();

   mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start   
    //mySerial.begin(BAUDRATE, RX_PIN, TX_PIN); // (ESP32 Example) device to MH-Z19 serial start   
    myMHZ19.begin(mySerial);                                // *Serial(Stream) refence must be passed to library begin(). 

    myMHZ19.autoCalibration(true); 


 // lv_ex_keyboard_1();
}

void loop() {
  lv_task_handler();

  delay(1);
  
  client.loop();
  if(enllestit){
    getCO2();
    delay(10000);
  }
  
}