# Floyd-Warshall Algorithm with Multithreading

## Project Overview

This project implements the Floyd-Warshall algorithm in both single-threaded and multi-threaded versions to compute the shortest paths between all pairs of vertices in a weighted graph. The multi-threaded implementation utilizes the readers-writers problem for synchronization.

## Group Members

- Varshitha Venkatesh []()
- Mohammad Zaid Khan [MAXIMUSSCORP](https://github.com/MAXIMUSSCORP)

## File Structure

- `SingleThUserInput.c`: Runs the algorithm with user input for vertices, edges, and weights.
- `SingleThRandomize.c`: Tests the algorithm with randomly generated graphs of sizes 10 to 1000 vertices.
- `MultiThUserInput.c`: Multi-threaded implementation with user input.
- `MultiThRandomize.c`: Multi-threaded tests with randomly generated graphs of sizes 10 to 125.

## Pseudocode

### Single Thread Version

1. Initialize variables for the number of vertices (V) and edges (E).
2. Depending on the input method:
   - **User input**: Prompt for V and E, initialize graph, read edges and weights.
   - **Randomization**: Calculate V and E, initialize graph, generate random edges and weights.
3. Execute Floyd-Warshall algorithm, update shortest path matrix, and print results.
4. Measure execution time and clean up resources.

### Multi-Threaded Version

1. Setup thread management and semaphores for synchronization.
2. Each thread updates a specific part of the matrix ensuring data consistency.
3. Optional reader threads may manage read operations.
4. Print the shortest distances and measure execution time.

## Performance

- **Single Thread**: Varies with graph size, generally faster on smaller graphs.
- **Multi Thread**: Shows slower performance due to sequential updates enforced by synchronization.

## Graphs

- Input Sizes vs. Time-taken curves for Single and Multi-Thread implementations.

     <img width="555" alt="Screenshot 2024-04-16 at 6 23 09 PM" src="https://github.com/MAXIMUSSCORP/Multithreaded-Floyd-Warshall-Algorithm/assets/86724477/07092ee8-2019-4083-b351-e9fad80b4fdd">


## References

- Implemented using provided Floyd-Warshall and readers-writers problem code.
- Debugged with tools like valgrind and gdb.
