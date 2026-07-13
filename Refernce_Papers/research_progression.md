# RL-Based Wi-Fi Optimization Research Progression

## Overview

The objective of this research journey is to gradually evolve from a simple Reinforcement Learning-based Wi-Fi optimization system into a sophisticated multi-AP hierarchical optimization framework.

Instead of attempting to solve every networking problem at once, each stage introduces only one major new concept while reusing knowledge from previous stages.

This ensures:

* Easier implementation
* Better understanding of the underlying networking concepts
* Clear research contributions
* Strong publication potential
* A logical progression from beginner-level RL networking research to advanced Wi-Fi optimization

---

# Big Picture

The entire research journey can be summarized as:

```text
Teach one AP to allocate resources

↓

Teach multiple APs to learn together

↓

Teach multiple APs to jointly optimize the entire Wi-Fi network
```

---

# Stage 1

# Can RL Manage One AP?

## Core Question

Can a Reinforcement Learning agent learn how to efficiently divide Wi-Fi resources among different traffic types?

---

## Scenario

Single Wi-Fi 6 Access Point

Traffic Types:

* Video Streaming
* Gaming
* IoT Devices

---

## What the RL Agent Learns

The agent learns how to allocate available resources between different traffic classes.

Example:

```text
Video  -> 50%
Gaming -> 30%
IoT    -> 20%
```

---

## Why Wi-Fi 6?

Wi-Fi 6 introduces:

* OFDMA
* Resource Units (RUs)
* Network Slicing

These technologies provide the foundation for RL-based resource allocation.

---

## Concepts Learned

Networking:

* Access Points
* OFDMA
* Resource Units
* Network Slicing
* Throughput
* Latency
* Queueing

Reinforcement Learning:

* State
* Action
* Reward
* PPO
* Policy Learning

Simulation:

* ns-3
* Wi-Fi 6 Modeling

---

## Outcome

A working RL-based network slicing framework for a single Wi-Fi Access Point.

---

# Stage 2

# Can Multiple APs Learn Together?

## Core Question

Can multiple Wi-Fi Access Points collaboratively learn better slicing policies than independent APs?

---

## Scenario

Four Wi-Fi Access Points

```text
AP1
AP2
AP3
AP4
```

Each AP runs its own RL agent.

---

## New Idea

Instead of learning independently:

```text
AP1 -> PPO

AP2 -> PPO

AP3 -> PPO

AP4 -> PPO
```

the APs periodically share knowledge through Federated Learning.

---

## System Architecture

```text
AP1 PPO
AP2 PPO
AP3 PPO
AP4 PPO

↓

Federated Aggregation

↓

Global Policy

↓

Redistribution to APs
```

---

## Why Keep Wi-Fi 6?

At this stage the objective is:

```text
Validate Federation
```

not

```text
Introduce Wi-Fi 7 complexity
```

Keeping the wireless technology unchanged allows performance improvements to be attributed directly to Federated Learning.

---

## New Concepts Learned

* Federated Learning
* Federated Reinforcement Learning (FRL)
* Distributed Learning
* Multi-AP Coordination
* Policy Aggregation
* Fairness Optimization

---

## Outcome

A Federated RL framework capable of coordinating slicing decisions across multiple Wi-Fi Access Points.

---

# Stage 3

# Can We Optimize More Than Just Slicing?

## Core Question

Can Reinforcement Learning jointly optimize multiple Wi-Fi resource management tasks instead of focusing only on slicing?

---

## Motivation

Previous stages only optimize:

```text
Network Slicing
```

Real Wi-Fi networks require optimization of:

```text
Channel Allocation
Network Slicing
Link Activation
```

simultaneously.

---

## New Idea

Introduce Hierarchical Reinforcement Learning.

Instead of one RL agent making all decisions, multiple decision layers are created.

---

## Hierarchical Structure

### Level 1

Channel Allocation

Question:

```text
Which channel should each AP use?
```

---

### Level 2

Network Slicing

Question:

```text
How should resources be divided among traffic classes?
```

---

### Level 3

Link Activation

Question:

```text
Which wireless links should be activated?
```

---

## Why Introduce Wi-Fi 7 Here?

Wi-Fi 7 introduces:

* Multi-Link Operation (MLO)
* Multiple simultaneous links
* More complex optimization opportunities

These capabilities naturally fit the Hierarchical RL framework.

---

## System Architecture

```text
Multiple APs

↓

Hierarchical RL

├── Level 1
│   Channel Allocation
│
├── Level 2
│   Network Slicing
│
└── Level 3
    Link Activation
```

---

## New Concepts Learned

* Hierarchical RL
* Multi-Level Decision Making
* Wi-Fi 7 MLO
* Joint Optimization
* Large-Scale Resource Management

---

## Outcome

A unified framework capable of jointly optimizing:

* Channel Selection
* Resource Allocation
* Link Activation

across multiple Wi-Fi Access Points.

---

# Research Evolution

The progression can be visualized as:

```text
Stage 1
──────────────────────────
Single AP
+
RL Slicing

↓

Stage 2
──────────────────────────
Multiple APs
+
Federated RL
+
Network Slicing

↓

Stage 3
──────────────────────────
Multiple APs
+
Hierarchical RL
+
Channel Allocation
+
Network Slicing
+
Link Activation
```

---

# Final Research Story

Paper A

RL-Based Wi-Fi 6 Network Slicing

↓

Paper B

Federated RL-Based Multi-AP Network Slicing

↓

Paper C

Hierarchical RL-Based Multi-AP Wi-Fi Resource Management

The key idea is that every stage extends the previous stage rather than replacing it, resulting in a coherent and scalable research roadmap.
