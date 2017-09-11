A = 6.9107755
B = 4.7105307
C = 2.5133061
D = 0.3160815


def dijkstra(graph, src, dest, visited=[], distances={}, predecessors={}):
    """ calculates a shortest path tree routed in src
    """
    # a few sanity checks
    if src not in graph:
        raise TypeError('The root of the shortest path tree cannot be found')
    if dest not in graph:
        raise TypeError('The target of the shortest path cannot be found')
        # ending condition
    if src == dest:
        # We build the shortest path and display it
        path = []
        pred = dest
        while pred != None:
            path.append(pred)
            pred = predecessors.get(pred, None)
        print('shortest path: ' + str(path) + " cost=" + str(distances[dest]))
    else:
        # if it is the initial  run, initializes the cost
        if not visited:
            distances[src] = 0
        # visit the neighbors
        for neighbor in graph[src]:
            if neighbor not in visited:
                new_distance = distances[src] + graph[src][neighbor]
                if new_distance < distances.get(neighbor, float('inf')):
                    distances[neighbor] = new_distance
                    predecessors[neighbor] = src
        # mark as visited
        visited.append(src)
        # now that all neighbors have been visited: recurse
        # select the non visited node with lowest distance 'x'
        # run Dijskstra with src='x'
        unvisited = {}
        for k in graph:
            if k not in visited:
                unvisited[k] = distances.get(k, float('inf'))
        x = min(unvisited, key=unvisited.get)
        dijkstra(graph, x, dest, visited, distances, predecessors)


if __name__ == "__main__":
    # import sys;sys.argv = ['', 'Test.testName']
    # unittest.main()
    graph = {'s': {'a': 0},
             'a': {'b': B, 'c': A},
             'b': {'d': B, 'e': C},
             'c': {'f': A, 'g': D},
             'd': {'h': B, 'i': C},
             'e': {'j': C, 'k': B},
             'f': {'h': B, 'i': C},
             'g': {'j': C, 'k': B},
             'h': {'l': D, 'm': A},
             'i': {'n': C, 'o': B},
             'j': {'l': B, 'm': C},
             'k': {'n': A, 'o': D},
             'l': {'t': 0},
             'm': {'t': 0},
             'n': {'t': 0},
             'o': {'t': 0},
             't': {},
             }
    dijkstra(graph, 's', 't')
