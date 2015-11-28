#include <pebble.h>

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 2

typedef struct{
  MenuLayer *layer;
  char *categories[NUM_MENU_ITEMS];
  GBitmap *icons[NUM_MENU_ITEMS];
} MainMenuData;

static Window *main_window;

static void main_window_load(Window *window) {
  //Grab user data 
  MainMenuData *data = window_get_user_data(window);
  
  //Icons for main menu
  data->icons[0] = gbitmap_create_with_resource(RESOURCE_ID_anime_pic);
  data->icons[1] = gbitmap_create_with_resource (RESOURCE_ID_manga_pic);
  
  //Categories for main menu
  data->categories[0] = "Anime";
  data->categories[1] = "Manga";
  
  //Setup main menu
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  data->layer = menu_layer_create(bounds);
}

static void main_window_unload(Window *window) {
  //Grab user data
  MainMenuData *data = window_get_user_data(window);
  
  for (uint8_t i = 0; i < 2; i++){
    gbitmap_destroy(data->icons[i]);
  }
  menu_layer_destroy(data->layer);
  free(data);
  window_destroy(window);
}

static void init() {
  // Create main Window
  main_window = window_create();
  
  menu_layer_set_click_config_onto_window(data->layer, window);
  
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  //Push window to display
  window_stack_push(main_window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(main_window);
}

//Main function
int main(void) {
  init();
  app_event_loop();
  deinit();
}
