#include<cstdio>
#include<functional>

struct load_status {
    int cur;        // 0 表示第一艘船, 1 表示第二艘船
    int weights[2];

    load_status() {}
    load_status(int loc, int lw1, int lw2):
        cur(loc), weights { lw1, lw2 } {}
};

constexpr int ARRAY_SIZE = 100;

int CARRIGES;
int WEIGHT_LIMIT[2];
int WEIGHTS[ARRAY_SIZE];

load_status solution[ARRAY_SIZE];

int backtrack()
{
    int sol = 0;
    static std::function<bool(load_status &)> constraint = [] (load_status &t) -> bool
    {
        for (int i = 0; i < 2; i++) {
            if (t.weights[i] > WEIGHT_LIMIT[i]) {
                return false;
            }
        }
        return true;
    };

    static std::function<void(int)> iter = [&] (int place)
    {
        if (place >= CARRIGES) {
            
            std::printf("=====解法%d=====\n", ++sol);
            for (int i = 0; i < CARRIGES; i++) {
                load_status &tls = solution[i];
                std::printf("将%d号集装箱放到%d号船上，此时1号船载重%d, 2号船载重%d\n", 
                    i + 1, tls.cur + 1, tls.weights[0], tls.weights[1]);
            }
            std::putchar('\n');
            return ;
        }

        load_status last = (place == 0) ? load_status { 0, 0, 0 } : solution[place - 1];
        for (int i = 0; i < 2; i++) {
            solution[place] = { i, last.weights[0], last.weights[1] };
            solution[place].weights[i] = last.weights[i] + WEIGHTS[place];

            if (constraint(solution[place])) {
                iter(place + 1);
            }
        }
    };

    iter(0);
    return sol;
}

int main(int argc, char *argv[])
{
    std::scanf(" %d %d %d", &CARRIGES, &WEIGHT_LIMIT[0], &WEIGHT_LIMIT[1]);

    for (int i = 0; i < CARRIGES; i++) {
        std::scanf(" %d", &WEIGHTS[i]);
    }

    std::printf("%s", backtrack() ? "": "无解\n");
    return 0;
}