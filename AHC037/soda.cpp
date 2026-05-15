#include<iostream>
#include<vector>
#include<algorithm>
#include<bitset>
using namespace std;
typedef long long ll;
const int N = 1004;

int n;

struct Soda
{
    ll x,y;
    int idx;
};
bitset<N> made; //from right to left, which soda is made

vector<Soda> sodas= {{0,0,0}};
vector<pair<Soda,Soda>> ans;

void blend(ll x, ll y)
{
    ll cnt = (2e9+7);
    for(size_t i = 0; i < sodas.size(); ++i)
    {
        if(sodas[i].x <= x && sodas[i].y <= y)
        {
            cnt = min(cnt, (x-sodas[i].x)+(y-sodas[i].y));
        }
    }
    for(size_t i = 0; i < sodas.size(); ++i)
    {
        if(sodas[i].x <= x && sodas[i].y <= y && (x-sodas[i].x)+(y-sodas[i].y) == cnt)
        {
            ans.push_back({sodas[i],{x,y,0}});
            return;
        }
    }
    return;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    vector<Soda> orders((size_t)n);

    vector<Soda> orders_by_x, orders_by_y;

    vector<ll>x_val((size_t)n),y_val((size_t)n);
    for (size_t i = 0; i < (size_t)n; i++)
    {
        cin>>orders[i].x>>orders[i].y;
        x_val[i] = orders[i].x;
        y_val[i] = orders[i].y;
        orders[i].idx = (int)i;
    }

    orders_by_x = orders;
    orders_by_y = orders;

    sort(orders_by_x.begin(),orders_by_x.end(), [](const Soda& a, const Soda& b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    sort(orders_by_y.begin(),orders_by_y.end(), [](const Soda& a, const Soda& b){
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    });

    sort(x_val.begin(),x_val.end());
    sort(y_val.begin(),y_val.end());
    x_val.erase(unique(x_val.begin(),x_val.end()),x_val.end());
    y_val.erase(unique(y_val.begin(),y_val.end()),y_val.end());
    


    size_t n_x_layer = x_val.size(), n_y_layer = y_val.size();
    
    for(size_t i =0, j =0, layer_i = 0,layer_j =0; layer_i < n_x_layer && layer_j < n_y_layer && i < (size_t)n  && j<(size_t)n; )
    {
        //cout<<orders_by_x[i].x<<" "<<orders_by_x[i].y<<" "<<orders_by_y[j].x<<" "<<orders_by_y[j].y<<" "<<layer_i<<" "<<layer_j<<"\n";
        if(made.count() == (size_t)n)break;
        blend(x_val[layer_i],y_val[layer_j]);
        //sodas.push_back({x_val[layer_i],y_val[layer_j],0});
        
        for(size_t k = 0; k < (size_t)n; ++k)
        {
            if(orders_by_x[k].x == x_val[layer_i] || orders_by_x[k].y >= y_val[layer_j])
            {if(!made[(size_t)orders_by_x[k].idx])
                {
                    blend(orders_by_x[k].x,orders_by_x[k].y);
                    made[(size_t)orders_by_x[k].idx] = true;
                }
            }
        }
        for(size_t k = 0; k < (size_t)n; ++k)
        {
            if(orders_by_y[k].x == y_val[layer_j] || orders_by_y[k].x >= x_val[layer_i])
            {
                if(!made[(size_t)orders_by_y[k].idx])
                {
                    blend(orders_by_y[k].x,orders_by_y[k].y);
                    made[(size_t)orders_by_y[k].idx] = true;
                }
            }
        }
       
        if(layer_i+1 <n_x_layer&&layer_j+1 <n_y_layer)++layer_i,++layer_j;
        else if(layer_i+1 <n_x_layer)++layer_i;
        else if(layer_j+1 <n_y_layer)++layer_j;
        else break;

    }



    cout<< ans.size()<<"\n";    
    for(const auto& [from, to]: ans)
    {
        cout<< from.x<<" "<<from.y<<" "<<to.x<<" "<<to.y<<"\n";
    }
    return 0;
}