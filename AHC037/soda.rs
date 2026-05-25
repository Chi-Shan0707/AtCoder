use std::cmp::min;
use std::io::{self, Read, BufWriter, Write};

#[derive(Clone, Copy, Debug)]
struct Soda {
    x: i64,
    y: i64,
    idx: usize,
}

fn main() {
    // 快速读取所有输入
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut tokens = input.split_whitespace();

    // 读取 n
    let n: usize = match tokens.next() {
        Some(s) => s.parse().unwrap(),
        None => return,
    };

    let mut orders = Vec::with_capacity(n);
    let mut x_val = Vec::with_capacity(n);
    let mut y_val = Vec::with_capacity(n);

    for i in 0..n {
        let x: i64 = tokens.next().unwrap().parse().unwrap();
        let y: i64 = tokens.next().unwrap().parse().unwrap();
        orders.push(Soda { x, y, idx: i });
        x_val.push(x);
        y_val.push(y);
    }

    let mut orders_by_x = orders.clone();
    let mut orders_by_y = orders.clone();

    // 按照 lambda 表达式排序
    orders_by_x.sort_unstable_by(|a, b| a.x.cmp(&b.x).then(a.y.cmp(&b.y)));
    orders_by_y.sort_unstable_by(|a, b| a.y.cmp(&b.y).then(a.x.cmp(&b.x)));

    x_val.sort_unstable();
    y_val.sort_unstable();
    x_val.dedup(); // 相当于 std::unique + erase
    y_val.dedup();

    let n_x_layer = x_val.len();
    let n_y_layer = y_val.len();

    let mut made = vec![false; n];
    let mut made_count = 0; // 记录已完成的订单数，代替 made.count()

    let sodas = vec![Soda { x: 0, y: 0, idx: 0 }];
    let mut ans = Vec::new();

    // 闭包代替全局 blend 函数
    let blend = |x: i64, y: i64, ans: &mut Vec<(Soda, Soda)>| {
        let mut cnt = 2_000_000_007_i64; // (2e9+7)
        for soda in &sodas {
            if soda.x <= x && soda.y <= y {
                cnt = min(cnt, (x - soda.x) + (y - soda.y));
            }
        }
        for soda in &sodas {
            if soda.x <= x && soda.y <= y && (x - soda.x) + (y - soda.y) == cnt {
                ans.push((*soda, Soda { x, y, idx: 0 }));
                return;
            }
        }
    };

    let mut layer_i = 0;
    let mut layer_j = 0;
    // 忠实还原 C++ 代码中声明但在循环内未自增的 i 和 j
    let i = 0;
    let j = 0;

    while layer_i < n_x_layer && layer_j < n_y_layer && i < n && j < n {
        if made_count == n {
            break;
        }
        
        blend(x_val[layer_i], y_val[layer_j], &mut ans);
        // sodas.push(Soda{ x: x_val[layer_i], y: y_val[layer_j], idx: 0 }); // 原代码已注释

        for k in 0..n {
            if orders_by_x[k].x == x_val[layer_i] || orders_by_x[k].y >= y_val[layer_j] {
                let idx = orders_by_x[k].idx;
                if !made[idx] {
                    blend(orders_by_x[k].x, orders_by_x[k].y, &mut ans);
                    made[idx] = true;
                    made_count += 1;
                }
            }
        }

        for k in 0..n {
            // 这里忠实于 C++ 的原始逻辑 (比较的 x 和 y_val)
            if orders_by_y[k].x == y_val[layer_j] || orders_by_y[k].x >= x_val[layer_i] {
                let idx = orders_by_y[k].idx;
                if !made[idx] {
                    blend(orders_by_y[k].x, orders_by_y[k].y, &mut ans);
                    made[idx] = true;
                    made_count += 1;
                }
            }
        }

        if layer_i + 1 < n_x_layer && layer_j + 1 < n_y_layer {
            layer_i += 1;
            layer_j += 1;
        } else if layer_i + 1 < n_x_layer {
            layer_i += 1;
        } else if layer_j + 1 < n_y_layer {
            layer_j += 1;
        } else {
            break;
        }
    }

    // 使用 BufWriter 快速输出
    let stdout = io::stdout();
    let mut out = BufWriter::new(stdout.lock());

    writeln!(out, "{}", ans.len()).unwrap();
    for (from, to) in ans {
        writeln!(out, "{} {} {} {}", from.x, from.y, to.x, to.y).unwrap();
    }
}
