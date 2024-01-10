#pragma once
#include <chrono>
#include <string>
#include "cryptopp/hex.h"
#include "cryptopp/integer.h"
#include "../helpers/convert.h"

namespace Cryptopia
{
    /**
     * Converts a hexadecimal string to a byte array (SecByteBlock).
     *
     * @param hex A string representing a hexadecimal value.
     *            The string may start with "0x" which will be removed if present.
     * @return A CryptoPP::SecByteBlock which is the byte array representation of the input hex string.
     */
    CryptoPP::SecByteBlock HexToByteArray(const std::string& hex)
    {
        std::string cleanHex = hex;

        // Remove "0x" prefix if present
        if (cleanHex.size() > 1 && cleanHex.substr(0, 2) == "0x") 
        {
            cleanHex = cleanHex.substr(2);
        }

        // Convert hex string to byte array
        CryptoPP::SecByteBlock byteBlock(cleanHex.size() / 2);
        CryptoPP::StringSource ss(cleanHex, true, new CryptoPP::HexDecoder);
        ss.Pump(byteBlock.size());
        ss.Get(byteBlock, byteBlock.size());

        return byteBlock;
    }

    /**
     * Converts a hexadecimal string to a CryptoPP::Integer.
     *
     * @param hex A string representing a hexadecimal number.
     * @return A CryptoPP::Integer which is the numeric representation of the input hex string.
     */
    CryptoPP::Integer HexToInteger(const std::string& hex)
    {
        return CryptoPP::Integer(("0x" + hex).c_str());
    }

    /**
     * Converts a CryptoPP::Integer to a string representation.
     *
     * @param num The CryptoPP::Integer to be converted to a string.
     * @param base The numerical base for the conversion.
     *             Supported bases are 10 (decimal) and 16 (hexadecimal).
     * @return A string representation of the input CryptoPP::Integer in the specified base.
     * @throws std::invalid_argument If the base is not supported (not 10 or 16).
     */
    std::string IntegerToString(const CryptoPP::Integer& num, int base)
    {
        std::ostringstream oss;
        if (base == 16)
        {
            oss << std::hex;
        }
        else if (base == 10)
        {
            oss << std::dec;
        }
        else
        {
            throw std::invalid_argument("Invalid base");
        }

        oss << num;

        // Remove trailing decimal point
        std::string str = oss.str();
        if (!str.empty() && str.back() == '.')
        {
            str.pop_back();
        }

        return str;
    }
}
