#ifndef CHEM30338_H_INCLUDED
#define CHEM30338_H_INCLUDED

#include "zsequence.h"

// reference: https://jckantor.github.io/CBE30338/05.04-Controller-Tuning-Rules-in-Frequency-Domain.html

namespace chem30338 {

using zseq::zsequence;

struct pid_params {
    float k_p, tau_i, tau_d;
    float alpha, beta, gamma;
};

struct p_gain {
    float value;
};

struct i_gain {
    float value;
};

struct d_gain {
    float value;
};

struct i_time {
    float value;
};

struct d_time {
    float value;
};

class pid_params_builder {
public:
    explicit constexpr pid_params_builder(p_gain p, i_time i, d_time d)
        : params_{ p.value, i.value, d.value, 0.2f, 1.f, 1.f }
    {
    }

    explicit constexpr pid_params_builder(p_gain p, i_gain i, d_gain d)
        : pid_params_builder(p_gain{p.value}, i_time{p.value/i.value}, d_time{d.value/p.value})
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

    constexpr pid_params_builder &alpha(float a) {
        params_.alpha = a;
        return *this;
    }

    constexpr pid_params_builder &lpf_tau(float tau) {
        params_.alpha = tau/params_.tau_d;
        return *this;
    }

    constexpr pid_params into_params() const {
        return params_;
    }

    constexpr operator pid_params() const {
        return into_params();
    }

private:
    pid_params params_;
};

class pid_bilinear {
public:
    explicit pid_bilinear(const pid_params &params, float h)
        : i_state_{}, d_state_{}, error_p_{}, error_i_{}, error_d_{}, time_step_(h), params_{params}
    {
    }

    float update(float sp, float meas) {

        // aliases
        const auto &k_p = params_.k_p;
        const auto &tau_i = params_.tau_i;
        const auto &tau_d = params_.tau_d;
        const auto &alpha = params_.alpha;
        const auto &beta = params_.beta;
        const auto &gamma = params_.gamma;

        error_p_.advance(beta*sp - meas);
        error_i_.advance(sp - meas);
        error_d_.advance(gamma*sp - meas);
        
        const float p = k_p * error_p_.curr();
        const float i =
            i_state_.prev() + k_p * time_step_* (error_i_.curr() + error_i_.prev()) / (2*tau_i);
        const float d =
            ((2*alpha*tau_d - time_step_) * d_state_.prev() + 2*k_p*tau_d*(error_d_.curr() - error_d_.prev()))
                / (2*alpha*tau_d + time_step_);

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
