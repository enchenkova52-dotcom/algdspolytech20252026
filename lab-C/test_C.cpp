#include <gtest/gtest.h>

#include "paper.h"
#include "pch.h"

TEST(PaperTest, CreatePaper_ValidDimensions_no1) {
    Paper* paper = create_paper(3, 4);
    ASSERT_NE(paper, nullptr);
    destroy_paper(paper);
}

TEST(PaperTest, CreatePaper_ZeroRows_no2) {
    Paper* paper = create_paper(0, 4);
    EXPECT_EQ(paper, nullptr);
}

TEST(PaperTest, CreatePaper_ZeroCols_no3) {
    Paper* paper = create_paper(3, 0);
    EXPECT_EQ(paper, nullptr);
}

TEST(PaperTest, CreatePaper_NegativeDimensions_no4) {
    Paper* paper = create_paper(-1, 4);
    EXPECT_EQ(paper, nullptr);
}

TEST(PaperTest, IsValidCell_ValidCell_no5) {
    Paper* paper = create_paper(3, 3);
    bool result = is_valid_cell(paper, 1, 1);
    EXPECT_TRUE(result);
    destroy_paper(paper);
}

TEST(PaperTest, IsValidCell_OutOfBounds_no6) {
    Paper* paper = create_paper(3, 3);
    bool result = is_valid_cell(paper, 5, 5);
    EXPECT_FALSE(result);
    destroy_paper(paper);
}

TEST(PaperTest, IsValidCell_RemovedCell_no7) {
    Paper* paper = create_paper(3, 3);
    int x_coords[] = { 1 };
    int y_coords[] = { 1 };
    remove_cells(paper, 1, x_coords, y_coords);
    bool result = is_valid_cell(paper, 1, 1);
    EXPECT_FALSE(result);
    destroy_paper(paper);
}

TEST(PaperTest, RemoveCells_SingleCell_no8) {
    Paper* paper = create_paper(3, 3);
    int x_coords[] = { 1 };
    int y_coords[] = { 1 };

    remove_cells(paper, 1, x_coords, y_coords);

    EXPECT_FALSE(is_valid_cell(paper, 1, 1));
    EXPECT_TRUE(is_valid_cell(paper, 0, 0));
    destroy_paper(paper);
}

TEST(PaperTest, RemoveCells_MultipleCells_no9) {
    Paper* paper = create_paper(3, 3);
    int x_coords[] = { 0, 1, 2 };
    int y_coords[] = { 0, 1, 2 };

    remove_cells(paper, 3, x_coords, y_coords);

    EXPECT_FALSE(is_valid_cell(paper, 0, 0));
    EXPECT_FALSE(is_valid_cell(paper, 1, 1));
    EXPECT_FALSE(is_valid_cell(paper, 2, 2));
    destroy_paper(paper);
}

TEST(PaperTest, RemoveCells_OutOfBounds_no10) {
    Paper* paper = create_paper(2, 2);
    int x_coords[] = { 5, -1 };
    int y_coords[] = { 5, -1 };

    remove_cells(paper, 2, x_coords, y_coords);

    EXPECT_TRUE(is_valid_cell(paper, 0, 0));
    EXPECT_TRUE(is_valid_cell(paper, 1, 1));
    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_EmptyPaper_no11) {
    Paper* paper = create_paper(3, 3);
    int components = count_connected_components(paper);
    EXPECT_EQ(components, 1);
    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_AllCellsRemoved_no12) {
    Paper* paper = create_paper(2, 2);
    int x_coords[] = { 0, 0, 1, 1 };
    int y_coords[] = { 0, 1, 0, 1 };

    remove_cells(paper, 4, x_coords, y_coords);
    int components = count_connected_components(paper);

    EXPECT_EQ(components, 0);
    destroy_paper(paper);
}
TEST(PaperTest, CountComponents_TwoComponents_no13) {
    Paper* paper = create_paper(3, 3);

    int x_coords[] = { 0, 1, 2 };
    int y_coords[] = { 1, 1, 1 };

    remove_cells(paper, 3, x_coords, y_coords);
    int components = count_connected_components(paper);
    EXPECT_EQ(components, 2);
    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_FourComponents_no14) {
    Paper* paper = create_paper(4, 4);

    int x_coords[] = { 1, 1, 2, 2 };
    int y_coords[] = { 1, 2, 1, 2 };

    remove_cells(paper, 4, x_coords, y_coords);
    int components = count_connected_components(paper);

    EXPECT_EQ(components, 1);
    destroy_paper(paper);
}
TEST(PaperTest, CountComponents_SingleCellPaper_no15) {
    Paper* paper = create_paper(1, 1);
    int components = count_connected_components(paper);
    EXPECT_EQ(components, 1);
    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_SingleRowPaper_no16) {
    Paper* paper = create_paper(1, 5);
    int x_coords[] = { 0, 0 };
    int y_coords[] = { 1, 3 };

    remove_cells(paper, 2, x_coords, y_coords);
    int components = count_connected_components(paper);

    EXPECT_EQ(components, 3);
    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_NullPaper_no17) {
    int components = count_connected_components(nullptr);
    EXPECT_EQ(components, 0);
}

TEST(PaperTest, RemoveCells_NullParameters_no18) {
    Paper* paper = create_paper(2, 2);

    remove_cells(nullptr, 1, nullptr, nullptr);
    remove_cells(paper, 0, nullptr, nullptr);

    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_CheckerboardPattern_no19) {
    Paper* paper = create_paper(4, 4);
    int x_coords[8], y_coords[8];
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((i + j) % 2 == 0) {
                x_coords[k] = i;
                y_coords[k] = j;
                k++;
            }
        }
    }

    remove_cells(paper, k, x_coords, y_coords);
    int components = count_connected_components(paper);

    EXPECT_EQ(components, 8);
    destroy_paper(paper);
}

TEST(PaperTest, CountComponents_IsolatedCell_no20) {
    Paper* paper = create_paper(3, 3);
    int x_coords[] = { 0, 0, 0, 1, 1, 1, 2, 2 };
    int y_coords[] = { 0, 1, 2, 0, 1, 2, 0, 1 };

    remove_cells(paper, 8, x_coords, y_coords);
    int components = count_connected_components(paper);

    EXPECT_EQ(components, 1);
    destroy_paper(paper);
}
