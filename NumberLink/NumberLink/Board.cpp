#include <iostream>
#include <random>
#include <ctype.h>
#include <assert.h>
#include "Board.h"

using namespace std;

typedef const char cchar;

random_device g_rnd;
mt19937 g_mt(g_rnd());

Board::Board(int h, int v)
{
	setSize(h, v);
}
void Board::setSize(int h, int v)
{
	m_type = TYPE_SAME;
	m_value = 0;
	if( !v ) v = h;
	m_nHorz = h;
	m_nVert = v;
	m_link.resize(h*v);
	m_number.resize(h*v);
	for(auto &x : m_number) x = 0;
	for(auto &x : m_link) x = 0;
}
void Board::setNumber(cchar *ptr)
{
	for(auto &x : m_number) {
		char ch = *ptr++;
		if( isdigit(ch) ) {
			x = ch - '0';
		} else if( ch >= 'A' && ch <= 'Z' ) {
			x = ch - 'A' + 10;
		} else if( ch >= 'a' && ch <= 'z' ) {
			x = ch - 'a' + 10;
		} else
			x = 0;
	}
	for(auto &x : m_link) x = 0;
}
void Board::setAddNumber(int sum, cchar *ptr)
{
	m_type = TYPE_ADD;
	m_value = sum;
	setNumber(ptr);
}
std::string Board::text0() const
{
	string txt;
	int ix = 0;
	for (int y = 0; y < m_nVert; ++y) {
		txt += '"';
		for (int x = 0; x < m_nHorz; ++x, ++ix) {
			if( m_number[ix] )
				txt += '0' + m_number[ix];
			else
				txt += '.';
		}
		txt += '"';
		if( y == m_nVert - 1 )
			txt += ',';
		txt += ' ';
		//txt += '\n';
	}
	return txt;
}
std::string Board::text() const
{
	string txt;
	int ix = 0;
	for (int y = 0; y < m_nVert; ++y) {
		for (int x = 0; x < m_nHorz; ++x, ++ix) {
			if( m_number[ix] ) {
				if( m_number[ix] < 10 ) 
					txt += '0' + m_number[ix];
				else
					txt += 'A' + (m_number[ix] - 10);
			} else
				txt += '.';
			txt += ' ';
		}
		txt += '\n';
		if( y != m_nVert - 1 ) {
			txt += '\n';
		}
	}
	txt += '\n';
	return txt;
}
std::string Board::text2() const
{
	string txt;
	int ix = 0;
	for (int y = 0; y < m_nVert; ++y) {
		for (int x = 0; x < m_nHorz; ++x, ++ix) {
			if( m_number[ix] ) {
				if( m_number[ix] < 10 ) 
					txt += '0' + m_number[ix];
				else
					txt += 'A' + (m_number[ix] - 10);
			} else
				txt += '.';
			if( m_link[ix] & LINK_RIGHT )
				txt += '-';
			else
				txt += ' ';
		}
		txt += '\n';
		if( y != m_nVert - 1 ) {
			ix -= m_nHorz;
			for (int x = 0; x < m_nHorz; ++x, ++ix) {
				if (m_link[ix] & LINK_DOWN)
					txt += "| ";
				else
					txt += "  ";
			}
			txt += '\n';
		}
	}
	txt += '\n';
	return txt;
}
std::string Board::text3() const
{
	static cchar *dig[] = {"０", "１", "２", "３", "４", "５", "６", "７", "８", "９","Ａ","Ｂ","Ｃ","Ｄ","Ｅ","Ｆ",};
	string txt;
	txt += "┏━";
	for (int x = 1; x < m_nHorz; ++x) txt += "┳━";
	txt += "┓\n";
	int ix = 0;
	for (int y = 0; y < m_nVert; ++y) {
		for (int x = 0; x < m_nHorz; ++x, ++ix) {
			txt += "┃";
			if( m_number[ix] ) {
				txt += dig[m_number[ix]];
				//txt += '0' + m_number[ix];
			} else {
				switch( m_link[ix] ) {
				case LINK_UP|LINK_DOWN:
					txt += "｜";
					break;
				case LINK_LEFT|LINK_RIGHT:
					txt += "－";
					break;
				case LINK_UP|LINK_LEFT:
					txt += "┘";
					break;
				case LINK_UP|LINK_RIGHT:
					txt += "└";
					break;
				case LINK_DOWN|LINK_LEFT:
					txt += "┐";
					break;
				case LINK_DOWN|LINK_RIGHT:
					txt += "┌";
					break;
				default:
					txt += "  ";
					break;
				}
			}
		}
		txt += "┃\n";
		if( y != m_nVert - 1 ) {
			txt += "┣━";
			for (int x = 1; x < m_nHorz; ++x) txt += "╋━";
			txt += "┫\n";
		} else {
			txt += "┗━";
			for (int x = 1; x < m_nHorz; ++x) txt += "┻━";
			txt += "┛\n";
		}
	}
	return txt;
}
std::string Board::textLink2() const
{
	string txt;
	int ix = 0;
	for (int y = 0; y < m_nVert; ++y) {
		for (int x = 0; x < m_nHorz; ++x, ++ix) {
			if( m_number[ix] )
				txt += '0' + m_number[ix];
			else
				txt += '.';
			if( m_link2[ix] & LINK_RIGHT )
				txt += '-';
			else
				txt += ' ';
		}
		txt += '\n';
		if( y != m_nVert - 1 ) {
			ix -= m_nHorz;
			for (int x = 0; x < m_nHorz; ++x, ++ix) {
				if (m_link2[ix] & LINK_DOWN)
					txt += "| ";
				else
					txt += "  ";
			}
			txt += '\n';
		}
	}
	return txt;
}
bool Board::isUniqBT()							//	バックトラッキング探索で解がユニークかどうかを調べる
{
	m_nSolved = 0;
	m_work.resize(m_number.size());
	for(auto &x : m_link) x = 0;
	isUniqBT(0, 0, 0);
	return m_nSolved == 1;
}
void Board::isUniqBT(int ix, int x, int y)		//	ix の状態を決める
{
	//cout << "ix = " << ix << ", x = " << x << ", y = " << y << "\n";
	//cout << text2() << "\n";
	if( ix == m_link.size() ) {
		//cout << text2() << "\n";
		if( checkLink() )
			++m_nSolved;
		return;
	}
	bool btmEdge = y == m_nVert - 1;
	int ny = y;			//	次のセル位置
	int nx = x + 1;
	bool rtEdge = nx == m_nHorz;		//	右端のセルか？
	if( rtEdge ) {
		++ny;
		nx = 0;
	}
	bool up = y != 0 && (m_link[ix-m_nHorz] & LINK_DOWN) != 0;	//	上からリンクが来ている
	bool left = x != 0 && (m_link[ix-1] & LINK_RIGHT) != 0;		//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return;		//	上・左両方からの接続不可
		if( up ) {		//	上と接続
			m_link[ix] = LINK_UP;
			//cout << text2() << "\n";
			if( checkLink(ix) )
				isUniqBT(ix+1, nx, ny);
			 m_link[ix] = 0;
			return;
		}
		if( left ) {	//	左と接続
			m_link[ix] = LINK_LEFT;
			//cout << text2() << "\n";
			if (checkLink(ix))
				isUniqBT(ix+1, nx, ny);
			 m_link[ix] = 0;
			return;
		}
		//	上左に接続できない場合
		if( !rtEdge ) {
			m_link[ix] = LINK_RIGHT;	//	右に接続して探索
			isUniqBT(ix+1, nx, ny);
			if( m_nSolved >= 2 )		//	複数の解を見つけた場合
				return;
		}
		if( !btmEdge ) {
			m_link[ix] = LINK_DOWN;	//	下に接続して探索
			isUniqBT(ix+1, nx, ny);
			m_link[ix] = 0;
		}
		return;
	} else {	//	セルに数字がない場合
		if( up && left ) {	//	上・左両方から接続されている場合
			m_link[ix] = LINK_UP | LINK_LEFT;
			if( m_number[ix-1] != 0 ) {	//	左に数字がある場合
				//cout << text2() << "\n";
				if( !checkLink(ix-1) ) {
					m_link[ix] = 0;
					return;
				}
			} else {
				if( m_number[ix-m_nHorz] != 0 ) {	//	上に数字がある場合
					//cout << text2() << "\n";
					if( !checkLink(ix-m_nHorz) ) {
						m_link[ix] = 0;
						return;
					}
				} else {
					//cout << text2() << "\n";
					int up = findNumber(ix-m_nHorz, LINK_DOWN);		//	上方向数字
					if( up != 0 ) {
						int left = findNumber(ix-1, LINK_RIGHT);	//	左方向数字
						if (left != 0 && up != left) {
							//cout << "NG\n";
							m_link[ix] = 0;
							return;
						}
					}
				}
			}
			isUniqBT(ix+1, nx, ny);
			 m_link[ix] = 0;
			return;
		}
		if( up ) {		//	上からのみ接続されている場合
			if( !rtEdge ) {
				m_link[ix] = LINK_UP | LINK_RIGHT;		//	上と右に接続して探索
				isUniqBT(ix+1, nx, ny);
				if( m_nSolved >= 2 )
					return;
			}
			if( !btmEdge ) {
				m_link[ix] = LINK_UP | LINK_DOWN;		//	上と下に接続して探索
				isUniqBT(ix+1, nx, ny);
			}
			m_link[ix] = 0;
			return;
		}
		if( left ) {		//	左からのみ接続されている場合
			if( !rtEdge ) {
				m_link[ix] = LINK_LEFT | LINK_RIGHT;		//	左と右に接続して探索
				isUniqBT(ix+1, nx, ny);
				if( m_nSolved >= 2 )
					return;
				m_link[ix] = 0;
			}
			if( !btmEdge ) {
				m_link[ix] = LINK_LEFT | LINK_DOWN;		//	左と下に接続して探索
				isUniqBT(ix+1, nx, ny);
			}
			m_link[ix] = 0;
			return;
		}
		if( rtEdge || btmEdge ) return;		//	右端or下端であれば接続不可
		m_link[ix] = LINK_DOWN | LINK_RIGHT;		//	右と下に接続して探索
		isUniqBT(ix+1, nx, ny);
		m_link[ix] = 0;
		return;
	}
}
bool Board::doSolveBT()
{
	m_work.resize(m_number.size());
	for(auto &x : m_link) x = 0;
	return doSolveBT(0, 0, 0);
}
bool Board::doSolveBT(int ix, int x, int y)		//	ix の状態を決める
{
	//cout << "ix = " << ix << ", x = " << x << ", y = " << y << "\n";
	//cout << text2() << "\n";
	if( ix == m_link.size() ) {
		//	undone: 数字の入っていないセルに数字を入れてチェック？
		//cout << text2() << "\n";
		return checkLink();
	}
	bool btmEdge = y == m_nVert - 1;
	int ny = y;			//	次のセル位置
	int nx = x + 1;
	bool rtEdge = nx == m_nHorz;		//	右端のセルか？
	if( rtEdge ) {
		++ny;
		nx = 0;
	}
	bool up = y != 0 && (m_link[ix-m_nHorz] & LINK_DOWN) != 0;	//	上からリンクが来ている
	bool left = x != 0 && (m_link[ix-1] & LINK_RIGHT) != 0;		//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return false;		//	上・左両方からの接続不可
		if( up ) {		//	上と接続
			m_link[ix] = LINK_UP;
			//cout << text2() << "\n";
			if( !checkLink(ix) ) return false;
			auto rc = doSolveBT(ix+1, nx, ny);
			if( !rc ) m_link[ix] = 0;
			return rc;
		}
		if( left ) {	//	左と接続
			m_link[ix] = LINK_LEFT;
			//cout << text2() << "\n";
			if (!checkLink(ix)) {
				//cout << "NG\n";
				m_link[ix] = 0;
				return false;
			}
			//cout << "OK\n";
			auto rc = doSolveBT(ix+1, nx, ny);
			if( !rc ) m_link[ix] = 0;
			return rc;
		}
		//	上左に接続できない場合
		if( !rtEdge ) {
			m_link[ix] = LINK_RIGHT;	//	右に接続して探索
			if( doSolveBT(ix+1, nx, ny) ) {
				//m_link[ix] = 0;
				return true;	//	解を発見した場合
			}
		}
		if( !btmEdge ) {
			m_link[ix] = LINK_DOWN;	//	下に接続して探索
			auto rc = doSolveBT(ix+1, nx, ny);
			if( !rc ) m_link[ix] = 0;
			return rc;
		}
		return false;
	} else {	//	セルに数字がない場合
		if( up && left ) {	//	上・左両方から接続されている場合
			m_link[ix] = LINK_UP | LINK_LEFT;
			if( m_number[ix-1] != 0 ) {	//	左に数字がある場合
				//cout << text2() << "\n";
				if( !checkLink(ix-1) ) {
					m_link[ix] = 0;
					return false;
				}
			} else {
				if( m_number[ix-m_nHorz] != 0 ) {	//	上に数字がある場合
					//cout << text2() << "\n";
					if( !checkLink(ix-m_nHorz) ) {
						m_link[ix] = 0;
						return false;
					}
				} else {
					//cout << text2() << "\n";
					int up = findNumber(ix-m_nHorz, LINK_DOWN);		//	上方向数字
					if( up != 0 ) {
						int left = findNumber(ix-1, LINK_RIGHT);	//	左方向数字
						if (left != 0 && up != left) {
							//cout << "NG\n";
							m_link[ix] = 0;
							return false;
						}
					}
				}
			}
			auto rc = doSolveBT(ix+1, nx, ny);
			if( !rc ) m_link[ix] = 0;
			return rc;
		}
		if( up ) {		//	上からのみ接続されている場合
			if( !rtEdge ) {
				m_link[ix] = LINK_UP | LINK_RIGHT;		//	上と右に接続して探索
#if	0		//	右はまだ未定なのでチェックは必要ない？
				if( m_number[ix-m_nHorz] != 0 ) {	//	上に数字がある場合
					if( !checkLink(ix-m_nHorz) )
						return false;
				}
#endif
				if( doSolveBT(ix+1, nx, ny) ) {
					//m_link[ix] = 0;
					return true;
				}
			}
			if( !btmEdge ) {
				m_link[ix] = LINK_UP | LINK_DOWN;		//	上と下に接続して探索
				auto rc = doSolveBT(ix+1, nx, ny);
				if( !rc ) m_link[ix] = 0;
				return rc;
			}
			m_link[ix] = 0;
			return false;
		}
		if( left ) {		//	左からのみ接続されている場合
			if( !rtEdge ) {
				m_link[ix] = LINK_LEFT | LINK_RIGHT;		//	左と右に接続して探索
#if	0		//	右はまだ未定なのでチェックは必要ない？
				if( m_number[ix-1] != 0 ) {	//	左に数字がある場合
					if( !checkLink(ix-1) )
						return false;
				}
#endif
				if( doSolveBT(ix+1, nx, ny) ) {
					//m_link[ix] = 0;
					return true;
				}
			}
			if( !btmEdge ) {
				m_link[ix] = LINK_LEFT | LINK_DOWN;		//	左と下に接続して探索
				auto rc = doSolveBT(ix+1, nx, ny);
				if( !rc ) m_link[ix] = 0;
				return rc;
			}
			m_link[ix] = 0;
			return false;
		}
		if( rtEdge || btmEdge ) return false;		//	右端or下端であれば接続不可
		m_link[ix] = LINK_DOWN | LINK_RIGHT;		//	右と下に接続して探索
		auto rc = doSolveBT(ix+1, nx, ny);
		if( !rc ) m_link[ix] = 0;
		return rc;
	}
}
int Board::findNumber(int ix, uchar dir)
{
	int ix0 = ix;
	do {
		if(!m_link[ix] ) return 0;	//	リンクが未完成の場合
		switch( m_link[ix] ^ dir ) {	//	元の方向フラグをオフにし、移動先方向のみを取り出す
		case LINK_UP:		dir = LINK_DOWN;	ix -= m_nHorz;	break;
		case LINK_DOWN:		dir = LINK_UP;		ix += m_nHorz;	break;
		case LINK_LEFT:		dir = LINK_RIGHT;	ix -= 1;	break;
		case LINK_RIGHT:	dir = LINK_LEFT;	ix += 1;	break;
		case 0: return 0;		//	リンクが未完成の場合
		}
		if( ix == ix0 ) return 0;	//	ループしている場合
	} while (m_number[ix] == 0);		//	数字セルに到達するまでループ
	return m_number[ix];
}
bool Board::checkLink(int ix)		//	ix からのリンクが正しいかどうかをチェック
{
	if( m_number[ix] == 0 ) return false;
	int x = ix % m_nHorz;
	int y = ix / m_nHorz;
	int ix0 = ix;
	auto num = m_number[ix];
	uchar dir = 0;
	do {
		if(!m_link[ix] ) return true;	//	リンクが未完成の場合
		switch( m_link[ix] ^ dir ) {	//	元の方向フラグをオフにし、移動先方向のみを取り出す
		case LINK_UP:		dir = LINK_DOWN;	ix -= m_nHorz;	break;
		case LINK_DOWN:		dir = LINK_UP;		ix += m_nHorz;	break;
		case LINK_LEFT:		dir = LINK_RIGHT;	ix -= 1;	break;
		case LINK_RIGHT:	dir = LINK_LEFT;	ix += 1;	break;
		case 0: return true;		//	リンクが未完成の場合
		default:
			checkLink(ix0);
			assert(0);
			return false;
		}
		if( ix == ix0 ) return false;	//	ループしている場合
	} while (m_number[ix] == 0);		//	数字セルに到達するまでループ
	return m_number[ix] == num;		//	同じ数字とリンクしているか？
}
bool Board::checkLink()		//	出来上がったリンクが正しいかどうかをチェック
{
	for(auto &x : m_work) x = 0;
	for(int ix = 0; ix != m_number.size(); ++ix) {
		if( m_number[ix] != 0 ) {
			int ix0 = ix;
			auto num = m_work[ix] = m_number[ix];
			uchar dir = 0;
			do {
				switch( m_link[ix] ^ dir ) {	//	元の方向フラグをオフにし、移動先方向のみを取り出す
				case LINK_UP:		dir = LINK_DOWN;	ix -= m_nHorz;	break;
				case LINK_DOWN:		dir = LINK_UP;		ix += m_nHorz;	break;
				case LINK_LEFT:		dir = LINK_RIGHT;	ix -= 1;	break;
				case LINK_RIGHT:	dir = LINK_LEFT;	ix += 1;	break;
				}
				if( ix == ix0 )
					return false;	//	ループしている場合
				m_work[ix] = num;
			} while (m_number[ix]==0);		//	数字セルに到達するまでループ
			if( m_number[ix] != num )		//	異なる数字とリンクしている場合はNG
				return false;
		}
	}
	//for(auto x : m_work ) {		//	すべてのセルの数字が決まっているかチェック	← 必要？
	//	if( x == 0 )
	//		return false;
	//}
	return true;	//	問題なし
} 
bool Board::doSolve()		//	ブルートフォースで解を探索。発見したら即リターン
{
	m_work = m_number;
	for(auto &x : m_link) x = 0;
	return doSolve(0, 0, 0);
}
//	undone: リンクがループしている場合
//	undone: リンク位置の数字が後で決まる場合
//	undone: 数字が自分自身とつながっている場合→失敗
bool Board::doSolve(int ix, int x, int y)		//	ix の状態を決める
{
	//cout << "ix = " << ix << ", x = " << x << ", y = " << y << "\n";
	//cout << text2() << "\n";
	if( ix == m_link.size() ) {
		//	undone: 数字の入っていないセルに数字を入れてチェック？
		return true;
	}
	bool btmEdge = y == m_nVert - 1;
	int ny = y;			//	次のセル位置
	int nx = x + 1;
	bool rtEdge = nx == m_nHorz;		//	右端のセルか？
	if( rtEdge ) {
		++ny;
		nx = 0;
	}
	bool up = y != 0 && (m_link[ix-m_nHorz] & LINK_DOWN) != 0;	//	上からリンクが来ている
	bool left = x != 0 && (m_link[ix-1] & LINK_RIGHT) != 0;		//	上からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return false;		//	接続不可
		if( up ) {
			if( m_work[ix-m_nHorz] != 0 && m_work[ix-m_nHorz] != m_work[ix] )
				return false;	//	数字が不一致
			m_link[ix] = LINK_UP;
			return doSolve(ix+1, nx, ny);
		}
		if( left ) {
			if( m_work[ix-1] != 0 && m_work[ix-1] != m_work[ix] )
				return false;		//	数字が不一致
			m_link[ix] = LINK_LEFT;
			return doSolve(ix+1, nx, ny);
		}
		if( !rtEdge && (m_work[ix+1] == 0 || m_work[ix+1] == m_work[ix]) ) {
			m_link[ix] = LINK_RIGHT;
			if( doSolve(ix+1, nx, ny) ) return true;
		}
		if( !btmEdge && (m_work[ix+m_nHorz] == 0 || m_work[ix+m_nHorz] == m_work[ix]) ) {
			m_link[ix] = LINK_DOWN;
			return doSolve(ix+1, nx, ny);
		}
		return false;
	} else {
		if( up && left ) {
			if( m_work[ix-m_nHorz] != 0 && m_work[ix-1] != 0 && 
				m_work[ix-m_nHorz] != m_work[ix-1] )
			{
				return false;		//	数字が不一致
			}
			if( m_work[ix-1] == 0 )
				m_work[ix] = m_work[ix-1] = m_work[ix-m_nHorz];
			else {
				m_work[ix] = m_work[ix-1];
				if( m_work[ix-m_nHorz] == 0 && m_work[ix-1] != 0 )
					m_work[ix-m_nHorz] = m_work[ix-1];
			}
			m_link[ix] = LINK_UP | LINK_LEFT;
			if( doSolve(ix+1, nx, ny) ) return true;
			m_work[ix] = 0;
			return false;
		}
		if( up ) {
			m_work[ix] = m_work[ix-m_nHorz];
			if( !rtEdge && (m_work[ix] == 0 || m_work[ix+1] == 0 || m_work[ix+1] == m_work[ix]) ) {
				m_link[ix] = LINK_UP | LINK_RIGHT;
				if( doSolve(ix+1, nx, ny) ) return true;
			}
			if( !btmEdge && (m_work[ix] == 0 || m_work[ix+m_nHorz] == 0 || m_work[ix+m_nHorz] == m_work[ix]) ) {
				m_link[ix] = LINK_UP | LINK_DOWN;
				if( doSolve(ix+1, nx, ny) ) return true;
			}
			m_work[ix] = 0;
			return false;
		}
		if( left ) {
			m_work[ix] = m_work[ix-1];
			if( !rtEdge && (m_work[ix] == 0 || m_work[ix+1] == 0 || m_work[ix+1] == m_work[ix]) ) {
				m_link[ix] = LINK_LEFT | LINK_RIGHT;
				if( doSolve(ix+1, nx, ny) ) return true;
			}
			if( !btmEdge && (m_work[ix] == 0 || m_work[ix+m_nHorz] == 0 || m_work[ix+m_nHorz] == m_work[ix]) ) {
				m_link[ix] = LINK_LEFT | LINK_DOWN;
				if( doSolve(ix+1, nx, ny) ) return true;
			}
			m_work[ix] = 0;
			return false;
		}
		if( rtEdge || btmEdge ) return false;
		if( m_work[ix+1] != 0 ) {
			if( m_work[ix+m_nHorz] != 0 ) {
				if( m_work[ix+1] != m_work[ix+m_nHorz] ) return false;
			}
			m_work[ix] = m_work[ix+1];
		} else {
			if( m_work[ix+m_nHorz] != 0 )
				m_work[ix] = m_work[ix+m_nHorz];
		}
		m_link[ix] = LINK_DOWN | LINK_RIGHT;
		if( doSolve(ix+1, nx, ny) ) return true;
		m_work[ix] = 0;
		return false;
	}
}
bool Board::isUniq()							//	解がユニーク？
{
	m_nSolved = 0;
	m_work.resize(m_number.size());
	for(auto &x : m_link) x = 0;
	isUniq(0, 0, 0);
	return m_nSolved == 1;
}
void Board::isUniq(int ix, int x, int y)		//	ix の状態を決める
{
	//cout << "ix = " << ix << ", x = " << x << ", y = " << y << "\n";
	//cout << text() << "\n";
	if( ix == m_link.size() ) {
		//	undone: 数字の入っていないセルに数字を入れてチェック？
		if( checkLink() ) {
			m_link2 = m_link;
			++m_nSolved;
		}
		return;
	}
	bool btmEdge = y == m_nVert - 1;
	int ny = y;			//	次のセル位置
	int nx = x + 1;
	bool rtEdge = nx == m_nHorz;		//	右端のセルか？
	if( rtEdge ) {
		++ny;
		nx = 0;
	}
	bool up = y != 0 && (m_link[ix-m_nHorz] & LINK_DOWN) != 0;	//	上からリンクが来ている
	bool left = x != 0 && (m_link[ix-1] & LINK_RIGHT) != 0;		//	上からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return;		//	接続不可
		if( up ) {
			m_link[ix] = LINK_UP;
			isUniq(ix+1, nx, ny);
			return;
		}
		if( left ) {
			m_link[ix] = LINK_LEFT;
			isUniq(ix+1, nx, ny);
			return;
		}
		if( !rtEdge ) {
			m_link[ix] = LINK_RIGHT;
			isUniq(ix+1, nx, ny);
			if( m_nSolved >= 2 ) return;
		}
		if( !btmEdge ) {
			m_link[ix] = LINK_DOWN;
			isUniq(ix+1, nx, ny);
		}
	} else {
		if( up && left ) {
			m_link[ix] = LINK_UP | LINK_LEFT;
			isUniq(ix+1, nx, ny);
			return;
		}
		if( up ) {
			if( !rtEdge ) {
				m_link[ix] = LINK_UP | LINK_RIGHT;
				isUniq(ix+1, nx, ny);
				if( m_nSolved >= 2 ) return;
			}
			if( !btmEdge ) {
				m_link[ix] = LINK_UP | LINK_DOWN;
				isUniq(ix+1, nx, ny);
			}
			return;
		}
		if( left ) {
			if( !rtEdge ) {
				m_link[ix] = LINK_LEFT | LINK_RIGHT;
				isUniq(ix+1, nx, ny);
				if( m_nSolved >= 2 ) return;
			}
			if( !btmEdge ) {
				m_link[ix] = LINK_LEFT | LINK_DOWN;
				isUniq(ix+1, nx, ny);
			}
			return;
		}
		if( rtEdge || btmEdge ) return;
		m_link[ix] = LINK_DOWN | LINK_RIGHT;
		isUniq(ix+1, nx, ny);
	}
}
#if	0
bool Board::isUniq()							//	解がユニーク？
{
	m_nSolved = 0;
	m_work = m_number;
	for(auto &x : m_link) x = 0;
	isUniq(0, 0, 0);
	return m_nSolved == 1;
}
void Board::isUniq(int ix, int x, int y)		//	ix の状態を決める
{
	//cout << "ix = " << ix << ", x = " << x << ", y = " << y << "\n";
	//cout << text() << "\n";
	if( ix == m_link.size() ) {
		//	undone: 数字の入っていないセルに数字を入れてチェック？
		++m_nSolved;
		return;
	}
	bool btmEdge = y == m_nVert - 1;
	int ny = y;			//	次のセル位置
	int nx = x + 1;
	bool rtEdge = nx == m_nHorz;		//	右端のセルか？
	if( rtEdge ) {
		++ny;
		nx = 0;
	}
	bool up = y != 0 && (m_link[ix-m_nHorz] & LINK_DOWN) != 0;	//	上からリンクが来ている
	bool left = x != 0 && (m_link[ix-1] & LINK_RIGHT) != 0;		//	上からリンクが来ている
	if( m_work[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return;		//	接続不可
		if( up ) {
			if( m_work[ix-m_nHorz] != 0 && m_work[ix-m_nHorz] != m_work[ix] )
				return;	//	数字が不一致
			m_link[ix] = LINK_UP;
			isUniq(ix+1, nx, ny);
			return;
		}
		if( left ) {
			if( m_work[ix-1] != 0 && m_work[ix-1] != m_work[ix] )
				return;		//	数字が不一致
			m_link[ix] = LINK_LEFT;
			isUniq(ix+1, nx, ny);
			return;
		}
		if( !rtEdge && (m_work[ix+1] == 0 || m_work[ix+1] == m_work[ix]) ) {
			m_link[ix] = LINK_RIGHT;
			isUniq(ix+1, nx, ny);
			if( m_nSolved >= 2 ) return;
		}
		if( !btmEdge && (m_work[ix+m_nHorz] == 0 || m_work[ix+m_nHorz] == m_work[ix]) ) {
			m_link[ix] = LINK_DOWN;
			isUniq(ix+1, nx, ny);
		}
	} else {
		if( up && left ) {
			if( m_work[ix-m_nHorz] != 0 && m_work[ix-1] != 0 && 
				m_work[ix-m_nHorz] != m_work[ix-1] )
			{
				return;		//	数字が不一致
			}
			if( m_work[ix-1] == 0 )
				m_work[ix] = m_work[ix-1] = m_work[ix-m_nHorz];
			else {
				m_work[ix] = m_work[ix-1];
				if( m_work[ix-m_nHorz] == 0 && m_work[ix-1] != 0 )
					m_work[ix-m_nHorz] = m_work[ix-1];
			}
			m_link[ix] = LINK_UP | LINK_LEFT;
			isUniq(ix+1, nx, ny);
			m_work[ix] = 0;
			return;
		}
		if( up ) {
			m_work[ix] = m_work[ix-m_nHorz];
			if( !rtEdge && (m_work[ix] == 0 || m_work[ix+1] == 0 || m_work[ix+1] == m_work[ix]) ) {
				m_link[ix] = LINK_UP | LINK_RIGHT;
				isUniq(ix+1, nx, ny);
				if( m_nSolved >= 2 ) {
					m_work[ix] = 0;
					return;
				}
			}
			if( !btmEdge && (m_work[ix] == 0 || m_work[ix+m_nHorz] == 0 || m_work[ix+m_nHorz] == m_work[ix]) ) {
				m_link[ix] = LINK_UP | LINK_DOWN;
				isUniq(ix+1, nx, ny);
			}
			m_work[ix] = 0;
			return;
		}
		if( left ) {
			m_work[ix] = m_work[ix-1];
			if( !rtEdge && (m_work[ix] == 0 || m_work[ix+1] == 0 || m_work[ix+1] == m_work[ix]) ) {
				m_link[ix] = LINK_LEFT | LINK_RIGHT;
				isUniq(ix+1, nx, ny);
				if( m_nSolved >= 2 ) {
					m_work[ix] = 0;
					return;
				}
			}
			if( !btmEdge && (m_work[ix] == 0 || m_work[ix+m_nHorz] == 0 || m_work[ix+m_nHorz] == m_work[ix]) ) {
				m_link[ix] = LINK_LEFT | LINK_DOWN;
				isUniq(ix+1, nx, ny);
			}
			m_work[ix] = 0;
			return;
		}
		if( rtEdge || btmEdge ) return;
		if( m_work[ix+1] != 0 ) {
			if( m_work[ix+m_nHorz] != 0 ) {
				if( m_work[ix+1] != m_work[ix+m_nHorz] ) return;
			}
			m_work[ix] = m_work[ix+1];
		} else {
			if( m_work[ix+m_nHorz] != 0 )
				m_work[ix] = m_work[ix+m_nHorz];
		}
		m_link[ix] = LINK_DOWN | LINK_RIGHT;
		isUniq(ix+1, nx, ny);
		m_work[ix] = 0;
	}
}
#endif
bool Board::isConsecutive() const		//	問題に数字が連続して並んでいるか？
{
	int ix = 0;
	for (int y = 0; y < m_nVert; ++y) {
		for (int x = 0; x < m_nHorz; ++x, ++ix) {
			if( m_number[ix] != 0 ) {
				if( y != 0 && m_number[ix] == m_number[ix-m_nHorz] )
					return true;
				if( x != 0 && m_number[ix] == m_number[ix-1] )
					return true;
			}
		}
	}
	return false;
}
bool Board::doesIncludeMeaninglessLink() const			//	コの字型のパスを含むか？
{
	for (int y = 0; y < m_nVert-1; ++y) {
		int ix = y * m_nHorz;
		for (int x = 0; x < m_nHorz-1; ++x, ++ix) {
			//if( m_number[ix] == 0 && m_number[ix+1] == 0 && 
			//	m_number[ix+m_nHorz] == 0 && m_number[ix+m_nHorz+1] == 0 )
			//{
				if( (m_link2[ix] & LINK_RIGHT) != 0 &&
					(m_link2[ix+1] & LINK_DOWN) != 0 && 
					(m_link2[ix+m_nHorz+1] & LINK_LEFT) != 0 )
				{
					return true;
				}
				if( (m_link2[ix+1] & LINK_DOWN) != 0 && 
					(m_link2[ix+m_nHorz+1] & LINK_LEFT) != 0 &&
					(m_link2[ix+m_nHorz] & LINK_UP) != 0 )
				{
					return true;
				}
				if( (m_link2[ix+m_nHorz+1] & LINK_LEFT) != 0 &&
					(m_link2[ix+m_nHorz] & LINK_UP) != 0 &&
					(m_link2[ix] & LINK_RIGHT) != 0 )
				{
					return true;
				}
				if( (m_link2[ix+m_nHorz] & LINK_UP) != 0 &&
					(m_link2[ix] & LINK_RIGHT) != 0 &&
					(m_link2[ix+1] & LINK_DOWN) != 0 )
				{
					return true;
				}
			//}
		}
	}
	return false;
}
void Board::setRandom(int N)
{
	for(;;) {
		for(auto &x : m_number) x = 0;
		for (int i = 0; i < N; ++i) {
			int ix;
			while( m_number[ix = g_mt() % size()] != 0 ) {}
			m_number[ix] = i + 1;
			while( m_number[ix = g_mt() % size()] != 0 ) {}
			m_number[ix] = i + 1;
		}
		if (!isConsecutive()) return;
	}
}
void Board::genRandom(int N)		//	[1, N] 数字*2 をランダムに配置
{
	for (;;) {
		setRandom(N);
		if( isUniq() && !doesIncludeMeaninglessLink() )
			return;
	}
}
void Board::genLinked(int N)		//	リンクされたN種数字を生成
{
	vector<int> pos;	//	リンク伸張可能な端点リスト
	vector<int> t;		//	伸張可能方向リスト
	for(auto &x : m_number) x = 0;
	//	N個の端点リストを pos に格納
	for (int i = 0; i < N; ++i) {
		int ix, x, y;
		for(;;) {
			while( m_number[ix = g_mt() % size()] != 0 ) {}
			pos.push_back(ix);
			x = ix % m_nHorz;
			y = ix / m_nHorz;
			t.clear();
			if( y != 0 && m_number[ix - m_nHorz] == 0 ) t.push_back(LINK_UP);
			if( x != 0 && m_number[ix - 1] == 0 ) t.push_back(LINK_LEFT);
			if( x != m_nHorz - 1 && m_number[ix + 1] == 0 ) t.push_back(LINK_RIGHT);
			if( y != m_nVert - 1 && m_number[ix + m_nHorz] == 0 ) t.push_back(LINK_DOWN);
			if( !t.empty() ) break;
			m_number[ix] = 0;		//	やり直し
		}
		switch( t[g_mt() % t.size()] ) {
		case LINK_UP:
			break;
		}
	}
}
