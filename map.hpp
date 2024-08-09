#pragma once
template <class Key, class Value> class Map {

public:
    class Iterator;
    class ConstIterator;

    struct Node
    {
        Node* left;
        Node* right;
        Node* parent;
        std::pair<Key, Value> data;
        Node& operator=(Node other)
        {
            left = other.left;
            right = other.right;
            parent = other.parent;
            data.first = other.data.first;
            data.second = other.data.second;
            return *this;
        }
    };

private:
    Node* root;
    size_t size_;

public:
    Map() : root(nullptr), size_(0) {}

    Map(const Map& other) :root(nullptr), size_(0)
    {
        copy(other.root);
    }

    void copy(Node* other)
    {
        if (other == nullptr)
        {
            return;
        }

        std::cout << other->data.first;
        push(other->data.first, other->data.second);
        copy(other->left);
        copy(other->right);
        return;
    }
    
    Map(Map&& other)
    {
        std::swap(size_, other.size_);
        std::swap(root, other.root);
        other.size_ = 0;
        other.root = nullptr;
    };

    Node* find_by_node(Node* node, const Key& key)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (node->data.first == key)
        {

            return node;
        }

        if (key > node->data.first)
        {
            return find_by_node(node->left, key);
        }

        if (key < node->data.first)
        {
            return find_by_node(node->right, key);
        }
    }

    const Node* find_by_node(Node* node, const Key& key) const
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (node->data.first == key)
        {
            return node;
        }

        if (key > node->data.first)
        {
            return find_by_node(node->left, key);
        }

        if (key < node->data.first)
        {
            return find_by_node(node->right, key);
        }
    }

    Node* find(const Key& key) { return find_by_node(root, key); }

    const Node* find(const Key& key) const { return find_by_node(root, key); }

    void push(const Key& key, const Value& value)
    {
        size_++;
        if (root == nullptr)
        {
            root = new Node{ nullptr, nullptr, nullptr, {key, value} };
            return;
        }

        Node* current = root;
        while (current != nullptr)
        {
            if (current->data.first > key)
            {
                if (!current->right)
                {
                    current->right = new Node{ nullptr, nullptr, current, {key, value} };
                    break;
                }

                else
                {
                    current = current->right;
                }
            }

            else
            {
                if (!current->left)
                {
                    current->left = new Node{ nullptr, nullptr, current, {key, value} };
                    break;
                }

                else
                {
                    current = current->left;
                }
            }
        }

    };

    Map& operator=(const Map& other)
    {
        Map tmp{ other };
        std::swap(size_, tmp.size_);
        std::swap(root, tmp.root);
        return *this;
    };

    Map& operator=(Map&& other)
    {
        Map tmp{ std::move(other) };
        std::swap(size_, tmp.size_);
        std::swap(root, tmp.root);
        return *this;
    };

    ~Map()
    {
        clear();
        root = nullptr;
    };

    Iterator begin()
    {
        Node* current = root;

        while (current && current->left)
        {
            current = current->left;
        }

        return Iterator{ current };
    };

    ConstIterator begin() const
    {
        Node* current = root;

        while (current && current->left)
        {
            current = current->left;
        }

        return ConstIterator{ current };
    };

    Iterator end() { return Iterator{ nullptr }; };

    ConstIterator end() const { return ConstIterator(nullptr); };

    std::size_t size() const { return size_; };

    bool contains(const Key& key) { return find(key); };

    const Value& operator[](const Key& key) const
    {
        if (find(key))
        {
            return find(key)->data.second;
        }

        else
        {
            throw std::out_of_range("Out of range");
        }
    };

    Value& operator[](const Key& key)
    {

        if (contains(key) == 0)
        {
            push(key, {});
        }
        return find(key)->data.second;
    }

    bool erase(const Key& key)
    {
        Node* found = find(key);

        if (found == nullptr)
        {
            return false;
        }

        size_--;

        if (found->left == nullptr && found->right == nullptr)
        {
            if (found->parent->left == found)
            {
                found->parent->left = nullptr;
            }

            else
            {
                found->parent->right = nullptr;
            }
            found->parent = nullptr;
            delete found;
            return true;
        }

        if (found->left == nullptr && found->right != nullptr)
        {
            if (found == root)
            {
                root = found->right;
            }

            found->right->parent = found->parent;
            found->parent->right = found->right;

            delete found;

            return true;
        }

        if (found->left != nullptr && found->right == nullptr)
        {

            if (found == root) {
                root = found->left;
            }
            found->left->parent = found->parent;
            found->parent = found->left->parent;
            delete found;

            return true;
        }
        if (found->left != nullptr && found->right != nullptr)
        {

            Node* successor = found->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            std::swap(found->data, successor->data);

            if (successor->right != nullptr) {
                successor->parent->left = successor->right;
            }
            delete successor;
            return true;
        }
    }

    void swap(Map& other)
    {
        Node* Temp;
        Node* Temp2;
        Temp = root;
        Temp2 = other.root;
        root = Temp2;
        other.root = Temp;
        size_t size1 = size_;
        size_t size2 = other.size_;
        size_ = size2;
        other.size_ = size1;
    };

    void clear()
    {
        size_ = 0;
        del(root);
        root = nullptr;
    };

    void del(Node* nod)
    {
        if (nod == nullptr)
        {
            return;
        }
        del(nod->left);
        del(nod->right);
        delete nod;
    }

    // // Возвращает итератор на первый элемент который не меньше чем переданный
    // // ключ. [O(h)]
    Node* find_lower_elem_by_node(Node* nodeT, const Key& key)
    {
        if (nodeT->left == nullptr && nodeT->right == nullptr)
        {


            return nodeT;

        }
        if (nodeT->data.first < key)
        {
            return find_lower_elem_by_node(nodeT->left, key);
        }
        if (nodeT->data.first > key)
        {
            return find_lower_elem_by_node(nodeT->right, key);
        }
        return nodeT;
    }

    Node* find_lower_elem(const Key& key)
    {
        Node* cur = find(key);
        if (cur == nullptr)
        {
            return find_lower_elem_by_node(root, key);
        }
        return cur;
    }

    // Возвращает итератор на первый элемент который не меньше чем переданный
    // ключ. [O(h)]
    Iterator lower_bound(const Key& key)
    {
        Node* cur = find_lower_elem(key);

        return Iterator{ cur };
    }


    class Iterator
    {
    public:
        // Заполнить
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        friend Map;
        Node* p;
        Iterator(Node* pi) : p(pi) {}
        // Инкремент. Движение к следующему элементу.
        Iterator& operator++()
        {
            if (p->right)
            {
                p = p->right;
                while (p && p->left)
                {
                    p = p->left;
                }
            }
            else
            {
                while (p->data.first < p->parent->data.first)
                {
                    p = p->parent;
                }
                p = p->parent;
            }
            return *this;
        };

        //   // Декремент. Движение к предыдущему элементу.
        Iterator& operator--()
        {
            if (p->left)
            {
                while (p && p->right)
                {
                    p = p->right;
                }
            }
            else
            {
                while (p->value > p->parent->value)
                {
                    p = p->parent;
                }
                p = p->parent;
            }
            return *this;
        };

        //   // Проверка на неравенство двух итераторов.
        bool operator!=(const Iterator& other) { return p != other.p; };

        //   // Разыменовывает указатель: std::cout << it->second;
        std::pair<Key, Value>& operator->()
        {
            std::pair<Key, Value> temp = { p->key, p->value };
            return *temp;
        };

        // Возвращает значение итератора: *it = {"travel", 42};
        std::pair<Key, Value>& operator*()
        {
            // std::pair<Key, Value> temp = {p->key, p->value};
            return p->data;
        };
    };
    class ConstIterator
    {
        Iterator it;

    public:
        // Заполнить
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
        ConstIterator(Node* n) : it{ n } {}

        ConstIterator& operator++()
        {
            ++it;
            return *this;
        }
        ConstIterator& operator--()
        {
            --it;
            return *this;
        }

        bool operator!=(const ConstIterator& other) { return it != other.it; }

        bool operator==(const ConstIterator& other) { return !(it != other.it); }

        // единственное отличие от Iterator, что возвращает константную ссылку
        const std::pair<Key, Value>& operator*() { return *it; }

        const std::pair<Key, Value>& operator->() { return it->pair; }
    };
};