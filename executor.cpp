#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
const std::string quote_sherlock =
    "I had had so many reasons to believe in my friend's subtle powers of\n"
    "reasoning and extraordinary energy in action that I felt that he must\n"
    "have some solid grounds for the assured and easy demeanour with which\n"
    "he treated the singular mystery which he had been called upon to\n"
    "fathom. Once only had I known him to fail, in the case of the King of\n"
    "Bohemia and of the Irene Adler photograph; but when I looked back to\n"
    "the weird business of the Sign of Four, and the extraordinary\n"
    "circumstances connected with the Study in Scarlet, I felt that it would\n"
    "be a strange tangle indeed which he could not unravel.\n";

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
const std::string quote_cat = R"(For he is of the tribe of Tiger.
For the Cherub Cat is a term of the Angel Tiger.
For he has the subtlety and hissing of a serpent, which in goodness he suppresses.
For he will not do destruction, if he is well-fed, neither will he spit without provocation.
For he purrs in thankfulness, when God tells him he's a good Cat.
For he is an instrument for the children to learn benevolence upon.
For every house is incomplete without him and a blessing is lacking in the spirit.
For the Lord commanded Moses concerning the cats at the departure of the Children of Israel from Egypt.
For every family had one cat at least in the bag.
For the English Cats are the best in Europe.
)";

enum class Command {
    EXIT,
    SHERLOCK,
    CAT,
    INVALID,
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    SHOW_MAP,
    START_MACRO,
    END_MACRO,
    RUN_MACRO,
    PUT_WALL,
    REMOVE_WALL
};

enum class Direction { UP, DOWN, LEFT, RIGHT };

const std::map<std::string, Command> commands{
    {"exit", Command::EXIT},
    {"quit", Command::EXIT},
    {"sherlock", Command::SHERLOCK},
    {"sherlock-holmes", Command::SHERLOCK},
    {"cat", Command::CAT},
    {"cat-jeoffry", Command::CAT},
    {"forward", Command::FORWARD},
    {"fwd", Command::FORWARD},
    {"backward", Command::BACKWARD},
    {"bwd", Command::BACKWARD},
    {"turn-right", Command::RIGHT},
    {"right", Command::RIGHT},
    {"turn-left", Command::LEFT},
    {"left", Command::LEFT},
    {"show-map", Command::SHOW_MAP},
    {"start-macro", Command::START_MACRO},
    {"end-macro", Command::END_MACRO},
    {"run-macro", Command::RUN_MACRO},
    {"put-wall", Command::PUT_WALL},
    {"remove-wall", Command::REMOVE_WALL}};

void print_invalid_command(const std::string &line);

class MyMap {
private:
    int x = 1, y = 1, left_bound = 0, right_bound = 2, low_bound = 0,
        up_bound = 2;
    Direction direction = Direction::UP;

    std::map<Direction, int> direction_for_x = {
        {Direction::RIGHT, 1},
        {Direction::UP, 0},
        {Direction::LEFT, -1},
        {Direction::DOWN, 0}};

    std::map<Direction, int> direction_for_y = {
        {Direction::RIGHT, 0},
        {Direction::UP, 1},
        {Direction::LEFT, 0},
        {Direction::DOWN, -1}};

    std::map<Direction, char> chars = {
        {Direction::RIGHT, '>'},
        {Direction::UP, '^'},
        {Direction::LEFT, '<'},
        {Direction::DOWN, 'v'}};

    std::map<std::pair<int, int>, char> data = {{{1, 1}, '^'}};

public:
    void update_bounds() {
        if (x <= left_bound) {
            left_bound = x - 1;
        }
        if (x >= right_bound) {
            right_bound = x + 1;
        }
        if (y <= low_bound) {
            low_bound = y - 1;
        }
        if (y >= up_bound) {
            up_bound = y + 1;
        }
    }

    bool check_way(int fl) {
        return data[{
                   x + direction_for_x[direction] * fl,
                   y + direction_for_y[direction] * fl}] == 0;
    }

    void forward() {
        if (check_way(1)) {
            data[{x, y}] = 'o';
            x += direction_for_x[direction] * 2;
            y += direction_for_y[direction] * 2;
            data[{x, y}] = chars[direction];
            update_bounds();
        } else {
            std::cout << "Bump!\n";
        }
    }

    void backward() {
        if (check_way(-1)) {
            data[{x, y}] = 'o';
            x -= direction_for_x[direction] * 2;
            y -= direction_for_y[direction] * 2;
            data[{x, y}] = chars[direction];
            update_bounds();
        } else {
            std::cout << "Bump!\n";
        }
    }

    void left() {
        if (direction == Direction::UP) {
            direction = Direction::LEFT;
        } else if (direction == Direction::LEFT) {
            direction = Direction::DOWN;
        } else if (direction == Direction::DOWN) {
            direction = Direction::RIGHT;
        } else {
            direction = Direction::UP;
        }

        data[{x, y}] = chars[direction];
    }

    void right() {
        if (direction == Direction::UP) {
            direction = Direction::RIGHT;
        } else if (direction == Direction::LEFT) {
            direction = Direction::UP;
        } else if (direction == Direction::DOWN) {
            direction = Direction::LEFT;
        } else {
            direction = Direction::DOWN;
        }

        data[{x, y}] = chars[direction];
    }

    void put_wall() {
        if (direction == Direction::RIGHT) {
            data[{x + 1, y}] = '|';

            data[{x + 1, y + 1}] = '+';
            data[{x + 1, y - 1}] = '+';

        } else if (direction == Direction::UP) {
            data[{x, y + 1}] = '-';

            data[{x + 1, y + 1}] = '+';
            data[{x - 1, y + 1}] = '+';

        } else if (direction == Direction::LEFT) {
            data[{x - 1, y}] = '|';

            data[{x - 1, y + 1}] = '+';
            data[{x - 1, y - 1}] = '+';

        } else if (direction == Direction::DOWN) {
            data[{x, y - 1}] = '-';

            data[{x - 1, y - 1}] = '+';
            data[{x + 1, y - 1}] = '+';
        }
    }

