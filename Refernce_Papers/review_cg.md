# Comprehensive Study Guide: Reinforcement Learning, Federated Learning, Digital Twins, and Wi-Fi Resource Optimization

---

# Table of Contents

1. Introduction
2. Relationship Between the Three Papers
3. Paper 1: Reinforcement Learning, Rule-Based, or Generative AI: A Comparison of Model-Free Wi-Fi Slicing Approaches

   * Motivation
   * Background Concepts
   * Problem Formulation
   * System Architecture
   * Algorithms Compared
   * SAC-RE Contribution
   * Neural Network Design
   * Simulation Environment
   * Experimental Scenarios
   * Results
   * Replication Guide
4. Paper 2: Beyond Wi-Fi 7: Enhanced Decentralized WLANs with Federated Reinforcement Learning

   * Motivation
   * Wi-Fi 7 Features
   * Federated Reinforcement Learning Architecture
   * System Model
   * Learning Process
   * Reward Aggregation
   * Simulation Setup
   * Results
   * Replication Guide
5. Paper 3: Reinforcement Learning for Channel Allocation Using Digital Twins (MELODWIN)

   * Motivation
   * Channel Allocation Problem
   * Digital Twin Concept
   * MELODWIN Architecture
   * Reinforcement Learning Integration
   * Real Network Validation
   * Results
   * Replication Guide
6. Comparison of the Three Papers
7. Recommended Study Path
8. Resources Required for Full Replication
9. Future Research Directions

---

# 1. Introduction

The three papers address a common challenge:

> Can Artificial Intelligence automatically optimize Wi-Fi networks better than traditional rule-based approaches?

Modern Wi-Fi networks suffer from:

* Channel congestion
* Interference
* Unpredictable latency
* Difficult resource management
* Dense deployments
* Growing QoS requirements

Each paper focuses on a different optimization layer.

| Paper         | Optimization Target      |
| ------------- | ------------------------ |
| MELODWIN      | Channel Allocation       |
| Wi-Fi Slicing | Resource Unit Allocation |
| Federated RL  | Multi-AP Coordination    |

Together they form a progression from local optimization to large-scale decentralized coordination.

---

# 2. Relationship Between the Three Papers

The papers can be viewed as three layers of Wi-Fi intelligence.

## Layer 1: Channel Selection

Paper:

**MELODWIN**

Question:

> Which channel should each Access Point use?

Technique:

* Reinforcement Learning
* Digital Twin

Output:

* Optimal channel assignment

---

## Layer 2: Resource Allocation

Paper:

**Wi-Fi Slicing**

Question:

> How should available bandwidth be divided among traffic flows?

Technique:

* RL
* Rule-Based Scheduling
* Large Language Models

Output:

* Optimal Resource Unit allocation

---

## Layer 3: Multi-AP Coordination

Paper:

**Federated Reinforcement Learning**

Question:

> How should multiple Access Points cooperate?

Technique:

* Federated Reinforcement Learning
* Distributed Reward Sharing

Output:

* Coordinated multi-link activation

---

# 3. Paper 1

# Reinforcement Learning, Rule-Based, or Generative AI: A Comparison of Model-Free Wi-Fi Slicing Approaches

Authors:

* Rafael Rosales
* Dave Cavalcanti

Published:

* Frontiers in Signal Processing (2025)

---

# Motivation

Wi-Fi 6 introduced OFDMA.

Instead of assigning an entire channel to one user:

80 MHz Channel

RU1 | RU2 | RU3 | RU4 | RU5

The Access Point can divide spectrum into smaller Resource Units (RUs).

Question:

> How should these RUs be distributed dynamically?

The objective is to maximize:

* Throughput
* QoS

while minimizing:

* Latency
* Resource waste

---

# Wi-Fi Resource Slicing

Resource slicing means partitioning available frequency resources among traffic flows.

Example:

Flow A → 50%

Flow B → 30%

Flow C → 20%

The AP decides this allocation every scheduling interval.

---

# Problem Formulation

State:

Traffic history and demand levels for each slice.

Example:

s = [Demand1, Demand2, Demand3]

---

Action:

Allocation vector:

a = [0.5, 0.3, 0.2]

meaning:

* 50% resources to Slice 1
* 30% resources to Slice 2
* 20% resources to Slice 3

---

Reward:

Total received bytes.

Equivalent to maximizing throughput.

---

Constraint:

Packet latency penalties.

Goal:

Maximize throughput while maintaining latency constraints.

---

# Algorithms Compared

## Reinforcement Learning

1. REINFORCE
2. A2C
3. PPO
4. SAC-RE

---

## Rule-Based Policies

### Uniform

Resources are split equally.

Example:

33%
33%
33%

---

### Proportional

Resources follow traffic demand.

Example:

Demand:

500
300
200

Allocation:

50%
30%
20%

---

## Generative AI Policy

GPT-4o used directly as a scheduling policy.

Inputs:

* Current traffic
* Traffic history
* Constraints

Output:

Allocation vector

Example:

[0.42,0.31,0.27]

No training required.

---

# Main Contribution

