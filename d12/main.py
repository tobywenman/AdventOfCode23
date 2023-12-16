def check(string, values):
    value = 0
    count = 0
    for i in string:
        if i == '#':
            count += 1
        elif count != 0:
            if value == len(values):
                return False
            elif values[value] != count:
                return False
            else:
                count = 0
                value += 1
    if count != 0:
        if value == len(values):
            return False
        elif values[value] != count:
            return False
        else:
            value += 1

    return value == len(values)

def incTest(test):
    out = []
    carry = True
    for i in test:
        if i and carry:
            out.append(False)
            carry = True
        elif i or carry:
            out.append(True)
            carry = False
        else:
            out.append(False)
            carry = False
    return out

def tryAll(string, values):
    unknowns = []

    for i in enumerate(string):
        if i[1] == '?':
            unknowns.append(i[0])
    
    count = 0

    curTest = [False]*len(unknowns)

    for i in range(2**len(unknowns)):
        for j in enumerate(unknowns):
            if curTest[j[0]]:
                string[j[1]] = '.'
            else:
                string[j[1]] = '#'
        if check(string, values):
            count += 1

        curTest = incTest(curTest)
    return count

with open("data.txt", "r") as file:
    total=0
    for line in file:
        data = []
        values = []
        for char in enumerate(line):
            if char[1] != ' ':
                data.append(char[1])
            else:
                values = list(map(int, line[char[0]:].split(",")))
                break
        
        total += tryAll(data, values)
    
    print(total)




