;;==================================
;; CS 2110 Spring 2017
;; Homework 7 - Functions & Recursion
;; Name: Joshua Reno
;;===================================

.orig x3000
    LD   R6, STACK       ; Initalize stack pointer (R6)

    ; Call leftshift()
    ADD  R6, R6, -2      ; Add arguments onto stack
    LD   R0, LEFTSHIFT_N ; Push args onto stack
    STR  R0, R6, 1
    LD   R0, LEFTSHIFT_NUMBER
    STR  R0, R6, 0
    LD   R0, LEFTSHIFT_ADDR
    JSRR R0              ; Call leftshift(number, n)
    LDR  R0, R6, 0       ; R0 = return value
    ST   R0, LEFTSHIFT_RESULT ; LEFTSHIFT_RESULT = return value
    ADD  R6, R6, 3       ; Pop off return value and 2 args

    ; Call set/clear/isset/toggle(), depending on the address you pass
    ; to the JSRR below (currently CLEAR_ADDR for clear())
    ADD  R6, R6, -2      ; Allocate stack space for arguments
    LD   R0, ARG_INDEX   ; Push args onto stack
    STR  R0, R6, 1
    LD   R0, ARG_BITS
    STR  R0, R6, 0
    LD   R0, CLEAR_ADDR  ; ← Change this line to call different functions
    JSRR R0              ; Call clear(bits, index)
    LDR  R0, R6, 0       ; R0 = return value
    ST   R0, RESULT      ; RESULT = return value
    ADD  R6, R6, 3       ; Pop off return value and 2 args

    HALT

; Args/result for leftshift()
LEFTSHIFT_NUMBER .fill 10
LEFTSHIFT_N      .fill 3
LEFTSHIFT_RESULT .blkw 1

; Args/result for set/clear/isset/toggle()
ARG_BITS         .fill x0000
ARG_INDEX        .fill 3
RESULT           .blkw 1

; Addresses
LEFTSHIFT_ADDR   .fill LEFTSHIFT
CLEAR_ADDR       .fill CLEAR
SET_ADDR         .fill SET
ISSET_ADDR       .fill ISSET
TOGGLE_ADDR      .fill TOGGLE

; Initial value of stack pointer (R6), so this is the address of the
; bottom of the stack
STACK            .fill xF000
.end

; ======================== LEFTSHIFT ========================
.orig x4000
LEFTSHIFT
        ; TODO: Implement leftshift() here
        ; build stack frame
        ADD R6, R6, -3 ; Allocate space for core stack frame (RV, RA, OldFP)
        STR R7, R6, 1  ; Save the Return Address
        STR R5, R6, 0  ; Save old FP
        ADD R5, R6, -1 ; Set new FP
        ; save register
        ADD R6, R6, -3  ; Make space to save R0-R2
        STR R0, R5, 0   ; Save R0 as 1st local
        STR R1, R5, -1  ; Save R1 as 2nd local
        ; logic
        LDR R0, R5, 4; number
        LDR R1, R5, 5; n
        BRz THING
BRANCH
        ADD R0, R0, R0;
        ADD R1, R1, -1;
        BRp BRANCH;
THING
        STR R0, R5, 3;
        ; restore register
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;

        RET


; ======================== CLEAR ========================

CLEAR
        ; TODO: Implement clear() here
        ; build stack frame
        ADD R6, R6, -3 ; Allocate space for core stack frame (RV, RA, OldFP)
        STR R7, R6, 1  ; Save the Return Address
        STR R5, R6, 0  ; Save old FP
        ADD R5, R6, -1 ; Set new FP
        ; save register
        ADD R6, R6, -3  ; Make space to save R0-R2
        STR R0, R5, 0   ; Save R0 as 1st local
        STR R1, R5, -1  ; Save R1 as 2nd local
        ; logic
        LDR R0, R5, 5; index
        AND R1, R1, 0;
        ADD R1, R1, 1;
        ; R6 is stack pointer
        ADD R6, R6, -2; make room for 2

        STR R1, R6, 0;
        STR R0, R6, 1;
        JSR LEFTSHIFT
        ; mask is stored at R6
        LDR R0, R6, 0; R6 has the address of return value
        NOT R0, R0;

        LDR R1, R5, 4; bits
        AND R1, R1, R0;

        STR R1, R5, 3;
        ; restore register
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;

        RET



