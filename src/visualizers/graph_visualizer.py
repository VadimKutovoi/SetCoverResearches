import matplotlib.pyplot as plt
import networkx as nx

fh=open("adj_list.txt", 'rb')
G=nx.read_adjlist("adj_list.txt", create_using=nx.Graph(), nodetype=int)
nx.draw(G)
#plt.show()
plt.savefig("graph.png")
