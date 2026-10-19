#ifndef XCUI_H
#define XCUI_H
#include <stddef.h>
#include <stdint.h>
#define XCUI_ABI_VERSION 1u
#define XCUI_MAX_NODES 128u
#define XCUI_TEXT_CAP 96u
#define XCUI_ACTION_CAP 48u
#define XCUI_APP_CAP 64u
#define XCUI_NO_NODE 0xffffu
enum xcui_kind { XCUI_WINDOW=1, XCUI_SCREEN, XCUI_ROW, XCUI_COLUMN, XCUI_PANEL, XCUI_TEXT, XCUI_BUTTON, XCUI_LABEL, XCUI_SPACER, XCUI_DIVIDER };
enum xcui_error_code { XCUI_OK=0, XCUI_ERR_ARGUMENT, XCUI_ERR_SYNTAX, XCUI_ERR_UNKNOWN_COMPONENT, XCUI_ERR_UNKNOWN_PROPERTY, XCUI_ERR_BAD_VALUE, XCUI_ERR_TOO_MANY_NODES, XCUI_ERR_TOO_DEEP, XCUI_ERR_UNCLOSED_BLOCK };
struct xcui_error { enum xcui_error_code code; uint32_t line; };
struct xcui_node {
    uint16_t kind, parent, first_child, next_sibling;
    int16_t x, y, width, height, declared_width, declared_height;
    uint16_t padding, gap, font_size;
    uint32_t foreground, background;
    uint8_t has_foreground, has_background;
    char text[XCUI_TEXT_CAP];
    char action[XCUI_ACTION_CAP];
};
struct xcui_tree { uint32_t abi_version; uint16_t node_count; char app[XCUI_APP_CAP]; struct xcui_node nodes[XCUI_MAX_NODES]; };
int xcui_parse(const char *source, struct xcui_tree *out, struct xcui_error *error);
int xcui_layout(struct xcui_tree *tree, int16_t width, int16_t height);
struct xcui_backend {
    void (*fill_rect)(void *context, int16_t x, int16_t y, int16_t width, int16_t height, uint32_t color);
    void (*draw_text)(void *context, int16_t x, int16_t y, const char *text, uint16_t size, uint32_t color);
};
typedef int (*xcui_action_dispatcher)(void *context, const char *action);
int xcui_render(const struct xcui_tree *tree, const struct xcui_backend *backend, void *context);
uint16_t xcui_hit_test(const struct xcui_tree *tree, int16_t x, int16_t y);
const char *xcui_node_action(const struct xcui_tree *tree, uint16_t node_index);
int xcui_dispatch_pointer(const struct xcui_tree *tree, int16_t x, int16_t y, xcui_action_dispatcher dispatch, void *context);
const char *xcui_error_name(enum xcui_error_code code);
#endif
