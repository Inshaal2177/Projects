INCLUDE Irvine32.inc

.data
    menuTitle BYTE "========== HANGMAN ==========",0

    menuOptions BYTE 0Dh,0Ah,"1. Start Game",0Dh,0Ah
                BYTE "2. Instructions",0Dh,0Ah
                BYTE "3. Exit",0Dh,0Ah
                BYTE "Choose an option: ",0

    instructionsText BYTE 0Dh,0Ah,"INSTRUCTIONS",0Dh,0Ah
                     BYTE "Guess the hidden word one letter at a time.",0Dh,0Ah
                     BYTE "The secret word has eight letters.",0Dh,0Ah
                     BYTE "You may make six incorrect guesses.",0Dh,0Ah
                     BYTE "Only letters A-Z are accepted.",0Dh,0Ah
                     BYTE "Repeated guesses do not cost an attempt.",0Dh,0Ah
                     BYTE "You get ONE hint per round. Press ? to use it.",0Dh,0Ah
                     BYTE "Reveal every letter to win!",0Dh,0Ah,0Dh,0Ah
                     BYTE "Press any key to return to the menu...",0

    secretWord BYTE "ASSEMBLY",0
    wordLength DWORD 8

    guessedLetters BYTE 26 DUP(0)
    remaining DWORD 6
    revealedCount DWORD 0
    score DWORD 0
    scanIndex DWORD 0
    displayIndex DWORD 0

    guessChar BYTE 0
    menuChoice BYTE 0
    replayChoice BYTE 0
    matchFound BYTE 0

    ; Hint variables
    hintUsed BYTE 0
    hintChar BYTE 0

    inputPrompt BYTE 0Dh,0Ah,"Enter a letter (? = Hint): ",0
    hintPrompt BYTE 0Dh,0Ah,"Your hint reveals: ",0
    hintUsedMsg BYTE 0Dh,0Ah,"You have already used your hint!",0

    invalidMessage BYTE 0Dh,0Ah,"Invalid input. Enter one letter from A to Z.",0
    duplicateMsg BYTE 0Dh,0Ah,"You already guessed that letter. Try another.",0
    correctMsg BYTE 0Dh,0Ah,"Correct guess!",0
    wrongMsg BYTE 0Dh,0Ah,"Sorry, that letter is not in the word.",0
    attemptsMsg BYTE 0Dh,0Ah,"Incorrect attempts remaining: ",0
    scoreMsg BYTE 0Dh,0Ah,"Total wins: ",0
    winMsg BYTE 0Dh,0Ah,"Congratulations! You guessed the word!",0
    loseMsg BYTE 0Dh,0Ah,"Game over! The secret word was: ",0

    replayPrompt BYTE 0Dh,0Ah,0Dh,0Ah,"1. Play again",0Dh,0Ah
                 BYTE "2. Return to menu",0Dh,0Ah
                 BYTE "Choose an option: ",0

    goodbyeMsg BYTE 0Dh,0Ah,"Thanks for playing Hangman!",0Dh,0Ah,0
    newline BYTE 0Dh,0Ah,0
    separator BYTE "  ",0

.code

main PROC

MenuLoop:
    call Clrscr
    mov edx, OFFSET menuTitle
    call WriteString
    mov edx, OFFSET menuOptions
    call WriteString
    call ReadChar
    mov menuChoice, al
    call WriteChar
    call Crlf

    cmp menuChoice, '1'
    je StartGame
    cmp menuChoice, '2'
    je ShowInstructions
    cmp menuChoice, '3'
    je ExitProgram
    jmp MenuLoop

ShowInstructions:
    call Clrscr
    mov edx, OFFSET instructionsText
    call WriteString
    call ReadChar
    jmp MenuLoop

StartGame:
    call InitializeGame

GameLoop:
    call Clrscr

    mov edx, OFFSET menuTitle
    call WriteString
    call Crlf

    call DisplayWord

    mov edx, OFFSET attemptsMsg
    call WriteString
    mov eax, remaining
    call WriteDec

    mov edx, OFFSET scoreMsg
    call WriteString
    mov eax, score
    call WriteDec

    cmp revealedCount, 8
    je PlayerWins

    cmp remaining, 0
    je PlayerLoses

    mov edx, OFFSET inputPrompt
    call WriteString

    call ReadChar
    mov guessChar, al
    call WriteChar
    call Crlf

    ; Check for hint request
    cmp guessChar, '?'
    je GiveHint

    ; Convert lowercase to uppercase
    cmp guessChar, 'a'
    jb ValidateLetter
    cmp guessChar, 'z'
    ja ValidateLetter
    sub guessChar, 20h

ValidateLetter:
    cmp guessChar, 'A'
    jb InvalidInput
    cmp guessChar, 'Z'
    ja InvalidInput

    ; Check duplicate guesses
    movzx esi, guessChar
    sub esi, 'A'

    cmp guessedLetters[esi], 1
    je RepeatedGuess

    mov guessedLetters[esi], 1

    ; Search secret word
    mov matchFound, 0
    mov scanIndex, 0

