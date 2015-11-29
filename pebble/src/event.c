#include "event.h"

enum {
    CONTENT_KEY,
    TITLE_KEY,
    SUBTITLE_KEY
};

void send_message(char *query) {
    Tuplet queryTuplet = TupletCString(CONTENT_KEY, query);
    DictionaryIterator *iter;

    app_message_outbox_begin(&iter);
    dict_write_tuplet(iter, &queryTuplet);

    app_message_outbox_send();
}

void inbox_received_callback(DictionaryIterator *received, void *context) {
    ReleaseMenu *menu = context;
    Tuple *title_tuplet = dict_find(received, TITLE_KEY);
    if (title_tuplet) {
        menu->titles = realloc(menu->titles, sizeof(char *) * (menu->count + 1));
        menu->titles[menu->count] = malloc(strlen(title_tuplet->value->cstring) + 1);
        strcpy(menu->titles[menu->count], title_tuplet->value->cstring);
    }

    Tuple *subtitle_tuplet = dict_find(received, SUBTITLE_KEY);
    if (subtitle_tuplet) {
       menu->subtitles = realloc(menu->subtitles, sizeof(char *) * (menu->count + 1));
       menu->subtitles[menu->count] = malloc(strlen(subtitle_tuplet->value->cstring) + 1);
       strcpy(menu->subtitles[menu->count], subtitle_tuplet->value->cstring);
   }
   menu->count++;
   menu_layer_reload_data(menu->layer);
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
