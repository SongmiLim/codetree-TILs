#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    int customer_num=0;
    vector <long long> total_customer;
    int lead_search_num;
    int member_search_num;
    long long ans = 0;
    std::cin>>n;

    for(int i=0;i<n;i++){
        std::cin>>customer_num;
        total_customer.push_back(customer_num);
    }

    std::cin>>lead_search_num;
    std::cin>>member_search_num;

    for(int i=0;i<n;i++){
        total_customer[i] -= lead_search_num;
        ans++;
        if(total_customer[i]>0){
            total_customer[i]%member_search_num==0 ? ans+=total_customer[i]/member_search_num : ans+=total_customer[i]/member_search_num+1;
        }
    }
    cout<<ans;

    return 0;
}