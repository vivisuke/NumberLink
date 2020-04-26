#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
#if	0
	Board bd(4, 4);
	//bd.setNumber("1234........1234");
	bd.setNumber("1231.....23.....");
#else
	Board bd(7, 7);
	//bd.setNumber("...4....3..25....31.....5............1....2...4..");
	bd.setNumber("....321""....1.."".......""..2....""......."".35..4.""4.....5");
#endif
#if	0
	cout << "text0():\n";
	cout << bd.text0() << "\n\n";
	cout << "text():\n";
	cout << bd.text() << "\n\n";
	cout << "text2():\n";
	cout << bd.text2() << "\n\n";
#endif
	cout << "text3():\n";
	cout << bd.text3() << "\n\n";
	//cout << bd.textLink2() << "\n";
#if	1
	if( bd.doSolveBT() ) {
		cout << "solved.\n";
		cout << bd.text3() << "\n";
	} else {
		cout << "can't solved.\n";
	}
#endif
	//
    std::cout << "\nOK\n";
}
