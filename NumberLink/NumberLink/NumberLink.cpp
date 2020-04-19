#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
	Board bd(4, 4);
	//bd.setNumber("1234........1234");
	bd.setNumber("1231.....23.....");
#if	0
	Board bd(7, 7);
	bd.setNumber("...4....3..25....31.....5............1....2...4..");
	cout << bd.text0() << "\n\n";
	cout << bd.text() << "\n\n";
	cout << bd.text2() << "\n\n";
#endif
	cout << bd.text3() << "\n\n";
	//cout << bd.textLink2() << "\n";
#if	0
	if( bd.doSolve2() ) {
		cout << "solved.\n";
		cout << bd.text2() << "\n";
	} else {
		cout << "can't solved.\n";
	}
#endif
	//
    std::cout << "\nOK\n";
}
