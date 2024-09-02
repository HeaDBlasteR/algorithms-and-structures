// Copyright 2024 Marina Usova

#include <gtest.h>
#include "../lib_easy_example/easy_example.h"

#define EPSILON 0.000001

TEST(TestEasyExampleLib, can_div) {
  // Arrange
  int x = 10;
  int y = 2;

  // Act & Assert
  ASSERT_NO_THROW(division(x, y));
}

TEST(TestEasyExampleLib, can_div_correctly) {
    // Arrange
    int x = 6;
    int y = 2;

    // Act
    int actual_result = division(x, y);

    // Assert
    int expected_result = 3;
    EXPECT_EQ(expected_result, actual_result);
}

TEST(TestEasyExampleLib, can_div_correctly_with_remainder) {
    // Arrange
    int x = 5;
    int y = 4;

    // Act
    float actual_result = division(x, y);

    // Assert
    float expected_result = 1.25;
    EXPECT_NEAR(expected_result, actual_result, EPSILON);
}

TEST(TestEasyExampleLib, throw_when_try_div_by_zero) {
  // Arrange
  int x = 10;
  int y = 0;

  // Act & Assert
  ASSERT_ANY_THROW(division(x, y));
}
