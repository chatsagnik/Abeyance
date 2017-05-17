# Abeyance
Finds the minimal average latency from a reservation table for a non-linear pipeline.

## Algorithm
1. Find non-repeating Forbidden Latencies from reservation table. Let the number be n.
2. Sort this list in ascending order and form the collision vector.
3. Start constructing the state diagram from the initial collision vector.
4. Convert the state diagram into a directed graph.
5. Find all elementary cycles in the aforementioned directed graph.
6. Find the average latency for each such mentioned elementary cycle.
7. Select all elementary cycles with average latency < n.
8. Select the smallest such latency.
