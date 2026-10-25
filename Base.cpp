#include <array>
#include <random>
#include <string>

#include <ncurses.h>

int cash = 30;
int apple = 3;
int bread = 3;
int water = 3;
int dis = 0;



namespace {

bool isEnter(int key) {
    return key == '\n' || key == '\r' || key == KEY_ENTER;
}

void waitForKey(const char* message = "\nPress any key to continue...") {
    printw("%s", message);
    refresh();
    getch();
}

void runJob(std::mt19937& generator) {
    std::uniform_int_distribution<int> jobDistribution(1, 5);
    const int job = jobDistribution(generator);

    if (job == 1) {
        printw("You work as a McDonald's worker.\n");
        cash += 13;
    } else if (job == 2) {
        printw("You work as a supermarket worker.\n");
        cash += 15;
    } else if (job == 3) {
        printw("You work as a dropshipper, and some random guy\n");
        printw("buys a random AI scam from you.\n");
        cash += 30;
    } else if (job == 4) {
        printw("You work as a gardener.\n");
        cash += 40;
    } else {
        printw("You work as a Discord mod (not a job, but I don't know).\n");
        cash += 20;
    }
}

void runStockEvent(std::mt19937& generator) {
    constexpr std::array<const char*, 5> choices{
        "Invest in Apple",
        "Invest in Nvidia",
        "Invest in Bitcoin",
        "Invest in random company",
        "Invest in Micron"
    };
    int highlight = 0;

    while (true) {
        clear();
        printw("You do stocks\n\n");
        printw("Use Up/Down arrows and Enter to choose:\n\n");
        for (int i = 0; i < static_cast<int>(choices.size()); ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            printw("  %s\n", choices[static_cast<std::size_t>(i)]);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }
        refresh();

        const int input = getch();
        if (input == KEY_UP) {
            highlight = (highlight + static_cast<int>(choices.size()) - 1) %
                        static_cast<int>(choices.size());
        } else if (input == KEY_DOWN) {
            highlight = (highlight + 1) % static_cast<int>(choices.size());
        } else if (isEnter(input)) {
            break;
        }
    }

    clear();
    switch (highlight) {
        case 0:
            printw("You invest in Apple, but your stocks go down just because.\n");
            cash = 0;
            break;
        case 1:
            printw("You invest in Nvidia and gain money.\n");
            cash += 25;
            break;
        case 2: {
            std::uniform_int_distribution<int> investment(-50, 50);
            const int change = investment(generator);
            printw("You invest in Bitcoin. Your balance changes by $%d.\n", change);
            cash += change;
            break;
        }
        case 3: {
            std::uniform_int_distribution<int> investment(-30, 30);
            const int change = investment(generator);
            printw("You invest in a random company. Your balance changes by $%d.\n", change);
            cash += change;
            break;
        }
        case 4:
            printw("You invest in Micron and lost all your money becasue I dont like them not makeing ddr5.\n");
            printw("And this is my game where i cant control the universe and make what i want happen\n");
            cash = 0;
            break;
        default:
            break;
    }
}

bool goOutside(std::mt19937& generator) {
    std::uniform_int_distribution<int> foodDistribution(1, 3);
    const int food = foodDistribution(generator);
    const char* consumed = nullptr;

    if (food == 1) {
        --apple;
        consumed = "an apple";
    } else if (food == 2) {
        --bread;
        consumed = "one bread";
    } else {
        --water;
        consumed = "one item of water";
    }

    clear();
    if (apple < 0) {
        printw("You ran out of apples, so you ate another red object:\n");
        printw("a fire hydrant. You fall over and die.\n");
    } else if (bread < 0) {
        printw("You ran out of bread and chose the key over the bread.\n");
        printw("You're out of food. You fall over and die.\n");
    } else if (water < 0) {
        printw("You ran out of water and drank from a water fountain.\n");
        printw("But that is how they get you. You fall over and die.\n");
    }

    if (apple < 0 || bread < 0 || water < 0) {
        waitForKey("\nBye bye");
        return false;
    }





    std::uniform_int_distribution<int> eventDistribution(1, 26);
    const int randomEvent = eventDistribution(generator);





    clear();
    printw("Random Event Time\n");
    int playerChoice = -1;

    if (randomEvent == 1) {
        printw("Nothing fun happens and you just take a normal walk.\n");
    } else if (randomEvent == 2) {
        printw("You feel energized from the caffeine this morning and go an extra long walk!\n");
        ++dis;
    } else if (randomEvent == 3) { 
        printw("It's way too cold and you decide to stay home.\n");
        --dis;
    } else if (randomEvent == 4) {
        printw("A DeLorean DMC-12 going 80 miles an hour runs into you\n");
        printw("and you get sent into the future.\n");
        dis += 2;
    } else if (randomEvent == 5) {
        printw("The wind sends you back in time and you lose two hours outside.\n");
        dis -= 2;
    } else if (randomEvent <= 12) {
        printw("You get a J*b. Very scary, I know.\n");
        runJob(generator);
    } else if (randomEvent == 13) {
        printw("You go to the casino.\n");
        std::uniform_int_distribution<int> gamble(700, 800);
        if (gamble(generator) == 777) {
            printw("You have won the jackpot!\n");
            cash += 1000;
        } else {
            printw("You did not win the jackpot.\n");
        }
    } else if (randomEvent >= 14 && randomEvent <= 18) {
        runStockEvent(generator);
    } else if (randomEvent >= 19 && randomEvent <= 20) {
        std::string eventChoices[] = {"Doctor", "CS man", "CEO"};
        constexpr int totalChoices = 3;
        int choiceHighlight = 0;

        while (playerChoice == -1) {
            clear();
            printw("You get an actual j*b\n");

            for (int i = 0; i < totalChoices; i++) {
                if (i == choiceHighlight) {
                    attron(A_REVERSE);
                }
                printw("  %s\n", eventChoices[i].c_str());
                if (i == choiceHighlight) {
                    attroff(A_REVERSE);
                }
            }
            refresh();

            const int input = getch();
            switch (input) {
                case KEY_UP:
                    choiceHighlight--;
                    if (choiceHighlight < 0) choiceHighlight = totalChoices - 1;
                    break;
                case KEY_DOWN:
                    choiceHighlight++;
                    if (choiceHighlight >= totalChoices) choiceHighlight = 0;
                    break;
                default:
                    if (isEnter(input)) {
                        playerChoice = choiceHighlight;
                    }
                    break;
            }
        }

        clear();
        if (playerChoice == 0) {
            printw("You work as a doctor and make money.\n");
            printw("However, because your school bills are so expensive, you only make $100.\n");
            cash += 100;
        } else if (playerChoice == 1) {
            printw("You work in CS, but AI takes over.\n");
            printw("You use AI to make more AI slop and make a ton of money.\n");
            cash += 150;
        } else {
            printw("You work as a CEO.\n");
            printw("However, you make a trash product and lose money.\n");
            cash -= 50;
        }
    } else if (randomEvent == 21) {
        printw("You go to a grocery store and steal an apple phone because why no\n");
        printw("However someone else has already taken the phone and all you get is an apple\n");
        apple++;
    } else if (randomEvent == 22) {
        printw("A five meter squiril like the ones in New Youk walks up to you and steals an apple from you\n");
        apple--;
    } else if (randomEvent == 23) {
        printw("You find a village and steal wheet and use three of it to craft bread");
        bread++;
    } else if (randomEvent == 24) {
        printw("You lose 5 hunger points and eat a piece of bread");
        bread--;
    } else if (randomEvent == 25) {
        printw("You get free water from a free sorce that got it from another free water place"); 
        water++;
    } else if (randomEvent == 26) {
        printw("You fall down but you use one water to do a water bucket clutch");
        water--;
    }

    waitForKey();
    ++dis;

    clear();
    printw("You went outside and explored!\n");
    printw("You used %s.\n", consumed);
    waitForKey();
    return true;
}

void viewInventory() {
    clear();
    printw("You have:\n");
    printw("Cash: $%d\n", cash);
    printw("Apples: %d\n", apple);
    printw("Bread: %d\n", bread);
    printw("Water: %d\n", water);
    waitForKey("\nPress any key to go back...");
}

void shop() {
    constexpr std::array<const char*, 4> options{
        "Buy Apple ($20)",
        "Buy Bread ($20)",
        "Buy Water ($20)*",
        "Back to Menu"
    };
    constexpr std::array<int, 3> prices{20, 20, 20};
    int highlight = 0;
    std::string message;
    bool inShop = true;

    while (inShop) {
        clear();
        printw("SHOP\n");
        printw("Your Cash: $%d\n", cash);
        printw("Use Up/Down arrows and Enter to buy:\n\n");
        printw("* Unfortruntly for you water is not a human right inside this game for balancing reasons\n");

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            printw(" %s\n", options[static_cast<std::size_t>(i)]);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }
        if (!message.empty()) {
            printw("\n%s\n", message.c_str());
        }
        refresh();

        const int input = getch();
        if (input == KEY_UP) {
            highlight = (highlight + static_cast<int>(options.size()) - 1) %
                        static_cast<int>(options.size());
        } else if (input == KEY_DOWN) {
            highlight = (highlight + 1) % static_cast<int>(options.size());
        } else if (isEnter(input)) {
            if (highlight == 3) {
                inShop = false;
            } else if (cash < prices[static_cast<std::size_t>(highlight)]) {
                message = "Not enough cash.";
            } else {
                cash -= prices[static_cast<std::size_t>(highlight)];
                if (highlight == 0) {
                   ++apple;
                    message = "Bought an apple.";
                } else if (highlight == 1) {
                    ++bread;
                    message = "Bought bread.";
                } else {
                    ++water;
                    message = "Bought water.";
                }
            }
        }
    }
}

