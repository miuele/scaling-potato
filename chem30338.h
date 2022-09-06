#ifndef CHEM30338_H_INCLUDED
#define CHEM30338_H_INCLUDED

#include "zsequence.h"

// reference: https://jckantor.github.io/CBE30338/05.04-Controller-Tuning-Rules-in-Frequency-Domain.html

namespace chem30338 {

using zseq::zsequence;

struct pid_params {
    float k_p, k_i, k_d;
    float tau, beta, gamma;
};

struct p_gain { float value; };
struct i_gain { float value; };
struct d_gain { float value; };
struct i_time { float value; };
struct d_time { float value; };

class pid_params_builder {
public:
    explicit constexpr pid_params_builder(p_gain kp, i_gain ki, d_gain kd)
        : params_{ kp.value, ki.value, kd.value, 0.02f, 1.f, 1.f }
    {
    }

    explicit constexpr pid_params_builder(p_gain kp, i_time ti, d_time td)
        : params_{ kp.value, kp.value/ti.value, kp.value*td.value, 0.15f*td.value, 1.f, 1.f }
    {
    }

    constexpr pid_params_builder(const pid_params_builder &) = default;
    constexpr pid_params_builder(pid_params_builder &&) = default;

    constexpr pid_params_builder &beta(float b) {
        params_.beta = b;
        return *this;
    }

    constexpr pid_params_builder &gamma(float c) {
        params_.gamma = c;
        return *this;
    }

    // division by zero when k_p == 0
    constexpr pid_params_builder &alpha(float a) {
        params_.tau = a * params_.k_d / params_.k_p;
        return *this;
    }

    constexpr pid_params_builder &lpf_tau(float tau) {
        params_.tau = tau;
        return *this;
    }

    constexpr pid_params params() const {
        return params_;
    }

    constexpr operator pid_params() const {
        return params();
    }

private:
    pid_params params_;
};

class pid_bilinear {
public:
    constexpr explicit pid_bilinear(const pid_params &params, float h)
        : i_state_{}, d_state_{}, error_p_{}, error_i_{}, error_d_{}, time_step_(h), params_{params}
    {
    }

    float update(float sp, float meas) {

        // aliases
        const auto &k_p = params_.k_p;
        const auto &k_i = params_.k_i;
        const auto &k_d = params_.k_d;
        const auto &tau = params_.tau;
        const auto &beta = params_.beta;
        const auto &gamma = params_.gamma;

        error_p_.advance(beta*sp - meas);
        error_i_.advance(sp - meas);
        error_d_.advance(gamma*sp - meas);
        
        const float p = k_p * error_p_.curr();
        const float i =
            i_state_.curr() + k_i * time_step_* (error_i_.curr() + error_i_.prev()) / 2;
        const float d =
            ((2*tau - time_step_) * d_state_.curr() + 2*k_d*(error_d_.curr() - error_d_.prev()))
                / (2*tau + time_step_);

        i_state_.advance(i);
        d_state_.advance(d);

        return p + i + d;
    }

private:
    zsequence<float, 2> i_state_;
    zsequence<float, 2> d_state_;
    zsequence<float, 2> error_p_;
    zsequence<float, 2> error_i_;
    zsequence<float, 2> error_d_;
    float time_step_;
    pid_params params_;
};

}

#endif
