Game = [[0 for _ in range(3)] for _ in range(3)]

def print_board():
    for row in Game:
        for cell in row:
            if cell == 0:
                print(" - ", end=" ")
            else:
                print(cell, end=" ")
        print() 
    print()

print("Initial Game Board:")
print_board()

for turn in range(9):  
    player = " X " if turn % 2 == 0 else " O " 
    print(f"Player {player}'s turn.")
    
    while True:
        row = int(input("Enter a row (1-3): ")) - 1
        col = int(input("Enter a column (1-3): ")) - 1
    
        if 0 <= row < 3 and 0 <= col < 3 and Game[row][col] == 0:
            Game[row][col] = player  
            break
        else:
            print("Invalid move. Try again.")
    
    print("Current Game Board:")
    print_board()

print("Game Over!")
