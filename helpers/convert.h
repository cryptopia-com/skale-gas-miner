#pragma once
#include <string>
#include "cryptopp/integer.h"

namespace Cryptopia 
{
    /**
     * Converts a hexadecimal string to a CryptoPP::Integer.
     *
     * @param hex A string representing a hexadecimal number.
     * @return A CryptoPP::Integer which is the numeric representation of the input hex string.
     */
    CryptoPP::SecByteBlock HexToByteArray(const std::string& hex);

    /**
     * Converts a CryptoPP::Integer to a byte array (SecByteBlock).
     *
     * @param integer An CryptoPP::Integer to convert.
     * @return A CryptoPP::SecByteBlock which is the byte array representation of the input integer.
     */
    CryptoPP::SecByteBlock IntegerToByteArray(const CryptoPP::Integer& integer);

    /**
     * Converts a CryptoPP::Integer to a string representation.
     *
     * @param num The CryptoPP::Integer to be converted to a string.
     * @param base The numerical base for the conversion.
     *             Supported bases are 10 (decimal) and 16 (hexadecimal).
     * @return A string representation of the input CryptoPP::Integer in the specified base.
     * @throws std::invalid_argument If the base is not supported (not 10 or 16).
     */
    std::string IntegerToString(const CryptoPP::Integer& num, int base = 10);
}
