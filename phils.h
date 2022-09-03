
inline namespace phils {

struct pid {
    zsequence<float, 2> i;
    zsequence<float, 2> d;

    template <std::size_t M, std::size_t N>
    float update(
            const zsequence<float, M> &sp,
            const zsequence<float, N> &meas,
            float kp, float ki, float kd, float tau,
            float h
            ) {
        const zsequence<float, 2> err = {
            sp.curr() - meas.curr(),
            sp.template prev<1>() - meas.template prev<1>()
        };
        const float p = kp * err.curr();
        this->i.advance(
                ki*h*(err.curr() + err.prev())*0.5f
                + i.curr());
        this->d.advance(
                2*kd*(err.curr() - err.prev())/(2*tau + h)
                + (2*tau - h)*d.curr()/(2*tau + h));
        return p + i.curr() + d.curr();
    }
};


}
