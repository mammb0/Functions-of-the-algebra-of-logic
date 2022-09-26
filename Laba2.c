#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

int CheckFile (FILE *fp){
	int i = 0;
	char cm;
	double flag;
	if(fp==NULL){
        printf("Ошибка открытия файла!\n");
		return(0);
    }
    while ((cm = fgetc(fp)) != EOF){
		if (cm == ' ' || cm == '0' || cm == '1'){
			if (cm == ' ') i++;
		} 
		else{
			printf("Некорректные значения в файле!\n");
			return(0);
		}
	}
	flag = log2 (i+1);
	if (((int)log2 (i+1) != flag) || (i<1)){
		printf("Некорректное количество значений функции в файле!\n");
		return(0);
	}
	printf("Количество аргументов функции - %d\n", (int)flag);
    fclose(fp);
    return(i+1);
}

void ReadFunction (FILE *fp, bool *m, int n, int flag){
	int i;
	if (flag == 2){
		printf("Введите значения функции:\n");
		fclose(fp);
		fp = stdin;
    }
	for (i=0;i<n;i++) fscanf(fp,"%d",&m[i]);
	if (fp != stdin) fclose(fp);
}

void WriteFunction (bool *m, int n){
	printf ("Заданная функция: F(");
	int a = (int)log2 (n), i;
	for (i=1;i<a;i++) printf ("X%d,", i);
	printf ("X%d) = ", i);
	for (i=0;i<n;i++) printf("%d ", m[i]);
}

void DecToBin (int n){
	if ( n >= 2 ) DecToBin( n/2 );
	printf("%d", n % 2);
}

int DecToBin_Beauty (int n, int i){
	if ( n >= 2 ) i = i + DecToBin_Beauty(n/2, i);
	i++;
	return (i);
}

int DecToBin_True (int n){
    int bin = 0, k = 1;
    while (n){
        bin += (n % 2) * k;
        k *= 10;
        n /= 2;
    }
    return bin;
}

/*
int GrauToBin(int n) 
{
    int bin;
    for (bin = 0; n; n >>= 1) {
      bin ^= n;
    }
    return bin;
}
*/

int GrayCode (int n){
	return n^(n >> 1);
}

void Delimiter (int c){
	int i;
	for (i=0;i<c;i++) printf ("—");
	printf ("\n");
}

void LineSeparator (int c){
	int i;
	for (i=0;i<c;i++) printf (" ");
}

int BinToDec (int n){
	int i=0, m=0, c;
	while (n > 0){
		c = n % 10;
		n = n/10;
		m = m + c*pow(2, i);
		i++;
	}
	return (m);
}

int Reverse (int n){
	int m=0;
	while (n > 0){
        m = m*10 + n%10;
        n = n/10;
	}
	return (m);
}

int Merger (int a, int b, int size_b){
	return (a*pow(10, size_b)+b);
}

int AddZero (int n, int size){
	return (n*pow(10, size));
}

int FunctionValue (int i, int j, int size_i, int size_j){
	int h, ki, kj, c;
	i = GrayCode (i);
	j = GrayCode (j);
	i = DecToBin_True (i);
	j = DecToBin_True (j);
	h = Merger (i, j, size_j);
	for (c=0;j>0;kj++) j = j/10;
	j = i;
	for (c=0;i>0;ki++) i = i/10;
	h = Reverse (h);
	if ((size_i>ki) && (j != 0)) h = AddZero (h, size_i-ki);
	if ((size_i>ki) && (j == 0)) h = AddZero (h, size_i+size_j-kj);
	h = BinToDec (h);
	return (h);
}

void TruthTable (bool *m, int n){
	int count, i, j, c, k;
	printf ("\n\nТаблица истинности:\n\n");
	count = (int)log2 (n);
	c = 1;
	for (i=0;i<count;i++){
		printf ("X%d|", i+1);
		for (j=0;j<n/(c*2);j++){
			for (k=0;k<c;k++) printf ("0|");
			for (k=0;k<c;k++) printf ("1|");	
		}
		c = c*2;
		printf ("\n");
		Delimiter (n*2+3);
	}
	printf (" F|");
	for (i=0;i<n;i++){
		printf ("%d|", m[i]);
	}
	printf ("\n");
	Delimiter (n*2+3);
}

