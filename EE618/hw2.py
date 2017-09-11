A = 6.9107755       # -ln(0.001)
B = 4.7105307       # -ln(0.009)
C = 2.5133061       # -ln(0.081)
D = 0.3160815       # -ln(0.729)

routes = []


def dijkstra(graph, src, dest, visited=[], distances={}, predecessors={}):
    if src not in graph:
        raise TypeError('The root of the shortest path tree cannot be found')
    if dest not in graph:
        raise TypeError('The target of the shortest path cannot be found')
    if src == dest:
        path = []
        pred = dest
        while pred != None:
            path.append(pred)
            pred = predecessors.get(pred, None)
        print('shortest path: ' + str(path) + " cost=" + str(distances[dest]))
    else:
        if not visited:
            distances[src] = 0
        for neighbor in graph[src]:
            if neighbor not in visited:
                new_distance = distances[src] + graph[src][neighbor]
                if new_distance < distances.get(neighbor, float('inf')):
                    distances[neighbor] = new_distance
                    predecessors[neighbor] = src
        visited.append(src)
        unvisited = {}
        for k in graph:
            if k not in visited:
                unvisited[k] = distances.get(k, float('inf'))
        x = min(unvisited, key=unvisited.get)
        dijkstra(graph, x, dest, visited, distances, predecessors)


def problem1():
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

def find_paths(node, cities, path, distance):
    path.append(node)

    if len(path) > 1:
        distance += cities[path[-2]][node]

    if (len(cities) == len(path)) and (cities[path[-1]].has_key(path[0])):
        global routes
        path.append(path[0])
        distance += cities[path[-2]][path[0]]
        print path, distance
        routes.append([distance, path])
        return

    for city in cities:
        if (city not in path) and (cities[city].has_key(node)):
            find_paths(city, dict(cities), list(path), distance)


def problem2():
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


def main():
    problem1()
    problem2()

if __name__ == '__main__':
    main()
