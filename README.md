# Introduction-to-Artificial-Intelligence
# Advanced Puzzle Solver: Project 1

## Overview
The project was designed to devise an efficient method for solving the Eight Piece Puzzles, and its capability was further extended to tackle even larger puzzles. Through the course of this project, three algorithms were extensively tested and benchmarked:

### 1. Uniform Cost Search
- Expands nodes based on the lowest associated cost.
- When costs are uniform, its behavior resembles that of a breadth-first search.
- Requires significant memory resources, especially as puzzle complexity escalates.

### 2. A* with Misplaced Tile Heuristic
- Determines the cost of a solution considering both the depth and the number of misplaced tiles present in the current puzzle.
- Uses tree data structures to effectively track nodes, which boosts its overall efficiency.

### 3. A* with the Euclidean Distance Heuristic
- Uses the Euclidean distance to compute the cost of transitioning to the goal state.
- Showcased a pronounced reduction in search cost, making it among the most optimized techniques employed in this project.

## Key Findings
Upon rigorous testing, it was deduced:
- Both variations of the A* algorithm significantly outpaced the Uniform Cost Search in efficiency and were less demanding in memory usage, especially as puzzle intricacy grew.
- The success of the A* algorithms was attributed to their dual consideration of both the transition cost to the goal and the cumulative cost from the starting node.

## Conclusion
This project underscored the efficacy of A* algorithms in cracking intricate puzzles. In contrast, while the Uniform Cost Search might be suboptimal for this particular application, its potential in other domains cannot be discounted.
