#ifndef ELM_C_H
#define ELM_C_H

// Original elm-c v0.1 — The starting point of our evolution
// This is kept for historical reference inside elomaxz

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* Model;
typedef void* Msg;

typedef Model (*ElmInitFn)(void);
typedef Model (*ElmUpdateFn)(Model current, Msg msg);
typedef void (*ElmViewFn)(Model model);
typedef Msg (*ElmParseInputFn)(const char* input_line);
typedef void (*ElmFreeModelFn)(Model model);
typedef void (*ElmFreeMsgFn)(Msg msg);

void elm_run_interactive_cli(
    ElmInitFn init,
    ElmUpdateFn update,
    ElmViewFn view,
    ElmParseInputFn parse_input,
    ElmFreeModelFn free_model,
    ElmFreeMsgFn free_msg
);

Model elm_run_step(
    Model current,
    Msg msg,
    ElmUpdateFn update,
    ElmFreeModelFn free_model
);

void elm_print_prompt(const char* prompt);

#endif // ELM_C_H