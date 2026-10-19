#include "xcui.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const char valid_program[] =
"app \"Test OS\" {\n"
" window \"Main\" {\n"
"  column gap 8 {\n"
"   text \"Hello\" size 22\n"
"   button \"Terminal\" action \"open_terminal\" height 40\n"
"  }\n"
" }\n"
"}\n";
static int dispatch_action(void *context, const char *action) {
    return strcmp(action, (const char *)context) == 0;
}
int main(void) {
    struct xcui_tree *tree=(struct xcui_tree*)calloc(1,sizeof(*tree));
    struct xcui_error error; uint16_t button=XCUI_NO_NODE; unsigned i;
    assert(tree != NULL);
    assert(xcui_parse(valid_program,tree,&error));
    assert(tree->abi_version==XCUI_ABI_VERSION);
    assert(strcmp(tree->app,"Test OS")==0 && tree->node_count==4);
    assert(xcui_layout(tree,800,600));
    for(i=0;i<tree->node_count;++i) if(tree->nodes[i].kind==XCUI_BUTTON) button=(uint16_t)i;
    assert(button!=XCUI_NO_NODE);
    assert(strcmp(xcui_node_action(tree,button),"open_terminal")==0);
    assert(tree->nodes[button].width>0 && tree->nodes[button].height==40);
    assert(xcui_hit_test(tree,tree->nodes[button].x+1,tree->nodes[button].y+1)==button);
    assert(xcui_dispatch_pointer(tree,tree->nodes[button].x+1,tree->nodes[button].y+1,dispatch_action,(void*)"open_terminal")==1);
    assert(xcui_dispatch_pointer(tree,tree->nodes[button].x+1,tree->nodes[button].y+1,dispatch_action,(void*)"wrong_action")==-1);
    assert(!xcui_parse("app \"Broken\" { unknown \"x\" }",tree,&error));
    assert(error.code==XCUI_ERR_UNKNOWN_COMPONENT);
    assert(!xcui_parse("app \"Broken\" { window \"x\" {",tree,&error));
    assert(error.code==XCUI_ERR_UNCLOSED_BLOCK);
    free(tree); puts("XCUI tests passed"); return 0;
}
