#include <utility>
#include <iostream>
#include <cstdio>

#include "chem30338.h"

int main() {

    setvbuf(stdin, NULL, _IOLBF, 0);
    setvbuf(stdout, NULL, _IOLBF, 0);

    using namespace chem30338;

    constexpr float h = 0.05f;

    constexpr pid_params params = pid_params_builder(
                p_gain{0.8f}, i_time{0.4f}, d_time{0.1f}
            )
            .alpha(0.1).beta(0.5f).gamma(0.5f)
            .params();

    pid_bilinear pid(params, h);

    for (;;) {
        float r, y;
        std::scanf("%f,%f", &r, &y);
        float u = pid.update(r, y);
        std::printf("%f\n", u);
    }
}
