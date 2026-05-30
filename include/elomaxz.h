#ifndef ELOMAXZ_H
#define ELOMAXZ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// elomaxz — Hybrid Functional MVU Framework for C (v0.3)
// Core: Explicit Tagged Message + Pure Update
// + Strong Cmd System (Functional Core + Imperative Shell)
// + Foundation for Composition (Actor Model)

typedef void *Model;
typedef void *Msg;
typedef void *Cmd;

typedef enum {
    CMD_NONE = 0,
    CMD_CUSTOM,
    CMD_DELAY,
    CMD_IO_READ,
    CMD_IO_WRITE,
    CMD_NETWORK_SEND,
    CMD_ML_TRAIN_STEP,
} CmdType;

typedef struct {
    CmdType type;
    void *data;
    size_t size;
    void (*on_complete)(Msg *result_msg);
} CmdData;

typedef struct {
    Model (*init)(void);
    Model (*update)(Model current, Msg msg, Cmd *cmds_out, size_t *num_cmds_out);
    void (*view)(Model model);

    void (*free_model)(Model);
    void (*free_msg)(Msg);
    void (*free_cmd)(Cmd);

    const char *(*msg_name)(Msg);
    void (*debug_model)(Model);

    void (*handle_cmd)(Cmd cmd,
                       Msg *result_msg); // For Functional Core + Imperative Shell

    void *parent;
    void *message_bus;

    bool debug;
    void *user_data;
} ElomaxzProgram;

void elomaxz_run_cli(const ElomaxzProgram *prog);
void elomaxz_run_with_msg_source(const ElomaxzProgram *prog,
                                 Msg (*next_msg)(void *user_data),
                                 void *user_data);
void elomaxz_run_batch(const ElomaxzProgram *prog, Msg *msgs, size_t count);
void elomaxz_execute_cmds(const ElomaxzProgram *prog, Cmd *cmds, size_t n);
void elomaxz_print_prompt(const char *prompt);
Cmd elomaxz_make_cmd(CmdType type, void *data, size_t size);

#endif // ELOMAXZ_H