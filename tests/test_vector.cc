#include "../include/s21/s21_containers.h"
#include <gtest/gtest.h>

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
  EXPECT_EQ(int_vec[0], 1);
  EXPECT_EQ(str_vec[1], "world");

  EXPECT_EQ(int_vec.at(2), 3);
  EXPECT_THROW(int_vec.at(3), std::out_of_range);

  EXPECT_EQ(int_vec.front(), 1);
  EXPECT_EQ(int_vec.back(), 3);
}

TEST_F(VectorTest, InsertErase) {
  int_vec.insert(int_vec.begin() + 1, 10);
  EXPECT_EQ(int_vec.size(), 4);
  EXPECT_EQ(int_vec[1], 10);

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

TEST(VectorEdgeCases, EmptyVector) {
  s21::vector<int> v;

  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 0);

  EXPECT_THROW(v.front(), std::out_of_range);
  EXPECT_THROW(v.back(), std::out_of_range);
}

TEST(VectorEdgeCases, ClearVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};

  EXPECT_FALSE(v.empty());
  v.clear();
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorEdgeCases, ReserveAndShrink) {
  s21::vector<int> v = {1, 2, 3};

  v.reserve(100);
  EXPECT_GE(v.capacity(), 100);
  EXPECT_EQ(v.size(), 3);

  v.shrink_to_fit();
  EXPECT_EQ(v.capacity(), 3);
  EXPECT_EQ(v.size(), 3);
}

TEST(VectorEdgeCases, CopyAndMove) {
  s21::vector<int> original = {1, 2, 3, 4, 5};

  s21::vector<int> copy(original);
  EXPECT_EQ(copy.size(), original.size());
  EXPECT_EQ(copy[0], original[0]);

  s21::vector<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), copy.size());
  EXPECT_TRUE(original.empty());

  s21::vector<int> assigned;
  assigned = copy;
  EXPECT_EQ(assigned.size(), copy.size());

  assigned = std::move(copy);
  EXPECT_EQ(assigned.size(), moved.size());
  EXPECT_TRUE(copy.empty()); 
}

TEST(VectorEdgeCases, AllocationFailure) {
  s21::vector<int> v;

  EXPECT_THROW(v.reserve(std::numeric_limits<size_t>::max()), std::bad_alloc);

  EXPECT_TRUE(v.empty());
  v.push_back(1);
  EXPECT_EQ(v.size(), 1);
}

TEST(VectorEdgeCases, DifferentDataTypes) {
  s21::vector<double> vd = {1.1, 2.2, 3.3};
  EXPECT_DOUBLE_EQ(vd[1], 2.2);

  s21::vector<std::string> vs = {"hello", "world"};
  EXPECT_EQ(vs[0], "hello");

  struct Point {
    int x, y;
  };
  s21::vector<Point> vp = {{1, 2}, {3, 4}};
  EXPECT_EQ(vp[1].x, 3);
}

TEST(VectorEdgeCases, IteratorValidity) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.begin() + 2;

  EXPECT_EQ(*it, 3);

  v.insert(v.begin(), 0);
  it = v.begin() + 3;
  EXPECT_EQ(*it, 3);
}

TEST(VectorErase, SingleElement) {
  s21::vector<int> v = {1, 2, 3, 4, 5};

  v.erase(v.begin() + 2); 

  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 4); 
  EXPECT_EQ(v[3], 5);
}

TEST(VectorErase, FirstElement) {
  s21::vector<int> v = {10, 20, 30};

  v.erase(v.begin());

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 20);
  EXPECT_EQ(v[1], 30);
}

TEST(VectorErase, CustomObjects) {
  struct Person {
    std::string name;
    int age;
  };

  s21::vector<Person> v = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};

  v.erase(v.begin() + 1); 

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0].name, "Alice");
  EXPECT_EQ(v[0].age, 30);
  EXPECT_EQ(v[1].name, "Charlie");
  EXPECT_EQ(v[1].age, 35);
}

TEST(VectorErase, StressTest) {
  s21::vector<int> v;
  const int N = 1000; 

 
  for (int i = 0; i < N; ++i)
    v.push_back(i);

  
  for (size_t i = 0; i < v.size();) {
    if (v[i] % 2 == 0)
      v.erase(v.begin() + i);
    else
      ++i;
  }

  
  EXPECT_EQ(v.size(), N / 2);
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v[i] % 2, 1); 
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
