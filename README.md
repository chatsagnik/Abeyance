# Abeyance
Finds the minimal average latency from a reservation table for a non-linear pipeline.

## Algorithm
- Find non-repeating Forbidden Latencies from reservation table. Let the number be n.
- Sort this list in ascending order and form the collision vector.
- Start constructing the state diagram from the initial collision vector.
- Convert the state diagram into a directed graph.
- Find all elementary cycles in the aforementioned directed graph.
- Find the average latency for each such mentioned elementary cycle.
- Select all elementary cycles with average latency < n.
- Select the smallest such latency.
