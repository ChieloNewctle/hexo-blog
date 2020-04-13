t = int(input())
v = 'aeiyou'
for _ in range(t):
    s = input()
    t = ''.join((i for i in s if i not in v))
    if s[0] in v:
        t = s[0] + t
    print(t)
