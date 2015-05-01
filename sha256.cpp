#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned int WORD;

std::vector<WORD> M; // Message to be hashed


/**
 * Calculate the required padding of the message.
 * Return the required padding.
 */
const int calc_padding(const int l)
{
    int k = 0;
    while ((l + 1 + k % 512) != 448) ++k;
    return k;
}

/**
 * Convert the given hex string to a byte array.
 */
const void hex_str_to_byte_vector(const std::string &hex_str,
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

/**
 * Take the given hex string and store it in a word vector.
 */
const void store_message(const std::string &hex_str)
{
    // Pick out whole words from the the hex string
    WORD i = 0;
    for (; i < hex_str.length() / 8; ++i)
    {
        WORD word = std::stoul(hex_str.substr(i * 8, 8), nullptr, 16);
        M.push_back(word);
    }
    // Pick out the last bytes which does not fit a word
    WORD word = std::stoul(hex_str.substr(i * 8, hex_str.length() - i * 8),
            nullptr, 16);
    M.push_back(word);
}

int main()
{
    // Read each line as a hex string to be hashed
    for (std::string line; std::getline(std::cin, line);) {
        store_message(line);
        for (WORD i = 0; i < M.size(); ++i)
            std::cout << M[i] << " ";
        std::cout << std::endl;
        M.clear(); // Reset message to hash a new one
    }
}
