#include <gtest/gtest.h>
#include "matrix_ops.h"

TEST(MatrixCreateTest, Create3x3) 
{
    int** m = matrix_create(3, 3);
    ASSERT_NE(m, nullptr);
    for (int i = 0; i < 3; i++) 
    {
        ASSERT_NE(m[i], nullptr);
        for (int j = 0; j < 3; j++) 
        {
            EXPECT_EQ(m[i][j], 0);
        }
    }
    matrix_delete(m, 3);
}

TEST(MatrixCreateTest, CreateEmpty) 
{
    int** m = matrix_create(0, 0);
    EXPECT_EQ(m, nullptr);
}

TEST(MatrixCreateTest, Create1x1) 
{
    int** m = matrix_create(1, 1);
    ASSERT_NE(m, nullptr);
    EXPECT_EQ(m[0][0], 0);
    matrix_delete(m, 1);
}

TEST(MatrixFillTest, FillWithValue) 
{
    int** m = matrix_create(2, 3);
    matrix_fill(m, 2, 3, 67);
    for (int i = 0; i < 2; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            EXPECT_EQ(m[i][j], 67);
        }
    }
    matrix_delete(m, 2);
}

TEST(MatrixFillTest, FillNullptr) 
{
    matrix_fill(nullptr, 2, 3, 5);
    SUCCEED();
}

TEST(MatrixPrintTest, PrintNonEmpty) 
{
    int** m = matrix_create(2, 2);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4;
    testing::internal::CaptureStdout();
    matrix_print(m, 2, 2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("1"), std::string::npos);
    EXPECT_NE(output.find("4"), std::string::npos);
    matrix_delete(m, 2);
}

TEST(MatrixPrintTest, PrintNullptr) 
{
    testing::internal::CaptureStdout();
    matrix_print(nullptr, 0, 0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("empty"), std::string::npos);
}

TEST(MatrixDeleteTest, DeleteAndNullify) 
{
    int** m = matrix_create(2, 2);
    ASSERT_NE(m, nullptr);
    matrix_delete(m, 2);
    EXPECT_EQ(m, nullptr);
}

TEST(MatrixDeleteTest, DeleteNullptr)
{
    int** m = nullptr;
    matrix_delete(m, 0);
    EXPECT_EQ(m, nullptr);
}

TEST(MatrixTransposeTest, Square2x2) 
{
    int** m = matrix_create(2, 2);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4;
    int** t = matrix_transpose(m, 2, 2);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t[0][0], 1);
    EXPECT_EQ(t[0][1], 3);
    EXPECT_EQ(t[1][0], 2);
    EXPECT_EQ(t[1][1], 4);
    matrix_delete(m, 2);
    matrix_delete(t, 2);
}

TEST(MatrixTransposeTest, Rectangular3x2) 
{
    int** m = matrix_create(3, 2);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4;
    m[2][0] = 5; m[2][1] = 6;
    int** t = matrix_transpose(m, 3, 2);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t[0][0], 1);
    EXPECT_EQ(t[0][1], 3);
    EXPECT_EQ(t[0][2], 5);
    EXPECT_EQ(t[1][0], 2);
    EXPECT_EQ(t[1][1], 4);
    EXPECT_EQ(t[1][2], 6);
    matrix_delete(m, 3);
    matrix_delete(t, 2);
}

TEST(MatrixTransposeTest, RowVector1xN) 
{
    int** m = matrix_create(1, 3);
    m[0][0] = 10; m[0][1] = 20; m[0][2] = 30;
    int** t = matrix_transpose(m, 1, 3);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t[0][0], 10);
    EXPECT_EQ(t[1][0], 20);
    EXPECT_EQ(t[2][0], 30);
    matrix_delete(m, 1);
    matrix_delete(t, 3);
}

TEST(MatrixTransposeTest, ColVectorNx1) 
{
    int** m = matrix_create(3, 1);
    m[0][0] = 7;
    m[1][0] = 8;
    m[2][0] = 9;
    int** t = matrix_transpose(m, 3, 1);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t[0][0], 7);
    EXPECT_EQ(t[0][1], 8);
    EXPECT_EQ(t[0][2], 9);
    matrix_delete(m, 3);
    matrix_delete(t, 1);
}

TEST(MatrixTransposeTest, Nullptr) 
{
    int** t = matrix_transpose(nullptr, 0, 0);
    EXPECT_EQ(t, nullptr);
}

TEST(MatrixTransposeTest, OriginalNotModified) 
{
    int** m = matrix_create(2, 3);
    m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
    m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;
    int** t = matrix_transpose(m, 2, 3);
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[0][2], 3);
    EXPECT_EQ(m[1][0], 4);
    EXPECT_EQ(m[1][1], 5);
    EXPECT_EQ(m[1][2], 6);
    matrix_delete(m, 2);
    matrix_delete(t, 3);
}