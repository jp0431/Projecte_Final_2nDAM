#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

#if USE_LV_LOG != 0
#define wh 320
#define he 240

lv_obj_t *btn;
lv_obj_t *label;
lv_obj_t *btn2;
lv_obj_t *label2;
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{

    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

/* Display flushing */
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

/* Reading input device (simulated encoder here) */
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
    static int32_t last_diff = 0;
    int32_t diff = 0; /* Dummy - no movement */
    int btn_state = LV_INDEV_STATE_REL; /* Dummy - no press */

    data->enc_diff = diff - last_diff;;
    data->state = btn_state;

    last_diff = diff;

    return false;
}

static void event_handler( lv_event_t event)
{

    if (event == LV_EVENT_CLICKED) {
        Serial.println("Clicked\n");
            lv_label_set_text(label, "Button ");

        
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Toggled\n");
    }
}

bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
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
      if((touchX>95&& touchX <224)&&(touchY>139&&touchY<181)){
        Serial.println("Boto1 premut");
        lv_event_t event =  LV_EVENT_CLICKED;
        event_handler(event);
      }
      else{
        Serial.println("Boto1 no premut");
          

      }
  /*
      Serial.print("Data x ");
      Serial.println(touchX);
      
      Serial.print("Data y ");
      Serial.println(touchY);
*/
    }

    return false; /*Return `false` because we are not buffering and no more data to read*/
}
void setup()
{

    Serial.begin(115200); /* prepare for possible serial debug */

    lv_init();

#if USE_LV_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin(); /* TFT init */
    tft.setRotation(1); /* Landscape orientation */

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
 uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  tft.setTouch(calData);
    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
    lv_indev_drv_register(&indev_drv);


    btn = lv_btn_create(lv_scr_act(), NULL);
  //  lv_obj_set_event_cb(btn, event_handler);
    lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, 40);
    lv_btn_set_checkable(btn, false);
    lv_btn_toggle(btn);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, "Button 1");
    /*

 btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn, event_handler);
    lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, -20);
    lv_btn_set_checkable(btn2, false);
    lv_btn_toggle(btn2);
    label2 = lv_label_create(btn, NULL);
    lv_label_set_text(label2, "Button 2");
    Serial.println("coordenades");
    Serial.println(btn2->coords.x1);
    Serial.println(btn2->coords.x2);
    Serial.println(btn2->coords.y1);
    Serial.println(btn2->coords.y2);*/


}


void loop()
{

    lv_task_handler(); /* let the GUI do its work */
    delay(5);
}
