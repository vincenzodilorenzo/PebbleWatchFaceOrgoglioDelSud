#include <pebble.h>
#include "main.h"
#include "num2words.h"

/* static pointer to the variabile Window to handle the main window */
static Window      *s_main_window;
/* static pointers to the variabile TextLayer to display hours and shaded hours */
static TextLayer   *s_hour_layer;
static TextLayer   *s_hour_layer_shadow;
/* static pointer to the variabile TextLayer to display minutes and shaded minutes */
static TextLayer   *s_minute_layer;
static TextLayer   *s_minute_layer_shadow;
/* static pointer to the variabile TextLayer to display days */
static TextLayer   *s_day_layer;
/* static pointer to the variabile TextLayer to display the date */
static TextLayer   *s_date_layer;
/* static pointer to the variabiles GBitmap and BitmapLayer to manage the background image */
static GBitmap     *s_bitmap;
static BitmapLayer *s_bitmap_layer;

/* battery level and static pointer to the variables GBitmap(s) and BitmapLayer to display the battery level */
static int         s_battery_level;
static GBitmap     *s_battery_bitmap_100;
static GBitmap     *s_battery_bitmap_90;
static GBitmap     *s_battery_bitmap_80;
static GBitmap     *s_battery_bitmap_70;
static GBitmap     *s_battery_bitmap_60;
static GBitmap     *s_battery_bitmap_50;
static GBitmap     *s_battery_bitmap_40;
static GBitmap     *s_battery_bitmap_30;
static GBitmap     *s_battery_bitmap_20;
static GBitmap     *s_battery_bitmap_10;
static BitmapLayer *s_battery_bitmap_layer;

/* static pointer to the variables GBitmap(s) and BitmapLayer to display the bluetooth status */
static BitmapLayer *s_bt_bitmap_layer;
static GBitmap     *s_bt_disconnected_bitmap; 
static GBitmap     *s_bt_connected_bitmap;

/* pinter to the custome font*/
static GFont custom_font;
static GFont custom_font_super_small;
static GFont custom_font_shadow;


/* 
   This static function extracts hours, minutes, day of the week and date, 
   It's invoked in init() and in tick_handler().
*/
static void update_time() {
  
  /* Get the struct tick_time e put in it the current localtime */
  time_t temp = time(NULL);
  struct tm tick_time = *localtime(&temp);

  /* Handle hours */
  /* Extract hours and fill a char buffer */
  static char s_hour_buffer[5];
  strftime(s_hour_buffer, sizeof(s_hour_buffer), "%H", &tick_time);
  /* Insert the char bufer into a couple of dedicated TextLayers */
  text_layer_set_text(s_hour_layer, s_hour_buffer);
  text_layer_set_text(s_hour_layer_shadow, s_hour_buffer);

  /* Handle minutes */
  /* Extract minutes and fill a char buffer */
  static char s_minute_buffer[5];
  strftime(s_minute_buffer, sizeof(s_minute_buffer), "%M" , &tick_time);
  /* Insert the char bufer into a couple of dedicated TextLayers */
  text_layer_set_text(s_minute_layer, s_minute_buffer);
  text_layer_set_text(s_minute_layer_shadow, s_minute_buffer);
  
  /* Handle days of week */
  /* Extract days, convert them in Napolitan Language and fill a char buffer */
  static char s_day_buffer[30];
  days_to_common_words(tick_time.tm_wday, s_day_buffer);
  /* Insert the char bufer into a dedicated TextLayer */
  text_layer_set_text(s_day_layer, s_day_buffer);
  
  /* Handle current date */  
  /* Extract current date, comvert in format DD/MM/YYYY and fill a char buffer */
  static char s_date_buffer[15];
  date_to_string(tick_time.tm_year, tick_time.tm_mon, tick_time.tm_mday, s_date_buffer);
  /* Insert the char bufer into a dedicated TextLayer */
  text_layer_set_text(s_date_layer, s_date_buffer);
}



