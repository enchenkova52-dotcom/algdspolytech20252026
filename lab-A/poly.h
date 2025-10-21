// poly.h
#ifndef POLY_H
#define POLY_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint32_t deg;
	int64_t coef;
} Mono;

typedef struct {
	Mono* terms;
	size_t size;
	size_t cap;
} Poly;

void poly_init(Poly* p);
void poly_free(Poly* p);
int poly_from_string(const char* s, Poly* out);
char* poly_to_string(const Poly* p);
int poly_add(const Poly* a, const Poly* b, Poly* res);
int poly_mul(const Poly* a, const Poly* b, Poly* res);
int poly_derivative(const Poly* a, Poly* res);

#endif