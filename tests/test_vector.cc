#include "s21/s21_containers.h"
#include <gtest/gtest.h>
#include <stdexcept>
class VectorTest : public testing::Test {
protected:
  s21::vector<int> int_vec;
  s21::vector<std::string> str_vec;

  void SetUp() override {
    int_vec.push_back(1);
    int_vec.push_back(2);
    int_vec.push_back(3);

    str_vec.push_back("hello");
    str_vec.push_back("world");
  }
};

TEST_F(VectorTest, Initialization) {
  s21::vector<int> empty;
  EXPECT_TRUE(empty.empty());
  EXPECT_EQ(empty.size(), 0);
}

TEST_F(VectorTest, PushBack) {
  EXPECT_EQ(int_vec.size(), 3);
  EXPECT_GE(int_vec.capacity(), 3);

  int_vec.push_back(4);
  EXPECT_EQ(int_vec.size(), 4);
  EXPECT_EQ(int_vec[3], 4);
}

TEST_F(VectorTest, ElementAccess) {
  // Operator[]
  EXPECT_EQ(int_vec[0], 1);
  EXPECT_EQ(str_vec[1], "world");

  // at() with bounds checking
  EXPECT_EQ(int_vec.at(2), 3);
  EXPECT_THROW(int_vec.at(3), std::out_of_range);

  // front()/back()
  EXPECT_EQ(int_vec.front(), 1);
  EXPECT_EQ(int_vec.back(), 3);
}

TEST_F(VectorTest, InsertErase) {
  // Insert in middle
  int_vec.insert(int_vec.begin() + 1, 10);
  EXPECT_EQ(int_vec.size(), 4);
  EXPECT_EQ(int_vec[1], 10);

  // Erase first element
  int_vec.erase(int_vec.begin());
  EXPECT_EQ(int_vec.size(), 3);
  EXPECT_EQ(int_vec[0], 10);
}

TEST_F(VectorTest, PopBack) {
  int_vec.pop_back();
  EXPECT_EQ(int_vec.size(), 2);
  EXPECT_EQ(int_vec.back(), 2);

  str_vec.pop_back();
  EXPECT_EQ(str_vec.size(), 1);
  EXPECT_EQ(str_vec.back(), "hello");
}

TEST_F(VectorTest, Swap) {
  s21::vector<int> other;
  other.push_back(10);
  other.push_back(20);

  int_vec.swap(other);

  EXPECT_EQ(int_vec.size(), 2);
  EXPECT_EQ(int_vec[0], 10);

  EXPECT_EQ(other.size(), 3);
  EXPECT_EQ(other[0], 1);
}

TEST(VectorEdgeCases, EmptyVectorOperations) {
  s21::vector<double> v;

  EXPECT_THROW(v.at(0), std::out_of_range);
  EXPECT_THROW(v.front(), std::out_of_range);
  EXPECT_THROW(v.back(), std::out_of_range);

  EXPECT_THROW(v.pop_back(), std::runtime_error);
}

TEST(VectorEdgeCases, LargeVector) {
  s21::vector<int> v;
  const int N = 10000;

  for (int i = 0; i < N; ++i) {
    v.push_back(i);
  }

  EXPECT_EQ(v.size(), N);
  EXPECT_GE(v.capacity(), N);
  EXPECT_EQ(v[9999], 9999);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
