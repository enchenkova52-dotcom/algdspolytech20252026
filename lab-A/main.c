// main.c
#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "poly.h"

#define MAX_LINE 1024

int main() {
    FILE* fin = fopen("input.txt", "r");
    if (!fin) {
        printf("Cannot open input.txt\n");
        return 1;
    }

    char line1[MAX_LINE], line2[MAX_LINE];

    if (!fgets(line1, sizeof(line1), fin) || !fgets(line2, sizeof(line2), fin)) {
        fclose(fin);
        printf("Invalid input file\n");
        return 1;
    }
    fclose(fin);

    line1[strcspn(line1, "\n")] = 0;
    line2[strcspn(line2, "\n")] = 0;

    Poly p1, p2, sum, prod, deriv;

    if (poly_from_string(line1, &p1) != 0) {
        printf("Error parsing first polynomial\n");
        return 1;
    }

    if (poly_from_string(line2, &p2) != 0) {
        poly_free(&p1);
        printf("Error parsing second polynomial\n");
        return 1;
    }

    if (poly_add(&p1, &p2, &sum) != 0) {
        printf("Error in addition\n");
        return 1;
    }

    if (poly_mul(&p1, &p2, &prod) != 0) {
        printf("Error in multiplication\n");
        return 1;
    }

    if (poly_derivative(&p1, &deriv) != 0) {
        printf("Error in derivative\n");
        return 1;
    }

    char* s_sum = poly_to_string(&sum);
    char* s_prod = poly_to_string(&prod);
    char* s_deriv = poly_to_string(&deriv);

    FILE* fout = fopen("output.txt", "w");
    if (fout) {
        fprintf(fout, "%s\n%s\n%s\n", s_sum, s_prod, s_deriv);
        fclose(fout);
        printf("\nResults written to output.txt\n");
    }
    else {
        printf("Cannot create output.txt\n");
    }

    free(s_sum);
    free(s_prod);
    free(s_deriv);
    poly_free(&p1);
    poly_free(&p2);
    poly_free(&sum);
    poly_free(&prod);
    poly_free(&deriv);

    return 0;
}