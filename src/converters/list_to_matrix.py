import os

with open("adj_list.txt", "r") as adj_list:
    adj_list = adj_list.readlines()
    graph_size = len(adj_list)

    matrix = [[0 for x in range(graph_size)] for y in range(graph_size)] 

    for line in adj_list:
        parent = line.split(" ")[0]
        childs = line.split()[1:len(line) - 1]

        for child in childs:
            matrix[int(parent)][int(child)] = 1

    with open("adj_matrix.txt", "w") as adj_matrix:
        adj_matrix.write(str(graph_size) + "\n")
        for i in range(0, graph_size):
            line = ""
            for el in matrix[i]:
                line += str(el) + " "
            line = line[0:len(line) - 1]
            adj_matrix.write(line + "\n")
