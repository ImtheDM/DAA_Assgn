import networkx as nx
import matplotlib.pyplot as plt

# Read data from file
with open('ip-gph.txt', 'r') as f:
    data = f.readlines()

# Create an empty bipartite graph
G = nx.Graph()

# Create sets of nodes
set_A = set()
set_B = set()
for line in data:
    node1, node2 = map(int, line.split())
    set_A.add(node1)
    set_B.add(node2)

# Add nodes to the graph
G.add_nodes_from(set_A, bipartite=0)
G.add_nodes_from(set_B, bipartite=1)

# Add edges to the graph
for line in data:
    node1, node2 = map(int, line.split())
    G.add_edge(node1, node2)

with open('output3.txt', 'r') as f:
    highlight = [tuple(map(int, line.strip().split())) for line in f]
edge_colors = ['red' if edge in highlight else 'black' for edge in G.edges()]

# Draw the graph
pos = {node: (0, i) for i, node in enumerate(set_A)}
pos.update({node: (1, i) for i, node in enumerate(set_B)})
nx.draw(G, pos=pos, with_labels=True,edge_color=edge_colors)

# Show the graph
plt.show()

