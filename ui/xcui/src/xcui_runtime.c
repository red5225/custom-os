#include "xcui.h"

static int is_container(uint16_t k){return k==XCUI_WINDOW||k==XCUI_SCREEN||k==XCUI_ROW||k==XCUI_COLUMN||k==XCUI_PANEL;}
static void layout_node(struct xcui_tree *tree, uint16_t index) {
    struct xcui_node *node = &tree->nodes[index];
    uint16_t child, count = 0;
    int available, cursor, gap_total, base, extra, horizontal;
    if (!is_container(node->kind) || node->first_child == XCUI_NO_NODE) return;
    horizontal = node->kind == XCUI_ROW;
    available = (horizontal ? node->width : node->height) - (int)node->padding * 2;
    if (available < 0) available = 0;
    for (child = node->first_child; child != XCUI_NO_NODE; child = tree->nodes[child].next_sibling) ++count;
    gap_total = count > 1 ? (int)(count - 1) * node->gap : 0;
    available = available > gap_total ? available - gap_total : 0;
    base = count ? available / count : 0;
    extra = count ? available % count : 0;
    cursor = horizontal ? node->x + node->padding : node->y + node->padding;
    for (child = node->first_child; child != XCUI_NO_NODE; child = tree->nodes[child].next_sibling) {
        struct xcui_node *item = &tree->nodes[child];
        int primary = base + (extra > 0 ? 1 : 0);
        if (extra > 0) --extra;
        if (horizontal) {
            if (item->declared_width > 0) primary = item->declared_width;
            item->x = (int16_t)cursor;
            item->y = (int16_t)(node->y + node->padding);
            item->width = (int16_t)primary;
            item->height = (int16_t)(node->height - (int)node->padding * 2);
            cursor += primary + node->gap;
        } else {
            if (item->declared_height > 0) primary = item->declared_height;
            item->x = (int16_t)(node->x + node->padding);
            item->y = (int16_t)cursor;
            item->width = (int16_t)(node->width - (int)node->padding * 2);
            item->height = (int16_t)primary;
            cursor += primary + node->gap;
        }
        if (item->declared_width > 0) item->width = item->declared_width;
        if (item->declared_height > 0) item->height = item->declared_height;
        layout_node(tree, child);
    }
}
int xcui_layout(struct xcui_tree *t,int16_t w,int16_t h){uint16_t i;if(!t||t->abi_version!=XCUI_ABI_VERSION||w<=0||h<=0)return 0;for(i=0;i<t->node_count;++i){struct xcui_node *n=&t->nodes[i];if(n->parent==XCUI_NO_NODE){n->x=0;n->y=0;if(n->width<=0)n->width=w;if(n->height<=0)n->height=h;layout_node(t,i);}}return 1;}
int xcui_render(const struct xcui_tree *t,const struct xcui_backend *b,void *ctx){uint16_t i;if(!t||!b||t->abi_version!=XCUI_ABI_VERSION)return 0;for(i=0;i<t->node_count;++i){const struct xcui_node *n=&t->nodes[i];if(n->width<=0||n->height<=0)continue;
    if((n->kind==XCUI_WINDOW||n->kind==XCUI_SCREEN||n->kind==XCUI_PANEL||n->kind==XCUI_BUTTON)&&b->fill_rect)b->fill_rect(ctx,n->x,n->y,n->width,n->height,n->has_background?n->background:(n->kind==XCUI_BUTTON?0x285bb4u:0xf6f7fbu));
    if((n->kind==XCUI_TEXT||n->kind==XCUI_LABEL||n->kind==XCUI_BUTTON)&&n->text[0]&&b->draw_text)b->draw_text(ctx,(int16_t)(n->x+(n->kind==XCUI_BUTTON?8:0)),(int16_t)(n->y+(n->height>(int16_t)n->font_size?(n->height-n->font_size)/2:0)),n->text,n->font_size,n->has_foreground?n->foreground:0x202938u);
    }return 1;}
uint16_t xcui_hit_test(const struct xcui_tree *t,int16_t x,int16_t y){uint16_t i,found=XCUI_NO_NODE;if(!t)return found;for(i=0;i<t->node_count;++i){const struct xcui_node *n=&t->nodes[i];if(n->kind==XCUI_BUTTON&&x>=n->x&&y>=n->y&&x<n->x+n->width&&y<n->y+n->height)found=i;}return found;}
const char *xcui_node_action(const struct xcui_tree *t,uint16_t i){if(!t||i>=t->node_count)return "";return t->nodes[i].action;}
const char *xcui_error_name(enum xcui_error_code c){switch(c){case XCUI_OK:return "ok";case XCUI_ERR_ARGUMENT:return "invalid argument";case XCUI_ERR_SYNTAX:return "syntax error";case XCUI_ERR_UNKNOWN_COMPONENT:return "unknown component";case XCUI_ERR_UNKNOWN_PROPERTY:return "unknown property";case XCUI_ERR_BAD_VALUE:return "invalid property value";case XCUI_ERR_TOO_MANY_NODES:return "too many UI nodes";case XCUI_ERR_TOO_DEEP:return "nesting limit exceeded";case XCUI_ERR_UNCLOSED_BLOCK:return "unclosed block";default:return "unknown XCUI error";}}

int xcui_dispatch_pointer(const struct xcui_tree *tree, int16_t x, int16_t y,
                          xcui_action_dispatcher dispatch, void *context) {
    uint16_t node;
    const char *action;
    if (!tree || !dispatch) return -1;
    node = xcui_hit_test(tree, x, y);
    if (node == XCUI_NO_NODE) return 0;
    action = xcui_node_action(tree, node);
    if (!action[0]) return 0;
    return dispatch(context, action) ? 1 : -1;
}
