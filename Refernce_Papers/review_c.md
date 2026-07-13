# Comprehensive Overview of Three Wi-Fi ML/RL Research Papers

> **Papers Covered:**
> 1. **MELODWIN** — RL + Digital Twin for Channel Allocation in Large-Scale Wi-Fi Networks
> 2. **Wi-Fi Slicing Comparison** — RL vs. Rule-Based vs. Generative AI for Resource Slicing
> 3. **Beyond Wi-Fi 7** — Federated Reinforcement Learning for Decentralized WLANs

---

## Table of Contents

1. [Paper 1 — MELODWIN](#paper-1--melodwin)
   - [Core Problem](#11-core-problem)
   - [Key Innovation](#12-key-innovation)
   - [System Architecture](#13-system-architecture)
   - [RL Algorithm](#14-rl-algorithm-q-learning-with-epsilon-greedy-policy)
   - [Reward Function](#15-reward-function--four-components)
   - [Three Optimization Approaches](#16-three-optimization-approaches)
   - [Simulation Setup](#17-simulation-setup--resources)
   - [Real-World Validation](#18-real-world-validation)
2. [Paper 2 — Wi-Fi Slicing Comparison](#paper-2--wi-fi-slicing-comparison)
   - [Core Problem](#21-core-problem)
   - [Problem Formulation](#22-problem-formulation)
   - [Approaches Evaluated](#23-approaches-evaluated-seven-total)
   - [Simulation Infrastructure](#24-simulation-infrastructure)
   - [Results & Pareto Analysis](#25-results--pareto-analysis)
3. [Paper 3 — Beyond Wi-Fi 7 (FRL)](#paper-3--beyond-wi-fi-7-federated-reinforcement-learning)
   - [Core Problem](#31-core-problem)
   - [DFRL Framework](#32-proposed-framework-distributed-federated-reinforcement-learning-dfrl)
   - [Survey of ML in Wi-Fi](#33-survey-component-ml-applications-in-wi-fi)
   - [System Model](#34-system-model-for-mlo-case-study)
   - [Simulation Setup](#35-simulation-setup--resources)
   - [Results](#36-results)
4. [Cross-Paper Synthesis](#cross-paper-synthesis)

---

---

# Paper 1 — MELODWIN

> **Full Title:** Reinforcement Learning for Channel Allocation: Digital Twin-Based Optimization in Real Large-Scale Wi-Fi Networks
>
> **Authors:** José Pulido, Sergio Fortes, Alfonso Fernández-Durán, Raquel Barco
>
> **Institutions:** Universidad de Málaga & Nokia Bell Labs, Stuttgart, Germany

---

## 1.1 Core Problem

Wi-Fi networks operating in the **2.4 GHz band** face severe **Channel Allocation (CA)** challenges in dense deployments:

- The 2.4 GHz band is critically congested due to overlapping networks
- Only **channels 1, 6, and 11** are truly non-overlapping in the 2.4 GHz spectrum
- Classical optimization and even standard ML approaches **fail when real-world data is outdated, incomplete, or irregularly reported** — a very common scenario in live operational networks
- Uncontrolled external APs ("aliens") introduce interference that cannot be directly managed

> **Key Observation:** In live networks, reports from APs are often not obtained regularly or in good quality. The system must be robust to *stale and incomplete data*, not just to dynamic network conditions.

---

## 1.2 Key Innovation

The authors propose **MELODWIN**, a system combining:

- **Q-Learning (Reinforcement Learning)** for decision-making
- A **Network Digital Twin (NDT)** as a safe simulation environment (no degradation to live network during exploration)
- A **dual strategy** that adapts based on the **age and freshness of incoming data**

---

## 1.3 System Architecture

The system has **two main layers:**

```
Real Network (APs + STAs)
        |
        | (Network Data — reports, metrics)
        v
+---------------------------+
|     Twin Decision Module  |  <-- Analyzes data age, configures RL
+---------------------------+
        |                |
        |                |
        v                v
+----------------+  +-------------------+
| Alien-Free DT  |  |   DT Instances    |
| (Fresh data)   |  | (Stale data: N=25)|
+----------------+  +-------------------+
        |                |
        +--------+-------+
                 |
                 v
         RL (Q-Learning)
         Rewards & Actions
                 |
                 v
     Channel Allocation → Real Network
```

### Layer 1 — Twin Decision Module

Receives real-time performance reports and:
- Estimates **data freshness (age)** of incoming reports
- Decides which **DT strategy** to invoke (Alien-Free vs. DT Instances)
- Configures the **RL algorithm** with appropriate reward weights and action space

### Layer 2 — Network Digital Twin (NDT): Two Modes

#### Mode A — Alien-Free DT *(used when data is recent)*

- Single up-to-date virtual replica of the network
- Incorporates an **alien pollution mask** — a channel-level RSSI map showing interference from external uncontrolled APs
- Does **not** need to model alien AP positions explicitly — pollution footprint is directly measured
- Simulates only manageable APs; alien effects are captured via mask reward

**Example Alien Pollution Mask:**

| AP ID | Ch 1 | Ch 6 | Ch 11 | ... |
|-------|------|------|-------|-----|
| AP1   | -52  | -89  | -48   | ... |
| AP2   | -44  | -55  | -50   | ... |
| AP3   | -64  | -87  | -57   | ... |

> Values are RSSI (dBm). Highlighted channels indicate problematic interference levels.

#### Mode B — DT Instances *(used when data is outdated)*

- Creates **N = 25 parallel DT simulations** with different assumptions about alien and STA positions
- Runs separate RL optimizations across all 25 instances
- Records the most **frequently selected channel** per AP across all simulations
- Applies a **minimum distance method** to select the single DT simulation whose channel assignments most closely match the majority votes
- Channels selected by APs with >33% frequency are considered high-confidence choices (since 3 channels are used: 1, 6, 11)

> **Why 25 DTs?** Because when data is stale, the network may have changed significantly. Multiple simulations with varied assumptions allow robust, uncertainty-aware channel selection.

---

## 1.4 RL Algorithm: Q-Learning with Epsilon-Greedy Policy

### State & Action Spaces

$$s_t = \{AP_1, AP_2, ..., AP_N\} \quad \text{(cluster conditions)}$$

$$a_t = \{channel_1, channel_2, ..., channel_{11}\}$$

### Epsilon-Greedy Action Selection

$$a_t = \begin{cases} \arg\max_{a_t} Q_t(s_t, a_t) & \text{with probability } 1 - \varepsilon_t \\ \text{random action} & \text{with probability } \varepsilon_t \end{cases}$$

**Epsilon Decay:**
- Initial ε = 1 (full exploration)
- Decay rate = 0.98 per iteration
- Minimum ε = 0.01

### Q-Table Update Rule

$$Q(s,a) = (1-\alpha) \cdot Q(s,a) + \alpha \cdot \left[r(s_t, a_t) + \gamma \cdot \max_a Q(s', a)\right]$$

| Hyperparameter | Value | Method |
|---|---|---|
| Learning rate α | 0.1 | Empirically tuned |
| Discount factor γ | 0.9 | Empirically tuned |
| Initial Q-values | −1 | Pessimistic initialization |

### Convergence Criterion

The algorithm converges when the **last 5 consecutive actions AND rewards are identical:**

$$a_{t-4} = a_{t-3} = a_{t-2} = a_{t-1} = a_t$$
$$r_{t-4} = r_{t-3} = r_{t-2} = r_{t-1} = r_t$$

---

## 1.5 Reward Function — Four Components

The total reward is a **normalized weighted sum of four sub-rewards:**

$$r(s_t, a_t) = w_1 \cdot \text{global}_{reward} + w_2 \cdot \text{indiv}_{reward} + w_3 \cdot \text{aliens}_{reward} + w_4$$

with constraint: $w_1 + w_2 + w_3 + w_4 = 1$

### Component 1 — Global Reward

Measures **overall network throughput and airtime:**

$$\overline{Th}_{tot} = \frac{Th_{AP_{mean}} - Th_{min}}{Th_{max} - Th_{min}}, \quad 0 < \overline{Th}_{tot} < 1$$

$$\overline{Air}_{tot} = \frac{Air_{AP_{mean}} - Air_{min}}{Air_{max} - Air_{min}}, \quad 0 < \overline{Air}_{tot} < 1$$

$$\text{global}_{reward} = v_{1_{tot}} \cdot \overline{Th}_{tot} - v_{2_{tot}} \cdot \overline{Air}_{tot}$$

> Maximizes average throughput while penalizing high airtime usage (a congestion indicator)

### Component 2 — Individual Reward

Targets **outlier APs** that fall below acceptable performance thresholds:

$$\overline{Th}_{ind} = \frac{NTh_{out} - Th_{ind_{min}}}{Th_{ind_{max}} - Th_{ind_{min}}}$$

$$\overline{Air}_{ind} = \frac{NAir_{out} - Air_{ind_{min}}}{Air_{ind_{max}} - Air_{ind_{min}}}$$

$$\text{indiv}_{reward} = v_{1_{ind}} \cdot \overline{Th}_{ind} - v_{2_{ind}} \cdot \overline{Air}_{ind}$$

> $NTh_{out}$ = number of APs below throughput threshold; $NAir_{out}$ = number of APs above airtime threshold

### Component 3 — Aliens Reward

Penalizes channels where **alien interference dominates:**

$$\bar{A} = \frac{(RSSI_{AP} - RSSI_{alien}) - RSSI_{min}}{RSSI_{max} - RSSI_{min}}$$

$$\bar{A} = \begin{cases} 1 & \text{if } (RSSI_{AP} - RSSI_{alien}) > 10\text{ dB} \\ \bar{A} & \text{if } (RSSI_{AP} - RSSI_{alien}) \leq 10\text{ dB} \end{cases}$$

> Reward = 1 (maximum) when own signal exceeds alien interference by more than 10 dB

### Component 4 — Key Channel Bonus (w₄)

Adds a priority bonus for **non-overlapping channels 1, 6, and 11** — the only channels in the 2.4 GHz band with zero frequency overlap.

---

## 1.6 Three Optimization Approaches

The weighting of $w_4$ defines three distinct CA strategies:

| Approach | w₁ | w₂ | w₃ | w₄ | Description |
|---|---|---|---|---|---|
| **1,6,11 only** | 0.25 | 0.25 | 0.25 | 0.25 | Strict use of non-overlapping channels |
| **1,6,11 preferred** | 0.33 | 0.33 | 0.33 | 0.01 | Soft preference for channels 1, 6, 11 |
| **All channels 1–11** | 0.33 | 0.33 | 0.33 | 0.33 | Unrestricted channel use |

> The **1,6,11 only** approach is the most conservative and was selected for real-network deployment as it gives maximum operator confidence.

---

## 1.7 Simulation Setup & Resources

### Configuration Parameters

| Parameter | Value |
|---|---|
| Standard | IEEE 802.11ax (Wi-Fi 6) |
| Band | 2.4 GHz |
| Transmit Power | 20 dBm |
| Simulation Durations | 48h (comparison), 128h (full evaluation) |
| Optimization Trigger | Daily at 6:00 AM (off-peak) |
| Report Generation Interval | Every 5 minutes (event-driven, random AP) |
| Traffic Update Interval | Hourly |
| Traffic Profile | Residential IPTV |
| Synthetic Scenario | 12 APs + 12 alien APs + 3 STAs per AP |
| Outer Wall Length / Attenuation | 9 m / 10 dB |
| Inner Wall Length / Attenuation | 4 m / 5 dB |

### Baselines Compared Against

| Baseline | Description |
|---|---|
| **Expert Knowledge** | Human-configured manual CA |
| **Standalone Q-Learning** | No DT — actions applied directly to live network (causes degradation during exploration) |
| **Random Channel Selection** | Assigns channels randomly |
| **Unboxing** | All APs on the same channel (worst-case scenario) |

### Key Preliminary Results (48h simulation, no aliens)

- MELODWIN was above airtime threshold only **13%** of the time (next best: ~26%)
- MELODWIN was below 100 Mbps threshold only **35%** of the time (Expert Knowledge: 44%)
- Unboxing had worst performance as expected; standalone Q-learning degraded live network performance during exploration

---

## 1.8 Real-World Validation

### Network Description

- **92 APs** with average 3 STAs per AP
- **231 alien APs** (positions and configurations unknown)
- Network **already optimized by the operator** before MELODWIN was applied
- Data collected was **not fully up-to-date** → DT Instances strategy (25 DTs) selected

### Validation Process

```
Step 1: Select congested cluster for optimization
        ↓
Step 2: Observe & monitor for 1 week (pre-optimization data)
        ↓
Step 3: Assess data age → Select DT strategy (DT Instances chosen)
        ↓
Step 4: Run MELODWIN offline → Generate channel configuration list
        ↓
Step 5: Upload new configuration to network during off-peak hours
        ↓
Step 6: Monitor network for 1 week (post-optimization)
        ↓
Step 7: Restore original configuration → Monitor 1 more week (confirmation)
```

### Metrics Used

- **Noise Outliers:** APs exceeding µ + 3σ noise floor threshold at rush hour (lower = better)
- **Intensive Users:** APs exceeding µ + 2σ airtime threshold (higher = better, more users able to use resources intensively)

### Final Results

| Period | Avg. Noise Outliers | Intensive Users |
|---|---|---|
| Before PoC | 1.28 | 2.0 |
| **MELODWIN PoC** | **0.72** | **2.8** |
| After PoC (restored) | 0.88 | 1.7 |

- **Noise outliers reduced by 18%–44%** during MELODWIN week
- **Intensive user count improved by 40%–65%**
- Zero Wi-Fi-related calls to the Communications Service Provider (CSP) during the PoC week

---
---

# Paper 2 — Wi-Fi Slicing Comparison

> **Full Title:** Reinforcement Learning, Rule-Based, or Generative AI: A Comparison of Model-Free Wi-Fi Slicing Approaches
>
> **Authors:** Rafael Rosales, Dave Cavalcanti
>
> **Institution:** Intel Labs & Intel CCG, Munich / Hillsboro
>
> **Published:** Frontiers in Signal Processing, May 2025

---

## 2.1 Core Problem

While **5G cellular networks** natively support radio resource slicing via Physical Resource Blocks (PRBs) allocated in both time and frequency, Wi-Fi has **no equivalent native mechanism.**

Since **Wi-Fi 6 (802.11ax)**, OFDMA enables frequency-domain resource allocation:
- The AP can split a channel into **Resource Units (RUs)** within a Transmission Opportunity (TXOP)
- Example: A 20 MHz channel can be divided into 9 RUs of 2 MHz each
- Since **Wi-Fi 7 (802.11be)**, multiple RUs can be aggregated per user

**The Challenge:** Prior state-of-the-art RL approaches for resource allocation required **differentiable system models** (gradient of reward w.r.t. policy parameters computable analytically). This is **impossible in real Wi-Fi systems or ns-3 simulators.** This paper explicitly targets **model-free RL** — applicable to real deployments without requiring a differentiable model.

---

## 2.2 Problem Formulation

A **constrained optimization problem** is defined:

### Objective
Maximize total throughput (bytes received) subject to latency constraints:

$$\max_\pi \lim_{T \to \infty} \mathbb{E}_{s,a \sim \pi} \left[ \frac{1}{T} \sum_{t=0}^T r(s_t, a_t) \right]$$

### Constraints (per slice j)

$$\lim_{T \to \infty} \mathbb{E}_{s,a \sim \pi} \left[ \frac{1}{T} \sum_{t=0}^T e_j(s_t, a_t) \right] \leq c_j, \quad j = 1, 2, ..., J$$

### State, Action, Constraint Definitions

| Element | Definition |
|---|---|
| **State** $s \in \mathbb{N}^{H \times N}$ | Traffic demand (queued packets) per slice, over last H timesteps |
| **Action** $a \in \mathbb{R}^N$ | Proportion of total RUs for each slice; must sum to 1 |
| **Reward** $r(s_t, a_t)$ | Total bytes received (throughput) |
| **Constraint** $e_j(s_t, a_t)$ | Average packet latency per slice |
| **Penalty** | 100 added for each undelivered packet (prevents trivial zero-latency solutions) |

> The **system dynamics are treated as a black box** — no transition model is specified. Only solutions with dynamic adaptation can handle non-stationary environments.

---

## 2.3 Approaches Evaluated (Seven Total)

### A. Policy-Gradient RL Methods

#### 1. REINFORCE (RE)
*Algorithm: Williams (1992)*

Two phases per episode:
- **Sampling phase:** Run full episode with current policy π to collect (state, action, reward) tuples
- **Evaluation phase:** Update policy to increase probability of actions that led to high rewards; decrease probability of low-reward actions

**Update rule:**
$$\theta \leftarrow \theta + \alpha \gamma^t G \nabla \ln \pi(a_t | s_t, \theta)$$

where G = discounted return from step t onwards.

Uses **mean baseline** (subtracts episode mean reward) to reduce gradient variance.

---

#### 2. A2C — Synchronous Advantage Actor-Critic
*Based on: Mnih et al. (2016)*

- Adds a **critic network** (value estimator) as baseline for REINFORCE
- **Advantage** = actual reward − critic's predicted value
- More stable training than vanilla REINFORCE
- **Implementation:** Stable-Baselines3 Python library
- n_step hyperparameter matched to other RL algorithms; other hyperparameters: default

---

#### 3. PPO — Proximal Policy Optimization
*Algorithm: Schulman et al. (2017)*

- Uses a **clipped surrogate loss** within a trust region to prevent large destabilizing policy updates
- Clips the probability ratio to prevent overshooting:
$$L^{CLIP}(\theta) = \mathbb{E}\left[\min(r_t(\theta)\hat{A}_t, \text{clip}(r_t(\theta), 1-\varepsilon, 1+\varepsilon)\hat{A}_t)\right]$$
- **Implementation:** Stable-Baselines3 Python library
- Default hyperparameters (except n_step)

---

#### 4. SAC-RE — State-Augmented Constrained REINFORCE *(Novel Contribution)*

*Based on: Calvo-Fullana et al. (2023)*

This is the paper's **primary algorithmic contribution** — adapting state-augmented constrained RL to model-free policy gradient methods.

**Core Idea:** Feed Lagrangian multipliers λⱼ as **additional policy inputs:**

$$\pi_\theta(s_t, \lambda_{j,t}) \rightarrow a_t$$

**Augmented Reward** integrates all constraints into one objective:

$$r_\lambda(s_t, a_t) = r(s_t, a_t) + \sum_{j=1}^J \lambda_{j,t}\left(e_j(s_t, a_t) - c_j\right)$$

**Lagrangian Update at Inference** (every T₀ = 4 steps):

$$\lambda_{j,k+1} = \left[\lambda_{j,k} - \frac{\eta_\lambda}{T_0} \sum_{t=kT_0}^{(k+1)T_0 - 1} (e_j(s_t, a_t) - c_j)\right]_+$$

> λ increases when constraint is violated → policy is pushed to satisfy constraint
> λ decreases when constraint is satisfied → policy focuses back on throughput

**Why this matters:** The policy learns **different behaviors for different constraint states** — enabling multi-modal solutions without retraining. Standard RL methods (REINFORCE, A2C, PPO) use fixed-weight combinations of objectives, which cannot adapt dynamically.

**Policy Network Architecture:**

```
Input: [current state + H previous states]
         ↓
Linear Layer (input_dim × 128) + Leaky ReLU
         ↓
Linear Layer (128 × 128) + Leaky ReLU
         ↓
Linear Layer (128 × N_slices)
         ↓
Dirichlet Distribution (α parameters, constrained to [1, 10,000])
         ↓
Sample allocation vector a (sums to 1)
```

| Training Parameter | Value |
|---|---|
| Optimizer | Adam |
| Learning rate | 0.0001 |
| Total training steps | 2,400 |
| Lagrangian update interval T₀ | 4 steps |
| Hidden layer size | 128 × 128 |

> **Why Dirichlet?** Naturally constrains output to sum to 1; encourages exploration of diverse allocations; captures uncertainty better than a deterministic Softmax

**Source Code:** Publicly released under Apache 2.0 license

---

### B. Rule-Based Heuristics

#### 5. Uniform Heuristic

$$a[i] = \frac{1}{N} \quad \forall i$$

- Divides resources **equally** among all slices at every step
- Simple, no training required, surprisingly competitive
- Cannot adapt to asymmetric or bursty traffic

#### 6. Proportional Heuristic

$$a[i] = \frac{w_i}{\sum_{k=1}^N w_k}$$

where $w_i$ is the current traffic demand of slice $i$.

- Allocates resources in **proportion to demand**
- Reactive to current state but ignores constraint history
- No training required

---

### C. Generative AI Approach

#### 7. LLM-Based Policy (GPT-4o)

- Commercial off-the-shelf foundational LLM — **not fine-tuned** on any Wi-Fi data
- At each inference step, the **current system state is embedded in a structured prompt**
- Prompt instructs the LLM to reason step-by-step about traffic demand, history, and constraints
- Key prompt directives:
  - Prioritize slices with higher demand
  - **Protect time-critical Slice 2** (bias explicitly encoded)
  - Maintain allocation sum = 1
  - Respond with final allocation on last line inside square brackets

**LLM Reasoning Sample:**
> "Total demand: 5357 + 535 + 5 = 5897. Allocate base 0.3 to Slice 2 (time-critical). Remaining 0.7 distributed proportionally: Slice 1: (5357/5897) × 0.7 ≈ 0.636. Final: [0.636, 0.300, 0.064]"

**Inference Latency:**

| Policy | Mean Inference Time |
|---|---|
| Uniform | 0.038 ms |
| Proportional | 0.092 ms |
| SAC-RE / PPO / A2C | ~2–5 ms |
| REINFORCE | ~4.7 ms |
| **LLM (GPT-4o)** | **~4,501 ms (σ = 4,248 ms)** |

> **Critical Limitation:** LLM inference is ~4.5 seconds on average — far too slow for sub-second Wi-Fi resource allocation. Suitable only for timescales of seconds or longer in current form.

---

## 2.4 Simulation Infrastructure

### Two-Layer Simulation Architecture

```
Training Phase:
  Python Simple Simulator (fast, no protocol overhead, black-box)
        ↓
  Train RL policies (2,400 steps each)
        ↓
Evaluation Phase:
  ns-3 Wi-Fi 6 Simulator (full 802.11ax protocol stack)
    + Slice-Aware MAC Layer (C++)
    + ns3-ai Framework (shared memory C++ ↔ Python)
    + Gymnasium RL interface
```

**Synchronization:** Heartbeat every 100ms (simulated time). 10 steps = 1 second simulated.

**Hardware:** Intel Core i7-11850H @ 2.50 GHz + NVIDIA RTX A3000 GPU

**Topology:** 1 AP + 3 STAs (one slice per STA, downlink only)

**Network Config:** 80 MHz channel, 20 dBm TX power, IEEE 802.11 D channel model

### Training Times

| Policy | SAC-RE | RE | A2C | PPO | Uniform | Proportional | LLM |
|---|---|---|---|---|---|---|---|
| Training Time (hrs) | 1.18 | 1.28 | 0.433 | 0.449 | 0 | 0 | 0 |

### Traffic Scenarios

| Scenario | Description | Tests |
|---|---|---|
| **Periodic** | 1 constant low-throughput slice + 2 alternating high-throughput slices | Predictable pattern adaptation |
| **Random Walk** | All 3 flows evolve as random walk (±500 packets/step, bounded 0–4000) | Generalization to unpredictable traffic |

---

## 2.5 Results & Pareto Analysis

Results are visualized as **Pareto fronts** (reward vs. latency penalty — lower-right is ideal). A solution dominates another if it improves at least one objective without worsening any other.

### Pareto Rankings

| Scenario | 1st Pareto Front | 2nd Pareto Front | 3rd Pareto Front |
|---|---|---|---|
| **Periodic** | Uniform, Proportional, LLM | SAC-RE, RE | A2C, PPO |
| **Random Walk** | Uniform, LLM | SAC-RE | RE, Proportional |

### Key Findings

| Policy | Observation |
|---|---|
| **LLM (GPT-4o)** | Consistently on 1st Pareto front — remarkable given zero training. Behavior adjustable via prompt. Impractical for real-time due to latency. |
| **Uniform** | Surprisingly strong baseline — competitive in both scenarios |
| **SAC-RE** | Best RL approach; state augmentation clearly benefits harder random walk case |
| **Proportional** | Strong in periodic case; drops off Pareto front in random walk |
| **REINFORCE (RE)** | 2nd Pareto front in both scenarios |
| **A2C / PPO** | Failed to adapt effectively — require extensive hyperparameter tuning |

### Impact of State Augmentation

- In **random walk** (harder): SAC-RE dominates the entire 2nd Pareto front alone, while vanilla RE falls to the 3rd
- In **periodic** (easier): SAC-RE and RE co-located on 2nd front, but RE has higher latency penalty tradeoff
- **Conclusion:** State augmentation provides clear benefit in non-stationary, multi-modal environments

---
---

# Paper 3 — Beyond Wi-Fi 7: Federated Reinforcement Learning

> **Full Title:** Beyond Wi-Fi 7: Enhanced Decentralized Wireless Local Area Networks with Federated Reinforcement Learning
>
> **Authors:** Rashid Ali, Alaa Omran Almagrabi
>
> **Institutions:** University West, Sweden & King Abdulaziz University, Saudi Arabia
>
> **Published:** Computers, Materials & Continua, January 2026

---

## 3.1 Core Problem

Dense, decentralized Wi-Fi environments with **multiple BSSs** create fundamental challenges:

| Challenge | Description |
|---|---|
| **Interference** | Multiple APs cause co-channel interference; overlapping channels degrade performance |
| **Partial Observability** | Each AP only has local, limited observations of the network |
| **Selfish Learning** | Independent per-AP RL converges to locally optimal but globally suboptimal decisions |
| **Privacy** | Centralized ML requires shipping sensitive raw data to a central server |
| **Latency** | Centralized aggregation introduces delays incompatible with real-time adaptation |
| **MLO Complexity** | Wi-Fi 7's Multi-Link Operation allows devices to transmit on multiple frequency bands simultaneously — link activation decisions become combinatorially complex |

**Wi-Fi 7 (IEEE 802.11be)** key features relevant to this paper:
- **Multi-Link Operation (MLO):** Devices can establish multiple wireless links on different bands simultaneously
- **Multi-AP Coordination (mAPC):** Coordinated transmission among neighboring APs
- **320 MHz channels:** Much wider channels requiring more intelligent band management
- **Extended Reality (XR) support:** Requires ultra-low latency and high reliability

---

## 3.2 Proposed Framework: Distributed Federated Reinforcement Learning (DFRL)

### Architecture Overview

```
                AP_i (Agent i)
               /              \
     Local Env.           Local Env.
        |                      |
   [LLM: MAB]            [LLM: MAB]
  Local reward           Local reward
        |                      |
        +--------> [GLM] <-----+
                  (Global)
                  MAR function
                  |
             Global reward
                  |
        +----+----+
        |         |
   AP_a (Agent a)  AP_n (Agent n)
   [LLM]          [LLM]
```

### Local Learning Model (LLM)

Each AP runs a **Multi-Armed Bandit (MAB)** algorithm locally:
- Each "arm" = a possible link activation (LA) configuration
- $p = 2^k - 1$ possible actions for $k$ links (no-link option excluded)
- With $k=2$ bands (2.4 GHz + 5 GHz): $p = 3$ actions (link1 only, link2 only, both links)

**Local Reward:**
$$\Omega^o_{i,t} = R^o_{i,t} \quad \text{(instantaneous data rate)}$$

**Why MAB?** Lightweight, interpretable, no full RL episode needed — suitable for real-time decisions at APs with limited compute.

### Global Learning Model (GLM)

APs share **compact scalar reward values** with neighbors via periodic beacon-like messages (not full model parameters — minimal overhead).

**Minimum Achieved Reward (MAR) — Max-Min Fairness:**

$$\text{MAR}^o(i,t) = \arg\min_{\forall n' \subseteq n} \hat{\Omega}^o_{n',t}$$

Each AP's global reward = the **lowest reward** reported by any neighbor for that action:
- Incentivizes APs to **avoid actions that harm neighbors**
- Implements max-min fairness: improve the worst-off AP rather than just maximizing average

**Mean Global Reward (for exploitation):**

$$\hat{\rho}^o_{i,t} = \frac{1}{V'} \sum_{v=1}^{V'} \rho^o_i(V')$$

**Optimal action selection:**
$$o^* = \arg\max_{\forall o \in O} \hat{\rho}^o_{i,t}$$

### Exploration vs. Exploitation

**ε-greedy with slow decay:**

$$\varepsilon = \sqrt{\frac{1}{t}} \quad \text{after 250 steps}$$

- High ε early → explore many link configurations
- Low ε later → exploit known good configurations
- Smooth transition prevents premature convergence

### Communication Overhead

> Each AP only exchanges **compact scalar reward values** (not model weights or raw data) — minimal bandwidth impact. Suitable for latency-sensitive Wi-Fi environments.

---

## 3.3 Survey Component: ML Applications in Wi-Fi

The paper reviews **8 key Wi-Fi optimization areas** with ML:

| # | Area | ML Approaches |
|---|---|---|
| 1 | **Channel Access (CW tuning)** | Q-learning (collision prob. estimation), Deep RL, Federated Learning |
| 2 | **Link Adaptation (Rate Selection)** | Thompson Sampling, regression, supervised learning, SNR-based models |
| 3 | **Beamforming** | Historical SNR prediction, SVM, regression, exhaustive search replacement |
| 4 | **Multi-User Transmissions (MU-MIMO/OFDMA)** | DRL for scheduling, STA grouping, RU allocation optimization |
| 5 | **Channel Selection/Bonding** | Multi-Armed Bandits, DRL (PPO), predictive models, online learning |
| 6 | **Multi-Link Operation (MLO)** | Neural networks for CSI prediction, DRL for AP selection, NN for full-duplex grouping |
| 7 | **Spatial Reuse (SR)** | Q-learning, MAB, supervised learning (MLP, decision trees) |
| 8 | **Multi-BSS Coordination** | RL for roaming prediction, channel interference prediction, AP placement optimization |

### Key Challenges Identified in Decentralized ML for Wi-Fi

1. **Interference:** Dense APs cause co-channel interference that degrades individual ML models
2. **Latency:** Wide geographic distribution introduces delays in data aggregation
3. **Security & Privacy:** Exchanging model parameters exposes inference leakage and model poisoning risks
   - Mitigations: robust reward aggregation (median/trimmed minima), authentication, quantization/noise addition
4. **Network Congestion:** More devices → slower speeds → worse training data quality
5. **Resource Allocation:** Bandwidth and power availability changes dynamically
6. **Multi-User Communication:** Realistic traffic patterns and QoS demands are hard to model
7. **Spatial Reuse:** Wi-Fi 7's TXOP sharing requires new cooperative ML approaches
8. **Decentralized MLO:** Optimal link-switching timing and traffic flow distribution across links remain open problems

---

## 3.4 System Model for MLO Case Study

### Network Topology

- $n$ AP-Multi-Link Devices (AP-MLDs): $A = \{A_1, A_2, ..., A_n\}$
- $m$ non-AP-MLDs (client devices): $S = \{S_1, S_2, ..., S_m\}$
- $k$ available links per AP-MLD: $L = \{L_1, L_2, ..., L_k\}$

**Total Bandwidth:**
$$B_j = \sum_{l=1}^k L_l \quad \text{(sum of activated links for AP-MLD } j\text{)}$$

### Path Loss Model (IEEE 802.11ax Residential)

$$PL(d) = 40.05 + 20\log_{10}\left(\frac{f_c}{2.4}\right) + 20\log_{10}(\min(d, d_{bp})) + (d > d_{bp}) \times 35\log_{10}\left(\frac{d}{d_{bp}}\right) + 18.3F\left(\frac{F+2}{F+1} - 0.46\right) + 5W$$

| Symbol | Meaning |
|---|---|
| $f_c$ | Carrier frequency (GHz) |
| $d_{bp}$ | Breakpoint distance (= 5m in simulation) |
| $F$ | Number of floors traversed |
| $W$ | Number of walls traversed |
| Shadowing | 5 dB standard deviation |

### Achievable Transmission Rate (Shannon Capacity)

$$R_{j,t} = B_{j,t} \times \log_2\left(1 + \frac{P_{j,t}}{I_{j,t} + N_j}\right)$$

| Symbol | Meaning |
|---|---|
| $B_{j,t}$ | Channel bandwidth of selected link(s) |
| $P_{j,t}$ | Received signal power |
| $I_{j,t}$ | Aggregate interference from other APs on same channel |
| $N_j$ | Noise power |

---

## 3.5 Simulation Setup & Resources

### Key Simulation Parameters

| Parameter | Value |
|---|---|
| AP-MLDs (n) | 4 |
| Non-AP MLDs (m) | 4 (1 per AP-MLD) |
| Grid size | 100 × 100 m |
| Channel bandwidth (per link) | 80 MHz |
| Transmit power | 20 dBm |
| RSSI threshold | −72 dBm |
| Data packet size | 12,000 bits |
| Available links (k) | 2 (2.4 GHz + 5 GHz bands) |
| Link activation actions (p) | 3 ($2^2 - 1$) |
| Simulation realizations | 500 (random AP positions) |
| Iterations per simulation | 1,000 steps |
| ε-decay start | After 250 steps |
| Breakpoint distance $d_{bp}$ | 10 m (evaluation) / 5 m (model) |
| Number of walls W | 3 |

### Baselines Compared

| Strategy | Description |
|---|---|
| **Fixed LA** | All APs always activate both links |
| **Random LA** | Each AP randomly activates 1 or 2 links each step |
| **RL-based (local MAB)** | Each AP greedily optimizes using only local reward — no coordination |
| **DFRL (proposed)** | Collaborative MAB with federated global reward via MAR function |

---

## 3.6 Results

### 90th Percentile Data Rates (500 random realizations)

| Strategy | 90th Percentile Data Rate |
|---|---|
| Fixed | 305.41 Mbps |
| Random | 416.05 Mbps |
| RL-based (local) | 467.66 Mbps |
| **DFRL (proposed)** | **631.73 Mbps** |

### Performance Improvements of DFRL

- **+28%–35% over local RL** (coordinated vs. selfish optimization)
- **>50% over Fixed and Random baselines**
- Higher and more **stable** data rates — lower variance across 500 random scenarios

### Correlation Heatmap Insight

| Strategy | Inter-AP Correlation | Interpretation |
|---|---|---|
| Fixed | High positive | APs heavily interfere — compete for same spectrum |
| Random | High positive | Interference not managed |
| RL-based | Reduced positive | Local decisions reduce some interference |
| **DFRL** | **Slightly negative** | **APs learn complementary behaviors — coordination achieved** |

> Negative correlations mean APs are **actively choosing different links**, minimizing shared interference — the hallmark of successful coordination.

---
---

# Cross-Paper Synthesis

## Comparison Table

| Dimension | MELODWIN (Paper 1) | Slicing Comparison (Paper 2) | FRL Beyond Wi-Fi 7 (Paper 3) |
|---|---|---|---|
| **Core Problem** | Channel allocation in 2.4 GHz dense networks | RU allocation (slicing) in Wi-Fi 6 OFDMA | Link activation in Wi-Fi 7 MLO (multi-band) |
| **RL Method** | Q-Learning (tabular) | Policy-gradient: REINFORCE, A2C, PPO, SAC-RE | Multi-Armed Bandit (MAB) |
| **Key Innovation** | Dual DT strategy based on data age | State-augmented constrained RL + LLM policy | Federated global reward with max-min fairness |
| **Non-RL Baselines** | Expert knowledge, random, unboxing | Uniform, Proportional, LLM (GPT-4o) | Fixed, Random, local RL |
| **Environment** | Real live network (92 APs) | ns-3 Wi-Fi 6 simulator | Monte Carlo simulation (500 runs) |
| **Scale** | 92 APs + 231 alien APs | 1 AP + 3 STAs | 4 AP-MLDs + 4 non-AP-MLDs |
| **Privacy Concerns** | Not addressed | Not addressed | Central concern — no raw data shared |
| **Data Staleness** | Explicitly handled (dual DT modes) | Not considered | Not considered |
| **Coordination** | Centralized (single cluster optimizer) | Single-node (one AP slicing) | Decentralized (peer-to-peer reward sharing) |
| **Standard Targeted** | IEEE 802.11ax (Wi-Fi 6) | IEEE 802.11ax (Wi-Fi 6) | IEEE 802.11be (Wi-Fi 7) and beyond |
| **Validation Level** | ✅ Real live operational network | ✅ High-fidelity ns-3 simulation | ✅ Monte Carlo statistical simulation |

---

## Common Themes Across All Three Papers

### 1. Classical Methods Are Insufficient
All three papers demonstrate that heuristic, rule-based, or manual approaches cannot cope with the dynamic complexity of modern dense Wi-Fi deployments.

### 2. RL Consistently Outperforms Baselines
Across all three papers, RL-based approaches outperform fixed or random strategies — but the right RL formulation matters significantly.

### 3. The Exploration-Exploitation Challenge
All three papers use epsilon-greedy or equivalent mechanisms to balance exploration (trying new configurations) vs. exploitation (using known good ones). The decay schedule critically impacts convergence.

### 4. Robustness to Imperfect Information
- Paper 1 addresses **data staleness** via dual DT modes
- Paper 2 addresses **non-stationary traffic** via state augmentation
- Paper 3 addresses **partial observability** via federated reward sharing

### 5. Simulated-to-Real Gap
- Paper 1 bridges this fully — validates on a live network
- Paper 2 uses ns-3 as a high-fidelity proxy
- Paper 3 uses statistical Monte Carlo — real-world validation is future work

---

## Design Axis Coverage

The three papers collectively cover the major design axes for next-generation Wi-Fi intelligence:

```
Centralized ←————————————————————————→ Decentralized
   Paper 1           Paper 2                Paper 3
(single cluster)  (single AP)        (multi-AP federation)

Single-Layer ←————————————————————————→ Multi-Layer
   Paper 2           Paper 1                Paper 3
  (slicing)      (DT + RL layers)     (LLM + GLM layers)

Known Environment ←————————————→ Unknown/Partial Environment
   Paper 2           Paper 3                Paper 1
  (ns-3 sim)    (random positions)    (live stale data)
```

---

## Future Directions Identified Across Papers

| Direction | Source |
|---|---|
| Advanced techniques for structuring network information at scale | Paper 1 |
| Improved methods for creating accurate digital representations of real networks | Paper 1 |
| Strategies for generating training data in data-scarce environments | Paper 1 |
| LLM fine-tuning / knowledge distillation for low-latency Wi-Fi inference | Paper 2 |
| Extending SAC-RE to larger multi-AP networks | Paper 2 |
| Formal convergence and stability proofs for DFRL | Paper 3 |
| Comparing DFRL vs. centralized RL, gossip learning, actor-critic frameworks | Paper 3 |
| Security mechanisms for federated reward exchange | Paper 3 |
| Extending to Wi-Fi 7 with 320 MHz channels and >2 links per AP | Paper 3 |

---

*Document compiled from three research papers on ML/RL optimization for Wi-Fi networks (2025–2026).*