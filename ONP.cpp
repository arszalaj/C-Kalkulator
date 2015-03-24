#include "StdAfx.h"
#include "ONP.h"


CONP::CONP(void)
{
	string s1 = "0x2.0x5 0x5 + ";
	string s2 = convert(s1,hex,dec);
	long double wynik;
	evaluateONP(s2,wynik);
	char buf[512];
	sprintf(buf,"%.15Lf",wynik);
	string wyn = buf;
	wyn = convert(wyn,CONP::dec,CONP::hex);
	
}

CONP::~CONP(void)
{
}

/**
* Zwraca priotytet operatora oraz jego �aczno��.
* @param[in] op operator
* @param[out] assoc ��czno��
* @return priorytet operatora (im wy�szy, tym wy�sza kolejno�� wykonywania)
*/
int CONP::getPrior(std::string op, int &assoc)
{
	assoc = LEFT_ASSOC;
	if(!op.compare("*") || !op.compare("/"))
		return 3;
	if(!op.compare("+") || !op.compare("-"))
		return 2;
	if(!op.compare("^"))
		return 4;

	return 0;
}
/**
* Pobiera kolejny element wyra�enia.
* Elementem mo�e by� zar�wno liczba, funkcja czy operator.
* @param[in] exp wyra�enie
* @param[out] buffer bufor
* @param position Pozycja aktualnie przetwarzanego znaku w wyra�eniu.
* @param[in] ONP 0 - je�li wyra�enie w postaci infiksowej, 1 - je�li wyra�enie w ONP
* @return typ rozpoznanego elementu (FUNCTION,OPERATOR,NUMBER)
*/
int CONP::getNextElement(string exp,string &buffer, int &position,bool ONP)
{
	buffer.clear();

	unsigned int i=position,j=0; // zmienne pomocnicze

	// pomijanie bia�ych znak�w
	while(i < exp.length() && (exp[i] == ' ' || exp[i] == '\t'))
		i++;

	// sprawdzamy czy znak jest operatorem
	if(!isdigit(exp[i]) && exp[i] != '.') // jezeli jest sprawdzamy czy nie jest to operator jednoargumentowy + -
		// nie jest to pierwszy znak wyrazenia oraz czy nie jest to operator jednoargumentowy np -6
		if((i == 0 && (exp[i] == '-' || exp[i] == '+')) || (i > 0 && (exp[i] == '-' || exp[i] == '+') && ((exp[i-1] == ' ' || exp[i-1] == '(') && (i+1) < exp.length() && isdigit(exp[i+1]) )  ))
			i++; // je�l ipowyzszy warunek jest spelniony to musimy potraktowa� ci�g jako liczb� z operatorem jednoarguemtnowym np -11 , wi�c i++
		else // a jezeli nie jest jednoargumentowy to jest dwuargumentowy
	{ 
		position = i+1; // zwieksz pozycje 
		buffer = exp[i]; // zapisz operator do bufora
		buffer+=' '; // dodaj odstep
		if(exp[i] == '(') // jezeli operator jest lewym nawiasem zwroc
			return LB; // zwroc LN
		else if(exp[i] == ')') // a jezeli prawym to
			return RB; // zwroc LN
		else // a jezeli nie jest nawiasem
		return OPERATOR; // zwracamy sygnal ze pobralismy OPERATOR zwykly
	}

	// blok dla przypadku napotkania liczby
	if(i < exp.length() && isdigit(exp[i]))
	{
	{
		// sprawdzamy czy przed znakiem nie stoi operator jednoargumentowy 
		if((i == 1 && (exp[0] == '-' || exp[0] == '+')) || ( i > 1 && (exp[i-1] == '-' || exp[i-1] == '+')  && (exp[i-2] == ' ' || exp[i-2] == '(')))
		{
			if((i == 1 && exp[0] == '-') || exp[i-1] == '-') // jezeli jesto to minus
				buffer += '-'; // to zapisujemy go w buforze bo mamy do czyneinia z ujemna liczba
			else ; // jezeli jest to plus to nie musimy nic robi�
		}

		while(isdigit(exp[i])) // dop�ki kolejne znaki s� cyframi
			buffer+= exp[i++]; // zapisujemy je do bufora
	}
	
	if(i < exp.length() && exp[i] == '.') // jezeli napotkamy kropke
		buffer+=exp[i++];// zapisujemy ja do bufora 
	while(i < exp.length() && isdigit(exp[i])) // jezeli sa cyfry po kropce to przepisujemy je
		buffer+=exp[i++]; // do bufora

	position = i; // uaktualniamy pozycje 
	buffer+=' '; // dodajemy odstep do bufora dla czytelnosci
	return NUMBER; // zwracamy ze pobralismy liczbe
	}
	position = i;
	return EOS; // jezeli zaden z powyzszych warunkow nie zosta� spe�niony to odczytano
	           // ostatni znak, ktory jest bialym znakiem
}
/**
* Przekszta�ca wyra�enie w notacji infiksowej do wyra�enia w notacji ONP.
* @param[in] infix wyra�enie w notacji infiksowej
* @param[out] onp obiekt klasy string , w kt�rym ma by� zapisane przekszta�cone do ONP wyra�enie.
* @return zwraca 0, je�eli przekszta�cenie sie powiedzie, -1 - je�li si� nie powiedzie
*/
int CONP::infixToONP(string infix,string &onp)
{
	CStack stack;
	int result;
	int position = 0;
	string buffer,buffer2;
	int assoc,assoc2;

	// dopoki jestesmy w obrebie wyrazenia
	while(position < infix.length())
	{
		// w zaleznosci od tego co pobrala funkcja getNextElement
		result = getNextElement(infix,buffer,position,0);

		switch(result)
		{
			case NUMBER: // jezeli pobrala liczbe
				onp += buffer;//dajemy na wyj�cie
				break;
			
			case OPERATOR : // jezeli operator
			// dop�ki na stackie znajduje sie operator ktorego kolejnosc(getPrior) wykonywania
			// jest wieksza niz aktualnei pobranego
				while(stack.look(buffer2) && buffer2.compare("( ") && buffer2.compare(") ") &&
					(getPrior(buffer.substr(0,buffer.length()-1),assoc) <= getPrior(buffer2.substr(0,buffer2.length()-1),assoc2) 
					&& assoc == LEFT_ASSOC )
					|| getPrior(buffer.substr(0,buffer.length()-1),assoc) < getPrior(buffer2.substr(0,buffer2.length()-1),assoc2) 
					&& assoc == RIGHT_ASSOC )
				{
					stack.pop(buffer2); // zdejmij operator ze stosu
					onp+= buffer2; // i dodaj na onpjscie
				}
				stack.push(buffer); // dodaj aktualny operator na stos
				break;
			
			case LB:
				stack.push(buffer); // wstaw go na stos
			break;
		
			case RB :// jezeli prawy nawias to
				buffer2[0] = 0;
				while(buffer2[0] != '(') //az do napotkania lewego nawiasu
				{
					if(!stack.pop(buffer2)) // zdejmuj operatory ze stosu
						return ERROR; // jezeli nie napotkano lewego nawiasu a stos pusty tzn ze wyrazenie zle skonstruowane zwroc BLAD
					if(buffer2[0] != '(')
						onp+= buffer2; // doloz pobrany ze stosu operator na wyj�cie
				}
				break;
			case ERROR: // jezeli funkcja getNextElement napotkala blad to 
				return ERROR;  // zwroc BLAD
		}
	}

	while(stack.pop(buffer2))
		onp += buffer2;

	return 0;
}