; ======================== SET ========================
SET
        ; TODO: Implement set() here
                ; build stack frame
        ADD R6, R6, -3 ; Allocate space for core stack frame (RV, RA, OldFP)
        STR R7, R6, 1  ; Save the Return Address
        STR R5, R6, 0  ; Save old FP
        ADD R5, R6, -1 ; Set new FP
        ; save register
        ADD R6, R6, -3  ; Make space to save R0-R2
        STR R0, R5, 0   ; Save R0 as 1st local
        STR R1, R5, -1  ; Save R1 as 2nd local
        ; logic
        LDR R0, R5, 5; index
        AND R1, R1, 0;
        ADD R1, R1, 1;
        ; R6 is stack pointer
        ADD R6, R6, -2; make room for 2

        STR R1, R6, 0;
        STR R0, R6, 1;
        JSR LEFTSHIFT
        ; mask is stored at R6
        LDR R0, R6, 0; R6 has the address of return value
        NOT R0, R0;

        LDR R1, R5, 4; bits
        NOT R1, R1;
        AND R1, R1, R0;
        NOT R1, R1;

        STR R1, R5, 3;
        ; restore register
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;

        RET


; ======================== ISSET ========================
ISSET
        ; TODO: Implement isset() here
                ; build stack frame
        ADD R6, R6, -3 ; Allocate space for core stack frame (RV, RA, OldFP)
        STR R7, R6, 1  ; Save the Return Address
        STR R5, R6, 0  ; Save old FP
        ADD R5, R6, -1 ; Set new FP
        ; save register
        ADD R6, R6, -3  ; Make space to save R0-R2
        STR R0, R5, 0   ; Save R0 as 1st local
        STR R1, R5, -1  ; Save R1 as 2nd local
        ; logic
        LDR R0, R5, 5; index
        AND R1, R1, 0;
        ADD R1, R1, 1;
        ; R6 is stack pointer
        ADD R6, R6, -2; make room for 2

        STR R1, R6, 0;
        STR R0, R6, 1;
        JSR LEFTSHIFT
        ; mask is stored at R6
        LDR R0, R6, 0; R6 has the address of mask return value
        LDR R1, R5, 4; bits
        AND R1, R1, R0;
        BRz ZERO
        BRnp NOTZERO
ZERO
        AND R1, R1, 0;
        BR DONE
NOTZERO
        AND R1, R1, 0;
        ADD R1, R1, 1;
DONE
        STR R1, R5, 3;
        ; restore register
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;

        RET

; ======================== TOGGLE ========================
TOGGLE
        ; TODO: Implement toggle() here
                ; build stack frame
        ADD R6, R6, -3 ; Allocate space for core stack frame (RV, RA, OldFP)
        STR R7, R6, 1  ; Save the Return Address
        STR R5, R6, 0  ; Save old FP
        ADD R5, R6, -1 ; Set new FP
        ; save register
        ADD R6, R6, -3  ; Make space to save R0-R2
        STR R0, R5, 0   ; Save R0 as 1st local
        STR R1, R5, -1  ; Save R1 as 2nd local
        ; logic
        LDR R0, R5, 5; index
        LDR R1, R5, 4; bits
        ; R6 is stack pointer
        ADD R6, R6, -2; make room for 2

        STR R1, R6, 0;
        STR R0, R6, 1;
        JSR ISSET
        ; mask is stored at R6
        LDR R0, R6, 0; R6 has the address of return value, mask
        ; this is once isset has been called
        Brz TOG
        BRnp OTHER

TOG
        ; make room
        ADD R6, R6, -2;
        STR R1, R6, 0;
        LDR R0, R5, 5; index
        STR R0, R6, 1;

        LDR R1, R5, 4; bits
        JSR SET
        BR DON
OTHER
        ; make room
        ADD R6, R6, -2;
        STR R1, R6, 0;
        LDR R0, R5, 5; index
        STR R0, R6, 1;

        LDR R0, R5, 5; index
        LDR R1, R5, 4; bits
        JSR CLEAR
DON
        LDR R0, R6, 0;
        STR R0, R5, 3;
        ; restore register
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;

        RET
.end
