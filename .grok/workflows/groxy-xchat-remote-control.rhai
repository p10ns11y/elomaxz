// Generated draft — Grok Build workflow template.
// Install: copy to .grok/workflows/groxy-xchat-remote-control.rhai (or ~/.grok/workflows/).
// Requires Grok host APIs: agent(), phase(), log(), complete() — not rhai-host.
// Skill chain: outcome-first-dm-replies → poll-loop-resilience → groxy-poller-watch

let meta = #{
    name: "groxy-xchat-remote-control",
    description: "End-to-end setup of XChat DM bidirectional control: branch from sentinel, implement resilient poller, outcome-first replies, and live E2E test",
    phases: [
        #{ title: "Explore", detail: "Read MCP docs and XChat constraints to decide on DM architecture" },
        #{ title: "Scaffold", detail: "Create feat/groxy-xchat-remote branch and basic inbound/outbound skeleton" },
        #{ title: "Refine", detail: "Strip system noise, lead with PR links, harden against rate limits" },
        #{ title: "Test", detail: "Run live poller and verify command-response loop" }
    ],
    when_to_use: "End-to-end setup of XChat DM bidirectional control: branch from sentinel, implement resilient poller, outcome-first replies, and live E2E test",
};

phase("Explore");
let r0 = agent(
    "Follow skill 'outcome-first-dm-replies'. Read MCP docs and XChat constraints to decide on DM architecture. Use read-only tools first when mapping; "
        + "then apply edits. Report {done: bool, notes: string}.",
    #{
        label: "outcome-first-dm-replies",
        capability_mode: "read-only",
        output_schema: #{
            "type": "object",
            "required": ["done", "notes"],
            "properties": #{
                "done": #{ "type": "boolean" },
                "notes": #{ "type": "string" },
            },
        },
    },
);
if r0 == () || !r0.success {
    log("phase failed: outcome-first-dm-replies");
}


phase("Scaffold");
let r1 = agent(
    "Follow skill 'poll-loop-resilience'. Create feat/groxy-xchat-remote branch and basic inbound/outbound skeleton. Use read-only tools first when mapping; "
        + "then apply edits. Report {done: bool, notes: string}.",
    #{
        label: "poll-loop-resilience",
        capability_mode: "read-write",
        output_schema: #{
            "type": "object",
            "required": ["done", "notes"],
            "properties": #{
                "done": #{ "type": "boolean" },
                "notes": #{ "type": "string" },
            },
        },
    },
);
if r1 == () || !r1.success {
    log("phase failed: poll-loop-resilience");
}


phase("Refine");
let r2 = agent(
    "Follow skill 'groxy-poller-watch'. Strip system noise, lead with PR links, harden against rate limits. Use read-only tools first when mapping; "
        + "then apply edits. Report {done: bool, notes: string}.",
    #{
        label: "groxy-poller-watch",
        capability_mode: "read-write",
        output_schema: #{
            "type": "object",
            "required": ["done", "notes"],
            "properties": #{
                "done": #{ "type": "boolean" },
                "notes": #{ "type": "string" },
            },
        },
    },
);
if r2 == () || !r2.success {
    log("phase failed: groxy-poller-watch");
}


phase("Test");
let r3 = agent(
    "Follow skill 'Test'. Run live poller and verify command-response loop. Use read-only tools first when mapping; "
        + "then apply edits. Report {done: bool, notes: string}.",
    #{
        label: "Test",
        capability_mode: "read-write",
        output_schema: #{
            "type": "object",
            "required": ["done", "notes"],
            "properties": #{
                "done": #{ "type": "boolean" },
                "notes": #{ "type": "string" },
            },
        },
    },
);
if r3 == () || !r3.success {
    log("phase failed: Test");
}

complete(#{
    summary: "Finished workflow groxy-xchat-remote-control",
    skill_chain: ["outcome-first-dm-replies","poll-loop-resilience","groxy-poller-watch"],
});