/**
* Oblicza warto�� wyra�enia w ONP.
* @param[in] onp wyra�enie w ONP
* @param[out] result wynik wyra�enia
* @return zwraca -1, gdy obliczenie nie powiod�o si�, w przeciwnym razie 0.
*/
int CONP::evaluateONP(std::string onp,long double &result)
{

	if(onp.length() == 0) // jezeli nie ma nic w buforze wej�ciowym
	{
		result = 0; // to wynik rowny jest zero
		return 0;
	}

	int position = 0;
	long double a,b,c;
	string buffer,buffer2;
	CStack stack;
	char buf[256];
	int res;
	// dopoki jestesmy w obrebie wyrazenia
	while(position < onp.length())
	{
		res = getNextElement(onp,buffer,position,1);

		switch(res)
		{
			case NUMBER: // jezeli napotkamy liczbe
				stack.push(buffer); // dajemy ja na stos
				break;
			case OPERATOR: // jezeli napotkamy operator
				if(buffer[0] == '+') // i jezeli jest to plus
			{

				stack.pop(buffer2); // sciagamy liczbe ze stosu
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stosu
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = a+b; // wykonujemy dodawanie
				sprintf(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stos
			}
			if(buffer[0] == '-') // analogiczie dla pozostalych operatorow ...
			{

				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = b-a; // wykonujemy dodawanie
				sprintf(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			if(buffer[0] == '*')
			{

				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = a*b; // wykonujemy dodawanie
				sprintf(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			if(buffer[0] == '/')
			{

				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				if(!a)
					return ERROR;
				c = b/a; // wykonujemy dodawanie
				sprintf(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			if(buffer[0] == '^')
			{
				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = pow(b,a); // wykonujemy dodawanie
				sprintf(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			break;
		case ERROR: // jezeli funkcja PobierzOp napotkala ERROR 
			return ERROR; // zwracamy -1 ERROR
			break;
		}
	}
	
	stack.pop(buffer2); // sciagamy ostateczny wynik ze stosu
	result = atof(buffer2.c_str()); // i przypisujemy do zmiennej wynik
}

/**
* Zwraca �a�cuch binarny, odpowiadaj�cy dziesi�tnemu
* @param[in] int_string �a�cuch dziesi�tny wej�ciowy
* @return �a�cuch binarny
*/
string CONP::decToBin(std::string int_string) const
{
	long long int liczba = my_atoll(int_string.c_str());
	string ret;

	do
	{
		if(!(liczba % 2))
			ret = '0' + ret;
		else
		{
			ret = '1' + ret;
		}
		liczba /= 2;
	}while(liczba >=1);
	return ret;
}
/**
* Zwraca �a�cuch hexadecymalny, odpowiadaj�cy dziesi�tnemu
* @param[in] int_string �a�cuch dziesi�tny wej�ciowy
* @return �a�cuch hexadecymalny
*/
string CONP::decToHex(string int_string) const
{
	long long int liczba = my_atoll(int_string.c_str());
	const string hex = "0123456789ABCDEF";
	string ret;
	long long int reszta;
	do
	{
		reszta = liczba % 16;
		liczba /= 16;
		ret = hex[reszta] + ret;
	}while(liczba >= 1);
	ret = "0x" + ret;
	return ret;
}
/**
* Zwraca �a�cuch �semkowy, odpowiadaj�cy dziesi�tnemu
* @param[in] int_string �a�cuch dziesi�tny wej�ciowy
* @return �a�cuch �semkowy
*/
string CONP::decToOct(std::string int_string) const
{
	long long int liczba = my_atoll(int_string.c_str());
	const string oct = "01234567";
	string ret;
	int reszta;
	do
	{
		reszta = liczba % 8;
		liczba /= 8;
		ret = oct[reszta] + ret;
	}while(liczba >= 1);
	return ret;
}
/**
* Zwraca �a�cuch dziesi�tny, odpowiadaj�cy binarnemu
* @param[in] bin_string �a�cuch binarny wej�ciowy
* @return �a�cuch dziesi�tny
*/
string CONP::binToDec(std::string bin_string) const
{
	string ret;
	int i,p = 0;
	int suma = 0,bliczba = 0;

	for(i = bin_string.length()-1; i >=0 ; i--)
	{
		if(bin_string[i] == '0' || bin_string[i] == '1')
		{
			bliczba = bin_string[i] - '0';
			suma += bliczba*pow((double)2,(double)p);
			p++;
		}
	}

	ostringstream os(ostringstream::out);
	os << suma;
	return os.str();
}
/**
* Zwraca �a�cuch dziesi�tny, odpowiadaj�cy hexadecymalnemu
* @param[in] hex_string �a�cuch hexadecymalny wej�ciowy
* @return �a�cuch dziesi�tny
*/
string CONP::hexToDec(string hex_string) const
{
	if(hex_string.find("0x") != -1)
		hex_string = hex_string.substr(2,hex_string.length()-2);
	string ret;
	int i,p = 0;
	int suma = 0;
	int bliczba = 0;
	const string hex = "0123456789ABCDEF";
	for(i = hex_string.length()-1; i >=0 ; i--)
	{
		bliczba = hex.find(toupper(hex_string[i]));
		suma += bliczba*pow((double)16,(double)p);
		p++;
		
	}
	ostringstream os(ostringstream::out);
	os << suma;
	return os.str();
}
/**
* Zwraca �a�cuch dziesi�tny, odpowiadaj�cy �semkowemu
* @param[in] oct_string �a�cuch �semkowy wej�ciowy
* @return �a�cuch dziesi�tny
*/
string CONP::octToDec(string oct_string) const
{
	string ret;
	int i,p = 0;
	int suma = 0;
	int bliczba = 0;
	const string oct = "01234567";
	for(i = oct_string.length()-1; i >=0 ; i--)
	{	bliczba = oct.find(oct_string[i]);
		suma += bliczba*pow((double)8,(double)p);
		p++;
		
	}
	ostringstream os(ostringstream::out);
	os << suma;
	return os.str();
}
/**
* Konwertuje wyra�enie z danego systemu liczbowego na inny
* @param[in] from system liczbowy wyra�enia wej�ciowego
* @param[in] to po��dany system liczbowy wyra�enia
* @return wyra�enie w po��daym systemie liczbowym
*/
string CONP::convert(string exp, sys from, sys to) const
{
	if(from == to)
		return exp;

	string akt_liczba;
	string konw;
	string ret;
	bool liczba_hex = false;
	int i = 0, j = 0;
	for(i = 0; i < exp.length(); /*i++*/)
	{
		if(exp[i] >= '0' && exp[i] <= '9')
		{
			akt_liczba = "";
			akt_liczba += exp[i];
			j = 1;
			while((exp[i+j] >= '0' && exp[i+j] <= '9') || exp[i+j] == 'x' || ((exp[i+j] >= 'a' && exp[i+j] <= 'f') || (exp[i+j] >= 'A' && exp[i+j] <= 'F')))
			{
				if(j == 1 && exp[i+j] == 'x')
				{
					liczba_hex = true;
					akt_liczba += exp[i+j];
				}
				else if((exp[i+j] >= '0' && exp[i+j] <= '9') 
					|| (liczba_hex && ((exp[i+j] >= 'a' && exp[i+j] <= 'f') || (exp[i+j] >= 'A' && exp[i+j] <= 'F'))))
						akt_liczba += exp[i+j];
				j++;
			}
			i = i + j;
			liczba_hex = false;
			switch(from)
			{
			case sys::dec :
				switch(to)
				{
				case sys::bin :
					konw = decToBin(akt_liczba);
					break;
				case sys::oct :
					konw = decToOct(akt_liczba);
					break;
				case sys::hex :
					konw = decToHex(akt_liczba);
					break;
				}
				break;
			case sys::bin :
				switch(to)
				{
				case sys::dec :
					konw = binToDec(akt_liczba);
					break;
				case sys::oct :
					konw = binToDec(akt_liczba);
					konw = decToOct(konw);
					break;
				case sys::hex :
					konw = binToDec(akt_liczba);
					konw = decToHex(konw);
					break;
				}
				break;
			case sys::oct :
				switch(to)
				{
				case sys::dec :
					konw = octToDec(akt_liczba);
					break;
				case sys::bin :
					konw = octToDec(akt_liczba);
					konw = decToBin(konw);
					break;
				case sys::hex :
					konw = octToDec(akt_liczba);
					konw = decToHex(konw);
					break;
				}
				break;
			case sys::hex :
				switch(to)
				{
				case sys::dec :
					konw = hexToDec(akt_liczba);
					break;
				case sys::bin :
					konw = hexToDec(akt_liczba);
					konw = decToBin(konw);
					break;
				case sys::oct :
					konw = hexToDec(akt_liczba);
					konw = decToOct(konw);
					break;
				}
			break;
			}
			ret += konw;
		}
		else
		{
			ret += exp[i];
			i++;
		}
	}
	return ret;
}
/**
* Zwraca �a�cuch zawieraj�cy warto�� w stopniach, r�wnowa�ny do �a�cucha wej�ciowego
* zawieraj�cego warto�� w radianach
* @param[in] rad �a�cuch zawieraj�cy warto�� w radianach
* @return �a�cuch zawieraj�cy warto�� w stopniach
*/
string CONP::radToDeg(string rad) const
{
	double lrad = atof(rad.c_str());
	long double deg = lrad * 180 / ::pi;
	ostringstream os(ostringstream::out);
	os << deg;
	return os.str();
}
/**
* Zwraca �a�cuch zawieraj�cy warto�� w radianach, r�wnowa�ny do �a�cucha wej�ciowego
* zawieraj�cego warto�� w stopniach
* @param deg �a�cuch zawieraj�cy warto�� w stopniach
* @return �a�cuch zawieraj�cy warto�� w radianach
*/
string CONP::degToRad(string deg) const
{
	double ldeg = atof(deg.c_str());
	long double rad = ldeg * ::pi/180;
	ostringstream os(ostringstream::out);
	os << rad;
	return os.str();
}

/**
* Zamienia �a�cuch zawieraj�cy liczb� ca�kowit� na warto�� liczbow� typu long long 
* @param *instr wska�nik do tablicy z �a�cuchem liczbowym
* @return warto�� ca�kowita typu long long odpowiadaj�ca �a�cuchowi wej�ciowemu
*/
long long CONP::my_atoll(const char *instr) const
{
  long long retval;
  int i;

  retval = 0;
  for (; *instr; instr++) {
    retval = 10*retval + (*instr - '0');
  }
  return retval;
}

int CONP::silnia(int n) const
{
	if(n<0)
		return 0;
	if(n == 0 || n == 1)
		return 1;
	return silnia(n-1)*n;
}





	












