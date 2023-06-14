import networkx as nx
import torch

def loop_counter(input:torch.Tensor):
    input_set = set(input)
    G = nx.DiGraph()
    for i in input_set:
        G.add_node(i)
    for i in range(len(input)-1):
        G.add_edge(input[i], input[i+1])
    return len(list(nx.simple_cycles(G))[0])