import random
import sys

# Get the number of nodes from command line argument
if len(sys.argv) != 2:
    print("Usage: python generate_input.py <number_of_nodes>")
    sys.exit(1)
num_nodes = int(sys.argv[1])

# Generate random edges with weights between 1 and 10
edges = []
for i in range(1, num_nodes):
    for j in range(num_nodes):
        if i != j and j!=0 and i!=num_nodes-1:
            wt = random.randint(1, 10)
            edges.append((i, j, wt))

# Make sure there is a path from source node 0 to sink node num_nodes-1
visited = [False] * num_nodes
visited[0] = True
visited[num_nodes-1] = True
while not visited[num_nodes-1]:
    i = random.randint(0, num_nodes-1)
    j = random.randint(0, num_nodes-1)
    if i != j and i != 0:
        edges.append((i, j, random.randint(1, 10)))
        visited[i] = True
        visited[j] = True

# Add outgoing edges from the source vertex and incoming edges to the sink vertex
for i in range(1, num_nodes):
    edges.append((0, i, random.randint(1, 10)))
for i in range(num_nodes-1):
    edges.append((i, num_nodes-1, random.randint(1, 10)))

# Write the input to file
with open("input.txt", "w") as f:
    f.write(f"{num_nodes}\n")
    for (i, j, wt) in edges:
        f.write(f"{i} {j} {wt}\n")
