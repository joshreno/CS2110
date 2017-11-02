;;===============================
;; CS 2110 Fall 2017
;; Homework 7 - Functions & Recursion
;; Name: Joshua Reno
;;===============================

; DO NOT REMOVE THIS LINE
;@plugin filename=lc3_udiv vector=x80

.orig x3000

;;=====================================
;; Call bin_search subroutine with correct arguments here
;;=====================================

	LD R6, STACK        ; Initialize stack

	; TODO: Setup BINSEARCH call with argumuents arr, target, start, end
    ADD R6, R6, -4;
    LD R0, ARR;
    STR R0, R6, 3;
    LD R0, TARGET;
    STR R0, R6, 2;
    AND R0, R0, 0; 0 is our start variable
    STR R0, R6, 1;
    LD R0, ARR_SIZE;
    STR R0, R6, 0;
	LD R0, BIN_SEARCH_ADDR
	JSRR R0 			; call BIN_SEARCH

	; TODO: Store the return value in ANSWER
    LDR R0, R6, 0;
    ST R0, ANSWER;
	HALT

ARR 				.fill x4000
ARR_SIZE 			.fill 16
TARGET 				.fill 7
ANSWER      		.blkw 1         ; Store ANSWER here
BIN_SEARCH_ADDR		.fill BIN_SEARCH
STACK       		.fill xF000


.end
;;=====================================
;; The array to be searched
;;=====================================

.orig x4000
.fill -7
.fill -5
.fill -2
.fill 2
.fill 7
.fill 10
.fill 12
.fill 13
.fill 14
.fill 16
.fill 20
.fill 25
.fill 26
.fill 40
.fill 50
.fill 90

.end
;;=====================================
;; Implement your bin_search subroutine here
;;=====================================

.orig x5000
BIN_SEARCH
        ; build stack frame
        ADD R6, R6, -3 ; Allocate space for core stack frame (RV, RA, OldFP)
        STR R7, R6, 1  ; Save the Return Address
        STR R5, R6, 0  ; Save old FP
        ADD R5, R6, -1 ; Set new FP
        ; save register
        ADD R6, R6, -5 ; make space to save R0-R4
        STR R0, R5, 0;
        STR R1, R5, -1;
        STR R2, R5, -2;
        STR R3, R5, -3;
        STR R4, R5, -4;
        ; logic
        LDR R0, R5, 4; arr
        LDR R1, R5, 5; target
        LDR R2, R5, 6; start
        LDR R3, R5, 7; end

        AND R4, R4, 0;
        ADD R4, R3, 0;
        NOT R4, R4;
        ADD R4, R4, 1; 2's complementing end so we have start - end
        ADD R4, R4, R2; start - end
        BRzp FIRSTCHECKNEGATIVE
        BRn THEREST
FIRSTCHECKNEGATIVE
        AND R0, R0, 0;
        ADD R0, R0, -1;
        STR R0, R5, 3;
        BR FINISH
THEREST
        AND R0, R0, 0;
        AND R1, R1, 0;
        ADD R0, R2, R3;
        ADD R1, R1, 2;
        UDIV
        ; R0 now has (start + end)/2

        LDR R3, R5, 7; end -> length, counter
        LDR R4, R5, 4; address
        ADD R4, R4, R0;
        LDR R4, R4, 0; value of the thing
        LDR R1, R5, 5;
        NOT R1, R1;
        ADD R1, R1, 1;
        ADD R1, R1, R4;
        BRz ZERO
        BRn NEGATIVE
        BRp POSITIVE
ZERO
        STR R0, R5, 3;
        BR FINISH
POSITIVE
        ADD R6, R6, -4
        ADD R3, R0, 0;
        LDR R0, R5, 4;
        LDR R1, R5, 5;
        LDR R2, R5, 6;
        STR R0, R6, 0
        STR R1, R6, 1
        STR R2, R6, 2
        STR R3, R6, 3;
        JSR BIN_SEARCH
        LDR R0, R6, 0;
        STR R0, R5, 3;
        BR FINISH
NEGATIVE
        ADD R6, R6, -4
        ADD R2, R0, 1;
        LDR R0, R5, 4;
        LDR R1, R5, 5;
        LDR R3, R5, 7
        STR R0, R6, 0;
        STR R1, R6, 1;
        STR R2, R6, 2;
        STR R3, R6, 3;
        JSR BIN_SEARCH
        LDR R0, R6, 0;
        STR R0, R5, 3;
        BR FINISH
FINISH
        LDR R0, R5, 0;
        LDR R1, R5, -1;
        LDR R2, R5, -2;
        LDR R3, R5, -3;
        LDR R4, R5, -4;
        LDR R7, R5, 2;
        ADD R6, R5, 3;
        LDR R5, R5, 1;
        RET


.end
