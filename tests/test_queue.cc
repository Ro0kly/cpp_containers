#include "../include/s21/s21_containers.h"
#include <gtest/gtest.h>

class Person {
public:
    std::string name;
    int age;
    
    Person() : name(""), age(0) {}
    Person(std::string n, int a) : name(n), age(a) {}
    
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << "Person{" << p.name << ", " << p.age << "}";
    }
};

class PersonQueueTest : public ::testing::Test {
protected:
    Person alice{"Alice", 25};
    Person bob{"Bob", 30};
    Person charlie{"Charlie", 35};
    
    s21::queue<Person> queue;
};

TEST_F(PersonQueueTest, StartsEmpty) {
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(PersonQueueTest, PushAndPop) {
    queue.push(alice);
    queue.push(bob);
    
    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front(), alice); 
    
    queue.pop();
    EXPECT_EQ(queue.front(), bob);
}

TEST_F(PersonQueueTest, FrontAndBack) {
    queue.push(alice);
    queue.push(bob);
    
    EXPECT_EQ(queue.front(), alice);  
    EXPECT_EQ(queue.back(), bob);   
}

TEST_F(PersonQueueTest, Clear) {
    queue.push(alice);
    queue.push(bob);
    
    while (!queue.empty()) {
        queue.pop();
    }
    
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(PersonQueueTest, CopyQueue) {
    queue.push(alice);
    
    s21::queue<Person> copy = queue;
    EXPECT_EQ(copy.front(), alice);
    
    copy.pop();
    EXPECT_FALSE(queue.empty()); 
}

TEST_F(PersonQueueTest, MoveQueue) {
    queue.push(alice);
    
    s21::queue<Person> moved = std::move(queue);
    EXPECT_EQ(moved.front(), alice);
    EXPECT_TRUE(queue.empty());  
}

TEST_F(PersonQueueTest, MoveObjectsIntoQueue) {
    Person alice_move = std::move(alice);  
    queue.push(std::move(alice_move));    
    
    EXPECT_EQ(queue.front().name, "Alice"); 
    EXPECT_EQ(queue.front().age, 25);
}

TEST_F(PersonQueueTest, SwapTwoQueues) {
    s21::queue<Person> other_queue;
    other_queue.push(charlie);
    
    queue.push(alice);
    queue.push(bob);
    
    queue.swap(other_queue);
    
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), charlie);
    
    EXPECT_EQ(other_queue.size(), 2);
    EXPECT_EQ(other_queue.front(), alice);
    EXPECT_EQ(other_queue.back(), bob);
}

TEST_F(PersonQueueTest, CopyAssignment) {
    s21::queue<Person> other_queue;
    other_queue.push(charlie);
    
    queue = other_queue; 
    
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), charlie);
    EXPECT_EQ(other_queue.size(), 1);  
}

TEST_F(PersonQueueTest, MoveAssignment) {
    s21::queue<Person> other_queue;
    other_queue.push(charlie);
    
    queue = std::move(other_queue);  
    
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), charlie);
    EXPECT_TRUE(other_queue.empty());  
}

TEST_F(PersonQueueTest, NestedQueues) {
    s21::queue<s21::queue<Person>> queue_of_queues;
    
    s21::queue<Person> q1;
    q1.push(Person{"Alice", 25});
    
    s21::queue<Person> q2;
    q2.push(Person{"Bob", 30});
    
    queue_of_queues.push(q1);
    queue_of_queues.push(std::move(q2));
    
    EXPECT_EQ(queue_of_queues.size(), 2);
    EXPECT_EQ(queue_of_queues.front().front().name, "Alice");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}