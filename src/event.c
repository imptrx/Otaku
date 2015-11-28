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

static void in_received_handler(DictionaryIterator *received, void *context) {
    Tuple *data = dict_read_first(received);
    while (data) {
        switch (data->key) {
            case TITLE_KEY:
            break;

            case SUBTITLE_KEY:
            break;
        }
    }
}

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
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
