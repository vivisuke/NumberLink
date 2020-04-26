#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
#if	1
	Board bd(9, 9);
	for (int i = 0; i < 1000000; ++i) {
		bd.setRandom(9);
		if( bd.isUniqBT() ) {
			cout << "i = " << i << "\n";
			break;
		}
	}
	//cout << bd.text2() << "\n";
	cout << bd.text3() << "\n";
	//bd.setNumber("1234........1234");
	//bd.setNumber("1231.....23.....");
#endif
#if	0
	//Board bd(7, 7);
	//bd.setNumber("...4....3..25....31.....5............1....2...4..");
	//bd.setNumber("....321""....1.."".......""..2....""......."".35..4.""4.....5");		//	7x7
	Board bd(10, 10);
	bd.setNumber(	".........4"
					".....5.67."
					"..12......"
					".........."
					"...13....."
					".....43..."
					".........."
					"......52.."
					".78.8....."
					"6........." );		//	10x10
#endif
#if	0
	Board bd(13, 13);
	bd.setNumber(	"............."
					".12.......34."
					"............."
					"..56..7..78.."
					"......9......"
					"...A3...45..."
					"2...........B"
					"...CD...B1..."
					"......8......"
					"..DC..9..FE.."
					"............."
					".6F.......EA."
					"............." );		//	15x10
#endif
#if	0
	Board bd(15, 10);
	bd.setNumber(	".....5........."
					"............17."
					"..2..8........."
					".........9....."
					".3......4..A.45"
					"12.3..9......6."
					".....7........."
					".........8..B.."
					".BA............."
					".........6....." );		//	15x10
#endif
#if	0
	cout << "text0():\n";
	cout << bd.text0() << "\n\n";
	cout << "text():\n";
	cout << bd.text() << "\n\n";
	cout << "text2():\n";
	cout << bd.text2() << "\n\n";
#endif
#if	0
	cout << "text():\n";
	cout << bd.text() << "\n\n";
#endif
	//cout << bd.textLink2() << "\n";
#if	0
	if( bd.doSolveBT() ) {
		cout << "solved.\n";
		cout << bd.text2() << "\n";
		cout << bd.text3() << "\n";
	} else {
		cout << "can't solved.\n";
	}
#endif
#if	0
	if( bd.isUniqBT() )
		cout << "is proper quest.\n";
	else
		cout << "is NOT proper quest.\n";
#endif
	//
    std::cout << "\nOK\n";
}
