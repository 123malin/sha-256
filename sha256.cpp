#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned int WORD;


int calc_padding(int l)
{
    int k = 0;
    while ((l + 1 + k % 512) != 448) ++k;
    return k;
}

/**
 * Convert the given hex string to a byte array.
 */
void hex_str_to_byte_vector(const std::string &hex_str,
        std::vector<char> &bytes)
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
    for (std::string line; std::getline(std::cin, line);) {
        /* std::vector<char> bytes; */

        /* hex_str_to_byte_vector(line, bytes); */

        /* for (std::vector<char>::iterator it = bytes.begin(), */
        /*         end = bytes.end(); it != end; ++it) */
        /* { */
        /*     std::cout << *it << " "; */
        /* } */

        WORD word = std::stoul(line, nullptr, 16);
        std::cout << word << std::endl;
    }
}
