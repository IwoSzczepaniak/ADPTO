from approx_vertex_cover.types import EdgeList


def two_approx(graph: EdgeList) -> set[int]:
    """
    Approximation algorithm for the Vertex Cover problem, which takes any edge
    and adds its ends to the solution.

    Approximation factor: 2

    :param graph: graph represented as a list of edges
    :return: set of vertices that approximate the cover
    """

    visited = set()
    for e in graph:
        if e[0] not in visited and e[1] not in visited:
            visited.add(e[0])
            visited.add(e[1])
    return visited
