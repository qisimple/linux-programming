#include <iostream>

using namespace std;

extern int dblogin(string, string);

int main(void)
{
	int Islogsuccess;
	Islogsuccess = dblogin("hubian", "who");
	if(Islogsuccess == 1)
		cout << "login success!" << endl;
	else
		cout << "login failure!" << endl;

	return 0;
}
