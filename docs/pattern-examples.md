# High-Level Code Flow Examples: 3 Major Functional Patterns in C

This document shows **practical high-level code flows** for the three most relevant functional-style patterns used in large C systems today.

---

## 1. Functional Core + Imperative Shell

**Philosophy**: Keep **all business logic pure**. Put all side effects (IO, network, allocation, threads) in a thin outer layer.

### High-Level Flow

```c
// ==================== IMPERATIVE SHELL (main.c / entry point) ====================
int main(void) {
    AppState state = load_initial_state();           // impure: read from disk/network

    while (running) {
        UserInput input = get_user_input();          // impure: stdin, socket, etc.

        // === CALL INTO PURE CORE ===
        AppState new_state = process_input(input, state);   // PURE FUNCTION

        render(new_state);                           // impure: draw / print
        save_state(new_state);                       // impure: persist

        state = new_state;
    }
}

// ==================== FUNCTIONAL CORE (pure logic - easy to test) ====================
AppState process_input(UserInput input, AppState current) {
    // NO side effects allowed here!
    switch (input.type) {
        case INPUT_ADD_ITEM:
            return add_item_to_state(current, input.item);
        case INPUT_REMOVE_ITEM:
            return remove_item_from_state(current, input.id);
        // ...
        default:
            return current;
    }
}

// Pure helper functions
AppState add_item_to_state(AppState s, Item item) {
    AppState new_s = copy_state(s);     // or use immutable update
    new_s.items[new_s.count++] = item;
    return new_s;
}
```

**Key Benefits**:
- `process_input()` is 100% testable with no mocks
- Side effects are isolated and obvious
- Easy to reason about

**Best for**: CLI tools, servers, data processors, most business logic

---

## 2. Actor Model / Message Passing

**Philosophy**: Everything is an **actor** that receives messages, updates its state purely, and can send messages to other actors.

### High-Level Flow

```c
// ==================== ACTOR DEFINITION ====================
typedef struct {
    State state;
    MessageQueue inbox;
} Actor;

// ==================== ACTOR LOOP (runs in its own thread or event loop) ====================
void actor_run(Actor* self) {
    while (true) {
        Message msg = receive_message(&self->inbox);     // blocking or polling

        // === PURE UPDATE ===
        State new_state = handle_message(self->state, msg);

        self->state = new_state;

        // Optional: send messages to other actors
        if (should_notify_others(msg)) {
            send_message(other_actor, create_notification(msg));
        }
    }
}

// ==================== PURE MESSAGE HANDLER ====================
State handle_message(State current, Message msg) {
    switch (msg.type) {
        case MSG_INCREMENT:
            return (State){ .count = current.count + msg.value };
        case MSG_RESET:
            return (State){ .count = 0 };
        default:
            return current;
    }
}
```

**Typical Usage**:
- One actor per major component (UI actor, Network actor, Database actor, etc.)
- Actors communicate only via messages (no shared mutable state)

**Best for**: Concurrent systems, games, high-performance servers, distributed systems

---

## 3. Explicit Tagged Message + Pure Update (Our Current elm-c Style)

**Philosophy**: All changes happen through **explicit messages**. The `update` function is pure and returns a new state.

This is the pattern we already implemented in elm-c v0.2.

### High-Level Flow

```c
// ==================== YOUR APPLICATION CODE ====================
typedef struct { int count; } Model;
typedef enum { MSG_INC, MSG_DEC, MSG_RESET } MsgType;
typedef struct { MsgType type; int value; } Msg;

Model update(Model current, Msg msg, Cmd* cmds, size_t* n_cmds) {
    Model new_model = copy_model(current);   // pure

    switch (msg.type) {
        case MSG_INC:
            new_model.count += msg.value;
            break;
        case MSG_DEC:
            new_model.count -= msg.value;
            break;
        case MSG_RESET:
            new_model.count = 0;
            break;
    }
    *n_cmds = 0;
    return new_model;
}

// ==================== FRAMEWORK / RUNNER ====================
void run_app(ElmProgram* prog) {
    Model model = prog->init();

    while (true) {
        Msg msg = get_next_message();           // from stdin, network, timer, etc.

        Cmd cmds[16];
        size_t n = 0;

        Model new_model = prog->update(model, msg, cmds, &n);

        prog->free_model(model);
        model = new_model;

        execute_cmds(cmds, n);                  // side effects
        prog->view(model);
    }
}
```

**Best for**: Interactive CLI tools, stateful utilities, REPLs, any system where you want maximum predictability

---

## Quick Comparison

| Pattern                        | Purity Level | Concurrency | Testability | Best Use Case                     | Complexity |
|--------------------------------|--------------|-------------|-------------|-----------------------------------|------------|
| **Functional Core + Imperative Shell** | Very High    | Medium      | Excellent   | Most applications                 | Low        |
| **Actor Model / Message Passing**      | High         | Excellent   | Very Good   | Concurrent / distributed systems  | Medium     |
| **Explicit Tagged Msg + Pure Update**  | Very High    | Good        | Excellent   | Interactive tools, state machines | Low-Medium |

---

## Recommendation for elm-c

Our current design is already very close to **Pattern 3** (Explicit Tagged Message + Pure Update).

**Future evolution ideas**:

- Add strong **Cmd** support → moves us closer to **Functional Core + Imperative Shell**
- Add **multiple actors** / component composition → moves us closer to **Actor Model**
- Keep the current clean `ElmProgram` structure (it's already excellent)
