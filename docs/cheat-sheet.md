# elomaxz Cheat Sheet — Hybrid Functional MVU in C

**Core Struct**: `ElomaxzProgram`

```c
ElomaxzProgram prog = {
    .init = my_init,
    .update = my_update,      // returns new Model + Cmds
    .view = my_view,
    .msg_name = my_msg_name,
    .free_model = my_free_model,
    .free_msg = my_free_msg,
    .free_cmd = my_free_cmd,
    .handle_cmd = my_handle_cmd,   // NEW - for Cmd effects
    .debug = true,
};
```

**Runners**:
- `elomaxz_run_cli(&prog)`
- `elomaxz_run_with_msg_source(&prog, next_msg, ctx)`
- `elomaxz_run_batch(&prog, msgs, count)`

**Cmd System**:
```c
Cmd cmd = elomaxz_make_cmd(CMD_IO_WRITE, data, size);
// In update(): return cmds via cmds_out array
```

**Memory Rule**: Framework owns `free_model`, `free_msg`, `free_cmd`.

*elomaxz v0.3 — Hybrid by design*