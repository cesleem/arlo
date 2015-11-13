#include <pebble.h>

#define NUM_WINDOWS 5
#define CHECKBOX_WINDOW_CELL_HEIGHT 30
#define COLOUR_MENU_HIGHLIGHT_BACKGROUND GColorFromHEX(0xE31837)
#define COLOUR_MENU_HIGHLIGHT_FOREGROUND GColorWhite


static Window *s_main_window;
static Window *s_rate_window;

static TextLayer *welcome_msg_layer;
static char welcome_msg_buffer[48];
static char time_buffer[8];

static MenuLayer *s_menu_layer;

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

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_WINDOWS;
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "1: Awesome!", NULL, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "2: Alright", NULL, NULL);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "3: Somewhere in Between", NULL, NULL);
      break;
    case 3:
      menu_cell_basic_draw(ctx, cell_layer, "4: A bit down", NULL, NULL);
      break;
    case 4:
      menu_cell_basic_draw(ctx, cell_layer, "5: Really bad!", NULL, NULL);
      break;

    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return CHECKBOX_WINDOW_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      break;
    case 1:
      break;
    case 2:
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    default:
      break;
  }
}

static void draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *context) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Rate 1-5.");
}

static int16_t get_header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static uint16_t get_num_sections_callback(struct MenuLayer *menu_layer, void *context) {
  return 1;
}

static void s_rate_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  // menu_layer_set_highlight_colors(s_menu_layer, COLOUR_MENU_HIGHLIGHT_BACKGROUND, COLOUR_MENU_HIGHLIGHT_FOREGROUND);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)get_num_rows_callback,
      .draw_row = (MenuLayerDrawRowCallback)draw_row_callback,
      .get_cell_height = (MenuLayerGetCellHeightCallback)get_cell_height_callback,
      .select_click = (MenuLayerSelectCallback)select_callback,
      .draw_header = (MenuLayerDrawHeaderCallback)draw_header_callback,
      .get_header_height = (MenuLayerGetHeaderHeightCallback)get_header_height_callback,
      .get_num_sections = (MenuLayerGetNumberOfSectionsCallback)get_num_sections_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

// static void s_rate_window_unload(Window *window) {
//   scroll_layer_destroy(s_scroll_layer);
//   text_layer_destroy(s_content_layer);
//   layer_destroy(s_indicator_up_layer);
//   layer_destroy(s_indicator_down_layer);
// }

static void s_rate_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
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

  s_rate_window = window_create();
  window_set_window_handlers(s_rate_window, (WindowHandlers) {
    .load = s_rate_window_load,
    .unload = s_rate_window_unload,
  });
  window_stack_push(s_rate_window, animated);
}

static void deinit(void) {
  window_destroy(s_main_window);
  window_destroy(s_rate_window);
}

int main(void) {
  init();
  // sleep(5);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_main_window);
  app_event_loop();
  deinit();
}