void KarnoMap (bool *m, int n){
	int a, count, i, j, c, k, p, h;
	printf ("\n\nКарта Карно:\n\n");
	count = (int)log2 (n);
	a = count / 2;
	// Вывод 1 строки + разделитель
	for (i=1;i<a+1;i++) printf ("X%d", i);
	printf ("/");
	for (;i<count+1;i++) printf ("X%d", i);
	printf ("|");
	for (i=0;i<pow(2,(count-a));i++){
		j = 0;
		j = DecToBin_Beauty(i, j);
		for (c=0;c<count-a-j;c++) printf ("0");
		c = GrayCode(i);
		DecToBin(c);
		printf ("|");
	}
	printf ("\n");
	c = count*2+2+pow(2,(count-a))*(count-a+1);
	Delimiter (c);
	// Вывод 2+ строка + разделитель
	j = count*2+1-a;
	for (i=0;i<pow(2,a);i++){
		k = 0;
		k = DecToBin_Beauty(i, k);
		LineSeparator (j);
		for (p=0;p<a-k;p++) printf ("0");
		DecToBin(GrayCode(i));
		printf ("|");
		for (p=0;p<pow(2,(count-a));p++){
			LineSeparator (count-a-1);
			h = FunctionValue (i, p, a, count-a);
			printf ("%d|", m[h]);
		}
		printf ("\n");
		Delimiter (c);
	}
}

void SdnfSknf (bool *m, int n, int k){
	int count, i, j, c;
	char a, b;
	if (k){
		a = '&', b = 'v';
	}
	else{
		a = 'v', b = '&';
	}
	printf ("\n\n      ");
	count = (int)log2 (n);
	c = 0;
	for (i=0;i<n;i++){
		if (m[i] == k){
			if (c) printf ("  ");
			else{
				printf (" ");
				c = 1;
			} 
			for (j=0;j<count;j++){
				if (j+1 != count ){
					if ((i & (1 << j))>>j != k) printf ("_  ");
					else printf ("   ");
				}
				else{
					if ((i & (1 << j))>>j != k) printf ("_ ");
					else printf ("  ");
				}
			}
			printf (" ");
		}
	}
	if (k) printf ("\nСДНФ: ");
	else printf ("\nСКНФ: ");
	c = 0;
	for (i=0;i<n;i++){
		if (m[i] == k){
			if (c) printf ("%c(", b);
			else{
				printf ("(");
				c = 1;
			} 
			for (j=0;j<count;j++){
				if (j+1 != count ) printf ("X%d%c", j+1, a);
				else printf ("X%d", j+1);
			}
			printf (")");
		}
	}
}

int main(){
	system ("color F0");
	setlocale(LC_ALL,"RUS");
	int n, flag, i, j;
	char *fname=(char *)malloc(256); 
do{
	strcpy(fname,"default.txt");
	printf("Вводить из файла или из клавиатуры? 1/2 - ");
    scanf("%d", &i);
	if (i == 1){
		printf("Ввести имя файла или использовать файл по умолчанию? 1/2 - ");
		scanf("%d", &j);
		if (j == 1){
			printf("Введите имя файла: \n");
    		scanf("%s", fname);
			printf("Содержимое файла %s:\n", fname);
    	}
    	else printf("Содержимое файла default.txt:\n");
		FILE *fp=fopen(fname,"r");
		n = CheckFile(fp);
    	if (n == 0) {
			printf("Ошибка проверки файла!\n");
			goto aa;
		}
	}
    else{
		nn:
    	printf("Введите количество аргументов функции: ");
    	scanf("%d", &n); 
    	if (n<=0){
       		printf("Количество аргументов должно быть не менее одного!\n");
       		goto nn;
    	}
    	n = pow(2, n);
	}		
	bool *m;
	m = malloc (n*sizeof(bool));
	if (m==NULL){
		printf("Память не выделена");
		goto aa;
	}
	FILE *fp=fopen(fname,"r");
	ReadFunction (fp, m, n, i);
	WriteFunction (m, n);
	TruthTable (m, n);
	if (n>2) KarnoMap (m, n);
	SdnfSknf (m, n, 1);
	SdnfSknf (m, n, 0);
	aa:
    free(m);
	printf("\n\nЗакончить программу? 1 - нет / 2 - да : ");
	scanf("%d",&flag);
}while (flag == 1); 
	system("PAUSE");
	getchar(); 
	return 0;
}

