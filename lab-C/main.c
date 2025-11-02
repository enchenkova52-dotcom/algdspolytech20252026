#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>

#include "paper.h"

int main() {
    int m, n, k;

    printf("Введите размеры листа (M N): ");
    scanf("%d %d", &m, &n);

    printf("Введите количество удаленных клеток K: ");
    scanf("%d", &k);

    int* x_coords = (int*)malloc(k * sizeof(int));
    int* y_coords = (int*)malloc(k * sizeof(int));

    printf("Введите координаты удаленных клеток (x y), начиная с 1:\n");
    for (int i = 0; i < k; i++) {
        scanf("%d %d", &x_coords[i], &y_coords[i]);

        x_coords[i] = x_coords[i] - 1;
        y_coords[i] = y_coords[i] - 1;
    }

    Paper* paper = create_paper(m, n);
    if (!paper) {
        printf("Ошибка создания листа!\n");
        free(x_coords);
        free(y_coords);
        return 1;
    }

    remove_cells(paper, k, x_coords, y_coords);
    int components = count_connected_components(paper);

    printf("Количество связных кусков: %d\n", components);

    destroy_paper(paper);
    free(x_coords);
    free(y_coords);

    return 0;
}
