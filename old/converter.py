input = open("R2.in").readlines()
newList = []
for i in input:
    newList.append(int(i.split("-")[1]))
print(newList)