/* 
   This static function initializes the main window. 
   It's linked in init() to the "load" event of the main window.
*/
static void main_window_load(Window *window) {
  
  /* load custom font */
  custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TWELVE_CENTURY_58));
  custom_font_shadow = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TWELVE_CENTURY_SHADOW_58));
  custom_font_super_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TWELVE_CENTURY_16));
   
  /* Prepare the background image */
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_IMAGE_RECT);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
  
  /* Get the root layer of the main window, get the main window limits */
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  /* Create two TextLayers to display hours, one of them is needed to implement a shadow effct */
  /* Base TextLayer for hours */
  s_hour_layer = text_layer_create(GRect(20, 25, bounds.size.w, 70));
  /* Define some more additional properties */
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorWhite);
  text_layer_set_font(s_hour_layer, custom_font);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentLeft);
  /* Additional TextLayer for hours to implement a shadow effct */
  s_hour_layer_shadow = text_layer_create(GRect(20, 25, bounds.size.w, 70));
  /* Define some more properties */
  text_layer_set_background_color(s_hour_layer_shadow, GColorClear);
  text_layer_set_text_color(s_hour_layer_shadow, GColorBlack);
  text_layer_set_font(s_hour_layer_shadow, custom_font_shadow);
  text_layer_set_text_alignment(s_hour_layer_shadow, GTextAlignmentLeft);

  /* Create two TextLayers to display minutes, one of them is needed to implement a shadow effct */
  /* Base TextLayer for minutes */
  s_minute_layer = text_layer_create(GRect(70, 72, bounds.size.w, 70));
  /* Define some more properties */
  text_layer_set_background_color(s_minute_layer, GColorClear);
  text_layer_set_text_color(s_minute_layer, GColorWhite);
  //text_layer_set_font(s_minute_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_minute_layer, custom_font);
  text_layer_set_text_alignment(s_minute_layer, GTextAlignmentLeft);
  /* Additional TextLayer for minutes to implement a shadow effct */
  s_minute_layer_shadow = text_layer_create(GRect(70, 72, bounds.size.w, 70));
  /* Define some more properties */
  text_layer_set_background_color(s_minute_layer_shadow, GColorClear);
  text_layer_set_text_color(s_minute_layer_shadow, GColorBlack);
  text_layer_set_font(s_minute_layer_shadow, custom_font_shadow);
  text_layer_set_text_alignment(s_minute_layer_shadow, GTextAlignmentLeft);
  
  /* Create a TextLayer to display the day of the week */
  s_day_layer = text_layer_create(GRect(0, 148, bounds.size.w, 50));
  /* Define some more properties */
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorBlack);
  text_layer_set_font(s_day_layer, custom_font_super_small);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentLeft);

  /* Create a TextLayer to display the date of the year */
  s_date_layer = text_layer_create(GRect(0, 148, bounds.size.w - 2, 50));  
  /* Define some more properties */
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, custom_font_super_small);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);

  /* Create 10 bitmaps a bitmap layer and to display battery level */
  s_battery_bitmap_100 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_100);
  s_battery_bitmap_90  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_90);  
  s_battery_bitmap_80  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_80);
  s_battery_bitmap_70  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_70);
  s_battery_bitmap_60  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_60);
  s_battery_bitmap_50  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_50);
  s_battery_bitmap_40  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_40);
  s_battery_bitmap_30  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_30);
  s_battery_bitmap_20  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_20);
  s_battery_bitmap_10  = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_10);
  s_battery_bitmap_layer = bitmap_layer_create(GRect(1,1, 25, 10));
  bitmap_layer_set_compositing_mode(s_battery_bitmap_layer, GCompOpSet);
  
  /* Create 2 bitmaps a bitmap layer and to display BT connection */
  /* Check position for pebble round */
  s_bt_disconnected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_OFF);
  s_bt_connected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_ON);  
  s_bt_bitmap_layer = bitmap_layer_create(GRect(118, 1, 25, 25));
  bitmap_layer_set_compositing_mode(s_bt_bitmap_layer, GCompOpSet);
  
  /* Add the top BitmapLayer (background image) to the root Layer of the main window */
  layer_add_child(window_layer,bitmap_layer_get_layer(s_bitmap_layer));

  /* Add all TextLayers to the root Layer of the main window */
  /* To get a correct shadow effect the order is important */
  layer_add_child(window_layer, text_layer_get_layer(s_hour_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_hour_layer_shadow));  
  layer_add_child(window_layer, text_layer_get_layer(s_minute_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_minute_layer_shadow));
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  /* add the battery and BT bitmap layers */
  layer_add_child(window_layer,bitmap_layer_get_layer(s_battery_bitmap_layer));
  layer_add_child(window_layer,bitmap_layer_get_layer(s_bt_bitmap_layer));
}



