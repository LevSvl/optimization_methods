# Визуализация графов
import networkx as nx
from networkx import complete_graph,DiGraph, draw, draw_shell
import matplotlib.pyplot as plt
import math
SUBPLT_START = 121

PX_MAX = 2 # Ограничение на количество вершин

NODES_NUM = 6
MAX_WEIGHT = 10
MIN_WEIGHT = 1

class MyGraph():
    def __init__(self, matrix):
        self.graph = nx.Graph()
        weights = []
        for i in range(1,NODES_NUM):
            for j in range(0, i):
                if matrix[i][j] <= 0:
                    continue
                weight = (j, i,matrix[i][j])
                weights.append(weight)
        self.graph.add_weighted_edges_from(weights)

    def getGraph(self):
        return self.graph

def check_for_cycle(tree, n1, n2):
    # Хотя бы одна вершина не добавлена
    if ((not tree[n1].__len__()) or (not tree[n2].__len__())):
        return True
    # Оба узла соединены
    for added_yet_for_node1 in tree[n1]:
        # Узлы соединены напрямую
        for added_yet_for_node2 in tree[n2]:
            if added_yet_for_node1 == added_yet_for_node2:
                return False
        # Узлы соединены ненапрямую
        if not check_for_cycle(tree, added_yet_for_node1, n2):
            return False
    return True

def find_min_marked(marked_rows, excluded_rows, excluded_cols):
    """
    Найдёт среди выделенных строк самый маленький вес
    Вернет номер строки и столбца
    """
    current_min_row,  current_min_col = 0, 0
    current_min_weight = MAX_WEIGHT
    for i in marked_rows.keys():
        if i in excluded_rows:
            continue
        for j in range(NODES_NUM):
            if j in excluded_cols:
                continue
            if marked_rows[i][j] < current_min_weight:
                current_min_row = i
                current_min_col = j
                current_min_weight = marked_rows[i][j]
                if marked_rows[i][j] == MIN_WEIGHT:
                    break
    return current_min_row,current_min_col


if __name__ == '__main__':
    matr = [[0,2,3,1,4,5],
            [2,0,4,2,6,7],
            [3,4,0,5,8,10],
            [1,2,5,0,1,2],
            [4,6,8,1,0,4],
            [5,7,10,2,4,0]]



    result = [[0 for i in range(NODES_NUM)] for j in range(NODES_NUM)]

    chosen_node = int(input("choose start node: "))

    px_for_nodes = [0 for i in range(NODES_NUM)]

    included_rows = {}
    excluded_cols = []
    excluded_rows = []
    # исключить столбец, выделить строку
    excluded_cols.append(chosen_node)
    marked_row = matr[chosen_node]
    included_rows[chosen_node] = marked_row

    while excluded_cols.__len__() < NODES_NUM:
        # Ищем минимум в выделенной строке
        # с учетом закрашенных столбцов
        chosen_node, minimum_edge_index = find_min_marked(included_rows,excluded_rows, excluded_cols)

        # инкрементируем значение локальной соединяемых двух вершин
        px_for_nodes[chosen_node] += 1
        px_for_nodes[minimum_edge_index] += 1

        # если px для узла достигает ограничения - вычеркиваем строку
        if (px_for_nodes[chosen_node] == PX_MAX):
            excluded_rows.append(chosen_node)

        if (px_for_nodes[minimum_edge_index] == PX_MAX):
            excluded_rows.append(minimum_edge_index)

        # выделение новой строки
        included_rows[chosen_node] = matr[chosen_node]
        included_rows[minimum_edge_index] = matr[minimum_edge_index]
        # вычеркивание столбца
        excluded_cols.append(minimum_edge_index)

        # симметричная вставка ребра
        result[chosen_node][minimum_edge_index] = matr[chosen_node][minimum_edge_index]
        result[minimum_edge_index][chosen_node] = matr[minimum_edge_index][chosen_node]

    G1 = MyGraph(matrix=matr)
    G2 = MyGraph(matrix=result)
    subax1 = plt.subplot(121)

    pos = {0:(-1, 2), 1:(2,1), 2:(2,0), 3:(-1,-1), 4: (-4,0),5: (-4, 1) }

    nx.draw(G1.getGraph(),pos, with_labels=True, font_weight='bold')
    labels1 = nx.get_edge_attributes(G1.getGraph(), 'weight')
    nx.draw_networkx_edge_labels(G1.getGraph(), pos, edge_labels=labels1, ax=subax1)


    subax2 = plt.subplot(122)
    nx.draw(G2.getGraph(),pos, with_labels=True, font_weight='bold')
    labels2 = nx.get_edge_attributes(G2.getGraph(), 'weight')
    nx.draw_networkx_edge_labels(G1.getGraph(), pos, edge_labels=labels2, ax=subax2)
    plt.show()