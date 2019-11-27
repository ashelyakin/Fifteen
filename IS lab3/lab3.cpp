#include "lab3.h"
#include<ctime>

int main()
{
	string text = "51EAFD7B94C83260";
	//string text = "123456789ABC0DEF";
	int time1 = clock();
	fifteen_A(text);
	int time2 = clock();
	cout << endl << "time A*: " << (time2 - time1)/1000 << endl;
	/*time1 = clock();
	fifteen_IDA(text);
	time2 = clock();
	cout << endl << "time IDA: " << (time2 - time1) / 1000 << endl;*/
	/*auto n = new Node({ 1, 1, vector<int>(), nullptr });
	n->state.resize(16);
	for (int i = 0; i < 16; i++)
		n->state[i] = i;
	delete n;
	cout<<n->state.capacity();*/
}