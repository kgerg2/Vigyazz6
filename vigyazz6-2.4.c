#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define PROBA 10000

struct preferencia{ /** 9: 1.kör 1.harmada, ... */
	int meret[9][10]; /** 1-10, 11-20, ... */
	int elh[9][6]; /** legkisebb végső lap előtt; utána, de a második előtt; ... */
	int sor[5]; /** szabad helyek száma az adott sorban */
};

struct jatekos{
	char nev[20];
	int eredm;
};

int hatv(int alap, int kitevo){
	int eredm=1;
	for(;kitevo>0;kitevo--){
		eredm *= alap;
	}
	//printf("eredm=%d\n", eredm);
	return eredm;
}

void masol(int cel[4][5], int forras[4][5]){
	int i, j;
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			cel[i][j] = forras[i][j];
		}
	}
}

void kiir(int tabla[4][5]){ /** Jelenlegi állás kiírása */
	int i, j;
	printf("\nT\240bla:\n");
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			if(tabla[i][j]!=0){
				printf("%4d", tabla[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void fajlkiir(int tabla[4][5], FILE *fa){ /** Jelenlegi állás kiírása fájlba */
	int i, j;
	fprintf(fa, "\nT:\n");
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			if(tabla[i][j]!=0){
				fprintf(fa, "%d\t", tabla[i][j]);
			}
		}
		fprintf(fa, "\n");
	}
}

int osszeg(int tabla[4][5], int sor){ /** Sorösszeg megállapítása */
	int i, osszeg=0;
	for(i=0;i<5;i++){
		if(tabla[sor][i]==0){
			// semmi
		} else if(tabla[sor][i]==55){
			osszeg += 7;
		} else if(tabla[sor][i]%11==0){
			osszeg += 5;
		} else if(tabla[sor][i]%10==0){
			osszeg += 3;
		} else if(tabla[sor][i]%5==0){
			osszeg += 2;
		} else{
			osszeg += 1;
		}
	}
	return osszeg;
}

/** hibás adatok manuális átírása */

void atir(int tabla[4][5], int *lapdb, int *kezdlapdb, int *jatdb, int *jatek_vege, int ossz_lap[104], int *letesz, int *korssz, int *min, int *jatrak, int *sorr, struct jatekos *jat, int *lap, int *eredm, struct jatekos *jat2){
	int temp, i, sor, ssz;
	printf("1. ");
	kiir(tabla);
	if(lapdb!=NULL){
		printf("2. lapdb (%d)\n", *lapdb);
	}
	if(kezdlapdb!=NULL){
		printf("3. kezdlapdb (%d)\n", *kezdlapdb);
	}
	if(jatdb!=NULL){
		printf("4. jatdb (%d)\n", *jatdb);
	}
	if(jatek_vege!=NULL){
		printf("5. jatek_vege (%d)\n", *jatek_vege);
	}
	if(ossz_lap!=NULL){
		printf("6. ossz_lap (");
		for(i=0;i<103;i++){
			if((i+1)%10==0 || i==0){
				printf("|%d| ", i+1);
			}
			printf("%d ", ossz_lap[i]);
		}
		printf("%d)\n", ossz_lap[103]);
	}
	if(letesz!=NULL){
		printf("7. letesz (%d)\n", *letesz);
	}
	if(korssz!=NULL){
		printf("8. korssz (%d)\n", *korssz);
	}
	if(min!=NULL){
		printf("9. min (%d)\n", *min);
	}
	if(jatrak!=NULL){
		printf("10. jatrak (");
		for(i=0;i<*jatdb-1;i++){
			printf("%d ", jatrak[i]);
		}
		printf("%d)\n", jatrak[*jatdb-1]);
	}
	if(sorr!=NULL){
		printf("11. sorr (");
		for(i=0;i<*jatdb-1;i++){
			printf("%d ", sorr[i]);
		}
		printf("%d)\n", sorr[*jatdb-1]);
	}
	if(jat!=NULL){
		printf("12. jat (");
		for(i=0;i<*jatdb-1;i++){
			printf("%s - %d; ", jat[i].nev, jat[i].eredm);
		}
		printf("%s - %d)\n", jat[*jatdb-1].nev, jat[*jatdb-1].eredm);
	}
	if(lap!=NULL){
		printf("13. lap (");
		for(i=0;i<*lapdb-1;i++){
			printf("%d ", lap[i]);
		}
		printf("%d)\n", lap[*lapdb-1]);
	}
	if(eredm!=NULL){
		printf("14. eredm (");
		for(i=0;i<*lapdb-1;i++){
			printf("%d ", eredm[i]);
		}
		printf("%d)\n", eredm[*jatdb-1]);
	}
	if(jat2!=NULL){
		printf("15. jat (%s - %d)\n", jat2->nev, jat2->eredm);
	}
	temp = -1;
	while(temp!=0){
		printf("\nAdd meg, melyik adatot akarod megv\240ltoztatni (0 - semmit): ");
		scanf("%d", &temp);
		switch(temp){
			case 1:
				printf("Melyik sor elem\202t cser\202led: ");
				scanf("%d", &sor);
				sor--;
				printf("H\240nyadik elemet cser\202led: ");
				scanf("%d", &ssz);
				ssz--;
				printf("Add meg az elem \243j \202rt\202k\202t: ");
				scanf("%d", &tabla[sor][ssz]);
			break;
			case 2:
				printf("lapdb = ");
				scanf("%d", lapdb);
			break;
			case 3:
				printf("kezdlapdb = ");
				scanf("%d", kezdlapdb);
			break;
			case 4:
				printf("jatdb = ");
				scanf("%d", jatdb);
			break;
			case 5:
				printf("jatek_vege = ");
				scanf("%d", jatek_vege);
			break;
			case 6:
				printf("ossz_lap = ");
				scanf("%d", ossz_lap);
			break;
			case 7:
				printf("letesz = ");
				scanf("%d", letesz);
			break;
			case 8:
				printf("korssz = ");
				scanf("%d", korssz);
			break;
			case 9:
				printf("min = ");
				scanf("%d", min);
			break;
			case 10:
				printf("H\240nyadik elemet cser\202led? ");
				scanf("%d", &ssz);
				ssz--;
				printf("Add meg az \243j \202rt\202ket: ");
				scanf("%d", &jatrak[ssz]);
			break;
			case 11:
				printf("H\240nyadik elemet cser\202led? ");
				scanf("%d", &ssz);
				ssz--;
				printf("Add meg az \243j \202rt\202ket: ");
				scanf("%d", &sorr[ssz]);
			break;
			case 12:
				printf("H\240nyadik elemet cser\202led? ");
				scanf("%d", &ssz);
				ssz--;
				printf("Add meg az \243j nevet: ");
				scanf("%[^\n]", jat[ssz].nev);
				printf("Add meg az \243j pontsz\240mot: ");
				scanf(" %d", &jat[ssz].eredm);
			break;
			case 13:
				printf("H\240nyadik elemet cser\202led? ");
				scanf("%d", &ssz);
				ssz--;
				printf("Add meg az \243j \202rt\202ket: ");
				scanf("%d", &lap[ssz]);
			break;
			case 14:
				printf("H\240nyadik elemet cser\202led? ");
				scanf("%d", &ssz);
				ssz--;
				printf("Add meg az \243j \202rt\202ket: ");
				scanf("%d", &eredm[ssz]);
			break;
		}
	}
}

int hozzarak(int uj, int tabla[4][5], struct jatekos jat){ /** Új lap lerakása */
	int i, j, tav[4], min;
	min = 0;
	for(i=0;i<4;i++){
		j = 4;
		while(j>=0 && tabla[i][j]==0){
			j--;
		}
		if(j==-1){
			printf("HIBA -- sor adat n\202lk\201l --\n");
		}
		tav[i] = uj-tabla[i][j];
		if(tav[i]<1){
			tav[i] = 104;
		}
		if(tav[i]<tav[min]){
			min = i;
		}
	}
	if(tav[min]==104){ /** cserélés */
		int csere=0;
		if(strcmp(jat.nev, "\220n")!=0){
			do{
				printf("Melyik sort cser\202li %s? (1-4) ", jat.nev);
				scanf("%d", &csere);
				if(csere==-111){atir(tabla, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &jat);}
			} while(csere==-111 || csere<1 || csere>4);
			csere--;
		} else{
			csere = 0;
			for(i=0;i<4;i++){
				if(osszeg(tabla, i)<osszeg(tabla, csere)){
					csere = i;
				}
			}
		}
		j = osszeg(tabla, csere);
		tabla[csere][0] = uj;
		for(i=1;i<5;i++){
			tabla[csere][i] = 0;
		}
		return j;
	}
	i = 4;
	while(tabla[min][i]==0 && i>=0){
		i--;
	}
	if(i==4){ /** elvisz */
		j = osszeg(tabla, min);
		tabla[min][0] = uj;
		for(i=1;i<5;i++){
			tabla[min][i] = 0;
		}
		return j;
	}
	tabla[min][i+1] = uj;
	return 0;
}

int mrandhely(int pref_ered[8], int ossz_lap[104], int jatdb){
	//printf("mrandhely kezd\n");
	int i, lapdb[8], osszlapdb, p;
	double pref[8], osszpref;
	for(i=0;i<10;i++){
		lapdb[i] = 0;
	}
	osszlapdb = 0;
	for(i=0;i<104;i++){
		if(ossz_lap[i]==-1){
			lapdb[i/13]++;
			osszlapdb++;
		}
	}
	osszpref = 0;
	for(i=0;i<8;i++){
		/*if(lapdb[i]<jatdb){
			lapdb[i] = 0;
		}*/
		//printf(" %d %d -", pref_ered[i], lapdb[i]);
		pref[i] = (double)pref_ered[i]*(double)lapdb[i];
		//printf("pref[%d] = %f\n", i, pref[i]);
		osszpref += pref[i];
	}
	//printf(" | %.0f\n", osszpref);
	p = rand();
	i = 0;
	while(p>0 && i<7){
		//printf("%d %.0f - ", p, pref[i]);
		p -= RAND_MAX*pref[i]/osszpref;
		//printf("p = %d\n", p);
		i++;
	}
	//printf("mrandhely vege, %d\n", i);
	return i;
}

/** ------------------------------------------------------------------------------------- */
/** -- KEZDÉS - main --------------------------------------------------- KEZDÉS - main -- */
/** ------------------------------------------------------------------------------------- */

int main(){
	int i, j, k, l, tabla[4][5], tabla2[4][5], lapdb, kezdlapdb, jatdb, temp, jatek_vege, ossz_lap[104], letesz, korssz, min, volthiba;
	char ch;
	 /** ossz_lap: -3 - kézben | -2 - táblán | -1 - pakliban/kérdéses | 0 - én elvittem | 1+ - adott ID-vel rendelkező játékos elvitte */
	struct preferencia pref;
	FILE *pr, *teszt, *el;
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			tabla[i][j] = 0;
		}
	}
	for(i=0;i<104;i++){
		ossz_lap[i] = -1;
	}
	teszt = fopen("ki2.txt", "w");
	
	/** --------------------------------------------------------------------------------- */
	/** -- ELŐKÉSZÜLETEK ----------------------------------------------- ELŐKÉSZÜLETEK -- */
	/** --------------------------------------------------------------------------------- */
	
	//printf("előkészületek\n");
	
	/** -- PREFERENCIÁK BEOLVASÁSA ------------------------------------- PREFERENCZÁK BEOLVASÁSA -- */
	
	pr = fopen("preferenciak.txt", "r");
	fscanf(pr, "%c", &ch);
	if(ch=='/'){
		ch = '*';
		while(ch!='\n'){
			fscanf(pr, "%c", &ch);
		}
	}
	for(i=0;i<9;i++){
		ch = '*';
		while(ch!='-'){
			fscanf(pr, "%c", &ch);
			//printf("%c ", ch);
		}
		for(j=0;j<8;j++){
			fscanf(pr, " %d", &pref.meret[i][j]);
			//printf("%d ", pref.meret[i][j]);
		}
		fscanf(pr, "\n");
	}
	for(i=0;i<9;i++){
		ch = '*';
		while(ch!='-'){
			fscanf(pr, "%c", &ch);
		}
		for(j=0;j<6;j++){
			fscanf(pr, " %d", &pref.elh[i][j]);
		}
		fscanf(pr, "\n");
	}
	ch = '*';
	while(ch!='-'){
		fscanf(pr, "%c", &ch);
	}
	for(i=0;i<5;i++){
		fscanf(pr, " %d", &pref.sor[i]);
	}
	fclose(pr);
	/*for(i=0;i<9;i++){
		printf("%d. ", i);
		for(j=0;j<8;j++){
			printf("%d ", pref.meret[i][j]);
		}
		printf("\n");
	}*/
	//printf("preferenciák beolvasva\n");
	
	/** -- JÁTÉK KEZDÉSE ----------------------------------------------- JÁTÉK KEZDÉSE */
	
	do{
		printf("Add meg a j\240t\202kosok sz\240m\240t (velem egy\201tt): ");
		scanf("%d", &jatdb);
		if(jatdb==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, NULL, NULL, NULL, NULL, NULL, NULL);}
	}while(jatdb==-111);
	int jatrak[jatdb], sorr[jatdb];
	struct jatekos jat[jatdb];
	for(i=0;i<jatdb;i++){
		jatrak[i] = 0;
		jat[i].eredm = 0;
	}
	strcpy(jat[0].nev, "\220n");
	printf("Add meg a j\240t\202kosok neveit:");
	for(i=1;i<jatdb;i++){
		scanf(" %[^\n]", jat[i].nev);
	}
	do{
		printf("Add meg a lapjaim darabsz\240m\240t (amennyiben enn\202l t\224bb is lesz, azt): ");
		scanf("%d", &lapdb);
		if(lapdb==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, NULL, NULL, NULL);}
	} while(lapdb==-111);
	kezdlapdb = lapdb;
	int lap[lapdb], eredm[lapdb];
	printf("Add meg a lapjaimat: ");
	for(i=0;i<lapdb;i++){
		do{
			scanf("%d", &lap[i]);
			system("cls");
			if(lap[i]==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
		} while(lap[i]==-111);
		ossz_lap[lap[i]-1] = -3;
	}
	printf("Add meg az indul\242lapokat: ");
	for(i=0;i<4;i++){
		do{
			scanf("%d", &tabla[i][0]);
			if(tabla[i][0]==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
		} while(tabla[i][0]==-111);
	}
	el = fopen("jatekmenet5.txt", "w");
	
	/** ------------------------------------------------------------------------- */
	/** -- JÁTÉK ------------------------------------------------------- JÁTÉK -- */
	/** ------------------------------------------------------------------------- */
	
	jatek_vege = 0;
	korssz = 1;
	while(jatek_vege==0){
		
		/** -- KÖR KEZDETE --------------------------------------------- KÖR KEZDETE -- */
		
		for(;lapdb>0;lapdb--){
			//printf("%d\n", lapdb);
			fprintf(el, "%d.\n", lapdb);
			for(i=0;i<kezdlapdb;i++){
				eredm[i] = 0;
			}
			
			/** -- Lerakandó lap kiválasztása -- */
			
			volthiba = 0;
			for(i=0;i<lapdb;i++){
				//printf("\t%d\n", i);
				/** Méret szerint */
				
				for(j=0;j<PROBA;j++){
					//printf("\t\t%d ", j);
					masol(tabla2, tabla);
					for(k=0;k<jatdb-1;k++){ /** mások által lerakott lapok (jatrak[]) */
						//printf("(%d-1)*3+(%d-%d)*3/%d=%d", korssz, kezdlapdb, lapdb, kezdlapdb, (korssz-1)*3+(kezdlapdb-lapdb)*3/kezdlapdb);
						temp = mrandhely(pref.meret[(korssz-1)*3+(kezdlapdb-lapdb)*3/kezdlapdb], ossz_lap, jatdb);
						l = 0;
						do{ /** a tempedik 13-as egységben véletlenszerű, még nem lerakott lap */
							jatrak[k] = rand()%13+13*temp;
							l++;
						}while(ossz_lap[jatrak[k]-1]!=-1 && l<100000);
						if(l==100000){
							if(volthiba==0){
								printf("HIBA - nem tal\240lhat\242 lap a megadott tartom\240nyban ");
							}
							volthiba++;
							k--;
						} else{
							ossz_lap[jatrak[k]-1] = -2;
						}
					}
					for(k=0;k<jatdb-1;k++){
						ossz_lap[jatrak[k]-1] = -1;
					}
					jatrak[jatdb-1] = lap[i]; /** saját lap */
					for(k=0;k<jatdb;k++){
						min = k; /** lapok sorbarakása */
						for(l=k;l<jatdb;l++){
							if(jatrak[l]<jatrak[min]){
								min = l;
							}
						}
						if(min!=k){ /** legkisebb előre */
							l = jatrak[min];
							jatrak[min] = jatrak[k];
							jatrak[k] = l;
						}
						if(jatrak[k]==lap[i]){/** általam elvitt lapok pontösszege */
							eredm[i] += hozzarak(jatrak[k], tabla2, jat[0]);
						} else{/** többiek által elvitt lp pozitív hatása ??? */
							eredm[i] -= hozzarak(jatrak[k], tabla2, jat[0])/5;
						}
						fprintf(teszt, "%d ", jatrak[k]);
					}
					fprintf(teszt, "\n %d - %d\n", lap[i], eredm[i]);
					fajlkiir(tabla2, teszt);
					fprintf(teszt, "%d. pr\242ba\n", j);
					//printf("kész\n");
				}
				/** Méret szerinti kiválasztás vége */
				
				fprintf(teszt, "Lap: %d - %d\n", lap[i], eredm[i]);
				//printf("1kész (i=%d)\n", i);
			}
			/** Lapok vérható értékei kiszámolva */
			
			letesz = 0;
			for(i=0;i<lapdb;i++){
				if(eredm[letesz]>eredm[i] || (eredm[letesz]==eredm[i] && rand()%hatv(2, i)==0) || (i==lapdb-1 && rand()%hatv(2, i)==0)){
					letesz = i;
				}
			}
			if(volthiba>0){
				printf("%d\n", volthiba);
			}
			//printf("2kész\n");
			
			/** -- Lekarandó lap kiválasztva -- */
			
			i = letesz;
			letesz = lap[letesz];
			for(;i<lapdb-1;i++){
				lap[i] = lap[i+1];
			}
			lap[lapdb-1] = 0;
			//printf("3kész\n");
			printf("Add meg a lerakott lapokat!\n"); /** többiek által lerakott lapok beolvasása */
			do{
				printf("%s: ", jat[1].nev);
				scanf("%d", &jatrak[1]);
				if(jatrak[1]==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
			} while(jatrak[1]==-111);
			printf("\220n: %d\n", letesz);
			jatrak[0] = letesz;
			//printf("4kész\n");
			for(i=2;i<jatdb;i++){
				do{
					printf("%s: ", jat[i].nev);
					scanf("%d", &jatrak[i]);
					if(jatrak[i]==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
				} while(jatrak[i]==-111);
			}
			//printf("5kész\n");
			for(i=0;i<jatdb;i++){ /** lapok sorbarendezése */
				min = -1;
				for(j=0;j<jatdb;j++){
					temp = 0;
					for(k=0;k<i;k++){
						if(sorr[k]==j){
							temp = -1;
						}
					}
					if(temp==0){
						if(min==-1){
							min = j;
						} else if(jatrak[j]<jatrak[min]){
							min = j;
						}
					}
				}
				sorr[i] = min;
				fprintf(el, "%d ", jatrak[i]);
			}
			for(i=0;i<jatdb;i++){ /** játékosok lapjainak lerakása */
				masol(tabla2, tabla);
				temp = hozzarak(jatrak[sorr[i]], tabla, jat[sorr[i]]);
				if(temp!=0){ /** elvitt lapok vizsgálata */
					jat[sorr[i]].eredm += temp;
					j = 0;
					//kiir(tabla);
					//kiir(tabla2);
					while(j<4 && tabla[j][0]==tabla2[j][0]){
						j++; /** megf. sor keresése */
					}
					if(j==4){ /** nem találta a sort */
						do{
							printf("Hiba. Add meg az elvitt lapok sz\240m\240t: ");
							scanf("%d", &temp);
							if(temp==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
						} while(temp==-111);
						printf("Add meg az elvitt lapokat: ");
						for(j=0;j<temp;j++){
							do{
								scanf("%d", &k);
								if(k==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
							} while(k==-111);
							ossz_lap[k-1] = sorr[i]; /** elvitt lapok feljegyzése */
						}
					} else{
						k = 0;
						while(tabla2[j][k]!=0 && k<5){ /** elvitt lapok feljegyzése */
							ossz_lap[tabla2[j][k]-1] = sorr[i];
							k++;
						}
					}
				}
			}
			kiir(tabla);
			fajlkiir(tabla, el);
		}
		
		/** -- KÖR VÉGE ------------------------------------------------ KÖR VÉGE -- */
		
		korssz++;
		do{
			printf("Add meg, hogy h\240ny lapot kapok: ");
			scanf("%d", &lapdb);
			if(lapdb==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
		} while(lapdb==-111);
		if(lapdb!=0){
			printf("Add meg a lapjaimat: ");
		}
		for(i=0;i<lapdb;i++){
			do{
				scanf("%d", &lap[i]);
				if(lap[i]==-111){atir(tabla, &lapdb, &kezdlapdb, &jatdb, &jatek_vege, ossz_lap, &letesz, &korssz, &min, jatrak, sorr, jat, lap, eredm, NULL);}
			} while(lap[i]==-111);
		}
		if(lapdb==0){
			jatek_vege = 1;
		}
	}
	
	
	/** ------------------------------------------------------------------------------- */
	/** -- STATISZTIKA ------------------------------------------------- STATISZTIKA -- */
	/** ------------------------------------------------------------------------------- */
	
	printf("\nK\224sz\224n\224m a j\240t\202kot!\n\n");
	printf("Elvitt lapok:\n");
	for(i=0;i<jatdb;i++){
		printf("%s: %d\n", jat[i].nev, jat[i].eredm);
	}
	fclose(el);
	/*for(i=0;i<4;i++){
		printf("%d.\n", i+1);
		for(j=0;j<5;j++){
			scanf("%d", &tabla[i][j]);
		}
	}
	kiir(tabla);
	scanf("%d", &temp);
	printf("%d\n", hozzarak(temp, tabla, jat[1]));
	kiir(tabla);*/
	return 0;
}
