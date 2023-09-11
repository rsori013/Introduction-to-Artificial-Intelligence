# Introduction-to-Artificial-Intelligence
# Project 1: Advanced Puzzle Solver

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

# Project 2: Feature Selection with Nearest Neighbor

## Overview
This project delves into the realm of feature selection for datasets, emphasizing the importance of choosing relevant features to improve classification accuracy. Three distinct algorithms are employed: Forward Selection, Backward Elimination, and Custom Random Feature Combinations.

## Highlights

- **Feature Selection Algorithms**: We implemented the Forward Selection and Backward Elimination methods to iteratively select optimal feature sets. The aim was to determine the feature combinations that offer the highest classification accuracy.

- **Custom Algorithm**: The project introduces a unique algorithm, "Random Feature Combinations," which generates random feature sets and employs Leave One Out Cross Validation (LOOCV) to evaluate their efficacy.

- **Normalization Impact**: An investigation was conducted on the effect of data normalization on the performance of the aforementioned algorithms. This involved comparing results obtained using normalized data versus unnormalized data.

- **Challenges**: Complexity in algorithm implementation, nuances of data preprocessing, and the potential pitfalls of overfitting were among the challenges faced. These served as significant learning points, emphasizing the balance required in feature selection and model validation.

In conclusion, the project underscores the importance of feature selection in improving model performance and provides tools and techniques to navigate the complex landscape of high-dimensional data.
