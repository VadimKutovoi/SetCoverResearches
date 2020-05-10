import cplex

def _main():
    prob = cplex.Cplex()
    prob.set_problem_name("Minimum Vertex Cover")
    prob.set_problem_type(cplex.Cplex.problem_type.LP)
    prob.objective.set_sense(prob.objective.sense.minimize)

    with open("vertex_weights.txt", "r") as weights_file:
        line = weights_file.readline()
        w_obj = line.split(" ")
        w_obj.pop()
        w_obj = [ int(x) for x in w_obj ]
    
    names = [ str(i) for i in range(len(w_obj)) ]
    low_bnd = [ 0 for i in range(len(w_obj)) ]
    upr_bnd = [ 1 for i in range(len(w_obj)) ]

    prob.variables.add(names=names, obj=w_obj, lb=low_bnd, ub=upr_bnd)

    all_int = [(var, prob.variables.type.integer) for var in names]
    prob.variables.set_types(all_int)

    constraints = []
    in_list = False
    with open("adj_list.txt", "r") as adj_list:
        for line in adj_list.readlines():
            parent = line.split(" ")[0]
            for child in line.split()[1:len(line) - 1]:
                for constraint in constraints:
                    if constraint[0][0] == child and constraint[0][1] == parent:
                        in_list = True
                if not in_list:
                    constraints.append([[parent, child], [1, 1]])
                in_list = False


    constraint_names = ["".join(x[0]) for x in constraints]

    # Each edge must have at least one vertex
    rhs = [1] * len(constraints)

    constraint_senses = ["G"] * len(constraints)

    # And add the constraints
    prob.linear_constraints.add(names=constraint_names,
                                lin_expr=constraints,
                                senses=constraint_senses,
                                rhs=rhs)
    # Solve the problem
    print("Problem Type: %s" % prob.problem_type[prob.get_problem_type()])
    prob.solve()
    print("Solution result is: %s" % prob.solution.get_status_string())
    print(prob.solution.get_values())


if __name__ == "__main__":
    _main()
