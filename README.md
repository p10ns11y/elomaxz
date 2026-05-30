# elomaxz — Hybrid Functional MVU Framework for C

<p align="center">
  <img src="elomaxz.jpg" alt="Elomaxz" width="720"/>
</p>

**elomaxz** (Elm + Maximum) is a thin, powerful hybrid MVU framework for C.

- Core: Explicit Tagged Message + Pure Update
- Strong Cmd / Effect System (Functional Core + Imperative Shell)
- Foundation for Actor-style Composition

```mermaid
flowchart TD
    subgraph UserCode["User Application Code"]
        A["Define MachineState + Msg types"]
        B["Implement init / update / view functions"]
        C["Create ElomaxzProgram struct"]
    end

    subgraph elomaxzCore["elomaxz Core Framework"]
        D["ElomaxzProgram"]
        E["Cmd / Effect System"]
        F["Message Source next_msg"]
        G["Runners elomaxz_run_cli / elomaxz_run_with_msg_source"]
    end

    subgraph System["System Layer"]
        H["Execute shell commands pacman, cp, systemctl..."]
    end

    A --> B
    B --> C
    C --> D
    D --> E
    D --> F
    F --> G
    G --> E
    E --> H

    style UserCode fill:#f0f8ff,stroke:#1a365d,stroke-width:3px,color:#1a365d
    style elomaxzCore fill:#fffaf0,stroke:#c05621,stroke-width:3px,color:#c05621
    style System fill:#f0fff4,stroke:#276749,stroke-width:3px,color:#276749
```

## Quick Start

```bash
make
./bin/counter
```

Or run directly:

```bash
make run
```

Build output goes to `bin/`.

## Key Files

- `include/elomaxz.h` — Main header
- `src/elomaxz.c` — Implementation
- `examples/counter/main.c` — Working demo
- `Cheat_Sheet_elomaxz.md` — Quick reference

See `legacy/elm-c/` for the original starting point.

---

*From simple elm-c → powerful hybrid elomaxz*
