#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <memory>

#define MIN_SIZE 10
#define node_ptr_t std::shared_ptr<node_t>
#define ENABLE_IF_STR_t std::enable_if<std::is_lteq(N <=> MIN_SIZE), std::string>::type
#define ENABLE_ELSE_STR_t std::enable_if<std::is_gt(N <=> MIN_SIZE), std::string>::type
#define ENABLE_IF_PTR_t std::enable_if<std::is_lteq(N <=> MIN_SIZE), node_ptr_t>::type
#define ENABLE_ELSE_PTR_t std::enable_if<std::is_gt(N <=> MIN_SIZE), node_ptr_t>::type
struct node_t {
    long data_t = 0;
    virtual ~node_t() = default;
};

struct n_ary_node_t : public node_t {
    class std::vector<node_ptr_t> adults;
};

template <size_t N = 0>
struct fixed_node_t : public node_t {
    static_assert(N <= MIN_SIZE && N >= 1);
    class std::array<node_ptr_t, N> adults = {nullptr};
};

class territory_charter_t {
public:
    static std::string get_map(node_ptr_t leaf);
    static node_ptr_t land_from_map(std::string encoded);
};

namespace MapMakerT {
    std::string usoiulfjsouor(node_ptr_t leaf);
    template <size_t N> std::string sfsdjfsdkjfdksljfsdk(
                                    node_ptr_t leaf);
    template <size_t N> ENABLE_IF_STR_t
    sfsdjfsdkjfdksljfsdkAaaaaaa(node_ptr_t leaf);
    template <size_t N> ENABLE_ELSE_STR_t
    sfsdjfsdkjfdksljfsdkAaaaaaa(node_ptr_t leaf);
    node_ptr_t lxvklcnvcxljcljvklcjvl(std::string &encoded,
                                                        int &index);
    template <size_t N> node_ptr_t rsvunseiosnesrsuvoru(
                                            std::string &encoded, int &index);
    template <size_t N>  ENABLE_IF_PTR_t
    rsvunseiosnesrsuvoruAaaaaaa(std::string &encoded, int &index, size_t n);
    template <size_t N> ENABLE_ELSE_PTR_t
    rsvunseiosnesrsuvoruAaaaaaa(std::string &encoded, int &index, size_t n);
};

node_ptr_t MapMakerT::lxvklcnvcxljcljvklcjvl(std::string &encoded, 
                                                    int &index) {
    std::string num = "";
    if (encoded[index] == '-') num += encoded[index++];
    while (encoded[index] >= '0' && encoded[index] <= '9') 
        num += encoded[index++];
    std::shared_ptr<n_ary_node_t> leaf = std::make_shared<n_ary_node_t>();
    leaf->data_t = std::stol(num);
    while (encoded[index] != ']') {
        if (encoded[index++] != '[') throw std::invalid_argument("Bad input");
        leaf->adults.push_back(MapMakerT::lxvklcnvcxljcljvklcjvl(encoded, index));
    }
    index++;
    return std::dynamic_pointer_cast<node_t>(leaf);
}

template <size_t N> node_ptr_t MapMakerT::rsvunseiosnesrsuvoru(
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
    std::shared_ptr<fixed_node_t<N>> leaf = std::make_shared<fixed_node_t<N>>();
    leaf->data_t = std::stol(num);
    for (size_t i = 0; i < N; i++) {
        if (encoded[index++] != '(') throw std::invalid_argument("Bad input");
        leaf->adults[i] = MapMakerT::rsvunseiosnesrsuvoru<N>(encoded, index);
    }
    if (encoded[index++] != ')') throw std::invalid_argument("Bad input");
    return std::dynamic_pointer_cast<node_t>(leaf);
}

template <size_t N> ENABLE_IF_PTR_t
MapMakerT::rsvunseiosnesrsuvoruAaaaaaa(std::string &encoded, int &index, size_t n) {
    if (n > N) {
        return MapMakerT::rsvunseiosnesrsuvoruAaaaaaa<N + 1>(encoded, index, n);
    } else {
        return MapMakerT::rsvunseiosnesrsuvoru<N>(encoded, index);
    }
}

template <size_t N> ENABLE_ELSE_PTR_t
MapMakerT::rsvunseiosnesrsuvoruAaaaaaa(std::string &encoded, int &index, size_t n) {
    throw std::invalid_argument("Invalid tree");
}

std::string MapMakerT::usoiulfjsouor(node_ptr_t leaf) {
    std::string result = "[" + std::to_string(leaf->data_t);
    std::shared_ptr<n_ary_node_t> __leaf = 
            std::dynamic_pointer_cast<n_ary_node_t>(leaf);
    for (node_ptr_t child : __leaf->adults) {
        result += MapMakerT::usoiulfjsouor(child);
    }
    result += "]";
    return result;
}

template <size_t N> std::string MapMakerT::sfsdjfsdkjfdksljfsdk(
                                    node_ptr_t leaf) {
    if (leaf == nullptr) return "(NULL)";
    std::shared_ptr<fixed_node_t<N>> __leaf = 
            std::dynamic_pointer_cast<fixed_node_t<N>>(leaf);
    std::string result = "(" + std::to_string(leaf->data_t);
    for (node_ptr_t child : __leaf->adults)
    {
        result += MapMakerT::sfsdjfsdkjfdksljfsdk<N>(child);
    }
    result += ")";
    return result;
}

template <size_t N> 
ENABLE_IF_STR_t 
MapMakerT::sfsdjfsdkjfdksljfsdkAaaaaaa(node_ptr_t leaf) {
    if (std::dynamic_pointer_cast<fixed_node_t<N>>(leaf) == nullptr) {
        return MapMakerT::sfsdjfsdkjfdksljfsdkAaaaaaa<N + 1>(leaf);
    } else {
        return std::to_string(N) + MapMakerT::sfsdjfsdkjfdksljfsdk<N>(leaf);
    }
}

template <size_t N> 
ENABLE_ELSE_STR_t
MapMakerT::sfsdjfsdkjfdksljfsdkAaaaaaa(node_ptr_t leaf) {
    throw std::invalid_argument("Invalid tree");
}

std::string territory_charter_t::get_map(node_ptr_t leaf) {
    // TODO: Write your code here
    if (std::dynamic_pointer_cast<n_ary_node_t>(leaf)) {
        return MapMakerT::usoiulfjsouor(leaf);
    } else {
        return MapMakerT::sfsdjfsdkjfdksljfsdkAaaaaaa<1>(leaf);
    }
    // End TODO
}

node_ptr_t territory_charter_t::land_from_map(std::string encoded) {
    // TODO: Write your code here
    int index = 0;
    if (encoded[index++] == '[') {
        return MapMakerT::lxvklcnvcxljcljvklcjvl(encoded, index);
    }
    size_t n = encoded[0] - '0';
    if (encoded[index++] != '(') {
        throw std::invalid_argument("Bad input");
    }
    return MapMakerT::rsvunseiosnesrsuvoruAaaaaaa<1>(encoded, index, n);
    // End TODO
}