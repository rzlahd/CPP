#include <iostream>
using namespace std;

void fi(char v[]);
void fp(char v[]);
void use(char v1[]);

int main()
{
    fi("me");
}

void fi(char v[])
{
    for (int i = 0; v[i] != 0; ++i)
        use(v[i]);
}
void fp(char v[])
{
    for (char* p = v; *p!=0; ++p)
        cout<< *p<<endl;
}

void use(char v1[]) {

}
