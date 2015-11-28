#include "event.h"

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 2

typedef struct {
  MenuLayer *layer;
  char *categories[NUM_MENU_ITEMS];
  GBitmap *icons[NUM_MENU_ITEMS];
} MainMenuData;

static Window *main_window;

static uint16_t menu_layer_get_num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_layer_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return NUM_MENU_ITEMS;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static int16_t menu_layer_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  return bounds.size.h / NUM_MENU_ITEMS;
}

static void menu_layer_select_click_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  MainMenuData *data = callback_context;
  send_message(data->categories[cell_index->row]);
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Draw title text in the section header
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, layer_get_frame(cell_layer), 1, GCornersAll);
	graphics_context_set_text_color(ctx, GColorBlack);
	graphics_draw_text(ctx, "OTAKU", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), layer_get_frame(cell_layer), GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

static void menu_layer_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
  MainMenuData *data = callback_context;
  uint16_t current_row = cell_index->row;
  menu_cell_basic_draw(ctx, cell_layer, data->categories[current_row], NULL, data->icons[current_row]);
}

static void main_window_load(Window *window) {
  // Grab user data 
  MainMenuData *data = window_get_user_data(window);
  
  // Icons for main menu
  data->icons[0] = gbitmap_create_with_resource(RESOURCE_ID_TELEVISION);
  data->icons[1] = gbitmap_create_with_resource (RESOURCE_ID_BOOK);
  
  // Categories for main menu
  data->categories[0] = "Anime";
  data->categories[1] = "Manga";
  
  // Setup main menu
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  data->layer = menu_layer_create(bounds);

  menu_layer_set_callbacks(data->layer, data, (MenuLayerCallbacks) {
    .get_num_rows = menu_layer_get_num_rows_callback,
    .get_num_sections = menu_layer_get_num_sections_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_layer_draw_row_callback,
    .get_cell_height = menu_layer_get_cell_height_callback,
    .select_click = menu_layer_select_click_callback
  });

  layer_add_child(window_layer, menu_layer_get_layer(data->layer));

  menu_layer_set_click_config_onto_window(data->layer, window);

  // Communication between phone and pebble
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void main_window_unload(Window *window) {
  // Grab user data
  MainMenuData *data = window_get_user_data(window);
  
  for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++){
    gbitmap_destroy(data->icons[i]);
  }
  menu_layer_destroy(data->layer);
  free(data);
  window_destroy(window);
}

static void init() {
  // Create main Window
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  MainMenuData *data = malloc(sizeof(MainMenuData));
  memset(data, 0, sizeof(MainMenuData));

  window_set_user_data(main_window, data);

  // Push window to display
  window_stack_push(main_window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(main_window);
}

// Main function
int main(void) {
  init();
  app_event_loop();
  deinit();
}
