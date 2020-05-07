import matplotlib.pyplot as plt
import networkx as nx

with open("adj_list.txt", 'r') as fh:
    lines = fh.readlines()
    G = nx.parse_adjlist(lines)
    pos = nx.circular_layout(G)
    nx.draw(G, pos=pos, node_color='#f6f578', with_labels=True, alpha=0.2)
    with open ("cover_list_twoapprox.txt", 'r') as cfh:
        cover = cfh.readline().split(" ")
        for i in range(G.number_of_nodes()):
            if str(i) not in cover:
                print("removing", i)
                G.remove_node(str(i))
        nx.draw(G, pos=pos, node_color='#ffaaaa', edge_color='#f40552', with_labels=True)
    plt.savefig("cover_twoapprox.png")