/* This static function destroys the main window and releases all resources. 
   It is linked in init() to the event "unload" of the main window.
*/
static void main_window_unload(Window *window) {
  
  /* Free all resources dynamically allocated */
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_hour_layer_shadow);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_minute_layer_shadow);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_date_layer);
  
  bitmap_layer_destroy(s_bitmap_layer);
  gbitmap_destroy(s_bitmap);
  
  bitmap_layer_destroy(s_battery_bitmap_layer);
  gbitmap_destroy(s_battery_bitmap_100);
  gbitmap_destroy(s_battery_bitmap_90);
  gbitmap_destroy(s_battery_bitmap_80);
  gbitmap_destroy(s_battery_bitmap_70);
  gbitmap_destroy(s_battery_bitmap_60);
  gbitmap_destroy(s_battery_bitmap_50);
  gbitmap_destroy(s_battery_bitmap_40);
  gbitmap_destroy(s_battery_bitmap_30);
  gbitmap_destroy(s_battery_bitmap_20);
  gbitmap_destroy(s_battery_bitmap_10);
  
  bitmap_layer_destroy(s_bt_bitmap_layer);
  gbitmap_destroy(s_bt_connected_bitmap);
  gbitmap_destroy(s_bt_disconnected_bitmap);
}



/* 
   This static function is the handler to manage change of time events. 
   It is registered in init().
*/
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  /* call the final business funcion to handle times and dates */
  update_time();
}



/*
   This static function is the handler to manage changes of battery level.
   It is registered and immediately called in init().
*/
static void battery_callback(BatteryChargeState state) {
  /* Get a new battery level */
  s_battery_level = state.charge_percent;
  
  /* Select the right icon according to the battery level */
  switch(s_battery_level) {
  case 100:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_100);
    break;
  case 90:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_90);
    break;    
  case 80:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_80);
    break;  
  case 70:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_70);
    break; 
  case 60:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_60);
    break;
  case 50:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_50);
    break;   
  case 40:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_40);
    break;  
  case 30:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_30);
    break; 
  case 20:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_20);
    break;
  case 10:
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_10);
    break; 
  case 0: 
    bitmap_layer_set_bitmap(s_battery_bitmap_layer, s_battery_bitmap_10);  
  }

}



/*
   This static function is the handler to manage changes of bluetooth status.
   It is registered  and immediately called in init().
*/
static void bluetooth_callback(bool connected) {
  
  if(!connected) {
    /* Not conected here */
    vibes_short_pulse();
    bitmap_layer_set_bitmap(s_bt_bitmap_layer, s_bt_disconnected_bitmap);
  } else {
    /* Conected here */
    bitmap_layer_set_bitmap(s_bt_bitmap_layer, s_bt_connected_bitmap);
  }
}



/* 
   This static function initalizes the watchface.
*/
static void init() {
  /* Create a main window */
  s_main_window = window_create();

  /* Set the main window handlers to manage load and unload */
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  /* Register the service TickTimerService to invoke the tick_handler function every minute */
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  /* Register the service BatteryStateService to invoke the battery_callback function for battery level updates */
  battery_state_service_subscribe(battery_callback);
  
  /* Register the service ConnectionService to invoke the bluetooth_callback function for Bluetooth connection updates */
  connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bluetooth_callback
  });

  /* Add the main window to the stack so that it can be displayed, animated=true */
  window_stack_push(s_main_window, true);
  
  /* Display time, day of the week and current date immediately */
  update_time();
  
  /* Display the battery level immediately */
  battery_callback(battery_state_service_peek());
  
  /* Display the current state of the BT connection immediately */
  bluetooth_callback(connection_service_peek_pebble_app_connection());
}



/*
   This static function destroys the watchface.
*/
static void deinit() {
  window_destroy(s_main_window);
}



/*
   Finally here is the main() function.
*/
int main(void) {
  
  /* initalize the watchface */
  init();
  
  /* wait for inputs and events */
  app_event_loop();
  
  /* close everything and release all resources */
  deinit();
}







