import random
secret = random.randint(1,10)
attempt = 0
max_attempt = 3
while attempt < max_attempt:
    num = int(input('Guess the correct number between 1 and 10: '))
    attempt += 1
    if num == secret:
        print(' You Win!')
        break
    else:
        print('Wrong Guess!')
print(secret)