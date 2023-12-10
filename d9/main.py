def getDiffs(data):
    if (len(data) == 1):
        return []
    out = []
    prev = data[0]
    for i in data[1:]:
        out.append(i-prev)
        prev = i
    return out

def solveNum(data):
    values = []
    values.append(data)
    while True:
        values.append(getDiffs(values[-1]))
        if values[-1].count(0) == len(values[-1]):
            values[-1].append(0)
            break
    
    prev = values[-1][0]
    for i in reversed(values[:-1]):
        i.insert(0,i[0]-prev)
        prev = i[0]
    
    print(values)

    return values[0][0]

with open("data.txt", "r") as file:
    data = []
    for line in file:
        data.append(list(map(int, line.split(" "))))

    total = 0
    for i in data:
        total += solveNum(i)
    print("total:", total)

                

