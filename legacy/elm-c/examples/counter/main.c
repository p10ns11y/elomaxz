#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/elm_c.h"

// Define YOUR struct with a different name
typedef struct {
    int count;
} CounterModel;

typedef enum {
    MSG_INC,
    MSG_DEC,
    MSG_RESET
} MsgType;

typedef struct {
    MsgType type;
} CounterMsg;

// Init - return as (Model)
Model init(void) {
    CounterModel* m = malloc(sizeof(CounterModel));
    if (m) m->count = 0;
    return (Model)m;                    // ← Cast to Model (void*)
}

// Update - take and return as Model/Msg
Model update(Model current, Msg msg) {
    CounterModel* old = (CounterModel*)current;   // ← Cast back
    CounterMsg* m = (CounterMsg*)msg;
    CounterModel* new_m = malloc(sizeof(CounterModel));
    if (!new_m || !old) return current;

    new_m->count = old->count;

    if (m->type == MSG_INC) new_m->count++;
    else if (m->type == MSG_DEC) new_m->count--;
    else if (m->type == MSG_RESET) new_m->count = 0;

    return (Model)new_m;                 // ← Cast to Model
}

void view(Model model) {
    CounterModel* m = (CounterModel*)model;
    printf("Count: %d | + - r q\n> ", m ? m->count : 0);
}

Msg parse(const char* input) {
    CounterMsg* m = malloc(sizeof(CounterMsg));
    if (strcmp(input, "+") == 0) m->type = MSG_INC;
    else if (strcmp(input, "-") == 0) m->type = MSG_DEC;
    else if (strcmp(input, "r") == 0) m->type = MSG_RESET;
    else m->type = -1;
    return (Msg)m;                       // ← Cast to Msg
}

void free_m(Model m) { free(m); }
void free_msg(Msg m) { free(m); }

int main(void) {
    printf("Original elm-c Counter (Fixed)\n");
    elm_run_interactive_cli(init, update, view, parse, free_m, free_msg);
    return 0;
}