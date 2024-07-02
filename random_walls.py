from random import randint
width = 1000
height = 600
for i in range(20):
    print(f"line wall{i + 1}({randint(0, width)}, {randint(0, width)}, {randint(0, height)}, {randint(0, height)});")
    print(f"walls.push_back(wall{i + 1});")