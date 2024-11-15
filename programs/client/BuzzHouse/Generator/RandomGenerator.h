#pragma once

#include <cassert>
#include <chrono>
#include <cstdint>
#include <map>
#include <random>
#include <set>
#include <string>
#include <tuple>

namespace BuzzHouse
{

typedef struct TestSetting
{
    const std::string tsetting;
    const std::set<std::string> options;

    TestSetting(const std::string & sett, const std::set<std::string> & noptions) : tsetting(sett), options(noptions) { }
} TestSetting;

const constexpr int seconds_per_day = 60 * 60 * 24;

const constexpr char hex_digits[] = "0123456789abcdef";

class RandomGenerator
{
private:
    uint32_t seed;

    std::uniform_int_distribution<int8_t> ints8;

    std::uniform_int_distribution<uint8_t> uints8, digits, hex_digits_dist;

    std::uniform_int_distribution<int16_t> ints16;

    std::uniform_int_distribution<uint16_t> uints16;

    std::uniform_int_distribution<int32_t> ints32;

    std::uniform_int_distribution<uint32_t> uints32, dist1, dist2, dist3, dist4, date_years, datetime_years, datetime64_years, months,
        hours, minutes;

    std::uniform_int_distribution<int64_t> ints64;

    std::uniform_int_distribution<uint64_t> uints64;

    std::uniform_real_distribution<double> zero_one;

    std::uniform_int_distribution<uint32_t> days[12]
        = {std::uniform_int_distribution<uint32_t>(1, 31),
           std::uniform_int_distribution<uint32_t>(1, 28),
           std::uniform_int_distribution<uint32_t>(1, 31),
           std::uniform_int_distribution<uint32_t>(1, 30),
           std::uniform_int_distribution<uint32_t>(1, 31),
           std::uniform_int_distribution<uint32_t>(1, 30),
           std::uniform_int_distribution<uint32_t>(1, 31),
           std::uniform_int_distribution<uint32_t>(1, 31),
           std::uniform_int_distribution<uint32_t>(1, 30),
           std::uniform_int_distribution<uint32_t>(1, 31),
           std::uniform_int_distribution<uint32_t>(1, 30),
           std::uniform_int_distribution<uint32_t>(1, 31)};

    const std::vector<std::string> common_english{
        "is",     "was",   "are",   "be",    "have",  "had",   "were",     "can",   "said",  "use",   "do",      "will",  "would",
        "make",   "like",  "has",   "look",  "write", "go",    "see",      "could", "been",  "call",  "am",      "find",  "did",
        "get",    "come",  "made",  "may",   "take",  "know",  "live",     "give",  "think", "say",   "help",    "tell",  "follow",
        "came",   "want",  "show",  "set",   "put",   "does",  "must",     "ask",   "went",  "read",  "need",    "move",  "try",
        "change", "play",  "spell", "found", "study", "learn", "should",   "add",   "keep",  "start", "thought", "saw",   "turn",
        "might",  "close", "seem",  "open",  "begin", "got",   "run",      "walk",  "began", "grow",  "took",    "carry", "hear",
        "stop",   "miss",  "eat",   "watch", "let",   "cut",   "talk",     "being", "leave", "water", "day",     "part",  "sound",
        "work",   "place", "year",  "back",  "thing", "name",  "sentence", "man",   "line",  "boy"};

    const std::vector<std::string> common_chinese{
        "认识你很高兴", "美国", "叫", "名字", "你们", "日本", "哪国人", "爸爸", "兄弟姐妹", "漂亮", "照片", "😉"};

    const std::vector<std::string> nasty_strings{"a\"a", "b\\tb", "c\\nc", "d\\'d", "e e", "",     "😉",   "\"",   "\\'",  "\\t",
                                                 "\\n",  " ",     "--",    "0",     "1",   "-1",   "{",    "}",    "(",    ")",
                                                 "[",    "]",     ",",     ".",     ";",   ":",    "\\\\", "/",    "_",    "%",
                                                 "*",    "\\0",   "{}",    "[]",    "()",  "null", "NULL", "TRUE", "FALSE"};

    /* use bad_utf8 on x' strings! */
    const std::vector<std::string> bad_utf8{
        "FF",
        "C328",
        "A0A1",
        "E228A1",
        "E28228",
        "F0288CBC",
        "F09028BC",
        "F0288C28",
        "C328A0A1",
        "E28228FF",
        "F0288CBCF0288CBC",
        "C328FF",
        "AAC328"};

    const std::vector<std::string> jcols{"c0", "c1", "c0.c1", "😆", "😉😉"};

public:
    std::mt19937 gen;