# SAC-RE

State-Augmented Constrained REINFORCE

Problem:

Previous constrained RL methods require differentiable network models.

ns-3 does not provide gradients.

Solution:

Adapt state-augmented RL into a model-free framework.

---

## State Augmentation

Traditional policy:

π(state)

SAC-RE policy:

π(state, λ)

where λ is a Lagrange multiplier.

λ indicates constraint violation severity.

Benefits:

* Dynamic adaptation
* Better constraint satisfaction
* Multiple operating modes

---

# Neural Network Architecture

Input:

Traffic history

↓

Linear Layer

↓

LeakyReLU

↓

Linear Layer

↓

LeakyReLU

↓

Linear Layer

↓

Dirichlet Distribution

---

Why Dirichlet?

Because allocations must satisfy:

a1 + a2 + a3 = 1

Perfect for resource allocation.

---

# Training Environment

Two simulators:

## Fast Python Simulator

Purpose:

* Rapid RL training

Reason:

* ns-3 is too slow

---

## ns-3 Wi-Fi 6 Simulator

Purpose:

* Performance evaluation

Features:

* OFDMA
* Slice-aware MAC
* AP scheduling

---

## Communication Framework

ns3-ai

Used for communication between:

* Python
* ns-3

---

# Experimental Scenarios

## Scenario A

Periodic Traffic

* Slice 1 constant
* Slice 2 alternates
* Slice 3 alternates

Predictable pattern.

---

## Scenario B

Random Walk Traffic

Traffic changes randomly.

More realistic.

More difficult.

---

# Results

Best overall:

1. Uniform
2. LLM

Both consistently appear on the Pareto front.

---

Best RL:

SAC-RE

Outperformed:

* PPO
* A2C
* REINFORCE

Especially under random traffic.

---

Interesting Finding

LLM-based scheduling:

* No training
* No fine tuning

Still competitive.

Limitation:

High inference latency.

---

# Replication Guide

Software:

* Python
* PyTorch
* Stable-Baselines3
* Gymnasium
* ns-3
* ns3-ai

Hardware:

* RTX A3000 GPU

Implementation Steps:

1. Build OFDMA environment
2. Train RL agents
3. Evaluate in ns-3
4. Compare with heuristic policies
5. Test LLM-based scheduler

---

# 4. Paper 2

# Beyond Wi-Fi 7: Enhanced Decentralized WLANs with Federated Reinforcement Learning

Authors:

* Rashid Ali
* Alaa Omran Almagrabi

Published:

* CMC 2026

---

# Motivation

Future Wi-Fi networks are moving toward:

* Multiple APs
* Multi-Link Operation
* Distributed decision making

Traditional RL assumes a single agent.

Problem:

Multiple APs learning independently can produce suboptimal behavior.

---

# Wi-Fi 7 Features

Important features:

* Multi-Link Operation (MLO)
* Multi-AP Coordination
* Advanced Beamforming
* Multi-BSS Cooperation

MLO allows simultaneous use of:

* 2.4 GHz
* 5 GHz
* 6 GHz

links.

---

# Why Federated RL?

Centralized ML faces:

* Privacy issues
* Scalability limitations
* Communication overhead
* Latency

Federated RL solves these issues.

Principle:

Train locally.

Share knowledge.

Do not share raw data.

---

# FRL Architecture

Two components:

## Local Learning Model (LLM)

Each AP learns independently.

Observes:

* Throughput
* Interference
* Link quality

Generates local policy.

---

## Global Learning Model (GLM)

Collects rewards from APs.

Aggregates knowledge.

Provides coordination signal.

---

# System Model

Environment:

4 APs

Each AP:

* AP-MLD
* Multiple links

Each AP selects:

* Link 1
* Link 2
* Both links

---

# Reinforcement Learning Formulation

State:

* Interference
* Link quality
* Available bandwidth

Action:

Link activation decision.

Reward:

Achieved throughput.

---

# Local Learning

Uses:

Multi-Armed Bandit (MAB)

Each link configuration represents an arm.

Reward:

Observed throughput.

Advantages:

* Lightweight
* Fast convergence
* Real-time execution

---

# Federated Learning Stage

Each AP shares reward information.

Not model parameters.

Not raw traffic data.

---

# Reward Aggregation

Uses:

Minimum Achieved Reward (MAR)

Global reward:

Minimum reward among participating APs.

Objective:

Max-Min fairness.

Prevent starvation.

Encourage cooperative behavior.

---

# Simulation Setup

Area:

100 m × 100 m

Nodes:

* 4 APs
* Associated stations

Links:

2 links per AP

Experiments:

500 runs

Iterations:

1000

Propagation:

IEEE 802.11ax path loss model

---

# Results

DFRL outperformed:

* Fixed activation
* Random activation
* Independent RL

Performance gains:

Approximately:

28–35%

over standalone RL

More than:

50%

over random and fixed strategies.

---

# Computational Characteristics

Advantages:

* Very low communication overhead
* Only scalar rewards exchanged
* No parameter synchronization
* Real-time operation

---

# Replication Guide

