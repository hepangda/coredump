#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<functional>

constexpr int QUEENS = 8;

int probs[QUEENS] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int solution[QUEENS];

int backtrack()
{
    int sol = 0;
    static std::function<bool(int)> constriant = [] (int n) -> bool
    {
        for (int i = 0; i < n; i++) {
            if (std::abs(n - i) == std::abs(solution[n] - solution[i])) {
                return false;
            }
        }
        return true;
    };

    static std::function<void(int)> iter = [&] (int n)
    {
        if (n == QUEENS) {
            std::printf("=====解法%d=====\n", ++sol);
            for (int i = 0; i < QUEENS; i++) {
                for (int j = 0; j < QUEENS; j++) {
                    std::putchar(solution[i] == j ? '@' : '.');
                }
                std::putchar('\n');
            }
            std::putchar('\n');
            return ;
        }

        for (int i = n; i < QUEENS; i++) {
            solution[n] = probs[i];
            std::swap(probs[n], probs[i]);
            if (constriant(n)) {
                iter(n + 1);
            }
            std::swap(probs[n], probs[i]);
        }
    };

    iter(0);
    return sol;
}

int main(int argc, char *argv[])
{
    std::printf("\n共有%d种解法。\n", backtrack());

    return 0;
}