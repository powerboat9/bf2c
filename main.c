#include <stdlib.h>
#include <stdio.h>

#define PUSH_STR(s) ((fputs(s, stdout) == EOF) ? die_write() : 0)

int die_write() {
    fputs("[WRITE ERROR]\n", stderr);
    exit(-1);
}

void push_indent(int cnt) {
    for (int i = 0; i < cnt; i++) {
        PUSH_STR("    ");
    }
}

int main(int argc, char **argv) {
    PUSH_STR(
"#include <stdlib.h>\n"
"#include <stdio.h>\n"
"#include <sys/mman.h>\n"
"#define MEM_SIZE (1L << 30)\n"
"\n"
"int die_write() {\n"
"    fputs(\"[WRITE ERROR]\", stderr);\n"
"    exit(-1);\n"
"}\n"
"\n"
"int main(int argc, char **argv) {\n"
"    unsigned char *ptr = (unsigned char *) mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);\n"
"    if (ptr == MAP_FAILED) {\n"
"        fputs(\"[MMAP ERROR]\", stderr);\n"
"    }\n"
"    ptr += MEM_SIZE >> 1;\n"
"    int tmp;\n"
);
    int r;
    int indent = 1;
    while ((r = getc(stdin)) != EOF) {
        char c = (char) r;
#define PUSH_LINE(s) push_indent(indent); PUSH_STR(s "\n")
#define SW(c, s) case c: PUSH_LINE(s); break;
        switch (c) {
            SW('+', "(*ptr)++;")
            SW('-', "(*ptr)--;")
            SW('<', "ptr--;")
            SW('>', "ptr++;")
            case ',':
                PUSH_LINE("tmp = getc(stdin);");
                PUSH_LINE("*ptr = (tmp == EOF) ? 0 : tmp;");
                break;
            case '.':
                PUSH_LINE("tmp = putchar(*ptr);");
                PUSH_LINE("if (tmp == EOF) die_write();");
                break;
            case '[':
                PUSH_LINE("while (*ptr) {");
                indent++;
                break;
            case ']':
                indent--;
                PUSH_LINE("}");
                break;
        }
    }
    PUSH_STR("}\n");
    return 0;
}
