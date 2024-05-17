#include <string_view> 
#include <chrono>
#include <string>
#include <iostream>
#include <algorithm>
#include <deque>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <functional>

using namespace std::chrono;

class time_parser {
public:
    bool parse(std::string_view sv) {
        auto const end = sv.find_last_not_of(" \r\t\n");
        auto const beg = sv.find_first_not_of(" \r\t\n");
        if (end - beg == 0)
            return false;

        if (sv.find_first_of('h') != std::string_view::npos and std::count(sv.begin(), sv.end(), ':'))
            return false;

        auto cursor = sv.begin() + static_cast<long long>(beg);
        auto const e = sv.begin() + static_cast<long long>(end) + 1;

        constexpr int max_possible_semicolons = 3;
        auto const semicolons = static_cast<int>(std::count_if(cursor, e, [](auto elem) {return elem == ':';}));

        if (semicolons > max_possible_semicolons)
            return false;

        auto duplicated_essences = [&] {        
            return std::min(semicolons, max_possible_semicolons) == max_possible_semicolons and sv.find_first_of("wd") != std::string_view::npos;
        };
        if (duplicated_essences())
            return false;

        std::deque<std::vector<std::string>> sym_deq {
            {"w", "wk", "week", "weeks"}
            , {"d", "day", "days"}
            , {"h", "hr", "hrs", "hour", "hours"}
            , {"m", "min", "mins", "minute", "minutes"}
            , {"s", "sec", "secs", "second", "seconds"}
        };

        std::unordered_map<std::string, long double> sym_to_sec {
            {"w", 604800}, {"wk", 604800}, {"week", 604800}, {"weeks", 604800}
            , {"d", 86400}, {"day", 86400}, {"days", 86400}
            , {"h", 3600}, {"hr", 3600}, {"hrs", 3600}, {"hour", 3600}, {"hours", 3600}
            , {"m", 60}, {"min", 60}, {"mins", 60}, {"minute", 60}, {"minutes", 60}
            , {"s", 1}, {"sec", 1}, {"secs", 1}, {"second", 1}, {"seconds", 1}
        };

        std::deque<long double> sec_deq;

        if (semicolons) {
            sec_deq.push_back(60); 
            sec_deq.push_back(1);
        }

        if (semicolons > 1)
            sec_deq.push_front(3600); 

        if (semicolons > 2)
            sec_deq.push_front(86400);

        enum class fun_type {digit, complete_time, word};
        fun_type next_fun = fun_type::digit;
        char c;
     
        auto initial_space_func = [&c, &cursor] {
            c = *cursor++;
            return true;
        };

        std::function<bool()> space_fn = initial_space_func;
        std::chrono::duration<long double, std::ratio<1>> total_sec {0};

        unsigned char commas = 0;

        std::string recent_num;

        while (cursor != e) {           
            auto space_fun = [&c, &cursor, &commas, &e] {
                if (commas > 1)
                    return false;
                for(;;) {
                    if (cursor == e or (c == ',' and ++commas > 1))
                        return false;
                    if (!(std::isspace(c = *cursor++)) and c != ',') 
                        return true; 
                }
            };

            auto compose_num = [&c, &cursor, &e] {
                unsigned dots = 0;
                auto num_char = [&c, &cursor, &dots] {
                    c = *cursor++;
                    dots += c == '.' ? 1 : 0;
                    return (dots < 2 and (std::isdigit(c) or c == '.'));
                };
                std::string s;
                do s += c; while (cursor != e and num_char());
                return s;
            };

            auto inc_sec = [&recent_num, &total_sec] (auto const & num) {
                struct sstringstream : std::stringstream {
                    sstringstream() : std::stringstream() { imbue(std::locale("C")); }
                };
                sstringstream ss;
                ss << recent_num;
                long double value;
                ss >> value;
                total_sec += std::chrono::duration<long double, std::ratio<1>>(value * num);
            }; 

            auto digit_fun = [&space_fn, &c, &space_fun, &compose_num, &cursor, &recent_num, &inc_sec, &sec_deq, &next_fun, &commas] {
                if (!space_fn() or !std::isdigit(c))
                    return false;
                if (space_fn.target<decltype(initial_space_func)>())
                    space_fn = space_fun;
                recent_num = compose_num();
                --cursor;
                if (c == ':') {
                    if (recent_num.find('.') != std::string::npos)
                        return false;
                    inc_sec(sec_deq.front());
                    sec_deq.pop_front();
                    next_fun = fun_type::complete_time;
                } else {
                    commas = 0;
                    next_fun = fun_type::word;
                }
                return true;                               
            }; 

            auto complete_time_fun = [&cursor, &e, &c, &recent_num, &compose_num, &inc_sec, &sec_deq] {
                if (++cursor == e or !(std::isdigit(c = *cursor++)))          
                    return false;
                recent_num = compose_num();
                if (cursor == e and std::isdigit(*(cursor - 1))) {
                    auto const dot_pos = recent_num.find_first_of('.');
                    if ((dot_pos != std::string::npos and dot_pos != 2) or (dot_pos == std::string::npos and recent_num.size() != 2))
                        return false; 
                    inc_sec(sec_deq.front());
                    sec_deq.pop_front(); 
                    return true;  
                }
                --cursor; 
                if (c == ':') {
                    if (recent_num.find('.') != std::string::npos or recent_num.size() != 2)
                        return false;
                } else {
                    while(cursor != e) {
                        if (!std::isspace(*cursor++))
                            return false;
                    }
                }
                inc_sec(sec_deq.front());
                sec_deq.pop_front(); 
                return true;
            };

            auto word_fun = [&space_fun, &c, &cursor, &e, &commas, &sym_deq, &inc_sec, &sym_to_sec
                    , &next_fun] {
                if (!space_fun() or !(std::isalpha(c)))
                    return false;

                auto isalpha_or_comma = [&] {
                    return std::isalpha(c = *cursor++) || c == ',';
                };
                std::string word;
                do {
                    if (c != ',')
                        word += c;
                    else
                        ++commas;
                }
                while (cursor != e and isalpha_or_comma());
                for (;;) {
                    if (sym_deq.empty())
                        return false;
                    auto const & front = sym_deq.front();
                    bool found = false;
                    for (auto & elem : front) {
                        if (elem == word) {
                            inc_sec(sym_to_sec[elem]);
                            found = true;
                            break;
                        }
                    }
                    sym_deq.pop_front();
                    if (found)
                        break;
                }

                if (cursor == e and std::isalpha(*(cursor - 1)))
                    return true;

                --cursor;
                next_fun = fun_type::digit;
                return true;
            };

            std::unordered_map<fun_type, std::function<bool()>>
            fun_map = {{fun_type::digit, digit_fun}, {fun_type::complete_time, complete_time_fun}, {fun_type::word, word_fun}};

            if (!fun_map[next_fun]())
                return false;
        }

        seconds_ = static_cast<decltype(seconds_)>(floor<std::chrono::seconds>(total_sec).count());
        fill_parts(total_sec);
        return true;
    }

