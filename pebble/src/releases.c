#include "releases.h"

static uint16_t menu_layer_get_num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
    return 1;
}

static uint16_t menu_layer_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
    ReleaseMenu *data = callback_context;
    return data->count;
}


static void menu_layer_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    ReleaseMenu *data = callback_context;

    menu_cell_basic_draw(ctx, cell_layer, data->titles[cell_index->row], data->subtitles[cell_index->row], NULL);
}

static void window_load(Window *window) {
    char *query = window_get_user_data(window);

    ReleaseMenu *data = malloc(sizeof(ReleaseMenu));
    memset(data, 0, sizeof(ReleaseMenu));

    // Setup essential parts of ReleaseMenu
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    data->layer = menu_layer_create(bounds);

    data->titles = malloc(sizeof(char *));
    data->subtitles = malloc(sizeof(char *));

    data->count = 0;

    menu_layer_set_callbacks(data->layer, data, (MenuLayerCallbacks) {
        .get_num_rows = menu_layer_get_num_rows_callback,
        .get_num_sections = menu_layer_get_num_sections_callback,
        .draw_row = menu_layer_draw_row_callback
    });

    layer_add_child(window_layer, menu_layer_get_layer(data->layer));
    menu_layer_set_click_config_onto_window(data->layer, window);

    // Send request for anime/manga data
    app_message_set_context(data);
    send_message(query);
}

static void window_unload(Window *window) {
    ReleaseMenu *data = window_get_user_data(window);

    menu_layer_destroy(data->layer);
    free(data);
    window_destroy(window);
}

void releases_init(char *query) {
    Window *window = window_create();

    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
    });

    window_set_user_data(window, query);
    window_stack_push(window, true);
}

