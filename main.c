#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef BIGNUM_H
#define BIGNUM_H
#include "bignum.h"
#endif
#ifndef LIST_H
#define LIST_H
#include "list.h"
#endif

/* Get num from input file. */
void readnum(list_t *num, FILE *input);

/* Calculate Ans = C(N,M). */
void combination(list_t *numAns, list_t *numN, list_t *numM);

int main(int argc, char *argv[])
{
    int line = 1;
    char *outputName;
    FILE *input, *output;
    list_t *numN, *numM, *numAns, *numAcc;

    if (argc < 2){
        fprintf(stderr, "Missing paramerter\n");
        abort();
    }
    else
        input = fopen(argv[1], "r");

    /* Add file extension. */
    outputName = malloc(sizeof(char) * (strlen(argv[1]) + 5));
    strcpy(outputName, argv[1]);
    strcat(outputName, ".out");
    output = fopen(outputName, "w");

    numN = list_init();
    numM = list_init();
    numAns = list_init();
    numAcc = list_init();
    list_append(numAcc, 0);

    while (1){
        readnum(numN, input);
        readnum(numM, input);

        /* Stop when N = 0 and M = 0. */
        if (numN->len == 1 && numN->tail->item == 0 && 
                numM->len == 1 && numM->tail->item == 0)
            break;

        combination(numAns, numN, numM);
        printf("%i\n", line);

        /* Add odd lines and subtract even lines. */
        if (line % 2 == 1)
            num_add(numAcc, numAcc, numAns);
        else
            num_sub(numAcc, numAcc, numAns);

        line++;

        list_empty(numN);
        list_empty(numM);
        list_empty(numAns);
    }

    /* Print answer */
    if (numAcc->len == 1 && numAcc->tail->item == 0)
        fprintf(output, "0");
    else {
        if (numAcc->head->item == 1)
            fprintf(output, "-");
        list_fprintrev(output, numAcc, "");
    }

    /* Clean up. */
    free(outputName);
    list_free(numN);
    list_free(numM);
    list_free(numAns);
    list_free(numAcc);

    fclose(input);
    fclose(output);
    return 0;
}

void readnum(list_t *num, FILE *input)
{
    int c;

    /* Ignore non numbers except '-'. */
    while (!(isdigit((c = fgetc(input)))) && c != '-');
    if (c == '-')
        num->head->item = 1;
    else
        list_insert(num, 0, c - '0');
    while (isdigit((c = fgetc(input))))
        list_insert(num, 0, c - '0');
}

void combination(list_t *numAns, list_t *numN, list_t *numM)
{
    list_t *numA, *numB, *numC, *num1;

    numA = list_init();
    numB = list_init();
    numC = list_init();
    num1 = list_init();
    list_append(num1, 1);

    if (numN->head->item == 1){
        fprintf(stderr, "Factorial of negative number\n");
        abort();
    }

    /* Eliminate unnecessary multiplications. */
    num_sub(numA, numN, numM);
    num_add(numA, numA, num1);
    if (numN->tail->item == 0 && numN->len == 1){
        list_empty(numB);
        list_append(numB, 1);
    } else {
        list_copy(numC, numN);
        list_copy(numB, numN);
        while (num_cmp(numC, numA) == 1){
            num_sub(numC, numC, num1);
            num_mul(numB, numB, numC);
        }
    }

    /* Final division. */
    num_fct(numC, numM);
    num_div(numAns, numB, numC);

    list_free(numA);
    list_free(numB);
    list_free(numC);
    list_free(num1);
}
