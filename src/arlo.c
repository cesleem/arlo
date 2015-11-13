#include <pebble.h>

static Window *s_main_window;
static TextLayer *welcome_msg_layer;
static char welcome_msg_buffer[48];
static char time_buffer[8];


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer

  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

}

static void welcome_message() {
  update_time();
  // function that sets load message...
  snprintf(welcome_msg_buffer, sizeof(welcome_msg_buffer), "Hey it's %s, how's it going so far \?", time_buffer);

  // Display this time on the TextLayer
  text_layer_set_text(welcome_msg_layer, welcome_msg_buffer);
}




// static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(welcome_msg_layer, "Select");
// }

// static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(welcome_msg_layer, "Up");
// }

// static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(welcome_msg_layer, "Down");
// }

// static void click_config_provider(void *context) {
//   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
//   window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
//   window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
// }

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  welcome_msg_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(welcome_msg_layer, GColorClear);
  text_layer_set_text_color(welcome_msg_layer, GColorBlack);
  text_layer_set_text(welcome_msg_layer, " ");
  text_layer_set_font(welcome_msg_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(welcome_msg_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(welcome_msg_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(welcome_msg_layer);
}

static void init(void) {
  s_main_window = window_create();
  // window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_main_window, animated);
  welcome_message(); 
}

static void deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_main_window);

  app_event_loop();
  deinit();
}
