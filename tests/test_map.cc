#include "../include/s21/s21_containers.h"
#include <gtest/gtest.h>

class Person {
public:
    std::string name;
    int age;
    bool is_student;

    Person() : name(""), age(0), is_student(false) {} 
    
    Person(std::string name, int age, bool is_student) 
        : name(std::move(name)), age(age), is_student(is_student) {}
    
    bool operator==(const Person& other) const {
        return name == other.name && 
               age == other.age && 
               is_student == other.is_student;
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << "Person{name=" << p.name 
                 << ", age=" << p.age
                 << ", is_student=" << (p.is_student ? "true" : "false") << "}";
    }
};

TEST(MyMapTest, ExistingKeyAccess) {
    s21::map<std::string, Person> map;
    
    map.insert({"alice", Person{"Alice", 25, true}});
    
    EXPECT_EQ(map["alice"].name, "Alice"); 
    EXPECT_EQ(map.size(), 1);              
}

TEST(MyMapTest, MissingKeyThrows) {
    s21::map<int, std::string> map;
    
    EXPECT_THROW(map[42], std::out_of_range);
    EXPECT_TRUE(map.empty()); 
}

TEST(MyMapTest, InsertValidates) {
    s21::map<std::string, int> prices;
    
    auto [it1, inserted1] = prices.insert({"apple", 50});
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(it1->second, 50);
    
    auto [it2, inserted2] = prices.insert({"apple", 100});
    EXPECT_FALSE(inserted2);
    EXPECT_EQ(it2->second, 50);  
}

TEST(MyMapTest, UpdateExisting) {
    s21::map<int, Person> db;
    db.insert({1, Person{"Alice", 25, true}});
    
    db[1] = Person{"Alice Updated", 26, false};
    
    EXPECT_EQ(db[1].name, "Alice Updated");
    EXPECT_EQ(db.size(), 1);
}

TEST(MyMapTest, AccessMissingKeyThrows) {
    s21::map<int, double> map;
    map.insert({10, 3.14});
    
    EXPECT_THROW(map[20], std::out_of_range);
}

TEST(MyMapTest, EraseElement) {
    s21::map<char, bool> map;
    map.insert({'A', true});
    
    map.erase(map.begin());
    
    EXPECT_TRUE(map.empty());
    EXPECT_THROW(map['A'], std::out_of_range); 
}

TEST(MyMapTest, ClearMap) {
    s21::map<int, std::string> map;
    map.insert({100, "hundred"});
    
    map.clear();
    
    EXPECT_TRUE(map.empty());
    EXPECT_THROW(map[100], std::out_of_range); 
}

TEST(MyMapTest, IteratorBasic) {
    s21::map<int, char> map;
    map.insert({3, 'C'});
    map.insert({1, 'A'});
    
    auto it = map.begin();
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ((++it)->first, 3);
}

TEST(MyMapTest, InsertDuplicate) {
    s21::map<std::string, int> map;
    map.insert({"test", 1});
    
    auto result = map.insert({"test", 2});
    
    EXPECT_FALSE(result.second); 
    EXPECT_EQ(map["test"], 1); 
}

TEST(MyMapTest, EraseFirstElement) {
    s21::map<int, char> map;
    map.insert({2, 'B'});
    map.insert({1, 'A'});
    map.insert({3, 'C'});
    
    map.erase(map.begin()); 
    
    EXPECT_EQ(map.size(), 2);
    EXPECT_THROW(map[1], std::out_of_range);
    EXPECT_EQ(map[2], 'B'); 
    EXPECT_EQ(map[3], 'C');
}

TEST(MyMapTest, EraseLastElement) {
    s21::map<std::string, int> map;
    map.insert({"A", 1});
    map.insert({"B", 2});
    
    auto it = map.begin();
    ++it; 
    
    map.erase(it);
    
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map["A"], 1);
    EXPECT_THROW(map["B"], std::out_of_range);
}

TEST(MyMapTest, EraseNodeWithTwoChildren) {
    s21::map<int, std::string> map;
    
    map.insert({10, "A"}); 
    map.insert({5, "B"});  
    map.insert({15, "C"}); 
    map.insert({12, "D"}); 

    auto root_it = map.begin();
    ++root_it; 
    ++root_it; 

    ASSERT_EQ(root_it->first, 12);
    ASSERT_EQ(root_it->second, "D");

    map.erase(root_it);

    auto new_root_it = map.begin();
    ++new_root_it; ++new_root_it;
    EXPECT_EQ(new_root_it->first, 15);
    EXPECT_EQ(new_root_it->second, "C");

    EXPECT_EQ(map.size(), 3);

    auto it = map.begin();
    EXPECT_EQ(it->first, 5);   
    EXPECT_EQ((++it)->first, 10); 
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}