# Help Me Win Mode - Implementation Documentation

## Feature Overview
The "Help Me Win" mode adds a visual helper that shows the shortest path from the snake's head to the next apple after each apple is eaten. This helps players navigate more efficiently while learning the game.

## Subtask Breakdown

### 1. macOS Build Compatibility
**Problem**: The original code used `#include <bits/stdc++.h>` which is not available on macOS/Apple Clang.
**Solution**: Replaced with specific standard library includes:
- `input.cpp`: Added `<iostream>` for cout
- `main.cpp`: Removed the non-portable header
**Verification**: Successfully compiled with `g++` on macOS with only minor deprecation warnings.

### 2. Configuration Setup
**Problem**: Need configurable parameters for the helper mode.
**Solution**: Added to `macros.h`:
```cpp
#define HELP_PATH_CHAR '+'        // Character to display the path
#define HELP_MAX_N 5             // Show helper for first N apples
```
**Verification**: Constants are properly defined and used throughout the code.

### 3. SnakeMap Interface Extension
**Problem**: Need to add pathfinding capabilities to the map class.
**Solution**: Extended `SnakeMap` class with:
- `std::vector<std::pair<int,int>> helper_path` - stores the computed path
- `compute_helper_path()` - calculates the path using BFS
- `bfs_find_path()` - implements the BFS algorithm
**Verification**: Interface compiles and integrates with existing code.

### 4. BFS Algorithm Implementation
**Problem**: Need to find the shortest path from snake head to apple while avoiding snake body.
**Solution**: Implemented BFS (Breadth-First Search) algorithm:
- Blocks all snake body cells as obstacles
- Allows snake head as starting point
- Uses 4-directional movement (up, down, left, right)
- Returns shortest path if one exists
**Verification**: Algorithm correctly finds paths and handles edge cases.

### 5. Integration with Game Loop
**Problem**: Need to trigger path computation and display at the right time.
**Solution**: 
- Call `compute_helper_path()` after spawning new apple in `update_snake_food()`
- Draw helper path in `redraw()` after drawing snake and food
- Only draw on empty map cells to avoid overwriting game elements
**Verification**: Path appears immediately after apple spawn and updates correctly.

### 6. Compiler Warning Cleanup
**Problem**: Switch statements missing `Error` case causing compiler warnings.
**Solution**: Added `case Error: break;` to all Direction enum switch statements.
**Verification**: No compiler warnings (except acceptable deprecation warning).

## Algorithmic Problem Solving

### Why BFS?
**Considered alternatives**:
1. **A* (A-star)**: More complex, requires heuristic function
2. **Dijkstra**: Overkill for unweighted grid
3. **Simple pathfinding**: Not guaranteed to find shortest path

**Chosen BFS because**:
- Guarantees shortest path in unweighted graphs
- Simple to implement and understand
- Optimal time complexity O(V+E) for this problem
- Handles obstacles (snake body) naturally

### BFS Implementation Details
```cpp
bool SnakeMap::bfs_find_path(std::pair<int,int> start,
                             std::pair<int,int> goal,
                             std::vector<std::pair<int,int>>& outPath)
```
- **Input**: Start position (snake head), goal position (apple), output path vector
- **Process**: 
  1. Mark snake body cells as blocked
  2. Use queue for BFS traversal
  3. Track parent relationships for path reconstruction
  4. Return path from start to goal
- **Output**: True if path found, false otherwise

### Runtime Complexity
- **Time**: O(V+E) where V = MAP_HEIGHT × MAP_WIDTH, E = 4V (4 neighbors per cell)
- **Space**: O(V) for visited array, parent map, and queue
- **Optimal**: BFS is optimal for unweighted shortest path problems

## Chosen Value of n

**n = 5** was chosen for the following reasons:
1. **Learning curve**: First 5 apples provide enough guidance for new players
2. **Game balance**: Prevents over-reliance on hints in later, more challenging levels
3. **Performance**: Keeps pathfinding overhead minimal
4. **User experience**: Encourages skill development after initial assistance

## Future Improvements (If More Time Available)

### 1. Dynamic Toggle Feature
- Add keyboard shortcut (e.g., 'H' key) to toggle help mode on/off
- Store toggle state in SnakeMap class
- Update UI to show current help mode status

### 2. Higher Level Support
- Implement adaptive n value based on game difficulty
- Use more sophisticated pathfinding for complex scenarios
- Add path prediction for multiple moves ahead

### 3. Enhanced Visual Feedback
- Different colors for different path types
- Animated path drawing
- Path confidence indicators

### 4. Advanced Pathfinding
- Consider snake growth when planning paths
- Implement path smoothing algorithms
- Add multiple path options for user choice

### 5. Performance Optimizations
- Cache pathfinding results for similar board states
- Use incremental pathfinding updates
- Implement pathfinding in separate thread

## Testing Strategy

### Unit Testing
- Test BFS algorithm with various obstacle patterns
- Verify path correctness with known test cases
- Test edge cases (no path available, start=goal)

### Integration Testing
- Verify path appears after apple consumption
- Test path updates correctly with snake movement
- Confirm path disappears after n apples

### User Experience Testing
- Verify timing doesn't disrupt gameplay
- Test visual clarity of path display
- Confirm help mode doesn't make game too easy

## Code Quality

### Documentation
- Clear function and class documentation
- Inline comments explaining complex logic
- README-style documentation for setup

### Maintainability
- Modular design with clear separation of concerns
- Consistent naming conventions
- Minimal changes to existing codebase

### Performance
- Efficient algorithm choice (BFS)
- Minimal memory allocation
- No unnecessary computations in game loop
