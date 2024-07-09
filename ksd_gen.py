# Визуализация графов
import random

import networkx as nx
from networkx import complete_graph,DiGraph, draw, draw_shell
import matplotlib.pyplot as plt
import math
SUBPLT_START = 121

"""Настройки графа"""
PX_MAX = 2 # Ограничение на количество вершин
NODES_NUM = 6
MAX_WEIGHT = 10
MIN_WEIGHT = 1
"""Настройки ГА"""
POPULATION_SIZE = 4 # Кол-во особей
NUM_OF_GENERATIONS = 3 # Длительность эволюции
Pm = 0.001 # Вероятность мутации
Pc = 0.6 # Вероятность скрещивания
Ps = 0.5 # вероятность отбора
MAX_TOTAL_SUM = 1000000000

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

class Model:
    def __init__(self, matr):
        self.matr = matr
        self.totalSum = None


def get_random_weight(marked_rows, excluded_rows, excluded_cols):
    current_min_row, current_min_col = -1,-1
    while (current_min_row < 0 or current_min_col < 0):
        current_min_row = random.randint(0, NODES_NUM-1)
        current_min_col = random.randint(0, NODES_NUM-1)
        if current_min_row in excluded_rows:   current_min_row = -1
        if current_min_row in excluded_cols:   current_min_row = -1

    return current_min_row,current_min_col

def randomize_ksd(matr):
    """Создание случайного ксд на основе matr"""
    matrRandomized = [[0 for j in range(NODES_NUM)] for i in range(NODES_NUM)]

    countAddedToNodes = [0 for i in range(NODES_NUM)]
    marked_row = matr[0]
    included_rows = {}
    excluded_cols = []
    excluded_rows = []
    included_rows[0] = marked_row

    while excluded_cols.__len__() < NODES_NUM:
        # Ищем минимум в выделенной строке
        # с учетом закрашенных столбцов
        chosen_node, minimum_edge_index = get_random_weight(included_rows, excluded_rows, excluded_cols)

        # инкрементируем значение локальной соединяемых двух вершин
        countAddedToNodes[chosen_node] += 1
        countAddedToNodes[minimum_edge_index] += 1

        # если px для узла достигает ограничения - вычеркиваем строку
        if (countAddedToNodes[chosen_node] == PX_MAX):
            excluded_rows.append(chosen_node)

        if (countAddedToNodes[minimum_edge_index] == PX_MAX):
            excluded_rows.append(minimum_edge_index)

        # выделение новой строки
        included_rows[chosen_node] = matr[chosen_node]
        included_rows[minimum_edge_index] = matr[minimum_edge_index]
        # вычеркивание столбца
        excluded_cols.append(minimum_edge_index)

        # симметричная вставка ребра
        matrRandomized[chosen_node][minimum_edge_index] = matr[chosen_node][minimum_edge_index]
        matrRandomized[minimum_edge_index][chosen_node] = matr[minimum_edge_index][chosen_node]

    return matrRandomized

def make_zero_weight(matr, i, j):
    matr[i][j] = 0
    matr[j][i] = 0
    return matr

def mix_matrix(matr1, matr2):
    cutting_point = random.randint(1, len(matr1) - 1)

    new_matr1 = [row[:] for row in matr1]
    new_matr2 = [row[:] for row in matr2]

    new_matr1[cutting_point:], new_matr2[cutting_point:] = new_matr2[cutting_point:], new_matr1[cutting_point:]

    # Убираем ненужные связи для создания ортогонального дерева
    for i in range(len(new_matr1)):
        new_matr1[i][cutting_point:] = [0] * (len(new_matr1) - cutting_point)
        new_matr2[i][cutting_point:] = [0] * (len(new_matr2) - cutting_point)

    for i in range(cutting_point, len(new_matr1)):
        new_matr2[i][:cutting_point] = [0] * cutting_point
        new_matr1[i][:cutting_point] = [0] * cutting_point

    return new_matr1, new_matr2


def genetic_count_fit(population:list):
    for m in population: # type: Model
        sum = 0
        for i in range(NODES_NUM):
            for j in range(i+1,NODES_NUM):
                sum += m.matr[i][j]
        m.totalSum = sum if (sum > 10) else MAX_TOTAL_SUM
    return population

def genetic_selection(population:list):
    population = sorted(population, key=lambda x: x.totalSum)

    selectedModels = []
    selSize = int(len(population) * Ps)

    for i in range(selSize):
        selectedModels.append(population[i])

    return selectedModels

def genetic_cross_over(population:list):
    selSize = int(POPULATION_SIZE * Ps)
    while (len(population) < POPULATION_SIZE):
        randIdx1 = random.randint(0,selSize-1)
        randIdx2 = random.randint(0,selSize-1)

        if randIdx1 == randIdx2:
            continue
        newMatr1,newMatr2 = mix_matrix(population[randIdx1].matr,population[randIdx2].matr)

        population.append(Model(newMatr2))
        population.append(Model(newMatr2))

    return population

def genetic_mutation(population:list):
    for m in population:
        chance = random.randint(1,10)

        if chance/10 <= Pm:
            i = random.randint(0,NODES_NUM-1)
            j = random.randint(0,NODES_NUM-1)

            if i == j:
                continue

            m.matr = make_zero_weight(m.matr, i, j)

    return population


if __name__ == '__main__':
    matr = [[0,2,3,1,4,5],
            [2,0,4,2,6,7],
            [3,4,0,5,8,10],
            [1,2,5,0,1,2],
            [4,6,8,1,0,4],
            [5,7,10,2,4,0]]

    # Создадим случайную популяцию
    population = [Model(randomize_ksd(matr)) for i in range(POPULATION_SIZE)]
    for _ in range(NUM_OF_GENERATIONS):
        population = genetic_count_fit(population)
        population = genetic_selection(population)
        population = genetic_cross_over(population)
        population = genetic_mutation(population)

    population = genetic_count_fit(population)
    population = sorted(population, key=lambda x: x.totalSum, reverse=True)
    result = population[-1].matr







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