#include <iostream>
#include <vector>
#include <cassert>

class LongNumber {
public:
    LongNumber() = default;

    explicit LongNumber(long long number) {
        while (number != 0) {
            m_digits.push_back(static_cast<char>(number % 10));
            number /= 10;
        }
    }

    friend LongNumber operator + (const LongNumber& number1, const LongNumber& number2) {
        if (number2.size() > number1.size())
            return number2 + number1;

        if (number2.zero()) {
            return number1;
        }

        LongNumber result;
        bool shift{false};
        for (std::size_t index = 0; index < number1.size(); ++index) {
            auto resultDigit = number1.m_digits[index] +
                    (index < number2.m_digits.size() ? number2.m_digits[index] : 0) + (shift ? 1 : 0);
            if (resultDigit > 9) {
                shift = true;
                resultDigit %= 10;
            } else {
                shift = false;
            }
            result.m_digits.push_back(static_cast<char>(resultDigit));
        }
        if (shift) {
            result.m_digits.push_back(1);
        }
        return result;
    }

    friend LongNumber operator * (const LongNumber& number1, const LongNumber& number2) {
        if (number2.size() > number1.size())
            return number2 * number1;

        if (number2.zero()) {
            return LongNumber{};
        }

        LongNumber result;
        for (std::size_t index2 = 0; index2 < number2.size(); ++index2) {
            int shift{0};
            LongNumber mul;
            for (std::size_t index1 = 0; index1 < number1.size(); ++index1) {
                auto resultDigit = number1.m_digits[index1] * number2.m_digits[index2] + (shift % 10);
                if (resultDigit > 9) {
                    shift = resultDigit / 10;
                    resultDigit %= 10;
                } else {
                    shift = 0;
                }
                mul.m_digits.push_back(static_cast<char>(resultDigit));
            }
            if (shift != 0) {
                mul.m_digits.push_back(static_cast<char>(shift));
            }
            mul.m_digits.insert(mul.m_digits.begin(), index2, 0);
            result += mul;
        }

        return result;
    }

    LongNumber operator += (const LongNumber& number) {
        *this = *this + number;
        return *this;
    }

    LongNumber operator *= (const LongNumber& number) {
        *this = *this * number;
        return *this;
    }

    friend std::ostream& operator << (std::ostream& stream, const LongNumber& number) {
        if (number.zero()) {
            stream << "0";
        } else {
            for (auto it = number.m_digits.rbegin(); it != number.m_digits.rend(); ++it) {
                stream << static_cast<int>(*it);
            }
        }
        return stream;
    }

    bool operator == (const LongNumber& number) const {
        return m_digits == number.m_digits;
    }

private:
    bool zero() const {
        return m_digits.empty();
    }

    std::size_t size() const {
        return m_digits.size();
    }

    std::vector<char> m_digits;
};

LongNumber calcFactorial(long long number) {
    if (number == 0 || number == 1) {
        return LongNumber{1};
    }
    LongNumber result{1};
    for (long long i = 2; i <= number; ++i) {
        result *= LongNumber{i};
    }
    return result;
}

void test() {
    {
        LongNumber number1{12345};
        LongNumber number2{678910};
        LongNumber result{691255};
        assert(number1 + number2 == result);
    }
    {
        LongNumber number1{12345};
        LongNumber number2{678910};
        LongNumber result{8381143950};
        assert(number1 * number2 == result);
    }
    {
        LongNumber number1{87178291200};
        LongNumber number2{15};
        LongNumber result{1307674368000};
        auto res = number1 * number2;
        assert(number1 * number2 == result);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }
    test();
    std::cout << calcFactorial(std::stol(argv[1])) << std::endl;
    return 0;
}
