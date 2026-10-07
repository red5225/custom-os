#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct { char name[64]; int argc; } Fn;
static Fn fns[256];
static int fn_count;

static char *trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) --e;
    *e = 0;
    return s;
}
static int is_fn(const char *s) { return strncmp(s, "fn ", 3) == 0; }

static void collect_fn(const char *line) {
    char n[64], a[128];
    int ok = sscanf(line, "fn %63[^ (] (%127[^)])", n, a) == 2 ||
             sscanf(line, "fn %63[^ (](%127[^)])", n, a) == 2;
    if (!ok || fn_count >= 256) return;
    strncpy(fns[fn_count].name, n, sizeof(fns[fn_count].name) - 1);
    fns[fn_count].name[sizeof(fns[fn_count].name) - 1] = 0;
    char *x = trim(a);
    fns[fn_count].argc = (*x && strcmp(x, "void") != 0) ? 1 : 0;
    fn_count++;
}

static void emit_string(FILE *out, const char *p, const char *end) {
    fputc('"', out);
    for (; p < end; p++) {
        if (*p == '"') fputs("\"", out);
        else if (*p == '\\' && p + 1 < end) {
            fputc('\\', out);
            fputc(p[1], out);
            p++;
        } else if (*p == '\n') fputs("\n", out);
        else fputc(*p, out);
    }
    fputc('"', out);
}

static int parse_print(FILE *out, char *s) {
    if (strncmp(s, "print ", 6) != 0) return 0;
    char *q = strchr(s + 6, '"');
    char *e = q ? strrchr(q + 1, '"') : NULL;
    if (!q || !e) return -1;
    fputs("terminal_write(", out);
    emit_string(out, q + 1, e);
    fputs(");\n", out);
    return 1;
}

static int parse_if(FILE *out, char *s) {
    const char *prefix = NULL;
    if (strncmp(s, "if cmd == ", 10) == 0) prefix = "if";
    else if (strncmp(s, "else if cmd == ", 15) == 0) prefix = "else if";
    if (!prefix) return 0;
    char *q = strchr(s, '"');
    char *e = q ? strrchr(q + 1, '"') : NULL;
    if (!q || !e) return -1;
    fprintf(out, "%s (nol_streq(cmd, ", prefix);
    emit_string(out, q + 1, e);
    fputs(")) {\n", out);
    return 1;
}

static int compile_file(FILE *out, const char *path) {
    FILE *in = fopen(path, "r");
    if (!in) { fprintf(stderr, "nolc: cannot open %s\n", path); return 0; }
    char buf[4096];
    int in_fn = 0;

    while (fgets(buf, sizeof(buf), in)) {
        char *s = trim(buf);
        if (!*s || *s == '#') continue;

        if (is_fn(s)) {
            char n[64], a[128];
            int ok = sscanf(s, "fn %63[^ (] (%127[^)])", n, a) == 2 ||
                     sscanf(s, "fn %63[^ (](%127[^)])", n, a) == 2;
            if (!ok) { fprintf(stderr, "nolc: bad function header in %s\n", path); fclose(in); return 0; }
            char *x = trim(a);
            fprintf(out, "void %s(%s) {\n", n, *x ? "const char *cmd" : "void");
            in_fn = 1;
            continue;
        }

        if (!in_fn) {
            fprintf(stderr, "nolc: statement outside function in %s: %s\n", path, s);
            fclose(in); return 0;
        }

        int r = parse_if(out, s);
        if (r < 0) { fprintf(stderr, "nolc: bad condition in %s\n", path); fclose(in); return 0; }
        if (r > 0) continue;

        r = parse_print(out, s);
        if (r < 0) { fprintf(stderr, "nolc: bad print in %s\n", path); fclose(in); return 0; }
        if (r > 0) continue;

        if (strcmp(s, "clear;") == 0) { fputs("terminal_clear();\n", out); continue; }
        if (strcmp(s, "backspace;") == 0) { fputs("terminal_backspace();\n", out); continue; }
        if (strcmp(s, "return;") == 0) { fputs("return;\n", out); continue; }

        if (strncmp(s, "call ", 5) == 0) {
            char n[64];
            if (sscanf(s, "call %63[^;];", n) == 1) {
                fprintf(out, "%s();\n", trim(n));
                continue;
            }
        }

        if (strcmp(s, "else {") == 0) { fputs("else {\n", out); continue; }
        if (strcmp(s, "}") == 0) { fputs("}\n", out); in_fn = 0; continue; }

        if (s[0] == '}' && strstr(s, "else")) {
            char *p = strstr(s, "else");
            fputs("}\n", out);
            if (strncmp(p, "else if cmd == ", 15) == 0) {
                char *q = strchr(p, '"');
                char *e = q ? strrchr(q + 1, '"') : NULL;
                if (!q || !e) { fclose(in); return 0; }
                fputs("else if (nol_streq(cmd, ", out);
                emit_string(out, q + 1, e);
                fputs(")) {\n", out);
            } else fputs("else {\n", out);
            continue;
        }

        fprintf(stderr, "nolc: unsupported NOL statement in %s: %s\n", path, s);
        fclose(in); return 0;
    }

    if (in_fn) { fprintf(stderr, "nolc: unterminated function in %s\n", path); fclose(in); return 0; }
    fclose(in);
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: nolc <input.nol> [input2.nol ...] <output.c>\n");
        return 2;
    }

    const char *outpath = argv[argc - 1];

    for (int i = 1; i < argc - 1; i++) {
        FILE *in = fopen(argv[i], "r");
        if (!in) { fprintf(stderr, "nolc: cannot open %s\n", argv[i]); return 1; }
        char line[4096];
        while (fgets(line, sizeof(line), in)) {
            char *s = trim(line);
            if (is_fn(s)) collect_fn(s);
        }
        fclose(in);
    }

    FILE *out = fopen(outpath, "w");
    if (!out) { perror(outpath); return 1; }

    fputs("#include \"terminal.h\"\n#include \"nol_runtime.h\"\n\n", out);
    for (int i = 0; i < fn_count; i++)
        fprintf(out, "void %s(%s);\n", fns[i].name, fns[i].argc ? "const char *cmd" : "void");
    fputc('\n', out);

    for (int i = 1; i < argc - 1; i++) {
        if (!compile_file(out, argv[i])) {
            fclose(out);
            remove(outpath);
            return 1;
        }
    }
    fclose(out);
    return 0;
}
