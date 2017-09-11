#!/usr/bin/env python

"""Label Correction algorithm. This assume the graph is directive"""


import logging
import sys

logging.basicConfig(format='%(asctime)s %(levelname)s %(message)s',
                    level=logging.DEBUG,
                    stream=sys.stdout)


class LIFO(list):
    """A LIFO storage."""

    def insert(self, el):
        self.append(el)


class Graph(object):
    """An undirected graph."""

    def __init__(self):
        self.nodes = []
        self.edges = []
        self.name2index = {}
        self.index2name = {}
        self.neighbors = []

    def add_node(self, name=None):
        """Add a new node and return its index."""
        node_index = len(self.nodes)
        if name.startswith('index-'):
            logging.warning('Node names beginning with "index-" may cause '
                            'problems.')
        if name is None:
            name = "index-%i" % node_index
        self.nodes.append(node_index)
        self.name2index[name] = node_index
        self.index2name[node_index] = name

        # Add weight from new node to other nodes and vice-versa
        self.edges.append([])
        for n1 in self.nodes:
            self.edges[node_index].append(float('inf'))
            if n1 != node_index:
                self.edges[n1].append(float('inf'))

        # From the node to itself has distance 0
        self.edges[node_index][node_index] = 0

        self.neighbors.append([])

        return node_index

    def get_node_index(self, name):
        """Get node index by name."""
        return self.name2index[name]

    def set_edge_by_name(self, a, b, weight):
        """
        Set edge weight by node names.

        Parameters
        ----------
        a : str
            First edge name
        b : str
            Second edge name
        weight : number
            New edge weight
        """
        i1 = self.get_node_index(a)
        i2 = self.get_node_index(b)
        self.edges[i1][i2] = weight
        # self.edges[i2][i1] = weight   # single direction
        self.neighbors[i1].append(i2)
        # self.neighbors[i2].append(i1) # single direction


def label_correction(graph,
                     start_node, t,
                     h=None, m=None,
                     K=None):
    """
    Label correction algorithm for graph searches.

    Parameters
    ----------
    graph :
        Needs 'graph.childs' which returns a list of child indices for each
        node, 'graph.edges[node1][node2]' which always returns an edge weight,
    start_node : int
        Index of start node as given by the graph node iterator
    t : int
        Index of target node as given by the graph node iterator
    h : lower_heuristic, optional
        Takes (graph, node1, node2) and returns a number which underestimates
        the distance from node1 to node2. If this is not given, the trivial
        distance 0 is chosen.
    m : upper_heuristic, optional
    K : list-like data structure, optional
        Needs 'insert', 'pop'
    """
    if h is None:
        h = lambda g, n1, n2: 0.0
    if m is None:
        m = lambda g, n1, n2: float('inf')
    if K is None:
        K = LIFO()
    d = []
    parents = []
    for node in graph.nodes:
        d.append(float('inf'))
        parents.append(None)
    d[start_node] = 0
    u = float('inf')  # shortest distance from start_node to t
    K.append(start_node)
    while len(K) > 0:
        logging.info("K=%s" % str(K))
        v = K.pop()
        for c in graph.neighbors[v]:
            if d[v] + graph.edges[v][c] + h(graph, c, t) < min(d[c], u):
                d[c] = d[v] + graph.edges[v][c]
                parents[c] = v
                if c != t and c not in K:
                    K.insert(c)
                if c == t:
                    u = d[v] + graph.edges[v][t]
            u = min(u, d[c] + m(graph, c, t))
    # Reconstruct the path
    path, named_path = [], []
    current = t
    while current != start_node:
        path.append(current)
        named_path.append(graph.index2name[current])
        current = parents[current]
    path.append(current)
    named_path.append(graph.index2name[current])
    return {'shortest_distance': u,
            'path': path[::-1],
            'named_path': named_path[::-1]}


def sample_1():
    """A simple search problem."""
    g = Graph()
    for i in range(13):
        g.add_node(name=chr(ord('A') + i))
    g.set_edge_by_name('A', 'B', 6)
    g.set_edge_by_name('A', 'C', 2)
    g.set_edge_by_name('A', 'D', 2)
    g.set_edge_by_name('A', 'E', 3)
    g.set_edge_by_name('B', 'A', 2)
    g.set_edge_by_name('B', 'C', 4)
    g.set_edge_by_name('B', 'D', 3)
    g.set_edge_by_name('B', 'E', 7)
    g.set_edge_by_name('C', 'A', 5)
    g.set_edge_by_name('C', 'B', 3)
    g.set_edge_by_name('C', 'D', 2)
    g.set_edge_by_name('C', 'E', 1)
    g.set_edge_by_name('D', 'A', 1)
    g.set_edge_by_name('D', 'B', 9)
    g.set_edge_by_name('D', 'C', 4)
    g.set_edge_by_name('D', 'E', 3)
    g.set_edge_by_name('E', 'A', 3)
    g.set_edge_by_name('E', 'B', 7)
    g.set_edge_by_name('E', 'C', 3)
    g.set_edge_by_name('E', 'D', 5)
    i1 = g.get_node_index('A')
    i2 = g.get_node_index('F')
    ret = label_correction(g, i1, i2)
    print(ret)



def find_paths(node, cities, path, distance):
    # Add way point
    path.append(node)

    # Calculate path length from current to last node
    if len(path) > 1:
        distance += cities[path[-2]][node]

    # If path contains all cities and is not a dead end,
    # add path from last to first city and return.
    if (len(cities) == len(path)) and (cities[path[-1]].has_key(path[0])):
        global routes
        path.append(path[0])
        distance += cities[path[-2]][path[0]]
        print path, distance
        routes.append([distance, path])
        return

    # Fork paths for all possible cities not yet used
    for city in cities:
        if (city not in path) and (cities[city].has_key(node)):
            find_paths(city, dict(cities), list(path), distance)


routes = []
if __name__ == '__main__':
    cities = {
        'A': {'B': 6, 'C': 2, 'D': 2, 'E': 3},
        'B': {'A': 2, 'C': 4, 'D': 3, 'E': 7},
        'C': {'A': 5, 'B': 3, 'D': 2, 'E': 1},
        'D': {'A': 1, 'B': 9, 'C': 4, 'E': 3},
        'E': {'A': 3, 'B': 7, 'C': 3, 'D': 5},
    }

    print "Start: A"
    find_paths('A', cities, [], 0)
    print "\n"
    routes.sort()
    if len(routes) != 0:
        print "Shortest route: %s" % routes[0]
    else:
        print "FAILED"


#if __name__ == '__main__':
    #sample_1()