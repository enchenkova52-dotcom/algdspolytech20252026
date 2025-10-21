// poly.c
#pragma warning(disable : 4996)
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "poly.h"

#define MAX_COEF 215
#define MAX_DEG 230

static int ensure_cap(Poly* p, size_t need) {
    if (p->cap >= need) return 0;
    size_t newcap = p->cap ? p->cap * 2 : 8;
    while (newcap < need) newcap *= 2;
    Mono* tmp = realloc(p->terms, newcap * sizeof(Mono));
    if (!tmp) return -1;
    p->terms = tmp;
    p->cap = newcap;
    return 0;
}

void poly_init(Poly* p) {
    p->terms = NULL;
    p->size = 0;
    p->cap = 0;
}

void poly_free(Poly* p) {
    free(p->terms);
    p->terms = NULL;
    p->size = 0;
    p->cap = 0;
}

static void poly_normalize(Poly* p) {
    for (size_t i = 0; i < p->size; ++i) {
        for (size_t j = i + 1; j < p->size; ++j) {
            if (p->terms[i].deg == p->terms[j].deg) {
                p->terms[i].coef += p->terms[j].coef;
                p->terms[j].coef = 0;
            }
        }
    }

    size_t write = 0;
    for (size_t i = 0; i < p->size; ++i) {
        if (p->terms[i].coef != 0) {
            p->terms[write++] = p->terms[i];
        }
    }
    p->size = write;

    for (size_t i = 0; i < p->size; ++i) {
        for (size_t j = i + 1; j < p->size; ++j) {
            if (p->terms[i].deg > p->terms[j].deg) {
                Mono temp = p->terms[i];
                p->terms[i] = p->terms[j];
                p->terms[j] = temp;
            }
        }
    }
}

int poly_from_string(const char* s, Poly* out) {
    poly_init(out);
    if (!s) return -1;

    const char* p = s;
    int sign = 1;
    int expect_term = 1;

    while (*p) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        if (*p == '+') {
            sign = 1;
            p++;
            expect_term = 1;
        }
        else if (*p == '-') {
            sign = -1;
            p++;
            expect_term = 1;
        }

        if (!expect_term) return -1;

        while (isspace((unsigned char)*p)) p++;
        if (!*p) return -1;

        int64_t coef = 0;
        uint32_t deg = 0;
        int has_digits = 0;
        int has_x = 0;

        const char* num_start = p;
        while (isdigit((unsigned char)*p)) {
            if (coef > (INT64_MAX - (*p - '0')) / 10) {
                return -1;
            }
            coef = coef * 10 + (*p - '0');
            has_digits = 1;
            p++;
        }

        if (has_digits && coef > MAX_COEF) {
            return -1;
        }

        while (isspace((unsigned char)*p)) p++;

        if (*p == 'x' || *p == 'X') {
            has_x = 1;
            p++;
            deg = 1;
        }
        else if (*p == '*') {
            p++;
            while (isspace((unsigned char)*p)) p++;
            if (*p == 'x' || *p == 'X') {
                has_x = 1;
                p++;
                deg = 1;
            }
            else {
                return -1;
            }
        }

        if (has_x) {
            while (isspace((unsigned char)*p)) p++;

            if (*p == '^') {
                p++;
                while (isspace((unsigned char)*p)) p++;

                if (!isdigit((unsigned char)*p)) return -1;

                uint64_t exp_val = 0;
                while (isdigit((unsigned char)*p)) {
                    if (exp_val > (UINT64_MAX - (*p - '0')) / 10) {
                        return -1;
                    }
                    exp_val = exp_val * 10 + (*p - '0');
                    p++;
                }

                if (exp_val > MAX_DEG) {
                    return -1;
                }
                deg = (uint32_t)exp_val;
            }

            if (!has_digits) {
                coef = 1;
            }
        }
        else {
            if (!has_digits) return -1;
            deg = 0;
        }

        int64_t final_coef = coef * sign;
        if (final_coef < -MAX_COEF || final_coef > MAX_COEF) {
            return -1;
        }

        if (ensure_cap(out, out->size + 1) != 0) {
            poly_free(out);
            return -1;
        }
        out->terms[out->size].deg = deg;
        out->terms[out->size].coef = final_coef;
        out->size++;

        expect_term = 0;

        while (isspace((unsigned char)*p)) p++;

        if (*p == '+' || *p == '-') {
            expect_term = 1;
        }
        else if (*p != '\0') {
            return -1;
        }
    }

    poly_normalize(out);
    return 0;
}

