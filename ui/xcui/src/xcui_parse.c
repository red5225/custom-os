#include "xcui.h"

#define XCUI_TOKEN_CAP 4096u
#define XCUI_DEPTH_CAP 32u
enum token_kind { TOK_EOF, TOK_IDENT, TOK_STRING, TOK_NUMBER, TOK_LBRACE, TOK_RBRACE, TOK_BAD };
struct token { enum token_kind kind; char text[XCUI_TEXT_CAP]; uint32_t line; };
struct parser { const char *cursor; uint32_t line; struct token current; struct xcui_tree *tree; struct xcui_error *error; unsigned depth; };
static int ch_space(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
static int ch_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
static int ch_digit(char c) { return c >= '0' && c <= '9'; }
static int str_equal(const char *a, const char *b) { size_t i=0; while(a[i]&&b[i]&&a[i]==b[i])++i; return a[i]==b[i]; }
static size_t str_len(const char *s) { size_t n=0; while(s[n])++n; return n; }
static void str_copy(char *d,size_t cap,const char *s) { size_t i=0; if(!cap)return; while(i+1<cap&&s[i]){d[i]=s[i];++i;} d[i]='\0'; }
static void set_error(struct parser *p,enum xcui_error_code code,uint32_t line) { if(p->error&&p->error->code==XCUI_OK){p->error->code=code;p->error->line=line;} }
static void next_token(struct parser *p) {
    struct token t; size_t n=0; const char *s=p->cursor;
    t.kind=TOK_EOF;t.text[0]='\0';t.line=p->line;
    for(;;){while(*s&&ch_space(*s)){if(*s=='\n')++p->line;++s;}if(*s=='#'){while(*s&&*s!='\n')++s;continue;}if(s[0]=='/'&&s[1]=='/'){while(*s&&*s!='\n')++s;continue;}break;}
    t.line=p->line;
    if(!*s){p->cursor=s;p->current=t;return;}
    if(*s=='{'){t.kind=TOK_LBRACE;t.text[0]='{';t.text[1]='\0';p->cursor=s+1;p->current=t;return;}
    if(*s=='}'){t.kind=TOK_RBRACE;t.text[0]='}';t.text[1]='\0';p->cursor=s+1;p->current=t;return;}
    if(*s=='"'){
        ++s;t.kind=TOK_STRING;
        while(*s&&*s!='"'&&*s!='\n'){char c=*s++;if(c=='\\'){if(!*s)break;c=*s++;if(c=='n')c='\n';else if(c=='t')c='\t';else if(c=='r')c='\r';}
            if(n+1>=sizeof(t.text)){t.kind=TOK_BAD;break;}t.text[n++]=c;}
        if(*s!='"')t.kind=TOK_BAD;else ++s;t.text[n]='\0';p->cursor=s;p->current=t;if(t.kind==TOK_BAD)set_error(p,XCUI_ERR_BAD_VALUE,t.line);return;
    }
    if(ch_alpha(*s)){t.kind=TOK_IDENT;while(ch_alpha(*s)||ch_digit(*s)){if(n+1>=sizeof(t.text)){t.kind=TOK_BAD;break;}t.text[n++]=*s++;}}
    else if(ch_digit(*s)||(*s=='-'&&ch_digit(s[1]))){t.kind=TOK_NUMBER;if(*s=='-')t.text[n++]=*s++;while(ch_digit(*s)){if(n+1>=sizeof(t.text)){t.kind=TOK_BAD;break;}t.text[n++]=*s++;}}
    else{t.kind=TOK_BAD;t.text[0]=*s++;n=1;}
    t.text[n]='\0';p->cursor=s;p->current=t;if(t.kind==TOK_BAD)set_error(p,XCUI_ERR_SYNTAX,t.line);
}
static int expect(struct parser *p,enum token_kind kind){if(p->current.kind!=kind){set_error(p,XCUI_ERR_SYNTAX,p->current.line);return 0;}next_token(p);return 1;}
static int component_kind(const char *s){
    if(str_equal(s,"window"))return XCUI_WINDOW;if(str_equal(s,"screen"))return XCUI_SCREEN;
    if(str_equal(s,"row")||str_equal(s,"hstack"))return XCUI_ROW;
    if(str_equal(s,"column")||str_equal(s,"vstack"))return XCUI_COLUMN;
    if(str_equal(s,"panel"))return XCUI_PANEL;if(str_equal(s,"text"))return XCUI_TEXT;
    if(str_equal(s,"label"))return XCUI_LABEL;if(str_equal(s,"button"))return XCUI_BUTTON;
    if(str_equal(s,"spacer"))return XCUI_SPACER;if(str_equal(s,"divider"))return XCUI_DIVIDER;return 0;
}
static int property_known(const char *s){return str_equal(s,"width")||str_equal(s,"height")||str_equal(s,"padding")||str_equal(s,"gap")||str_equal(s,"size")||str_equal(s,"color")||str_equal(s,"background")||str_equal(s,"action");}
static int parse_int(const char *s,int *out){int sign=1,value=0;size_t i=0;if(s[0]=='-'){sign=-1;i=1;}if(!s[i])return 0;for(;s[i];++i){if(!ch_digit(s[i]))return 0;value=value*10+(s[i]-'0');if(value>32767)return 0;}*out=value*sign;return 1;}
static int parse_color(const char *s,uint32_t *out){unsigned value=0;size_t i;if(s[0]!='#'||str_len(s)!=7)return 0;for(i=1;i<7;++i){char c=s[i];unsigned d;if(c>='0'&&c<='9')d=(unsigned)(c-'0');else if(c>='a'&&c<='f')d=(unsigned)(c-'a'+10);else if(c>='A'&&c<='F')d=(unsigned)(c-'A'+10);else return 0;value=(value<<4)|d;}*out=value;return 1;}
static uint16_t add_node(struct parser *p,uint16_t parent,int kind,uint32_t line){
    uint16_t i;struct xcui_node *n;if(p->tree->node_count>=XCUI_MAX_NODES){set_error(p,XCUI_ERR_TOO_MANY_NODES,line);return XCUI_NO_NODE;}
    i=p->tree->node_count++;n=&p->tree->nodes[i];n->kind=(uint16_t)kind;n->parent=parent;n->first_child=XCUI_NO_NODE;n->next_sibling=XCUI_NO_NODE;
    n->x=n->y=n->width=n->height=0;n->declared_width=n->declared_height=0;n->padding=4;n->gap=4;n->font_size=16;n->foreground=0xffffffu;n->background=0x202735u;n->has_foreground=n->has_background=0;n->text[0]=n->action[0]='\0';
    if(parent!=XCUI_NO_NODE){uint16_t *link=&p->tree->nodes[parent].first_child;while(*link!=XCUI_NO_NODE)link=&p->tree->nodes[*link].next_sibling;*link=i;}return i;
}
static int parse_children(struct parser *p,uint16_t parent);
static int parse_node(struct parser *p,uint16_t parent){
    struct token start=p->current;int kind=component_kind(start.text);uint16_t i;struct xcui_node *n;
    if(start.kind!=TOK_IDENT||!kind){set_error(p,XCUI_ERR_UNKNOWN_COMPONENT,start.line);return 0;}next_token(p);i=add_node(p,parent,kind,start.line);if(i==XCUI_NO_NODE)return 0;n=&p->tree->nodes[i];
    if(p->current.kind==TOK_STRING){str_copy(n->text,sizeof(n->text),p->current.text);next_token(p);}
    while(p->current.kind==TOK_IDENT&&property_known(p->current.text)){
        char key[XCUI_TEXT_CAP],v[XCUI_TEXT_CAP];int number;str_copy(key,sizeof(key),p->current.text);next_token(p);
        if(p->current.kind!=TOK_STRING&&p->current.kind!=TOK_NUMBER&&p->current.kind!=TOK_IDENT){set_error(p,XCUI_ERR_BAD_VALUE,p->current.line);return 0;}
        str_copy(v,sizeof(v),p->current.text);next_token(p);
        if(str_equal(key,"action"))str_copy(n->action,sizeof(n->action),v);
        else if(str_equal(key,"color")||str_equal(key,"background")){uint32_t c;if(!parse_color(v,&c)){set_error(p,XCUI_ERR_BAD_VALUE,start.line);return 0;}if(str_equal(key,"color")){n->foreground=c;n->has_foreground=1;}else{n->background=c;n->has_background=1;}}
        else{if(!parse_int(v,&number)||number<0){set_error(p,XCUI_ERR_BAD_VALUE,start.line);return 0;}if(str_equal(key,"width"))n->declared_width=(int16_t)number;else if(str_equal(key,"height"))n->declared_height=(int16_t)number;else if(str_equal(key,"padding"))n->padding=(uint16_t)number;else if(str_equal(key,"gap"))n->gap=(uint16_t)number;else if(str_equal(key,"size"))n->font_size=(uint16_t)number;}
    }
    if(p->current.kind==TOK_LBRACE){next_token(p);if(!parse_children(p,i))return 0;}return 1;
}
static int parse_children(struct parser *p,uint16_t parent){
    if(++p->depth>XCUI_DEPTH_CAP){set_error(p,XCUI_ERR_TOO_DEEP,p->current.line);return 0;}
    while(p->current.kind!=TOK_RBRACE&&p->current.kind!=TOK_EOF){if(p->error&&p->error->code!=XCUI_OK){--p->depth;return 0;}if(!parse_node(p,parent)){--p->depth;return 0;}}
    if(p->current.kind!=TOK_RBRACE){set_error(p,XCUI_ERR_UNCLOSED_BLOCK,p->current.line);--p->depth;return 0;}next_token(p);--p->depth;return 1;
}
int xcui_parse(const char *source,struct xcui_tree *out,struct xcui_error *error){
    struct parser p;struct xcui_error local_error;if(!error)error=&local_error;error->code=XCUI_OK;error->line=1;
    if(!source||!out){error->code=XCUI_ERR_ARGUMENT;return 0;}out->abi_version=XCUI_ABI_VERSION;out->node_count=0;out->app[0]='\0';
    p.cursor=source;p.line=1;p.tree=out;p.error=error;p.depth=0;p.current.kind=TOK_EOF;next_token(&p);
    if(p.current.kind!=TOK_IDENT||!str_equal(p.current.text,"app")){set_error(&p,XCUI_ERR_SYNTAX,p.current.line);return 0;}next_token(&p);
    if(p.current.kind!=TOK_STRING){set_error(&p,XCUI_ERR_BAD_VALUE,p.current.line);return 0;}str_copy(out->app,sizeof(out->app),p.current.text);next_token(&p);
    if(!expect(&p,TOK_LBRACE)||!parse_children(&p,XCUI_NO_NODE))return 0;
    if(p.current.kind!=TOK_EOF){set_error(&p,XCUI_ERR_SYNTAX,p.current.line);return 0;}return error->code==XCUI_OK;
}