    [[nodiscard]] unsigned days() const {return days_; }
    [[nodiscard]] unsigned clock_hours() const {return clock_hours_; }
    [[nodiscard]] unsigned clock_minutes() const {return clock_minutes_; }
    [[nodiscard]] unsigned clock_seconds() const {return clock_seconds_; }
    [[nodiscard]] unsigned microseconds() const {return microseconds_; }
    [[nodiscard]] unsigned nanoseconds() const {return nanoseconds_; }

    [[nodiscard]] std::string time() const {
        return std::to_string(seconds_) + '.' + std::to_string(round<std::chrono::microseconds>(rest_).count());
    }

    [[nodiscard]] std::tuple<std::size_t, unsigned, unsigned> time_tuple() const {
        auto const microsecs = floor<std::chrono::microseconds>(rest_).count();
        auto const nanosecs = round<std::chrono::nanoseconds>(rest_ - floor<std::chrono::microseconds>(rest_)).count();
        return {seconds_, microsecs, nanosecs};
    }

    enum output_resolution {
        MICRO,
        NANO
    };

    [[nodiscard]] std::string str(output_resolution r = MICRO) const {
        if (r == MICRO) {
            microseconds_ = static_cast<decltype(microseconds_)>(round<std::chrono::microseconds>(rest_).count());
            nanoseconds_ = 0;
        } else {
            microseconds_ = static_cast<decltype(microseconds_)>(floor<std::chrono::microseconds>(rest_).count());
            nanoseconds_ = static_cast<decltype(microseconds_)>(round<std::chrono::nanoseconds>(rest_ - floor<std::chrono::microseconds>(rest_)).count());
        }

        std::stringstream ss;
        ss.imbue(std::locale("C"));
        ss.fill('0');
        if (days_)
            to_stream(ss, days_, (days_ == 1 ? " day, " : " days, "));
        to_stream(ss,  std::setw(clock_hours_ > 9 ? 2 : 1), clock_hours_, ":", std::setw(2), clock_minutes_, ":", std::setw(2), clock_seconds_);
        if (microseconds_)
            to_stream(ss, '.', microseconds_);
        if (nanoseconds_)
            to_stream(ss, ',', nanoseconds_, "ns");
        return ss.str();
    }

    std::string str(long double seconds, output_resolution r = MICRO) const {
        std::chrono::duration<double, std::ratio<1>> secs {seconds};
        fill_parts(secs);
        return str(r);
    }

private:
    std::size_t seconds_;
    mutable std::chrono::duration<long double, std::ratio<1>> rest_; 
    mutable unsigned days_;
    mutable unsigned clock_hours_;
    mutable unsigned clock_minutes_;
    mutable unsigned clock_seconds_;
    mutable unsigned microseconds_;
    mutable unsigned nanoseconds_;

    void fill_parts(std::chrono::duration<long double, std::ratio<1>> src) const {
        days_ = static_cast<decltype(days_)>(floor<std::chrono::days>(src).count());
        src -= floor<std::chrono::days>(src);
        clock_hours_ = static_cast<decltype(clock_hours_)>(floor<std::chrono::hours>(src).count());
        src -= floor<std::chrono::hours>(src);
        clock_minutes_ = static_cast<decltype(clock_minutes_)>(floor<std::chrono::minutes>(src).count());
        src -= floor<std::chrono::minutes>(src);
        clock_seconds_ = static_cast<decltype(clock_seconds_)>(floor<std::chrono::seconds>(src).count());
        src -= floor<std::chrono::seconds>(src);
        rest_ = src;
    }

    template<class SS, class... Ts>
    void to_stream(SS & ss, Ts&&... ts) const {
        (ss << ... << std::forward<Ts>(ts));
    }

};
