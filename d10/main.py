import sys

sys.setrecursionlimit(100000)


#enter from 0 bottom, 1 top, 2 left, 3 right


def findRoute(path, grid, dir):
    curSym = grid[path[-1][0]][path[-1][1]]

    if curSym == "S":
        return True

    nextDirs = {("|", 0):0,
               ("|", 1):1,
               ("-", 2):2,
               ("-", 3):3,
               ("L", 1):2,
               ("L", 3):0,
               ("J", 1):3,
               ("J", 2):0,
               ("7", 0):3,
               ("7", 2):1,
               ("F", 0):2,
               ("F", 3):1}

    nextDir = nextDirs.get((curSym, dir))

    if nextDir == None:
        return False
    elif nextDir == 0:
        if path[-1][0] == 0:
            return False
        path.append([path[-1][0]-1,path[-1][1]])
    elif nextDir == 1:
        if path[-1][0] == len(grid)-1:
            return False
        path.append([path[-1][0]+1,path[-1][1]])
    elif nextDir == 2:
        if path[-1][1] == len(grid[0])-1:
            return False
        path.append([path[-1][0],path[-1][1]+1])
    elif nextDir == 3:
        if path[-1][1] == 0:
            return False
        path.append([path[-1][0],path[-1][1]-1])
    
    return findRoute(path, grid, nextDir)
        
def startRoute(entry, grid):
    path = []
    path.append(entry)

    if path[-1][0] != 0:
        path.append([path[-1][0]-1,path[-1][1]])
        if findRoute(path, grid, 0):
            return path
    
    path = []
    path.append(entry)

    if path[-1][0] != len(grid)-1:
        path.append([path[-1][0]+1,path[-1][1]])
        if findRoute(path, grid, 1):
            return path
    
    path = []
    path.append(entry)

    if path[-1][1] != len(grid[0])-1:
        path.append([path[-1][0],path[-1][1]+1])
        if findRoute(path, grid, 2):
            return path

    path = []
    path.append(entry)

    if path[-1][1] != 0:
        path.append([path[-1][0],path[-1][1]-1])
        if findRoute(path, grid, 3):
            return path


with open("data.txt", "r") as file:
    data = []
    for line in file:
        data.append(list(line[:-1]))

    entry = [0,0]

    for i in enumerate(data):
        try:
            entry[1] = i[1].index('S')
            entry[0] = i[0]
            break
        except ValueError:
            pass

    route = startRoute(entry, data)

    print(route)
    print("mid num:", len(route)//2)
    print("mid:",route[len(route)//2])