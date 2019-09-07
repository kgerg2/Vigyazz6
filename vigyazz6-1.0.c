#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define PROBA 1000

struct preferencia{ /** 9: 1.kör 1.harmada, ... */
	int meret[9][10]; /** 1-10, 11-20, ... */
	int elh[9][6]; /** legkisebb végső lap előtt; utána, de a második előtt; ... */
	int sor[5]; /** szabad helyek száma az adott sorban */
};

struct jatekos{
	char nev[20];
	int eredm;
};

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
	printf("\nTábla:\n");
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

void fajlkiir(int tabla[4][5], FILE *fa){ /** Jelenlegi állás kiírása */
	int i, j;
	fprintf(fa, "\nTábla:\n");
	for(i=0;i<4;i++){
		for(j=0;j<5;j++){
			if(tabla[i][j]!=0){
				fprintf(fa, "%4d", tabla[i][j]);
			}
		}
		fprintf(fa, "\n");
	}
	fprintf(fa, "\n");
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

int hozzarak(int uj, int tabla[4][5], struct jatekos jat){ /** Új lap lerakása */
	int i, j, tav[4], min;
	min = 0;
	for(i=0;i<4;i++){
		j = 4;
		while(j>=0 && tabla[i][j]==0){
			j--;
		}
		if(j==-1){
			printf("HIBA -- sor adat nélkül --\n");
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
		if(strcmp(jat.nev, "szg.")!=0){
			printf("Melyik sort cseréli %s? (1-4) ", jat.nev);
			while(csere<1 || csere>4){
				scanf("%d", &csere);
			}
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
		pref[i] = (double)pref_ered[i]*(double)lapdb[i];
		//printf("pref[%d] = %f\n", i, pref[i]);
		osszpref += pref[i];
	}
	p = rand();
	i = 0;
	while(p>0 && i<7){
		p -= RAND_MAX*pref[i]/osszpref;
		//printf("p = %d\n", p);
		i++;
	}
	//printf("mrandhely vege\n");
	return i;
}

/** ------------------------------------------------------------------------------------- */
/** -- KEZDÉS - main --------------------------------------------------- KEZDÉS - main -- */
/** ------------------------------------------------------------------------------------- */

int main(){
	int i, j, k, l, tabla[4][5], tabla2[4][5], lapdb, kezdlapdb, jatdb, temp, jatek_vege, ossz_lap[104], letesz, korssz, min;
	char ch;
	 /** ossz_lap: -3 - kézben | -2 - táblán | -1 - pakliban/kérdéses | 0 - én elvittem | 1+ - adott ID-vel rendelkező játékos elvitte */
	struct preferencia pref;
	FILE *pr, *teszt;
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
	printf("előkészületek\n");
	/** Preferenciák beolvasása */
	
	pr = fopen("preferenciak.txt", "r");
	fscanf(pr, "%c", &ch);
	if(ch=='/'){
		while(ch!='\n'){
			fscanf(pr, "%c", &ch);
		}
	}
	for(i=0;i<9;i++){
		while(ch!='-'){
			fscanf(pr, "%c", &ch);
		}
		for(j=0;j<10;j++){
			fscanf(pr, " %d", &pref.meret[i][j]);
		}
		fscanf(pr, "\n");
	}
	for(i=0;i<9;i++){
		while(ch!='-'){
			fscanf(pr, "%c", &ch);
		}
		for(j=0;j<6;j++){
			fscanf(pr, " %d", &pref.elh[i][j]);
		}
		fscanf(pr, "\n");
	}
	while(ch!='-'){
		fscanf(pr, "%c", &ch);
	}
	for(i=0;i<5;i++){
		fscanf(pr, " %d", &pref.sor[i]);
	}
	fclose(pr);
	printf("preferenciák beolvasva\n");
	
	/** Játék kezdése */
	
	printf("Add meg a játékosok számát (velem együtt): ");
	scanf("%d", &jatdb);
	int jatrak[jatdb], sorr[jatdb];
	struct jatekos jat[jatdb];
	for(i=0;i<jatdb;i++){
		jatrak[i] = 0;
		jat[i].eredm = 0;
	}
	strcpy(jat[0].nev, "szg.");
	printf("Add meg a játékosok neveit:");
	for(i=1;i<jatdb;i++){
		scanf(" %[^\n]", jat[i].nev);
	}
	printf("Add meg a lapjaim darabszámát (amennyiben ennél több is lesz, azt): ");
	scanf("%d", &lapdb);
	kezdlapdb = lapdb;
	int lap[lapdb], eredm[lapdb];
	printf("Add meg a lapjaimat: ");
	for(i=0;i<lapdb;i++){
		scanf("%d", &lap[i]);
		ossz_lap[lap[i]-1] = -3;
	}
	printf("Add meg a kezdőlapokat: ");
	for(i=0;i<4;i++){
		scanf("%d", &tabla[i][0]);
	}
	
	/** ------------------------------------------------------------------------- */
	/** -- JÁTÉK ------------------------------------------------------- JÁTÉK -- */
	/** ------------------------------------------------------------------------- */
	
	jatek_vege = 0;
	korssz = 1;
	while(jatek_vege==0){
		
		/** -- KÖR KEZDETE --------------------------------------------- KÖR KEZDETE -- */
		
		for(;lapdb>0;lapdb--){
			for(i=0;i<kezdlapdb;i++){
				eredm[i] = 0;
			}
			
			/** -- Lerakandó lap kiválasztása -- */
			
			for(i=0;i<lapdb;i++){
				/** Méret szerint */
				
				for(j=0;j<PROBA;j++){
					masol(tabla2, tabla);
					for(k=0;k<jatdb-1;k++){ /** mások által lerakott lapok (jatrak[]) */
						temp = mrandhely(pref.meret[(korssz-1)*3+(kezdlapdb-lapdb)*3/kezdlapdb], ossz_lap, jatdb);
						l = 0;
						do{ /** a tempedik 13-as egységben véletlenszerű, még nem lerakott lap */
							jatrak[k] = rand()%13+13*temp;
							l++;
						}while(ossz_lap[jatrak[k]-1]!=-1 && l<100000);
						if(l==100000){
							printf("HIBA - nem található lap a megadott tartományban\n");
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
					fprintf(teszt, "%d. próba\n", j);
				}
				/** Méret szerinti kiválasztás vége */
				
				fprintf(teszt, "Lap: %d - %d\n", lap[i], eredm[i]);
			}
			
			/** -- Lekarandó lap kiválasztva -- */
			
			letesz = 0;
			for(i=0;i<lapdb;i++){
				if(eredm[letesz]>eredm[i] || (eredm[letesz]==eredm[i] && rand()%2==0)){
					letesz = i;
				}
			}
			i = letesz;
			letesz = lap[letesz];
			for(;i<lapdb-1;i++){
				lap[i] = lap[i+1];
			}
			lap[lapdb-1] = 0;
			printf("Add meg a lerakott lapokat!\n");
			printf("%s: ", jat[1].nev);
			scanf("%d", &jatrak[1]);
			printf("Én: %d\n", letesz);
			jatrak[0] = letesz;
			for(i=2;i<jatdb;i++){
				printf("%s: ", jat[i].nev);
				scanf("%d", &jatrak[i]);
			}
			for(i=0;i<jatdb;i++){
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
				printf("%d\n", min);
			}
			masol(tabla2, tabla);
			temp = hozzarak(jatrak[sorr[0]], tabla, jat[sorr[0]]);
			if(temp!=0){
				jat[sorr[0]].eredm += temp;
				i = 0;
				while(i<4 && tabla[i][1]==0 && tabla2[i][1]!=0){
					i++;
				}
				if(i==4){
					printf("Hiba. Add meg az elvitt lapok számát: ");
					scanf("%d", &temp);
					printf("Add meg az elvitt lapokat: ");
					for(i=0;i<temp;i++){
						scanf("%d", &j);
						ossz_lap[j-1] = sorr[0];
					}
				} else{
					j = 0;
					while(tabla2[i][j]!=0 && j<5){
						ossz_lap[tabla2[i][j]-1] = sorr[0];
						j++;
					}
				}
			}
			for(i=1;i<jatdb;i++){
				masol(tabla2, tabla);
				temp = hozzarak(jatrak[sorr[i]], tabla, jat[sorr[i]]);
				if(temp!=0){
					jat[sorr[i]].eredm += temp;
					j = 0;
					while(j<4 && tabla[j][1]==0 && tabla2[j][1]!=0){
						j++;
					}
					if(j==4){
						printf("Hiba. Add meg az elvitt lapokat: ");
						for(j=0;j<temp;j++){
							scanf("%d", &k);
							ossz_lap[k-1] = sorr[i];
						}
					} else{
						k = 0;
						while(tabla2[j][k]!=0 && k<5){
							ossz_lap[tabla2[j][k]-1] = sorr[i];
							k++;
						}
					}
				}
			}
			kiir(tabla);
		} /** Kör vége ---------------------------------------------- */
		korssz++;
		printf("Add meg, hogy hány lapot kapok: ");
		scanf("%d", &lapdb);
		printf("Add meg a lapjaimat: ");
		for(i=0;i<lapdb;i++){
			scanf("%d", &lap[i]);
		}
		if(lapdb==0){
			jatek_vege = 1;
		}
	} /** -- VÉGE ------------------------------------------------------ VÉGE -- */
	
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
