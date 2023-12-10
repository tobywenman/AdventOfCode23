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
    
    prev = values[-1][-1]
    for i in reversed(values[:-1]):
        i.append(i[-1] + prev)
        prev = i[-1]
    
    return values[0][-1]

with open("data.txt", "r") as file:
    data = []
    for line in file:
        data.append(list(map(int, line.split(" "))))

    total = 0
    for i in data:
        total += solveNum(i)
    print("total:", total)

                

