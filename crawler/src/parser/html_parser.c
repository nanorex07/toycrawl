#include "html_parser.h"

typedef enum {
    TOKEN_TAG_START,
    TOKEN_TAG_END,
    TOKEN_SELF_CLOSING,
    TOKEN_TEXT,
    TOKEN_ATTR_NAME,
    TOKEN_ATTR_VALUE,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef enum {
    CTX_INITIAL,
    CTX_IN_TAG,
    CTX_IN_ATTR
} Ctx;

typedef struct {
    FILE* fp;
    size_t scanned;
    CharList* curr;
    Ctx ctx;
} Scanner;

typedef struct {
    TokenType type;
    char* value;
    size_t length;
} Token;


html_parser* initialize_parser(const size_t *max_bytes) {
    html_parser *parser = malloc(sizeof(html_parser));
    parser->max_bytes = *(size_t*)max_bytes;
    return parser;
}

void free_parser(html_parser *parser) {
    // free(parser->stack);
    free(parser);
}

// void stack_push(html_parser *parser, char item) {
//     if (parser->stack_top >= (long) parser->max_bytes) {
//         log_error("parser stack overflow.");
//         exit(0);
//     }
//     parser->stack[++parser->stack_top] = item;
// }

// char stack_pop(html_parser *parser, char item) {
//     if (parser->stack_top < 0) {
//         log_error("parser stack underflow");
//         exit(0);
//     }
//     return parser->stack[parser->stack_top--];
// }

char peek(Scanner *scanner) {
    char c = fgetc(scanner->fp);
    ungetc(1, scanner->fp);
    return c;
}

char advance_scanner(Scanner *scanner) {
    char c = fgetc(scanner->fp);
    append_char(scanner->curr, c);
    scanner->scanned++;
    return c;
}

void skip_whitespace(Scanner *scanner) {
    char c;
    for (;;) {
        c = peek(scanner);
        switch (c) {
            case '<': {
                advance_scanner(scanner);
                c = peek(scanner);
                if (c == 'a') return;
            }
            default:
                advance_scanner(scanner);
        }
    }
}

static Token make_token(Scanner *scanner, TokenType type) {
    Token t;
    t.type = type;
    t.length = scanner->curr->length;
    t.value = resolve(scanner->curr);
    return t;
}

static Token text(Scanner *scanner) {
    for(;;) {
        char c = peek(scanner);
        if (c == '<') {
            break;
        } else {
            advance_scanner(scanner);
        }
    }
    return make_token(scanner, TOKEN_TEXT);
}

static Token start_tag(Scanner *scanner) {
    char c = advance_scanner(scanner);
    switch (c) {
        case EOF: {
            return make_token(scanner, TOKEN_EOF);
        }
        case '<':
        default: {
            return text(scanner);
        }
    }
}

Token scan_token(Scanner *scanner) {
    switch (scanner->ctx) {
        case CTX_INITIAL: {
            skip_whitespace(scanner);
            return start_tag(scanner);
        }
        case CTX_IN_TAG:
        case CTX_IN_ATTR: 
        default:
            return make_token(scanner, TOKEN_ERROR);
    }
}

void parse_file(char *filename, char* output_folder, html_parser *parser) {
    FILE *fp = open_file_with_error(filename);

    Token token;

    CharList *list = init_char_list();
    Scanner scanner = {fp, 0, list, CTX_INITIAL};

    for (;;) {
        token = scan_token(&scanner);

        printf("%d | Len: %d | %s\n", token.type, (int)token.length, token.value);

        if (token.type == TOKEN_EOF) {
            break;
        }
    }


    fclose(fp);
}
