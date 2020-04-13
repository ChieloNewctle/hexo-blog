a = []
for i in range(1, 1000):
    if i % 7 == 0 and i % 4 != 0:
        a.append(i)
t = int(input())
for _ in range(t):
    n = int(input())
    for i in a:
        if i >= n:
            print(i)
            break
