---
name: groxy-xchat-remote-control
description: End-to-end setup of XChat DM bidirectional control: branch from sentinel, implement resilient poller, outcome-first replies, and live E2E test
kind: workflow
skill_chain: ["outcome-first-dm-replies", "poll-loop-resilience", "groxy-poller-watch"]
---

# groxy-xchat-remote-control

End-to-end setup of XChat DM bidirectional control: branch from sentinel, implement resilient poller, outcome-first replies, and live E2E test

## Skill chain

1. `outcome-first-dm-replies`
2. `poll-loop-resilience`
3. `groxy-poller-watch`

## Phases

### Explore

Read MCP docs and XChat constraints to decide on DM architecture

### Scaffold

Create feat/groxy-xchat-remote branch and basic inbound/outbound skeleton

### Refine

Strip system noise, lead with PR links, harden against rate limits

### Test

Run live poller and verify command-response loop

## Support

- sessions: 1
- rank: 29
