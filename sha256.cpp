#include <iostream>
#include <vector>
#include <sstream>


void hex_str_to_byte_vector(const std::string hex_str, std::vector<char> bytes)
{
    std::stringstream converter;
    std::istringstream ss(hex_str);

    std::string word;
    while(ss >> word)
    {
        char temp;
        converter << std::hex << word;
        converter >> temp;
        bytes.push_back(temp);
    }
}

int main()
{
    std::cout << "Hello" << std::endl;
}
