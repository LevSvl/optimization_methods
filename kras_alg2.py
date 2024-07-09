

import networkx as nx
import matplotlib.pyplot as plt

NODES_NUM = 6


class MyGraph():
    def __init__(self, matrix):
        self.graph = nx.DiGraph()
        weights = []
        for i in range(1, NODES_NUM):
            for j in range(0, i):
                if matrix[i][j] <= 0:
                    continue
                weight = (j, i, matrix[i][j])
                weights.append(weight)
        self.graph.add_weighted_edges_from(weights)

    def getGraph(self):
        return self.graph

def check_for_cycle(tree, n1, n2):
    stack = []
    stack.append(n1)
    visited = set()

    while stack:
        node = stack.pop()
        if node in visited:
            continue
        visited.add(node)
        for neighbor in tree[node]:
            if neighbor == n2:
                return False
            stack.append(neighbor)

    return True


if __name__ == '__main__':
    matr = [[0,2,3,1,4,5],
            [2,0,4,2,6,7],
            [3,4,0,5,8,10],
            [1,2,5,0,1,2],
            [4,6,8,1,0,4],
            [5,7,10,2,4,0]]

    result = [[0 for _ in range(NODES_NUM)] for _ in range(NODES_NUM)]
    U = []
    for i in range(1, NODES_NUM):
        for j in range(0, i):
            node1 = j
            node2 = i
            U.append((node1, node2, matr[i][j]))

    U = sorted(U, key=lambda x: x[2])

    pairs_added = {}
    for edge in U:
        pairs_added[edge[0]] = []
        pairs_added[edge[1]] = []

    for edge in U:
        node1 = edge[0]
        node2 = edge[1]
        weight = edge[2]

        if len(pairs_added[node2]) >= 2 or len(pairs_added[node1]) >= 2:
            continue

        if not check_for_cycle(pairs_added, node2, node1):
            continue

        result[node2][node1] = weight
        pairs_added[node2].append(node1)
        pairs_added[node1].append(node2)

    for row in result:
        print(row)

    G1 = MyGraph(matrix=matr)
    G2 = MyGraph(matrix=result)

    pos = {0: (-1, 2), 1: (2, 1), 2: (2, 0), 3: (-1, -1), 4: (-4, 0), 5: (-4, 1)}

    plt.subplot(121)
    nx.draw(G1.getGraph(), pos, with_labels=True, font_weight='bold')
    labels1 = nx.get_edge_attributes(G1.getGraph(), 'weight')
    nx.draw_networkx_edge_labels(G1.getGraph(), pos, edge_labels=labels1)

    plt.subplot(122)
    nx.draw(G2.getGraph(), pos, with_labels=True, font_weight='bold')
    labels2 = nx.get_edge_attributes(G2.getGraph(), 'weight')
    nx.draw_networkx_edge_labels(G2.getGraph(), pos, edge_labels=labels2)

    plt.show()