使い方
```cpp
#include "chem30338.h"

int main() {

    using namespace chem30338;

    constexpr float h = 0.05f;

    constexpr pid_params params = pid_params_builder(
                p_gain{0.8f}, i_time{0.4f}, d_time{0.1f}
            )
            .alpha(0.1f).beta(0.5f).gamma(0.5f)
            .params();

    pid_bilinear pid(params, h);

    for (;;) {
        float r, y;
        std::scanf("%f,%f", &r, &y);
        float u = pid.update(r, y);
        std::printf("%f\n", u);
    }
}
```
C++14 以上が必要

## `chem30338.h`

### `class pid_bilinear`
PID制御器

参照: https://jckantor.github.io/CBE30338/05.04-Controller-Tuning-Rules-in-Frequency-Domain.html

- 目標値重み付け、近似微分を用いる
- 双一次変換による離散化
- クランピングやワインドアップ防止はされない

`pid_bilinear::pid_bilinear(const pid_params &params, float h)`
- `params`: パラメータ
- `h`: サンプリング周期

`float pid_bilinear::update(float sp, float meas)`
- `sp`: 目標値
- `meas`: 制御量
- 戻り値: 制御入力

### `class pid_params_builder`
パラメータビルダ

- `pid_params`はこれを使って作成することを推奨
- `alpha`, `beta`, `gamma` はデフォルト値が存在する
- デフォルト値は今後変わるかもしれないので、(特にローパスフィルタのパラメータ`alpha` or `lpf_tau`は) それぞれのメンバ関数によって明示的に設定することを推奨

`pid_params_builder::pid_params_builder(p_gain kp, i_gain ki, d_gain kd)`

- 比例ゲイン、積分時間、微分時間から初期化
- `alpha = 0.02f`, `beta = 1.f`, `gamma = 1.f` に設定される

`pid_params_builder::pid_params_builder(p_gain kp, i_time ti, d_time td)`

- 比例ゲイン、積分ゲイン、微分ゲインから初期化
- `alpha = 0.15f*td`, `beta = 1.f`, `gamma = 1.f` に設定される

`pid_params_builder &pid_params_builder::{alpha, beta, gamma, lpf_tau}(float value)`

- `alpha`, `beta`, `gamma`をそれぞれ設定
- `lpf_tau`は近似微分におけるローパスフィルタの時定数

`pid_params pid_params_builder::params() const`
- `pid_params`を生成する

## 参考
[CBE30338 Controller Tuning Rules in Frequency Domain](https://jckantor.github.io/CBE30338/05.04-Controller-Tuning-Rules-in-Frequency-Domain.html)
[PID Controller Implementation in Software](https://www.youtube.com/watch?v=zOByx3Izf5U)
[ディジタル制御システム 第３章 ディジタル PI 制御と離散化](https://nagasaki-u.repo.nii.ac.jp/?action=repository_uri&item_id=3653&file_id=17&file_no=7&nc_session=5722c0uq7djout21sksdqpbpu6)

## `zsequence.h`

### `template <class T, std::size_t N> class zsequence<T, N>`
- 時系列データを表す
- `curr()`によって現在の値、`prev<I>()`によって`I`個前の値を取得 (`prev(i)`はない)
- `advance(next)`によって次の時刻の値をプッシュ
- リスト初期化により初期値を設定(`zsequence<float, 2> seq = { 2, 3 };`)
- N == 2 のとき
    - 現在と一つ前の値を直接移動させる
    - `prev()` をもつ (`prev<1>()`と等価)
- N > 2 のとき
    - リングバッファによるキュー
