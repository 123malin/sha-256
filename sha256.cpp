#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned int WORD;

std::vector<WORD> M; // Message to be hashed
int l = 0;           // Message length in bits


/**
 * Return the number of bytes needed to store the word.
 */
const int bytes_to_store(const WORD i)
{
    int bytes = 1;
    if (i == 0)       return 0;
    if (i > 255)      ++bytes;
    if (i > 65535)    ++bytes;
    if (i > 16777215) ++bytes;
    return bytes;
}

/**
 * Count the number of bits needed to represent the given word.
 */
const int bit_size(WORD w)
{
    int bit_size = 0;
    while(w != 0)
    {
        ++bit_size;
        w >>= 1;
    }
    return bit_size;
}

/**
 * Take the given hex string and store it in a word vector.
 * Also update the message length.
 */
const void store_message(const std::string &hex_str)
{
    WORD str_length = hex_str.length();
    // Pick out whole words from the the hex string
    WORD i = 0;
    for (; i < str_length / 8; ++i)
    {
        WORD word = std::stoul(hex_str.substr(i * 8, 8), nullptr, 16);
        M.push_back(word);
        l += 32;
    }

    // Pick out the last bytes which does not fit a word
    if (str_length % 8 != 0)
    {
        WORD word = std::stoul(hex_str.substr(i * 8, hex_str.length() - i * 8),
                nullptr, 16);
        M.push_back(word);
        l += bytes_to_store(word) * 8;
    }
}

/**
 * Calculate the required padding of the message.
 * Return the required padding.
 */
const int calc_padding()
{
    int k = 0;
    while ((l + 1 + k % 512) != 448) ++k;
    return k;
}

/**
 * Pad the message M according to the specification.
 */
const void pad_message()
{
    int k = calc_padding();
    int k_remainder = k % 32;
    int bits;
    WORD last_word;
    // TODO Handle l > 512
    if (l % 32 != 0)
    {
        last_word = M.back();
        bits = bit_size(last_word);
        // Append 1 before the least significant bit
        last_word <<= 1;
        last_word |= 0x1;

        // Append k zeroes after the word
        // TODO Assumes k > 32 - bits
        for (WORD i = 0; i < (WORD) (32 - bits); ++i)
        {
            last_word <<= 1;
            --k;
        }
        M.back() = last_word; // TODO Works?
        for (WORD i = 0; i < (WORD) (k / 32); ++i) M.push_back(0);
        // Check if one word is needed for trailing zeroes
        if (k_remainder != 0) M.push_back(0);

        // Finally append l in binary to the message
        // TODO Why 64 bits in spec?
        M.push_back(0); // Pad like l was a 64-bit number
        M.push_back(0 | l); // Pad with the size in binary at the end
    }
}

int main()
{
    // Read each line as a hex string to be hashed
    for (std::string line; std::getline(std::cin, line);) {
        // TODO Handle empty lines and the byte 0x00
        store_message(line);

        // DEBUG
        std::cout << "M plain: ";
        for (WORD i = 0; i < M.size(); ++i)
            std::cout << M[i] << " ";
        std::cout << std::endl;
        std::cout << "l: " << l << std::endl;

        // Check if message needs padding
        if (l % 512 != 0) pad_message();

        // DEBUG
        std::cout << "M padded: ";
        for (WORD i = 0; i < M.size(); ++i)
            std::cout << M[i] << " ";
        std::cout << std::endl;

        std::cout << "# words: " << M.size() << std::endl;

        M.clear(); // Reset message to hash a new one
    }
}
