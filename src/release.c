#include <pebble.h>
#include "release.h"

static Window *releases_window;

static void menu_layer_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
	//   MainMenuData *data = callback_context;
	uint16_t current_row = cell_index->row;
	// menu_cell_basic_draw(ctx, cell_layer, data->categories[current_row], NULL, data->icons[current_row]);
}

static uint16_t menu_layer_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
	//   return NUM_MENU_ITEMS;
	return 0;
}

static int16_t menu_layer_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
	GRect bounds = layer_get_bounds(window_get_root_layer(releases_window));
	// return bounds.size.h / NUM_MENU_ITEMS
	return 0;
}

static uint16_t menu_layer_get_num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
	//   return NUM_MENU_SECTIONS;
	return 0;
}

static void releases_window_load(Window *window) {
	//temporary function to store menu name
	char *user_data = window_get_user_data(window);

	//Allocates memory for struct 
	ReleasesMenu *currentMenu = malloc(sizeof(ReleasesMenu));
}

static void releases_window_unload(Window *window) {

}

static void releases_init(char *menu_item) {
	// Create main Window
	releases_window = window_create();

	//Associates with window type(anime/manga)
	window_set_user_data(releases_window, menu_item);
	window_set_window_handlers(releases_window, (WindowHandlers) {
		.load = releases_window_load,
			.unload = releases_window_unload,
	});
	window_stack_push(releases_window, true);
}

static void releases_deinit() {
	// Destroy main Window
	window_destroy(releases_window);
}

