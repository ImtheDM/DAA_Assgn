import networkx as nx
import matplotlib.pyplot as plt

# create a graph
G = nx.DiGraph()

# add nodes
G.add_node('source')
G.add_node('A')
G.add_node('B')
G.add_node('C')
G.add_node('sink')

# add edges
G.add_edge('source', 'A', capacity=3)
G.add_edge('source', 'B', capacity=2)
G.add_edge('A', 'B', capacity=1)
G.add_edge('A', 'C', capacity=3)
G.add_edge('B', 'C', capacity=1)
G.add_edge('B', 'sink', capacity=2)
G.add_edge('C', 'sink', capacity=3)

# print the graph
nx.draw(G, with_labels=True)
plt.show()
