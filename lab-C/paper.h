#ifndef PAPER_H
#define PAPER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct Paper Paper;

	Paper* create_paper(int m, int n);
	void destroy_paper(Paper* paper);

	void remove_cells(Paper* paper, int k, int* x_coords, int* y_coords);
	int count_connected_components(Paper* paper);

	bool is_valid_cell(Paper* paper, int row, int col);

#ifdef __cplusplus
}
#endif

#endif
