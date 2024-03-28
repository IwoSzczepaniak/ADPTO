from approx_vertex_cover.types import EdgeList


def logn_approx(graph: EdgeList) -> set[int]:
    """
    Approximation algorithm for the Vertex Cover problem, which takes the
    vertex with the highest degree and adds it to the solution.

    Approximation factor: log(n)

    :param graph: graph represented as a list of edges
    :return: set of vertices that approximate the cover
    """
    result = set()
    my_dict = {}

    for el in graph:
        if el[0] not in my_dict:
            my_dict[el[0]] = []
        else:
            my_dict[el[0]].append(el[1])

        if el[1] not in my_dict:
            my_dict[el[1]] = []
        else:
            my_dict[el[1]].append(el[0])

    visited = set()
    while len(visited) < len(graph)*2 and len(my_dict) > 0:
        max_key = max(my_dict, key=lambda k: len(my_dict[k]))
        result.add(max_key)

        elements = my_dict.pop(max_key)
        for el in elements:
            visited.add((el, max_key))
            visited.add((max_key, el))

    return result