    RandomGenerator(const uint32_t in_seed)
        : ints8(std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max())
        , uints8(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max())
        , digits(static_cast<uint8_t>('0'), static_cast<uint8_t>('9'))
        , hex_digits_dist(0, 15)
        , ints16(std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max())
        , uints16(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max())
        , ints32(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max())
        , uints32(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())
        , dist1(UINT32_C(1), UINT32_C(10))
        , dist2(UINT32_C(1), UINT32_C(100))
        , dist3(UINT32_C(1), UINT32_C(1000))
        , dist4(UINT32_C(1), UINT32_C(2))
        , date_years(0, 2149 - 1970)
        , datetime_years(0, 2106 - 1970)
        , datetime64_years(0, 2299 - 1900)
        , months(1, 12)
        , hours(0, 23)
        , minutes(0, 59)
        , ints64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max())
        , uints64(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
        , zero_one(0, 1)
    {
        std::random_device rand_dev;

        seed = in_seed ? in_seed : rand_dev();
        gen = std::mt19937(seed);
    }

    uint32_t getSeed() const { return seed; }

    uint32_t nextSmallNumber() { return dist1(gen); }

    uint32_t nextMediumNumber() { return dist2(gen); }

    uint32_t nextLargeNumber() { return dist3(gen); }

    uint8_t nextRandomUInt8() { return uints8(gen); }

    int8_t nextRandomInt8() { return ints8(gen); }

    uint16_t nextRandomUInt16() { return uints16(gen); }

    int16_t nextRandomInt16() { return ints16(gen); }

    uint32_t nextRandomUInt32() { return uints32(gen); }

    int32_t nextRandomInt32() { return ints32(gen); }

    uint64_t nextRandomUInt64() { return uints64(gen); }

    int64_t nextRandomInt64() { return ints64(gen); }

    char nextDigit() { return static_cast<char>(digits(gen)); }

    bool nextBool() { return dist4(gen) == 2; }

    //range [1970-01-01, 2149-06-06]
    void nextDate(std::string & ret)
    {
        const uint32_t month = months(gen), day = days[month - 1](gen);

        ret += std::to_string(1970 + date_years(gen));
        ret += "-";
        if (month < 10)
        {
            ret += "0";
        }
        ret += std::to_string(month);
        ret += "-";
        if (day < 10)
        {
            ret += "0";
        }
        ret += std::to_string(day);
    }

    //range [1900-01-01, 2299-12-31]
    void nextDate32(std::string & ret)
    {
        const uint32_t month = months(gen), day = days[month - 1](gen);

        ret += std::to_string(1900 + datetime64_years(gen));
        ret += "-";
        if (month < 10)
        {
            ret += "0";
        }
        ret += std::to_string(month);
        ret += "-";
        if (day < 10)
        {
            ret += "0";
        }
        ret += std::to_string(day);
    }

    //range [1970-01-01 00:00:00, 2106-02-07 06:28:15]
    void nextDateTime(std::string & ret)
    {
        const uint32_t month = months(gen), day = days[month - 1](gen), hour = hours(gen), minute = minutes(gen), second = minutes(gen);

        ret += std::to_string(1970 + datetime_years(gen));
        ret += "-";
        if (month < 10)
        {
            ret += "0";
        }
        ret += std::to_string(month);
        ret += "-";
        if (day < 10)
        {
            ret += "0";
        }
        ret += std::to_string(day);
        ret += " ";
        if (hour < 10)
        {
            ret += "0";
        }
        ret += std::to_string(hour);
        ret += ":";
        if (minute < 10)
        {
            ret += "0";
        }
        ret += std::to_string(minute);
        ret += ":";
        if (second < 10)
        {
            ret += "0";
        }
        ret += std::to_string(second);
    }

    //range [1900-01-01 00:00:00, 2299-12-31 23:59:59.99999999]
    void nextDateTime64(std::string & ret)
    {
        const uint32_t month = months(gen), day = days[month - 1](gen), hour = hours(gen), minute = minutes(gen), second = minutes(gen);

        ret += std::to_string(1900 + datetime64_years(gen));
        ret += "-";
        if (month < 10)
        {
            ret += "0";
        }
        ret += std::to_string(month);
        ret += "-";
        if (day < 10)
        {
            ret += "0";
        }
        ret += std::to_string(day);
        ret += " ";
        if (hour < 10)
        {
            ret += "0";
        }
        ret += std::to_string(hour);
        ret += ":";
        if (minute < 10)
        {
            ret += "0";
        }
        ret += std::to_string(minute);
        ret += ":";
        if (second < 10)
        {
            ret += "0";
        }
        ret += std::to_string(second);
    }

    template <typename T>
    T thresholdGenerator(const double always_on_prob, const double always_off_prob, T min_val, T max_val)
    {
        const double tmp = zero_one(gen);

        if (tmp <= always_on_prob)
        {
            return min_val;
        }
        if (tmp <= always_on_prob + always_off_prob)
        {
            return max_val;
        }
        if constexpr (std::is_same<T, uint32_t>::value)
        {
            std::uniform_int_distribution<uint32_t> d{min_val, max_val};
            return d(gen);
        }
        if constexpr (std::is_same<T, double>::value)
        {
            std::uniform_real_distribution<double> d{min_val, max_val};
            return d(gen);
        }
        assert(0);
        return 0;
    }

    double RandomGauss(const double mean, const double stddev)
    {
        std::normal_distribution d{mean, stddev};
        return d(gen);
    }

    double RandomZeroOne() { return zero_one(gen); }

    template <typename T>
    T RandomInt(const T min, const T max)
    {
        std::uniform_int_distribution<T> d(min, max);
        return d(gen);
    }

    template <typename T>
    const T & pickRandomlyFromVector(const std::vector<T> & vals)
    {
        std::uniform_int_distribution<size_t> d{0, vals.size() - 1};
        return vals[d(gen)];
    }

    template <typename T>
    const T & pickRandomlyFromSet(const std::set<T> & vals)
    {
        std::uniform_int_distribution<size_t> d{0, vals.size() - 1};
        auto it = vals.begin();
        std::advance(it, d(gen));
        return *it;
    }

    template <typename K, typename V>
    const K & pickKeyRandomlyFromMap(const std::map<K, V> & vals)
    {
        std::uniform_int_distribution<size_t> d{0, vals.size() - 1};
        auto it = vals.begin();
        std::advance(it, d(gen));
        return it->first;
    }

    template <typename K, typename V>
    const V & pickValueRandomlyFromMap(const std::map<K, V> & vals)
    {
        std::uniform_int_distribution<size_t> d{0, vals.size() - 1};
        auto it = vals.begin();
        std::advance(it, d(gen));
        return it->second;
    }

    template <typename K, typename V>
    std::tuple<K, V> pickPairRandomlyFromMap(const std::map<K, V> & vals)
    {
        std::uniform_int_distribution<size_t> d{0, vals.size() - 1};
        auto it = vals.begin();
        std::advance(it, d(gen));
        return std::make_tuple(it->first, it->second);
    }

    void nextJSONCol(std::string & ret)
    {
        const std::string & pick = pickRandomlyFromVector(jcols);

        ret += pick;
    }

    void nextString(std::string & ret, const std::string & delimiter, const bool allow_nasty, const uint32_t limit)
    {
        bool use_bad_utf8 = false;

        if (delimiter == "'" && this->nextMediumNumber() < 4)
        {
            ret += "x";
            use_bad_utf8 = true;
        }
        ret += delimiter;
        const std::string & pick = pickRandomlyFromVector(
            use_bad_utf8
                ? bad_utf8
                : (allow_nasty && this->nextSmallNumber() < 3 ? nasty_strings : (this->nextBool() ? common_english : common_chinese)));

        if ((pick.length() >> (use_bad_utf8 ? 1 : 0)) < limit)
        {
            ret += pick;
            /* A few times, generate a large string */
            if (this->nextLargeNumber() < 4)
            {
                uint32_t i = 0, len = pick.size();
                const uint32_t max_iterations = this->nextBool() ? 10000 : this->nextMediumNumber();

                while (i < max_iterations)
                {
                    const std::string & npick = pickRandomlyFromVector(
                        use_bad_utf8 ? bad_utf8
                                     : (allow_nasty && this->nextSmallNumber() < 3 ? nasty_strings
                                                                                   : (this->nextBool() ? common_english : common_chinese)));

                    len += (npick.length() >> (use_bad_utf8 ? 1 : 0));
                    if (len < limit)
                    {
                        ret += npick;
                    }
                    else
                    {
                        break;
                    }
                    i++;
                }
            }
        }
        else
        {
            ret += "a";
        }
        ret += delimiter;
    }

    void nextUUID(std::string & ret)
    {
        for (uint32_t i = 0; i < 8; i++)
        {
            ret += hex_digits[hex_digits_dist(gen)];
        }
        ret += "-";
        for (uint32_t i = 0; i < 4; i++)
        {
            ret += hex_digits[hex_digits_dist(gen)];
        }
        ret += "-";
        for (uint32_t i = 0; i < 4; i++)
        {
            ret += hex_digits[hex_digits_dist(gen)];
        }
        ret += "-";
        for (uint32_t i = 0; i < 4; i++)
        {
            ret += hex_digits[hex_digits_dist(gen)];
        }
        ret += "-";
        for (uint32_t i = 0; i < 12; i++)
        {
            ret += hex_digits[hex_digits_dist(gen)];
        }
    }

    void nextIPv4(std::string & ret)
    {
        ret += std::to_string(this->nextRandomUInt8());
        ret += ".";
        ret += std::to_string(this->nextRandomUInt8());
        ret += ".";
        ret += std::to_string(this->nextRandomUInt8());
        ret += ".";
        ret += std::to_string(this->nextRandomUInt8());
    }

    void nextIPv6(std::string & ret)
    {
        for (uint32_t i = 0; i < 8; i++)
        {
            ret += hex_digits[hex_digits_dist(gen)];
            if (i < 7)
            {
                ret += ":";
            }
        }
    }
};

}
