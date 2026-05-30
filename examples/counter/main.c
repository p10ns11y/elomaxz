#include "../../include/elomaxz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int count;
} CounterModel;

typedef enum { MSG_INC, MSG_DEC, MSG_RESET, MSG_QUIT } MsgType;

typedef struct {
    MsgType type;
} CounterMsg;

Model init(
    void
) {
    CounterModel *m = malloc(sizeof(CounterModel));
    if (m) {
        m->count = 0;
    }
    return (Model) m;
}

Model update(
    Model current,
    Msg msg,
    Cmd *cmds_out,
    size_t *n_cmds
) {
    (void) cmds_out;
    (void) n_cmds; // Suppress unused warning

    CounterModel *old = (CounterModel *) current;
    CounterMsg *m = (CounterMsg *) msg;
    CounterModel *new_m = malloc(sizeof(CounterModel));
    if (!new_m || !old) {
        return current;
    }

    new_m->count = old->count;

    switch (m->type) {
        case MSG_INC:
            new_m->count++;
            break;
        case MSG_DEC:
            new_m->count--;
            break;
        case MSG_RESET:
            new_m->count = 0;
            break;
        case MSG_QUIT:
            break;
    }
    return (Model) new_m;
}

void view(
    Model model
) {
    CounterModel *m = (CounterModel *) model;
    printf("\n=== elomaxz Counter ===\nCount: %d\nCommands: + - r q\n",
           m ? m->count : 0);
}

const char *msg_name(
    Msg msg
) {
    CounterMsg *m = (CounterMsg *) msg;
    if (!m) {
        return "NULL";
    }
    switch (m->type) {
        case MSG_INC:
            return "INC";
        case MSG_DEC:
            return "DEC";
        case MSG_RESET:
            return "RESET";
        case MSG_QUIT:
            return "QUIT";
        default:
            return "UNKNOWN";
    }
}

void free_model(
    Model m
) {
    free(m);
}

void free_msg(
    Msg m
) {
    free(m);
}

void free_cmd(
    Cmd c
) {
    (void) c;
}

Msg next_msg_from_stdin(
    void *user_data
) {
    (void) user_data;
    char line[128];
    if (!fgets(line, sizeof(line), stdin)) {
        return NULL;
    }
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, "q") == 0 || strcmp(line, "quit") == 0) {
        return NULL; // ← Signal to stop the runner
    }

    CounterMsg *msg = malloc(sizeof(CounterMsg));
    if (!msg) {
        return NULL;
    }

    if (strcmp(line, "+") == 0) {
        msg->type = MSG_INC;
    } else if (strcmp(line, "-") == 0) {
        msg->type = MSG_DEC;
    } else if (strcmp(line, "r") == 0) {
        msg->type = MSG_RESET;
    } else {
        free(msg);
        printf("(unknown command)\n> ");
        return next_msg_from_stdin(user_data);
    }

    return (Msg) msg;
}

int main(
    void
) {
    printf("elomaxz Counter Demo — Clean Build\n\n");

    ElomaxzProgram prog = {
        .init = init,
        .update = update,
        .view = view,
        .msg_name = msg_name,
        .free_model = free_model,
        .free_msg = free_msg,
        .free_cmd = free_cmd,
        .debug = false,
    };

    elomaxz_run_with_msg_source(&prog, next_msg_from_stdin, NULL);
    printf("\nGoodbye!\n");
    return 0;
}