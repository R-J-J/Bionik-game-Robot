/*
 * Network.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: Tomek Ferens (fero)
 */

#include "main.hpp"
#include <stdlib.h>

using namespace jsonrpc;
using namespace std;

struct Command move;

class MyStubServer : public AbstractStubServer
{
public:
	MyStubServer(AbstractServerConnector &connector,Network *n);
	Network * N;
	virtual void notifyServer();
	virtual string moveRobot(int x,int y,int z);
};

MyStubServer::MyStubServer(AbstractServerConnector &connector,Network *n) :
    		AbstractStubServer(connector),N(n)
{
}
void MyStubServer::notifyServer()
{
	cout << "ROBOT#1 got notified" << endl;
}
string MyStubServer::moveRobot(int x,int y, int z)
{
	move.xCent=x;
	move.yCent=y;
	move.zRad=z;
	if(x){
		move.type=HORIZONTAL;
	}
	else if(y){
		move.type=VERTICAL;
	}
	else if(z){
		move.type=ROTATE;
	}
	N->sendSig((void*)(&move));
#ifdef DEBUG
	cout<<"po stronie servera otrzymano wartość x = "<<x<<endl;
	cout<<"po stronie servera otrzymano wartość y = "<<y<<endl;
	cout<<"po stronie servera otrzymano wartość z = "<<z<<endl;
#endif
	return "Robot executed command";
}




Network::Network(int port,Controller * C): httpserver(port),robotId(C->robotId){
	name="NETWORK";

}


bool Network::work(void *wsk)
{

	MyStubServer s(httpserver,this);
	s.StartListening();
	getchar();
	s.StopListening();
	return 0;
}

