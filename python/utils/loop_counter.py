# import networkx as nx
import torch

# def loop_counter(input:torch.Tensor):
#     input_set = set(input)
#     G = nx.DiGraph()
#     for i in input_set:
#         G.add_node(i)
#     for i in range(len(input)-1):
#         G.add_edge(input[i], input[i+1])
#     return len(list(nx.simple_cycles(G))[0])

def loop_counter(input:torch.Tensor):
    loop = [input[-1]]
    for i in range(len(input)):
        if input[-1-(i+1)] == loop[0]:
            break
        else:
            loop.append(input[-1-(i+1)])
    return len(loop)