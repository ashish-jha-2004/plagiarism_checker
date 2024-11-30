#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <memory>

#define MAX_SIZE 10
struct node_t {
    long val = 0;
    virtual ~node_t() {}
};

struct n_ary_node_t : public node_t {
    class std::vector<std::shared_ptr<node_t>> children;
};

template <size_t N = 0>
struct fixed_node_t : public node_t {
    static_assert(N <= MAX_SIZE && N >= 1);
    class std::array<std::shared_ptr<node_t>, N> children = {nullptr};
};

class territory_charter_t {
public:
    static std::string encode_tree(std::shared_ptr<node_t> root);
    static std::shared_ptr<node_t> decode_tree(std::string encoded);
};

namespace encoder {
    std::string encode_n_ary_tree(std::shared_ptr<node_t> root);
    template <size_t N> std::string encode_fixed_tree(
                                    std::shared_ptr<node_t> root);
    template <size_t N> 
    typename std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::string>::type
    encode_fixed_tree_tester(std::shared_ptr<node_t> root);
    template <size_t N> 
    typename std::enable_if<std::is_gt(N <=> MAX_SIZE), std::string>::type
    encode_fixed_tree_tester(std::shared_ptr<node_t> root);
};

std::string encoder::encode_n_ary_tree(std::shared_ptr<node_t> root) {
    std::string result = "[" + std::to_string(root->val);
    std::shared_ptr<n_ary_node_t> __root = 
            std::dynamic_pointer_cast<n_ary_node_t>(root);
    for (std::shared_ptr<struct node_t> child : __root->children) {
        result += encoder::encode_n_ary_tree(child);
    }
    result += "]";
    return result;
}

template <size_t N> std::string encoder::encode_fixed_tree(
                                    std::shared_ptr<node_t> root) {
    if (root == nullptr) return "(NULL)";
    std::shared_ptr<fixed_node_t<N>> __root = 
            std::dynamic_pointer_cast<fixed_node_t<N>>(root);
    std::string result = "(" + std::to_string(root->val);
    for (std::shared_ptr<struct node_t> child : __root->children)
    {
        result += encoder::encode_fixed_tree<N>(child);
    }
    result += ")";
    return result;
}

template <size_t N> 
typename std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::string>::type 
encoder::encode_fixed_tree_tester(std::shared_ptr<node_t> root) {
    if (std::dynamic_pointer_cast<fixed_node_t<N>>(root) == nullptr) {
        return encoder::encode_fixed_tree_tester<N + 1>(root);
    } else {
        return std::to_string(N) + encoder::encode_fixed_tree<N>(root);
    }
}

template <size_t N> 
typename std::enable_if<std::is_gt(N <=> MAX_SIZE), std::string>::type
encoder::encode_fixed_tree_tester(std::shared_ptr<node_t> root) {
    throw std::invalid_argument("Invalid tree");
}

std::string territory_charter_t::encode_tree(std::shared_ptr<node_t> root) {
    // TODO: Write your code here
    if (std::dynamic_pointer_cast<n_ary_node_t>(root)) {
        return encoder::encode_n_ary_tree(root);
    } else {
        return encoder::encode_fixed_tree_tester<1>(root);
    }
    // End TODO
}

namespace decoder {
    std::shared_ptr<node_t> decode_n_ary_tree(std::string &encoded,
                                                        int &index);
    template <size_t N> std::shared_ptr<node_t> decode_fixed_tree(
                                            std::string &encoded, int &index);
    template <size_t N> typename 
    std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
    decode_fixed_tree_tester(std::string &encoded, int &index, size_t n);
    template <size_t N> typename
    std::enable_if<std::is_gt(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
    decode_fixed_tree_tester(std::string &encoded, int &index, size_t n);
};

std::shared_ptr<struct node_t> decoder::decode_n_ary_tree(std::string &encoded, 
                                                    int &index) {
    std::string num = "";
    if (encoded[index] == '-') num += encoded[index++];
    while (encoded[index] >= '0' && encoded[index] <= '9') 
        num += encoded[index++];
    std::shared_ptr<n_ary_node_t> root = std::make_shared<n_ary_node_t>();
    root->val = std::stol(num);
    while (encoded[index] != ']') {
        if (encoded[index++] != '[') throw std::invalid_argument("Bad input");
        root->children.push_back(decoder::decode_n_ary_tree(encoded, index));
    }
    index++;
    return std::dynamic_pointer_cast<node_t>(root);
}

template <size_t N> std::shared_ptr<node_t> decoder::decode_fixed_tree(
                                            std::string &encoded, int &index) {
    if (encoded[index] == 'N') {
        if (encoded[++index] != 'U') throw std::invalid_argument("Bad input");
        if (encoded[++index] != 'L') throw std::invalid_argument("Bad input");
        if (encoded[++index] != 'L') throw std::invalid_argument("Bad input");
        if (encoded[++index] != ')') throw std::invalid_argument("Bad input");
        index++;
        return nullptr;
    }
    std::string num = "";
    if (encoded[index] == '-') num += encoded[index++];
    while (encoded[index] >= '0' && encoded[index] <= '9') 
        num += encoded[index++];
    std::shared_ptr<fixed_node_t<N>> root = std::make_shared<fixed_node_t<N>>();
    root->val = std::stol(num);
    for (size_t i = 0; i < N; i++) {
        if (encoded[index++] != '(') throw std::invalid_argument("Bad input");
        root->children[i] = decoder::decode_fixed_tree<N>(encoded, index);
    }
    if (encoded[index++] != ')') throw std::invalid_argument("Bad input");
    return std::dynamic_pointer_cast<node_t>(root);
}

template <size_t N> typename
std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
decoder::decode_fixed_tree_tester(std::string &encoded, int &index, size_t n) {
    if (n > N) {
        return decoder::decode_fixed_tree_tester<N + 1>(encoded, index, n);
    } else {
        return decoder::decode_fixed_tree<N>(encoded, index);
    }
}

template <size_t N> typename
std::enable_if<std::is_gt(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
decoder::decode_fixed_tree_tester(std::string &encoded, int &index, size_t n) {
    throw std::invalid_argument("Invalid tree");
}

std::shared_ptr<node_t> territory_charter_t::decode_tree(std::string encoded) {
    // TODO: Write your code here
    int index = 0;
    if (encoded[index++] == '[') {
        return decoder::decode_n_ary_tree(encoded, index);
    }
    size_t n = encoded[0] - '0';
    if (encoded[index++] != '(') {
        throw std::invalid_argument("Bad input");
    }
    return decoder::decode_fixed_tree_tester<1>(encoded, index, n);
    // End TODO
}