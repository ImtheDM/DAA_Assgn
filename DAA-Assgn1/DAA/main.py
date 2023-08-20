import matplotlib.pyplot as plt

file = open("output.txt", "r")

X = []
Y = []
flag = 1

while flag != 0:
    coord = []
    while 1:
        line = file.readline()
        if line == "\n":
            break
        if line == "":
            flag = 0
            break
        coord.append(line.split())
        print(coord)

    if flag == 0:
        break
    coord.append(coord[0])

    xs, ys = zip(*coord)

    X.append(list(map(float, xs)))
    Y.append(list(map(float, ys)))

plt.figure()
for i in range(0, len(X)):
    plt.plot(X[i], Y[i])
plt.show()
