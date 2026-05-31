#include<iostream>
#include<vector>
#include<algorithm>
#include<bitset>
#include<random>
#include<chrono>
#include<climits>
using namespace std;
typedef long long ll;
const int N = 1004;

int n;
struct Soda { ll x,y; int idx; };

vector<Soda> orders, orders_by_x, orders_by_y;
vector<ll> x_val, y_val;
size_t n_x_layer, n_y_layer;

struct Result {
    vector<pair<Soda,Soda>> ans;
    ll cost;
};

Result solve(mt19937& rng)
{
    vector<Soda> sodas = {{0,0,0}};
    bitset<N> made;
    vector<pair<Soda,Soda>> ans;
    ll total_cost = 0;

    auto blend = [&](ll tx, ll ty) -> bool {
        ll best = LLONG_MAX;
        int best_i = -1;
        for(size_t i = 0; i < sodas.size(); ++i)
        {
            if(sodas[i].x <= tx && sodas[i].y <= ty)
            {
                ll c = (tx - sodas[i].x) + (ty - sodas[i].y);
                if(c < best) { best = c; best_i = (int)i; }
            }
        }
        if(best_i < 0) return false;
        ans.push_back({sodas[best_i], {tx, ty, 0}});
        sodas.push_back({tx, ty, 0});
        total_cost += best;
        return true;
    };

    size_t layer_i = 0, layer_j = 0;
    while(layer_i < n_x_layer || layer_j < n_y_layer)
    {
        if(made.count() == (size_t)n) break;

        bool do_x = (layer_i < n_x_layer);
        bool do_y = (layer_j < n_y_layer);

        if(do_x && do_y)
        {
            int r = rng() % 100;
            if(r < 45) { do_x = true; do_y = true; }
            else if(r < 72) { do_x = true; do_y = false; }
            else { do_x = false; do_y = true; }
        }

        size_t cur_x = layer_i, cur_y = layer_j;
        if(do_x) cur_x = layer_i;
        if(do_y) cur_y = layer_j;

        if(do_x && do_y)
        {
            blend(x_val[cur_x], y_val[cur_y]);
        }
        else if(do_x)
        {
            if(layer_j > 0) blend(x_val[cur_x], y_val[layer_j - 1]);
            else blend(x_val[cur_x], 0);
        }
        else
        {
            if(layer_i > 0) blend(x_val[layer_i - 1], y_val[cur_y]);
            else blend(0, y_val[cur_y]);
        }

        auto try_make = [&](const vector<Soda>& obs, ll xv, ll yv) {
            for(size_t k = 0; k < (size_t)n; ++k)
            {
                if(!made[(size_t)obs[k].idx])
                {
                    if(obs[k].x <= xv && obs[k].y <= yv)
                    {
                        blend(obs[k].x, obs[k].y);
                        made[(size_t)obs[k].idx] = true;
                    }
                }
            }
        };

        ll reach_x = do_x ? x_val[cur_x] : (layer_i > 0 ? x_val[layer_i - 1] : 0);
        ll reach_y = do_y ? y_val[cur_y] : (layer_j > 0 ? y_val[layer_j - 1] : 0);
        try_make(orders_by_x, reach_x, (ll)2e9);
        try_make(orders_by_y, (ll)2e9, reach_y);

        if(do_x && layer_i < n_x_layer) ++layer_i;
        if(do_y && layer_j < n_y_layer) ++layer_j;

        if(!do_x && !do_y) break;
    }

    for(size_t i = 0; i < (size_t)n; ++i)
    {
        if(!made[i])
        {
            blend(orders[i].x, orders[i].y);
            made[i] = true;
        }
    }

    return {ans, total_cost};
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    orders.resize((size_t)n);
    x_val.resize((size_t)n);
    y_val.resize((size_t)n);
    for(size_t i = 0; i < (size_t)n; i++)
    {
        cin >> orders[i].x >> orders[i].y;
        x_val[i] = orders[i].x;
        y_val[i] = orders[i].y;
        orders[i].idx = (int)i;
    }

    orders_by_x = orders;
    orders_by_y = orders;
    sort(orders_by_x.begin(), orders_by_x.end(), [](const Soda& a, const Soda& b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    sort(orders_by_y.begin(), orders_by_y.end(), [](const Soda& a, const Soda& b){
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    });
    sort(x_val.begin(), x_val.end());
    sort(y_val.begin(), y_val.end());
    x_val.erase(unique(x_val.begin(), x_val.end()), x_val.end());
    y_val.erase(unique(y_val.begin(), y_val.end()), y_val.end());
    n_x_layer = x_val.size();
    n_y_layer = y_val.size();

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    const int TRIALS = 20;
    Result best;
    best.cost = LLONG_MAX;

    for(int t = 0; t < TRIALS; t++)
    {
        Result r = solve(rng);
        if(r.cost < best.cost) best = move(r);
    }

    cerr << "best cost: " << best.cost << "\n";
    cout << best.ans.size() << "\n";
    for(const auto& [from, to] : best.ans)
    {
        cout << from.x << " " << from.y << " " << to.x << " " << to.y << "\n";
    }
    return 0;
}
