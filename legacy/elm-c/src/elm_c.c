#include "elm_c.h"

void elm_print_prompt(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
}

void elm_run_interactive_cli(
    ElmInitFn init, ElmUpdateFn update, ElmViewFn view,
    ElmParseInputFn parse_input, ElmFreeModelFn free_model, ElmFreeMsgFn free_msg
) {
    Model model = init();
    if (!model) return;

    char line[1024];
    bool running = true;

    view(model);

    while (running) {
        elm_print_prompt("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "quit") == 0 || strcmp(line, "q") == 0) {
            running = false;
            continue;
        }

        Msg msg = parse_input(line);
        if (msg) {
            Model new_model = update(model, msg);
            if (free_model && model) free_model(model);
            model = new_model;
            if (free_msg) free_msg(msg);
            view(model);
        }
    }
    if (free_model && model) free_model(model);
}

Model elm_run_step(Model current, Msg msg, ElmUpdateFn update, ElmFreeModelFn free_model) {
    if (!update || !current) return current;
    Model new_model = update(current, msg);
    if (free_model && current) free_model(current);
    return new_model;
}