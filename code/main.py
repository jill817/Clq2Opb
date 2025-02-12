# 不要修改以下代码段******
data = {
    "Vertices": ["1", "2", "3", "4", "5"],
    "Edges": ["{1,2}", "{1,3}", "{3,5}", "{4,5}"]
}
# 不要修改以上代码段******
from pyscipopt import Model, quicksum

# Model
model = Model("VertexCover")
model.setParam('display/verblevel', 0)

# Variables
vertex_in_subgraph = {}
for v in data["Vertices"]:
    vertex_in_subgraph[v] = model.addVar(vtype="B", name=f"vertex_in_subgraph_{v}")

# Constraints
for edge in data["Edges"]:
    vertices = edge.strip('{}').split(',')
    model.addCons(quicksum(vertex_in_subgraph[v] for v in vertices) >= 1, name=f"edge_cover_{edge}")

# Objective
model.setObjective(quicksum(vertex_in_subgraph[v] for v in data["Vertices"]), "minimize")

# Solve
model.optimize()

# Check if a feasible solution was found
if model.getStatus() == "optimal":
# 输出解
    value = model.getObjVal()
    print("object value: ", value)
    for i in model.getVars():
        if model.getVal(i)> 1e-6:
            print(i,":",model.getVal(i))
else:
    print("No solution")