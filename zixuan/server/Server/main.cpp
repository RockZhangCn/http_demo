#include <iostream>
#include <stdio.h>
#include<server.h>
using namespace std;

int  main()
{
    Server server  ;
    if(server.Listen()<0){
        cout<<"Server failed to start"<<endl;
    }
    server.Start();

}
