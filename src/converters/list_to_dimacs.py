# 198 2742 0
# 8 24 35 42 46 60 74 78 81 95 98 99 100 101 103 104 108 131 132 154 159 168 171 
# 
# p edge 450 17827    
# e 1 2
import sys
    
in_file = sys.argv[1]
out_file = in_file.split(".")[0] + ".mis"

in_list = False
edges = []

with open(in_file, "r") as adj_list:
    with open(out_file, "w") as misfile:
        lines = adj_list.readlines()
        for i in range(0, len(lines)):
            if i == 0:
                misfile.write("p edge " + lines[i].split(" ")[i] + " " + lines[i].split(" ")[1] + "\n")
            else:
                parent = i
                for child in lines[i].split(" "):
                    if child != "" and child != " " and child != "\n":
                        in_list = False
                        for edge in edges:
                            
                            if edge[0] == int(child) and edge[1] == int(parent):
                                in_list = True

                        if not in_list:
                            edges.append([int(parent), int(child)])
                            misfile.write("e " + str(parent) + " " + str(child) + "\n")