CheckWordLoop:
    mov esi, scanIndex
    mov al, secretWord[esi]

    cmp al, guessChar
    jne NextWordCharacter

    mov matchFound, 1
    inc revealedCount

NextWordCharacter:
    inc scanIndex

    mov eax, scanIndex
    cmp eax, wordLength
    jb CheckWordLoop

    cmp matchFound, 1
    je CorrectGuess

    dec remaining

    mov edx, OFFSET wrongMsg
    call WriteString
    call WaitMsg
    jmp GameLoop

CorrectGuess:
    mov edx, OFFSET correctMsg
    call WriteString
    call WaitMsg
    jmp GameLoop

InvalidInput:
    mov edx, OFFSET invalidMessage
    call WriteString
    call WaitMsg
    jmp GameLoop

RepeatedGuess:
    mov edx, OFFSET duplicateMsg
    call WriteString
    call WaitMsg
    jmp GameLoop


; =================================
; ONE-TIME HINT FEATURE
; =================================

GiveHint:

    ; Check if hint has already been used
    cmp hintUsed, 1
    je HintAlreadyUsed

    ; Mark hint as used
    mov hintUsed, 1

    ; Find first unrevealed letter
    mov scanIndex, 0

FindHintLetter:

    mov esi, scanIndex
    mov al, secretWord[esi]

    movzx ebx, al
    sub ebx, 'A'

    cmp guessedLetters[ebx], 1
    jne RevealHintLetter

    inc scanIndex

    mov eax, scanIndex
    cmp eax, wordLength
    jb FindHintLetter

    jmp GameLoop

RevealHintLetter:

    mov hintChar, al

    ; Mark hinted letter as guessed
    movzx ebx, hintChar
    sub ebx, 'A'

    mov guessedLetters[ebx], 1

    ; Count all occurrences of hinted letter
    mov scanIndex, 0

CountHintMatches:

    mov esi, scanIndex
    mov al, secretWord[esi]

    cmp al, hintChar
    jne NextHintCharacter

    inc revealedCount

NextHintCharacter:

    inc scanIndex

    mov eax, scanIndex
    cmp eax, wordLength
    jb CountHintMatches

    ; Display hint
    mov edx, OFFSET hintPrompt
    call WriteString

    mov al, hintChar
    call WriteChar

    call Crlf
    call WaitMsg

    jmp GameLoop


HintAlreadyUsed:

    mov edx, OFFSET hintUsedMsg
    call WriteString

    call WaitMsg

    jmp GameLoop


; =================================
; WIN / LOSS
; =================================

PlayerWins:

    inc score

    mov edx, OFFSET winMsg
    call WriteString
    call Crlf

    call DisplayWord

    jmp RoundFinished

PlayerLoses:

    mov edx, OFFSET loseMsg
    call WriteString

    mov edx, OFFSET secretWord
    call WriteString

    call Crlf


RoundFinished:

    mov edx, OFFSET scoreMsg
    call WriteString

    mov eax, score
    call WriteDec

    mov edx, OFFSET replayPrompt
    call WriteString

    call ReadChar
    mov replayChoice, al

    call WriteChar
    call Crlf

    cmp replayChoice, '1'
    je StartGame

    cmp replayChoice, '2'
    je MenuLoop

    jmp RoundFinished


ExitProgram:

    mov edx, OFFSET goodbyeMsg
    call WriteString

    exit

main ENDP


; =================================
; INITIALIZE GAME
; =================================

InitializeGame PROC

    mov remaining, 6
    mov revealedCount, 0
    mov scanIndex, 0
    mov displayIndex, 0
    mov matchFound, 0

    ; Reset hint for new round
    mov hintUsed, 0
    mov hintChar, 0

    mov ecx, 26
    mov edi, OFFSET guessedLetters

ClearGuesses:

    mov BYTE PTR [edi], 0
    inc edi

    loop ClearGuesses

    ret

InitializeGame ENDP


; =================================
; DISPLAY HIDDEN WORD
; =================================

DisplayWord PROC

    mov displayIndex, 0

DisplayLoop:

    mov esi, displayIndex
    mov al, secretWord[esi]

    movzx ebx, al
    sub ebx, 'A'

    cmp guessedLetters[ebx], 1
    jne ShowUnderscore

    mov al, secretWord[esi]
    call WriteChar

    jmp DisplaySeparator

ShowUnderscore:

    mov al, '_'
    call WriteChar

DisplaySeparator:

    mov edx, OFFSET separator
    call WriteString

    inc displayIndex

    mov eax, displayIndex
    cmp eax, wordLength
    jb DisplayLoop

    call Crlf

    ret

DisplayWord ENDP

END main