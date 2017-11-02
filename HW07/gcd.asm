;;===============================
;; CS 2110 Fall 2017
;; Homework 7 - Functions & Recursion
;; Name: Joshua Reno
;;===============================

; DO NOT REMOVE THIS LINE
;@plugin filename=lc3_udiv vector=x80

.orig x3000
    LD R6, STACK

    ADD R6, R6, -2;
    LD R0, A;
    STR R0, R6, 1;
    LD R0, B;
    STR R0, R6, 0;

    JSR GCD
    LDR R0, R6, 0;
    ST R0, ANSWER;
    HALT


A       .fill 20
B       .fill 16
ANSWER  .blkw 1
STACK   .fill xF000



GCD
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
        LDR R0, R5, 4; A
        LDR R1, R5, 5; B
        UDIV

        ADD R1, R1, 0;
        BRz RETURNB
        BRnp OTHER
RETURNB
        LDR R0, R5, 5; B
        STR R0, R5, 3;
        BR FINISH
OTHER
        ADD R6, R6, -2;
        LDR R0, R5, 5;
        STR R0, R6, 0;
        STR R1, R6, 1;



        JSR GCD
        LDR R0, R6, 0;
        STR R0, R5, 3;
FINISH
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;

        RET

.end