Software:

* Python

Libraries:

* NumPy
* RL framework

Implementation:

1. Create AP agents
2. Implement MAB learners
3. Implement reward sharing
4. Compute MAR reward
5. Update policies
6. Compare against baselines

---

# 5. Paper 3

# Reinforcement Learning for Channel Allocation: Digital Twin-Based Optimization in Real Large-Scale Wi-Fi Networks

Authors:

* José Pulido
* Sergio Fortes
* Alfonso Fernández-Durán
* Raquel Barco

System Name:

MELODWIN

---

# Motivation

Wi-Fi networks contain limited channels.

Examples:

* Channel 1
* Channel 6
* Channel 11

Improper allocation causes:

* Interference
* Contention
* Throughput reduction

Traditional optimization methods struggle in large-scale deployments.

---

# Channel Allocation Problem

Question:

> Which channel should each AP use?

Objective:

* Maximize throughput
* Minimize interference
* Reduce congestion

---

# Digital Twin Concept

A Digital Twin is a virtual representation of the real network.

Architecture:

Real Network

↓

Telemetry

↓

Digital Twin

↓

RL Agent

↓

Optimized Configuration

↓

Deployment

---

# Advantages of Digital Twins

* Safe experimentation
* Continuous monitoring
* Real-time optimization
* No service disruption

---

# MELODWIN Architecture

Components:

1. Real Wi-Fi Network
2. Monitoring System
3. Digital Twin
4. RL Optimization Engine
5. Configuration Deployment Module

---

# Key Innovation

Two optimization modes.

Selection depends on:

* Data freshness
* Data quality

System remains robust even when monitoring information is incomplete.

---

# Reinforcement Learning Integration

RL interacts with:

* Network state
* Traffic measurements
* Interference statistics

Outputs:

Channel allocation decisions.

---

# Real Deployment Validation

One of the strongest contributions.

Evaluation performed on:

92-node operational Wi-Fi network.

Not only simulations.

---

# Results

MELODWIN improved:

* Throughput
* Network efficiency
* Resource utilization

Compared to existing optimization methods.

---

# Replication Guide

Requirements:

## Data Collection

Need:

* RSSI
* Throughput
* Channel occupancy
* Interference measurements

---

## Software

* RL framework
* Digital Twin platform
* Telemetry collection system

---

## Infrastructure

Preferably:

50–100 AP deployment

for realistic validation.

---

# 6. Comparison of the Three Papers

| Feature         | Paper 1          | Paper 2               | Paper 3            |
| --------------- | ---------------- | --------------------- | ------------------ |
| Objective       | Resource Slicing | Multi-AP Coordination | Channel Allocation |
| Learning        | RL + LLM         | Federated RL          | RL                 |
| Simulator       | ns-3             | Custom Simulation     | Digital Twin       |
| Scale           | Single AP        | Multiple APs          | Large Network      |
| Novelty         | SAC-RE           | Reward Federation     | DT + RL            |
| Real Deployment | No               | No                    | Yes                |

---

# 7. Recommended Study Path

Step 1

Study MELODWIN

Learn:

* Channel allocation
* Digital twins
* RL control loops

---

Step 2

Study Wi-Fi Slicing

Learn:

* OFDMA
* Resource Units
* PPO
* A2C
* REINFORCE
* Constrained RL

---

Step 3

Study Federated RL

Learn:

* Multi-agent RL
* Federated learning
* Distributed optimization
* Wi-Fi 7 MLO

---

# 8. Resources Required for Full Replication

Core Software:

* Python
* PyTorch
* NumPy
* Gymnasium
* Stable-Baselines3
* ns-3
* ns3-ai

---

Networking Knowledge:

* IEEE 802.11ax
* IEEE 802.11be
* OFDMA
* MLO
* Beamforming
* Channel Allocation

---

Machine Learning Knowledge:

* Reinforcement Learning
* PPO
* A2C
* REINFORCE
* Multi-Armed Bandits
* Federated Learning
* Constrained Optimization
* Lagrangian Methods

---

# 9. Future Research Directions

Potential thesis/project extensions:

1. Replace MAB with PPO in Federated RL.
2. Combine MELODWIN and DFRL.
3. Implement Federated PPO across APs.
4. Extend Wi-Fi slicing to Wi-Fi 7 MLO.
5. Replace LLM scheduler with local lightweight LLM.
6. Integrate Digital Twins with Federated Learning.
7. Add security mechanisms against model poisoning.
8. Evaluate on real enterprise Wi-Fi networks.

---

# Final Takeaway

These three papers collectively represent the evolution of intelligent Wi-Fi management:

1. MELODWIN optimizes channel allocation using RL and Digital Twins.
2. Wi-Fi Slicing optimizes OFDMA resource allocation using RL, heuristics, and Generative AI.
3. Federated RL extends optimization to decentralized Wi-Fi 7 environments where multiple APs learn cooperatively.

Together they provide a complete roadmap from single-AP optimization to large-scale intelligent Wi-Fi network management and form an excellent foundation for future research in AI-driven wireless networking.