    void update_corners(int x2, int y2) {
        if ((data[{x2, y2}] != 0) && (data[{x2 + 1, y2}] == 0) &&
            (data[{x2 - 1, y2}] == 0) && (data[{x2, y2 + 1}] == 0) &&
            (data[{x2, y2 - 1}] == 0)) {
            data.erase({x2, y2});
        }
    }

    void remove_wall() {
        int x1 = 0;
        int y1 = 0;
        if (direction == Direction::RIGHT) {
            x1 = x + 1;
            y1 = y;

        } else if (direction == Direction::UP) {
            x1 = x;
            y1 = y + 1;

        } else if (direction == Direction::LEFT) {
            x1 = x - 1;
            y1 = y;

        } else if (direction == Direction::DOWN) {
            x1 = x;
            y1 = y - 1;
        }
        data.erase({x1, y1});
        if (x1 % 2 == 0) {
            update_corners(x1, y1 + 1);
            update_corners(x1, y1 - 1);
        } else {
            update_corners(x1 + 1, y1);
            update_corners(x1 - 1, y1);
        }
    }

    void show_map() {
        for (int i = up_bound; i >= low_bound; i--) {
            for (int j = left_bound; j <= right_bound; j++) {
                if (data[{j, i}] != 0) {
                    std::cout << data[{j, i}];
                } else if ((abs(i % 2) == 1) && (abs(j % 2) == 1)) {
                    std::cout << ".";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }
};

void process_some_commands(Command command, MyMap &map);

class Macros {
private:
    bool macros_is_going = false;
    std::vector<Command> macros_commands;
    std::set<Command> acceptable_commmands{
        Command::FORWARD,    Command::BACKWARD, Command::RIGHT,
        Command::LEFT,       Command::SHOW_MAP, Command::PUT_WALL,
        Command::REMOVE_WALL};

public:
    [[nodiscard]] bool check_status() const {
        return macros_is_going;
    }

    void end_macro() {
        macros_is_going = false;
        std::cout << "New macro of " << macros_commands.size()
                  << " command(s)\n";
    }

    void check_for_program_end() {
        if (macros_is_going) {
            end_macro();
        }
    }

    void record_command(const Command &command) {
        macros_commands.push_back(command);
        std::cout << "Recorded\n";
    }

    void process_command_macro(Command command, const std::string &line) {
        if (acceptable_commmands.count(command) != 0U) {
            record_command(command);

        } else if (command == Command::END_MACRO) {
            end_macro();

        } else {
            print_invalid_command(line);
        }
    }

    void start_macro() {
        macros_commands.clear();
        macros_is_going = true;
    }

    void run_macro(MyMap &map) {
        for (auto i : macros_commands) {
            process_some_commands(i, map);
        }
    }
};

void print_invalid_command(const std::string &line) {
    std::cout << "Invalid command: '" << line << "'"
              << "\n";
}

void print_quote_sherlock() {
    std::cout << quote_sherlock;
}

void print_quote_cat() {
    std::cout << quote_cat;
}

void process_some_commands(Command command, MyMap &map) {
    if (command == Command::FORWARD) {
        map.forward();
    } else if (command == Command::BACKWARD) {
        map.backward();
    } else if (command == Command::RIGHT) {
        map.right();
    } else if (command == Command::LEFT) {
        map.left();
    } else if (command == Command::SHOW_MAP) {
        map.show_map();
    } else if (command == Command::PUT_WALL) {
        map.put_wall();
    } else if (command == Command::REMOVE_WALL) {
        map.remove_wall();
    }
}

void process_command(
    Command command,
    const std::string &line,
    MyMap &map,
    Macros &macros
) {
    if (macros.check_status()) {
        macros.process_command_macro(command, line);
    } else {
        if (command == Command::SHERLOCK) {
            print_quote_sherlock();
        } else if (command == Command::CAT) {
            print_quote_cat();
        } else if (command == Command::INVALID || command == Command::END_MACRO) {
            print_invalid_command(line);
        } else if (command == Command::START_MACRO) {
            macros.start_macro();
        } else if (command == Command::RUN_MACRO) {
            macros.run_macro(map);
        } else {
            process_some_commands(command, map);
        }
    }
}

Command parse_line(const std::string &line) {
    std::string new_line = line;

    int i = 0;
    int j = static_cast<int>(new_line.size()) - 1;
    while (i < j && (std::isspace(line[i]) != 0)) {
        ++i;
    }
    while (j > i && (std::isspace(line[j]) != 0)) {
        --j;
    }
    new_line = new_line.substr(i, j - i + 1);

    if (commands.count(new_line) != 0U) {
        return commands.at(new_line);
    }

    return Command::INVALID;
}

void parse_testing() {
    std::string const test_string1 = "    sherlock-holmes ";
    std::string const test_string2 = "cat";
    std::string const test_string3 = "aaaa";
    assert(parse_line(test_string1) == Command::SHERLOCK);
    assert(parse_line(test_string2) == Command::CAT);
    assert(parse_line(test_string3) == Command::INVALID);
}

int main() {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

    parse_testing();

    MyMap map;
    Macros macros;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (not macros.check_status() && parse_line(line) == Command::EXIT) {
            break;
        }
        process_command(parse_line(line), line, map, macros);
    }
    macros.check_for_program_end();
    return 0;
}
