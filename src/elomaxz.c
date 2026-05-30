#include "elomaxz.h"

void elomaxz_print_prompt(
    const char *prompt
) {
    if (prompt) {
        fputs(prompt, stdout);
        fflush(stdout);
    }
}

Cmd elomaxz_make_cmd(
    CmdType type,
    void *data,
    size_t size
) {
    CmdData *cmd = malloc(sizeof(CmdData));
    if (!cmd) {
        return NULL;
    }
    cmd->type = type;
    cmd->data = data;
    cmd->size = size;
    cmd->on_complete = NULL;
    return (Cmd) cmd;
}

void elomaxz_execute_cmds(
    const ElomaxzProgram *prog,
    Cmd *cmds,
    size_t n
) {
    if (!prog || !cmds || n == 0) {
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (!cmds[i]) {
            continue;
        }
        if (prog->handle_cmd) {
            Msg result = NULL;
            prog->handle_cmd(cmds[i], &result);
            if (result && prog->free_msg) {
                prog->free_msg(result);
            }
        }
        if (prog->free_cmd) {
            prog->free_cmd(cmds[i]);
        }
    }
}

void elomaxz_run_cli(
    const ElomaxzProgram *prog
) {
    if (!prog || !prog->init || !prog->update || !prog->view) {
        return;
    }
    Model model = prog->init();
    if (!model) {
        return;
    }
    char line[2048];
    bool running = true;
    prog->view(model);
    while (running) {
        elomaxz_print_prompt("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "quit") == 0 || strcmp(line, "q") == 0) {
            running = false;
            continue;
        }
        printf("[elomaxz] Use elomaxz_run_with_msg_source for production\n");
        running = false;
    }
    if (prog->free_model && model) {
        prog->free_model(model);
    }
}

void elomaxz_run_with_msg_source(
    const ElomaxzProgram *prog,
    Msg (*next_msg)(void *),
    void *user_data
) {
    if (!prog || !prog->init || !prog->update || !prog->view || !next_msg) {
        return;
    }
    Model model = prog->init();
    if (!model) {
        return;
    }
    prog->view(model);
    while (true) {
        Msg msg = next_msg(user_data);
        if (!msg) {
            break;
        }
        Cmd cmds[32] = {0};
        size_t num_cmds = 0;
        Model new_model = prog->update(model, msg, cmds, &num_cmds);
        if (prog->debug && prog->msg_name) {
            fprintf(stderr, "[DEBUG] Msg=%s\n", prog->msg_name(msg));
        }
        if (prog->free_model && model) {
            prog->free_model(model);
        }
        model = new_model;
        if (num_cmds > 0) {
            elomaxz_execute_cmds(prog, cmds, num_cmds);
        }
        if (prog->free_msg) {
            prog->free_msg(msg);
        }
        prog->view(model);
    }
    if (prog->free_model && model) {
        prog->free_model(model);
    }
}

void elomaxz_run_batch(
    const ElomaxzProgram *prog,
    Msg *msgs,
    size_t count
) {
    if (!prog || !msgs || count == 0) {
        return;
    }
    Model model = prog->init();
    if (!model) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        Cmd cmds[16] = {0};
        size_t n = 0;
        Model new_model = prog->update(model, msgs[i], cmds, &n);
        if (prog->debug && prog->msg_name) {
            fprintf(stderr, "[BATCH] #%zu %s\n", i, prog->msg_name(msgs[i]));
        }
        if (prog->free_model && model) {
            prog->free_model(model);
        }
        model = new_model;
        if (n > 0) {
            elomaxz_execute_cmds(prog, cmds, n);
        }
        if (prog->free_msg) {
            prog->free_msg(msgs[i]);
        }
    }
    prog->view(model);
    if (prog->free_model && model) {
        prog->free_model(model);
    }
}