void showInstructions() {
    clear();
    printw("You are a random person who is trying to go outside a lot.\n");
    printw("Each trip uses one randomly selected food item.\n");
    printw("Run out of any food category and you die.\n");
    printw("Random events can change your time outside and your cash.\n");
    printw("GLHF\n");
    waitForKey("\nPress any key to go back...");
}

} // namespace

int main() {
    if (initscr() == nullptr) {
        return 1;
    }
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    constexpr std::array<const char*, 5> options{
        "Go Outside",
        "View Inventory",
        "Buy Stuff",
        "How to Play",
        "Exit Game"
    };
    std::mt19937 generator(std::random_device{}());
    int highlight = 0;
    bool running = true;

    while (running) {
        clear();
        printw("Going Outside Sim (Very realistic)\n");
        printw("You have gone out %d hours\n", dis);
        printw("Use Up and Down arrows to find and use Enter to select options\n\n");

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            printw(" %s\n", options[static_cast<std::size_t>(i)]);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }
        refresh();

        const int input = getch();
        if (input == KEY_UP) {
            highlight = (highlight + static_cast<int>(options.size()) - 1) %
                        static_cast<int>(options.size());
        } else if (input == KEY_DOWN) {
            highlight = (highlight + 1) % static_cast<int>(options.size());
        } else if (isEnter(input)) {
            switch (highlight) {
                case 0:
                    if (!goOutside(generator)) {
                        running = false;
                    }
                    break;
                case 1:
                    viewInventory();
                    break;
                case 2:
                    shop();
                    break;
                case 3:
                    showInstructions();
                    break;
                case 4:
                    running = false;
                    break;
                default:
                    break;
            }
        }
    }

    endwin();
    return 0;
}
