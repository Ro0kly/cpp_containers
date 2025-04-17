#include <functional>
#include <initializer_list>
#include <memory>
#include <utility>

template <typename Key, typename T> class map {
public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using key_compare = std::less<Key>;
  using allocator_type = std::allocator<value_type>;

private:
  struct Node {
    value_type data;
    Node *left;
    Node *right;
    Node *parent;
    bool is_black;

    Node(const value_type &val, Node *p = nullptr, bool black = false)
        : data(val), parent(p), left(nullptr), right(nullptr), is_black(black) {
    }
  };

  template <typename K, typename H> class iterator {
    using Node = typename map<K, H>::Node; // Forward declare map's Node
    Node *current;

  public:
    using value_type = std::pair<const K, H>;
    using reference = value_type &;
    using pointer = value_type *;
    using iterator_category = std::bidirectional_iterator_tag;

    explicit iterator(Node *node = nullptr) : current(node) {}

    reference operator*() const { return current->data; }
    pointer operator->() const { return &current->data; }

    iterator &operator++() {
      if (current->right) {
        current = current->right;
        while (current->left)
          current = current->left;
      } else {
        Node *p = current->parent;
        while (p && current == p->right) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const {
      return current == other.current;
    }
    bool operator!=(const iterator &other) const {
      return current != other.current;
    }
  };

  using node_allocator = std::allocator<Node>;
  using alloc_traits = std::allocator_traits<node_allocator>;

  Node *root;
  size_type count;
  [[no_unique_address]] key_compare comp;
  [[no_unique_address]] node_allocator alloc;

  Node *create_node(const value_type &val, Node *parent = nullptr,
                    bool black = false) {
    Node *node = alloc_traits::allocate(alloc, 1);
    try {
      alloc_traits::construct(alloc, node, val, parent, black);
      return node;
    } catch (...) {
      alloc_traits::deallocate(alloc, node, 1);
      throw;
    }
  }

  void destroy_node(Node *node) noexcept {
    alloc_traits::destroy(alloc, node);
    alloc_traits::deallocate(alloc, node, 1);
  }

  void clear_recursive(Node *node) noexcept {
    if (node) {
      clear_recursive(node->left);
      clear_recursive(node->right);
      destroy_node(node);
    }
  }

  Node *copy_tree(Node *other_node, Node *parent = nullptr) {
    if (!other_node)
      return nullptr;
    Node *new_node =
        create_node(other_node->data, parent, other_node->is_black);
    new_node->left = copy_tree(other_node->left, new_node);
    new_node->right = copy_tree(other_node->right, new_node);
    return new_node;
  }

public:
  map() noexcept : root(nullptr), count(0) {}

  template <typename InputIt> map(InputIt first, InputIt last) : map() {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  map(std::initializer_list<value_type> init) : map(init.begin(), init.end()) {}

  map(const map &other)
      : root(nullptr), count(0), comp(other.comp),
        alloc(
            alloc_traits::select_on_container_copy_construction(other.alloc)) {
    root = copy_tree(other.root);
    count = other.count;
  }

  map(map &&other) noexcept
      : root(other.root), count(other.count), comp(std::move(other.comp)),
        alloc(std::move(other.alloc)) {
    other.root = nullptr;
    other.count = 0;
  }

  ~map() { clear_recursive(root); }

  map &operator=(const map &other) {
    if (this != &other) {
      clear_recursive(root);
      comp = other.comp;
      if (alloc_traits::propagate_on_container_copy_assignment::value) {
        alloc = other.alloc;
      }
      root = copy_tree(other.root);
      count = other.count;
    }
    return *this;
  }

  map &operator=(map &&other) noexcept {
    if (this != &other) {
      clear_recursive(root);
      comp = std::move(other.comp);
      if (alloc_traits::propagate_on_container_move_assignment::value) {
        alloc = std::move(other.alloc);
      }
      root = other.root;
      count = other.count;
      other.root = nullptr;
      other.count = 0;
    }
    return *this;
  }

  map &operator=(std::initializer_list<value_type> ilist) {
    clear_recursive(root);
    root = nullptr;
    count = 0;
    for (const auto &item : ilist) {
      insert(item);
    }
    return *this;
  }

  std::pair<iterator<Key, T>, bool> insert(const value_type &value) {
    Node *parent = nullptr;
    Node *current = root;
    bool is_left = false;

    while (current) {
      parent = current;
      if (comp(value.first, current->data.first)) {
        current = current->left;
        is_left = true;
      } else if (comp(current->data.first, value.first)) {
        current = current->right;
        is_left = false;
      } else {
        return {iterator<Key, T>(current), false};
      }
    }

    Node *new_node = create_node(value, parent);

    if (!parent) {
      root = new_node;
    } else if (is_left) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    count++;
    return {iterator<Key, T>(new_node), true};
  }
  size_type size() const noexcept { return count; }
  bool empty() const noexcept { return count == 0; }
  void clear() noexcept {
    clear_recursive(root);
    root = nullptr;
    count = 0;
  }
};
