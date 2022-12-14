#include <iostream>
#include <map>
#include <ranges>

int main() {
    //std::cout << "Hello, World!" << std::endl;

    struct Object
    {
        int id;
        uint timestamp;
    };
    std::map<int, Object> semantic_map;

    semantic_map.insert(std::make_pair(0,Object{0, 45}));
    semantic_map.insert(std::make_pair(1,Object{1, 3}));
    auto removable = semantic_map | std::views::values | std::views::filter([p = params](auto o) { return o.timestamp > 5; });


    for(const auto r: removable)
        std::cout << r << std::endl;
        //semantic_map.erase(o.id);


    return 0;
}
