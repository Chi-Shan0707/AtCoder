# AHC037 - Soda

[Contest Link](https://atcoder.jp/contests/ahc037) | 11th Asprova Programming Contest | 2024-09-15

## Problem

Beverage factory. Start with (0,0). Given N=1000 targets (A_i, B_i), produce them all
by repeatedly picking an existing beverage (x,y) and creating (x',y') where x'>=x, y'>=y,
at cost (x'-x)+(y'-y). Minimize total cost. Up to 5N operations allowed.

**Score**: `round(10^6 * N*L / (1 + C))` where L = max coordinate, C = total cost.

## Result

| Metric | Value |
|--------|-------|
| Rank | 588 / 1135 |
| Score | 3,443,723,458 |
| Time | 2:46 |

## Score History

| Version | Approach | Local Cost | Local Score |
|---------|----------|------------|-------------|
| v0 | Naive greedy from (0,0), bugs | 997,273,043,141 | 1,002,734 |
| v1 | Fix 3 bugs + randomized onion-peeling (20 trials) | 43,659,090,095 | 22,904,738 |

## Approach

Onion-peeling (剥洋葱): sweep from (0,0) outward in layers defined by sorted unique x/y values.
At each layer, create an intermediate soda at the corner, then blend all reachable orders.

Randomization: instead of always advancing both x and y layers together, randomly choose
to advance x only / y only / both (45/28/27 split). Run 20 trials, pick lowest cost.

### Bugs Fixed (v0 → v1)

1. Intermediate sodas never added back to pool (line 84 commented out) — every blend was from (0,0)
2. Y-sweep condition compared x-coordinate against y-value (`orders_by_y[k].x == y_val[...]`)
3. Dead loop variables `i`, `j`
