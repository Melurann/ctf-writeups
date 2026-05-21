from itertools import combinations

calories = [285, 254, 265, 150, 194, 95, 105, 62, 67]
target = 1337

found = False

for r in range(1, len(calories) + 1):
    for combo in combinations(calories, r):
        if sum(combo) == target:
            print("Found combination:", combo)
            found = True

if not found:
    print("No combination adds up to", target)