char* poly_to_string(const Poly* p) {
    if (!p || p->size == 0) {
        char* zero = malloc(2);
        if (zero) strcpy(zero, "0");
        return zero;
    }

    char* result = malloc(256);
    if (!result) return NULL;
    result[0] = '\0';

    for (size_t i = 0; i < p->size; ++i) {
        Mono m = p->terms[i];
        if (m.coef == 0) continue;

        char term[64];

        if (m.deg == 0) {
            snprintf(term, sizeof(term), "%lld", (long long)m.coef);
        }
        else if (m.deg == 1) {
            if (m.coef == 1) {
                strcpy(term, "x");
            }
            else if (m.coef == -1) {
                strcpy(term, "-x");
            }
            else {
                snprintf(term, sizeof(term), "%lld*x", (long long)m.coef);
            }
        }
        else {
            if (m.coef == 1) {
                snprintf(term, sizeof(term), "x^%u", m.deg);
            }
            else if (m.coef == -1) {
                snprintf(term, sizeof(term), "-x^%u", m.deg);
            }
            else {
                snprintf(term, sizeof(term), "%lld*x^%u", (long long)m.coef, m.deg);
            }
        }

        if (i == 0) {
            strcpy(result, term);
        }
        else {
            if (term[0] == '-') {
                strcat(result, term);
            }
            else {
                strcat(result, "+");
                strcat(result, term);
            }
        }
    }

    return result;
}

int poly_add(const Poly* a, const Poly* b, Poly* res) {
    poly_init(res);

    for (size_t i = 0; i < a->size; ++i) {
        if (ensure_cap(res, res->size + 1) != 0) {
            poly_free(res);
            return -1;
        }
        res->terms[res->size++] = a->terms[i];
    }

    for (size_t i = 0; i < b->size; ++i) {
        if (ensure_cap(res, res->size + 1) != 0) {
            poly_free(res);
            return -1;
        }
        res->terms[res->size++] = b->terms[i];
    }

    poly_normalize(res);
    return 0;
}

int poly_mul(const Poly* a, const Poly* b, Poly* res) {
    poly_init(res);

    for (size_t i = 0; i < a->size; ++i) {
        for (size_t j = 0; j < b->size; ++j) {
            uint32_t deg = a->terms[i].deg + b->terms[j].deg;

            if (deg < a->terms[i].deg || deg < b->terms[j].deg) {
                poly_free(res);
                return -1;
            }

            int64_t coef = a->terms[i].coef * b->terms[j].coef;

            if (a->terms[i].coef != 0 &&
                coef / a->terms[i].coef != b->terms[j].coef) {
                poly_free(res);
                return -1;
            }

            if (ensure_cap(res, res->size + 1) != 0) {
                poly_free(res);
                return -1;
            }
            res->terms[res->size].deg = deg;
            res->terms[res->size].coef = coef;
            res->size++;
        }
    }

    poly_normalize(res);
    return 0;
}

int poly_derivative(const Poly* a, Poly* res) {
    poly_init(res);

    for (size_t i = 0; i < a->size; ++i) {
        Mono m = a->terms[i];
        if (m.deg > 0) {
            int64_t new_coef = m.coef * (int64_t)m.deg;

            if (new_coef < -MAX_COEF || new_coef > MAX_COEF) {
                poly_free(res);
                return -1;
            }

            if (ensure_cap(res, res->size + 1) != 0) {
                poly_free(res);
                return -1;
            }
            res->terms[res->size].deg = m.deg - 1;
            res->terms[res->size].coef = new_coef;
            res->size++;
        }
    }

    poly_normalize(res);
    return 0;
}