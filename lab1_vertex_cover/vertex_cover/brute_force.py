from itertools import combinations
from typing import Optional

from vertex_cover.types import VertexSets, EdgeList

from utils.dimacs import isVC
from copy import deepcopy

def graph_no_edges(graph: VertexSets) -> bool:
    for vertex in graph:
        if len(vertex) > 0:
            return False
    return True

def choose_vertex(graph: VertexSets) -> int:
    for i in range(len(graph)):
        if len(graph[i]) > 0:
            return i
    return -1

def get_neighbours(graph: VertexSets, vertex: int) -> set[int]:
    return graph[vertex]


def brute_force(graph, k: int) -> Optional[set[int]]:
    """
    :param graph: graph represented as GRAPH REPRESENTATION?
    :param k: this many vertices have to cover the graphs
    :return: set of vertices that create the cover if the solution exists,
    otherwise None
    """

    S = set()
    return VC(graph, k, S)


def VC(graph, k, S):

    if k < 0: return None
    
    if graph_no_edges(graph): return S

    if k == 0: return None


    u = choose_vertex(graph)

    S_1 = S.copy()
    S_1.add(u)
    graph_1 = deepcopy(graph)
    u_to_delete = set()
    for v in graph_1[u]:
        u_to_delete.add(v)
    for v in u_to_delete: graph_1[u].remove(v)

    graph_1[u] = set()


    S1 = VC(graph_1,k - 1, S_1)



    N_u = get_neighbours(graph, u)
    S_2 = S.copy()
    for v in N_u: S_2.add(v)

    graph_2 = deepcopy(graph)
    for u in N_u:
        u_to_delete = set()
        if u in graph_2:
            for v in graph_2[u]:
                u_to_delete.add(v)
            for v in u_to_delete: graph_2[u].remove(v)

            graph_2[u] = set()
            

    S2 = VC(graph_2, k - len(N_u), S_2)

    if S1: 
        return S1
    return S2
    
