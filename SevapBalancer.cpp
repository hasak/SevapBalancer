#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

#define TOTSTR 604
#define MINSTR 10
#define MAXSTR 50
#define POKUSAJA 10000

using namespace std;

const char pathToInput[]="tempo.txt";
const char pathToOutput[]="out.txt";
const char delimeter[]="--------------------------------";

unsigned int i;
unsigned int spc,spf;
unsigned tempStr;
string tempIme, tempPrezime, temp;

ifstream inputFile;
ofstream ufajl;

HANDLE hndl;
FILETIME ft;
SYSTEMTIME stutc;

const char naslov[]="SevapBalancer v1.0";
const char instk[]="\n\n  1. Napisati u .txt fajl ucace:\n\n     Ime1 Prezime1 #1 (gdje je # broj stranica po satu)\n     Ime2 Prezime2 #2\n           ...\n\n  2. Spremiti fajl u odgovarajucem imenu\n  3. Pokrenuti program (ostale korake)\n  4. Poceti uciti";
const char instd[]="\n  U ime Allaha, Milostivog, Samilosnog!\n\n  Prije ucenja hatmi uglavnom se nejednako podijele stranice/dzuzevi pa oni\n  brzi zavrse to za cas a mi sporiji nikad. Zato je otkucan ovaj program koji za\n  unijete brzine ucaca u stranicama po satu, racuna koliko i koje stranice treba prouciti.\n  Stranice ne idu odjednom, nego, da bi bilo jos pravednije, nasumicno se dodijeljuju\n  stranice koje mogu biti do 50 stranica zaredom.";
const char omeni[]="\n\n  \t\t\tFeb 2020.\n\n  Himzo Hasak, ponosni Tesnjak.\n  Copyright, Pasteleft, ma samo kopiraj, mnozi, dijeli i uci! Open source maximalno!";

bool citajFile(vector<string> &im, vector<string> &pr, vector<unsigned int> &str){
    bool jelOtvoren;
    inputFile.open(pathToInput);
    jelOtvoren=inputFile;
	while(1){
        getline(inputFile,temp);
        if(temp==delimeter)
            break;
	}
	im.resize(0);pr.resize(0);str.resize(0);
	while(inputFile>>tempIme>>tempPrezime>>tempStr){
		im.push_back(tempIme);
		pr.push_back(tempPrezime);
		str.push_back(tempStr);
	}
	inputFile.close();
	hndl=CreateFile(pathToInput, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(hndl, NULL, NULL, &ft);
	FileTimeToSystemTime(&ft, &stutc);
	return jelOtvoren && hndl!=INVALID_HANDLE_VALUE;
}

double getFaktor(vector<unsigned int> &str){
    unsigned int a=0;
    for(i=0;i<str.size();i++)
        a+=str.at(i);
    return 1.0*TOTSTR/a;
}

int zaokruzi(double a){
    int b=a;
    //if(a==b)
        //return b;
    //if(a-b<0.5)
        return b;
    //else return b+1;
}

bool jelPuno(int strs[]){
    for(i=0;i<TOTSTR;i++)
        if(strs[i]==-1)
            return false;
    return true;
}

bool jelDodijeljeno(int str[], unsigned int rs, unsigned int rp){
    for(i=rp;i<rp+rs;i++)
        if(str[i]!=-1)
            return true;
    return false;
}

void dodijeli(int str[],unsigned int rs, unsigned int rp, unsigned int ucac, vector<unsigned int> &dod){
    for(i=rp;i<rp+rs;i++)
        str[i]=ucac;
    dod.at(ucac)+=rs;
}

unsigned int sumastranica(vector<unsigned int> &bsu){
    unsigned int a=0;
    for(i=0;i<bsu.size();i++)
        a+=bsu.at(i);
    return a;
}

bool jelSveDodijeljeno(vector<unsigned int> &bsu){
    return sumastranica(bsu)==TOTSTR;
}

void napraviStrPoUcacu(vector<unsigned int> &strnh, vector<unsigned int> &bsu){
    unsigned int r;
    double faktor=getFaktor(strnh);
    bsu.resize(0);
    for(i=0;i<strnh.size();i++)
        bsu.push_back(zaokruzi(1.0*strnh.at(i)*faktor));
    while(!jelSveDodijeljeno(bsu)){
        r=rand()%strnh.size();
        bsu.at(r)++;
    }
}

void resetStranica(int str[]){
    for(i=0;i<TOTSTR;i++)
        str[i]=-1;
}

string itos(unsigned int a){
    char ch;
    string s="",tt="";
    unsigned int c=0,t,k;
    if(a==0)
        return "0";
    while(a!=0){
        t=a%10;
        a/=10;
        ch=t+48;
        tt="";
        tt.append(new char(ch));
        s.append(tt.substr(0,1));
        c++;
    }
    for(k=0;k<c/2;k++){
        t=s[k];
        s[k]=s[c-k-1];
        s[c-k-1]=t;
    }
    return s;
}

string dtos(double a){
    int aa=a;
    string r;
    char b[100] = {};
    if(aa==a)
        sprintf(b, "%.0f", a);
    else
        sprintf(b, "%.1f", a);
    r=b;
    return r;
}

void uString(int str[], vector<string> &szp){
    unsigned int j;
    fill(szp.begin(),szp.end(),"");
    for(i=0;i<TOTSTR;i++){
        j=i;
        while(str[i]==str[j+1])
            j++;
        if(i!=j)
            szp.at(str[i]).append(itos(i+1)).append("-").append(itos(j+1)).append(", ");
        else
            szp.at(str[i]).append(itos(i+1)).append(", ");
        i=j;
    }
    for(i=0;i<szp.size();i++)
        szp.at(i)=szp.at(i).substr(0,szp.at(i).length()-2);
}

void theCode(int str[], vector<unsigned int> &dod, vector<unsigned int> &bsu, vector<string> &szp){
    unsigned int j,rs,rp,counter=0,zaSmanjivat=MINSTR;
    szp.resize(bsu.size());
    while(!jelPuno(str)){
        for(j=0;j<bsu.size();j++){
            if(dod.at(j)==bsu.at(j))
                continue;
            if(zaSmanjivat==MINSTR)
                rs=rand()%(MAXSTR-MINSTR+1)+MINSTR;

            if(counter==POKUSAJA){
                rs=zaSmanjivat;
                if(zaSmanjivat>1)
                    zaSmanjivat--;
                counter=0;
            }

            rp=rand()%TOTSTR;
            if(dod.at(j)+rs<=bsu.at(j) && !jelDodijeljeno(str,rs,rp)){
                dodijeli(str,rs,rp,j,dod);
                counter=0;
            }
            else{
                counter++;
            }
        }
    }
    uString(str,szp);
}

void np(string n){
    cout<<"\t"<<naslov<<"\n\n\t"<<n<<endl<<endl;
}

void displayMenu(char &izb){
    system("cls");
    cout<<"\n\n";
    np("");
    cout<<"\n"<<endl;
    cout<<"\n  Izaberi:"<<endl;
    cout<<"  1. Instrukcije"<<endl;
    cout<<"  2. Unos iz fajla"<<endl;
    cout<<"  3. Pregled podataka"<<endl;
    cout<<"  4. Racunanje"<<endl;
    cout<<"  5. Pregled rezultata"<<endl;
    cout<<"  6. Ispisi u fajl"<<endl;
    cout<<"  7. O programeru"<<endl;
    cout<<"  9. Izlaz"<<endl;
    cout<<"  ";izb=getch();
    system("cls");
}

void paktc(){
    cout<<"\n\n\n\n  Pritisni bilo koju tipku za nastavak\n  ";getch();
}

void nijeUcitan(){
    cout<<"\n\n  Fajl nije ucitan. Kliknite 2 na glavnom meniju.\n ";
}

void nijeIzracunat(){
    cout<<"\n\n  Fajl nije izracunat. Kliknite 4 na glavnom meniju.\n ";
}

void vecUcitan(){
    cout<<"\n\n  Fajl je vec ucitan. Kliknite slijedece brojeve na glavnom meniju.\n ";
}

void vecIzracunat(){
    cout<<"\n\n  Vec je izracunato.\n  Kliknite 5 na meniju za rezultate.\n ";
}

void p(string p){
    cout<<p;
}

void p(const char p){
    cout<<p;
}

void p(const char p, bool e){
    cout<<p;
    if(e)
        cout<<endl;
}

void p(unsigned const int s, const char p){
    for(i=0;i<s;i++)
        cout<<p;
}

void p(unsigned const int s, const char p, bool e){
    for(i=0;i<s;i++)
        cout<<p;
    if(e)
        cout<<endl;
}

void s(unsigned const int s){
    for(i=0;i<s;i++)
        cout<<" ";
}

void s(unsigned const int s, bool e){
    for(i=0;i<s;i++)
        cout<<" ";
    if(e)
        cout<<endl;
}

void pp(string p){
    ufajl<<p;
}

void pp(const char p){
    ufajl<<p;
}

void pp(const char p, bool e){
    ufajl<<p;
    if(e)
        ufajl<<endl;
}

void pp(unsigned const int s, const char p){
    for(i=0;i<s;i++)
        ufajl<<p;
}

void pp(unsigned const int s, const char p, bool e){
    for(i=0;i<s;i++)
        ufajl<<p;
    if(e)
        ufajl<<endl;
}

void ss(unsigned const int s){
    for(i=0;i<s;i++)
        ufajl<<" ";
}

void ss(unsigned const int s, bool e){
    for(i=0;i<s;i++)
        ufajl<<" ";
    if(e)
        ufajl<<endl;
}

unsigned int len(string a){
    unsigned int c=0;
    while(a[c++]!='\0');
    return c-1;
}

unsigned int len(unsigned int a){
    int c=0;
    while(a!=0){
        a/=10;
        c++;
    }
    return c;
}

unsigned int mx(string v[]){
    unsigned int mx;
    if(!v)
        return 0;
    mx=len(v[0]);
    for(i=1;i<sizeof(v)/sizeof(string);i++)
        if(len(v[i])>mx)
            mx=len(v[i]);
    return mx;
}

unsigned int mx(vector<string> &v){
    unsigned int mx;
    if(!v.size())
        return 0;
    mx=len(v.at(0));
    for(i=1;i<v.size();i++)
        if(len(v.at(i))>mx)
            mx=len(v.at(i));
    return mx;
}

unsigned int mx(vector<unsigned int> &v){
    unsigned int mx;
    if(!v.size())
        return 0;
    mx=len(v.at(0));
    for(i=1;i<v.size();i++)
        if(len(v.at(i))>mx)
            mx=len(v.at(i));
    return mx;
}

string maxSekUString(unsigned int s){
    int c=0;
    double a;
    unsigned int usek[]={31104000,2592000,86400,3600,60,1};
    string str[]={"godina","mjeseci","dana","sati","minuta","sekundi"};
    string r="";
    while(s/usek[c]==0)
        c++;
    a=1.0*s/usek[c];
    r.append(dtos(a)).append(" ").append(str[c]);
    return r;
}

unsigned int zaogore(unsigned int dij, unsigned int dje){
    double a=dij,b=dje;
    unsigned int rezultat;
    rezultat=dij/dje;
    if(1.0*a/b!=rezultat)
        rezultat++;
    return rezultat;
}

void ispisiQuickFact(vector<unsigned int> &strnh, bool fajl){
    double zaisp;
    unsigned const int MJJ=6,zadku=5;
    unsigned int sss=sumastranica(strnh),ccc;
    unsigned int sek=3600*TOTSTR/sss,sek2=sek,intdana;
    unsigned int arr[MJJ],c=0,j,sc[2],scc[2],maxlenstr;
    unsigned int usek[]={31104000,2592000,86400,3600,60,1};
    unsigned int dku[]={15*60,30*60,1*60*60,1.5*60*60,2*60*60};
    string str[]={"godina","mjeseci","dana","sati","minuta","sekundi"};
    string strr[]={"Dnevno","Ukupno*"};

    for(i=0;i<MJJ;i++){
        arr[i]=sek/usek[i];
        sek%=usek[i];
    }
    sc[0]=max((unsigned int)2,len(arr[0]));sc[1]=mx(str);
    while(!arr[c++]);

    maxlenstr=len(maxSekUString(dku[0]));
    for(j=1;j<zadku;j++)
        if(len(maxSekUString(dku[i]))>maxlenstr)
            maxlenstr=len(maxSekUString(dku[i]));

    scc[0]=9;scc[1]=7;

    if(fajl){
        unsigned const int sp=2;
        const char u='|',v='-',du='|',dv='-',gd='+',gl='+',dl='+',dd='+',ts='+',tj='+',ti='+',tz='+',pl='+',tsd='+',tjd='+',tid='+',tzd='+',pld='+';
        ufajl.open(pathToOutput,ios::app);

        ufajl<<"\n  Quick fact:\n  Uceci ovim tempom hatma se zavrsava svakih:"<<endl;
        ss(2);pp(gl);pp(sc[0]+sc[1]+4*sp+1,dv);pp(gd,true);
        for(j=c-1;j<MJJ;j++){
            ss(2);pp(du);ss(sc[0]-len(arr[j])+2*sp);ufajl<<arr[j];ss(1);pp(str[j]);ss(sc[1]-len(str[j])+2*sp);pp(du,true);
        }
        ss(2);pp(dl);pp(sc[0]+sc[1]+4*sp+1,dv);pp(dd,true);

        ufajl<<"\n  Ili po danu:"<<endl;

        //scc[0]=max(maxlenstr,len(strr[0]));scc[1]=7;

        ss(2);pp(gl);pp(scc[0]+2*sp,dv);pp(ts);pp(scc[1]+2*sp,dv);pp(gd,true);
        ss(2);pp(du);ss(sp);ss(scc[0]-len(strr[0]));pp(strr[0]);ss(sp);pp(u);ss(sp);ss(scc[1]-len(strr[1]));pp(strr[1]);ss(sp);pp(du,true);
        ss(2);pp(tzd);pp(scc[0]+2*sp,dv);pp(pld);pp(scc[1]+2*sp,dv);pp(tid,true);
        for(j=0;j<zadku;j++){
            intdana=zaogore(sek2,dku[j]);
            ss(2);pp(du);ss(sp);ss(scc[0]-len(maxSekUString(dku[j])));pp(maxSekUString(dku[j]));ss(sp);pp(u);ss(sp);ss(scc[1]-7);intdana<10?ufajl<<" ":ufajl<<"";ufajl<<intdana<<" dana";ss(sp);pp(du,true);
        }
        ss(2);pp(dl);pp(scc[0]+2*sp,dv);pp(tj);pp(scc[1]+2*sp,dv);pp(dd,true);
        ufajl<<"   *Zaokruzeno na dan vise\n"<<endl;
        ufajl.close();
    }
    else{
        unsigned const int sp=1;
        const char u=179,v=196,du=186,dv=205,gd=187,gl=201,dl=200,dd=188,ts=209,tj=207,ti=182,tz=199,pl=197,tsd=185,tjd=202,tid=185,tzd=204,pld=216;

        cout<<"\n  Quick fact:\n  Uceci ovim tempom hatma se zavrsava svakih:"<<endl;
        s(2);p(gl);p(sc[0]+sc[1]+4*sp+1,dv);p(gd,true);
        for(j=c-1;j<MJJ;j++){
            s(2);p(du);s(sc[0]-len(arr[j])+2*sp);cout<<arr[j];s(1);p(str[j]);s(sc[1]-len(str[j])+2*sp);p(du,true);
        }
        s(2);p(dl);p(sc[0]+sc[1]+4*sp+1,dv);p(dd,true);

        cout<<"\n  Ili po danu:"<<endl;

        //scc[0]=max(maxlenstr,len(strr[0]));scc[1]=7;

        s(2);p(gl);p(scc[0]+2*sp,dv);p(ts);p(scc[1]+2*sp,dv);p(gd,true);
        s(2);p(du);s(sp);s(scc[0]-len(strr[0]));p(strr[0]);s(sp);p(u);s(sp);s(scc[1]-len(strr[1]));p(strr[1]);s(sp);p(du,true);
        s(2);p(tzd);p(scc[0]+2*sp,dv);p(pld);p(scc[1]+2*sp,dv);p(tid,true);
        for(j=0;j<zadku;j++){
            intdana=zaogore(sek2,dku[j]);
            s(2);p(du);s(sp);s(scc[0]-len(maxSekUString(dku[j])));p(maxSekUString(dku[j]));s(sp);p(u);s(sp);s(scc[1]-7);intdana<10?cout<<" ":cout<<"";cout<<intdana<<" dana";s(sp);p(du,true);
        }
        s(2);p(dl);p(scc[0]+2*sp,dv);p(tj);p(scc[1]+2*sp,dv);p(dd,true);
        cout<<"   *Zaokruzeno na dan vise\n"<<endl;
    }
}

void ispisiRezultate(int str[], vector<unsigned int> &dod, vector<unsigned int> &bsu, vector<string> &ime, vector<string> &prez, vector<string> &szp, bool fajl){
    unsigned int j;
    const string col[]={"#","Ime i prezime","Ukupno*","Stranice"};
    unsigned int sc[]={
        max(len(col[0]),len(bsu.size()+1)),
        max(len(col[1]),mx(ime)+mx(prez)+1),
        max(len(col[2]),mx(bsu)),
        max(len(col[3]),mx(szp))
    };

    if(fajl){
        unsigned const int sp=3;
        const char u='|',v='-',du='|',dv='-',gd='+',gl='+',dl='+',dd='+',ts='+',tj='+',ti='+',tz='+',pl='+',tsd='+',tjd='+',tid='+',tzd='+',pld='+';
        ufajl.open(pathToOutput);
        ufajl<<"\n\t\t\t"<<naslov<<endl;
        ufajl<<"\n\t\t\tRezultati racunanja"<<endl;
        ufajl<<"\n"<<endl;

        ss(1);pp(gl);pp(sc[0]+2*sp,dv);pp(ts);pp(sc[1]+2*sp,dv);pp(ts);pp(sc[2]+2*sp,dv);pp(ts);pp(sc[3]+2*sp,dv);pp(gd,true);
        ss(1);pp(du);ss(sp);ss(sc[0]-len(col[0]));pp(col[0]);ss(sp);pp(u);ss(sp);ss(sc[1]-len(col[1]));pp(col[1]);ss(sp);pp(u);ss(sp);ss(sc[2]-len(col[2]));pp(col[2]);ss(sp);pp(u);ss(sp);pp(col[3]);ss(sc[3]-len(col[3]));ss(sp);pp(du,true);
        ss(1);pp(tzd);pp(sc[0]+2*sp,dv);pp(pld);pp(sc[1]+2*sp,dv);pp(pld);pp(sc[2]+2*sp,dv);pp(pld);pp(sc[3]+2*sp,dv);pp(tid,true);
        for(j=0;j<dod.size();j++){
            ss(1);pp(du);ss(sp);
            ss(sc[0]-len(j+1));ufajl<<j+1;ss(sp);pp(u);
            ss(sp);ss(sc[1]-len(ime.at(j))-len(prez.at(j))-1);pp(ime.at(j));ss(1);pp(prez.at(j));ss(sp);pp(u);
            ss(sp);ss(sc[2]-len(dod.at(j)));ufajl<<dod.at(j);ss(sp);pp(u);
            ss(sp);pp(szp.at(j));ss(sc[3]-len(szp.at(j)));ss(sp);pp(du,true);
            if(j+1!=dod.size()){
                ss(1);pp(tz);pp(sc[0]+2*sp,v);pp(pl);pp(sc[1]+2*sp,v);pp(pl);pp(sc[2]+2*sp,v);pp(pl);pp(sc[3]+2*sp,v);pp(ti,true);
            }
        }
        ss(1);pp(dl);pp(sc[0]+2*sp,dv);pp(tj);pp(sc[1]+2*sp,dv);pp(tj);pp(sc[2]+2*sp,dv);pp(tj);pp(sc[3]+2*sp,dv);pp(dd,true);
        ufajl<<"  *Brzina ucaca na dan "<<stutc.wDay<<". "<<stutc.wMonth<<". "<<stutc.wYear<<".\n"<<endl;
        ufajl.close();
        spf=sc[0]+sc[1]+sc[2]+sc[3]+sp*8+4;
    }
    else{
        unsigned const int sp=1;
        const char u=179,v=196,du=186,dv=205,gd=187,gl=201,dl=200,dd=188,ts=209,tj=207,ti=182,tz=199,pl=197,tsd=185,tjd=202,tid=185,tzd=204,pld=216;
        np("Rezultati racunanja");
        cout<<"\n"<<endl;

        s(1);p(gl);p(sc[0]+2*sp,dv);p(ts);p(sc[1]+2*sp,dv);p(ts);p(sc[2]+2*sp,dv);p(ts);p(sc[3]+2*sp,dv);p(gd,true);
        s(1);p(du);s(sp);s(sc[0]-len(col[0]));p(col[0]);s(sp);p(u);s(sp);s(sc[1]-len(col[1]));p(col[1]);s(sp);p(u);s(sp);s(sc[2]-len(col[2]));p(col[2]);s(sp);p(u);s(sp);p(col[3]);s(sc[3]-len(col[3]));s(sp);p(du,true);
        s(1);p(tzd);p(sc[0]+2*sp,dv);p(pld);p(sc[1]+2*sp,dv);p(pld);p(sc[2]+2*sp,dv);p(pld);p(sc[3]+2*sp,dv);p(tid,true);
        for(j=0;j<dod.size();j++){
            s(1);p(du);s(sp);s(sc[0]-len(j+1));cout<<j+1;s(sp);p(u);
            s(sp);s(sc[1]-len(ime.at(j))-len(prez.at(j))-1);p(ime.at(j));s(1);p(prez.at(j));s(sp);p(u);
            s(sp);s(sc[2]-len(dod.at(j)));cout<<dod.at(j);s(sp);p(u);
            s(sp);p(szp.at(j));s(sc[3]-len(szp.at(j)));s(sp);p(du,true);
            if(j+1!=dod.size()){
                s(1);p(tz);p(sc[0]+2*sp,v);p(pl);p(sc[1]+2*sp,v);p(pl);p(sc[2]+2*sp,v);p(pl);p(sc[3]+2*sp,v);p(ti,true);
            }
        }
        s(1);p(dl);p(sc[0]+2*sp,dv);p(tj);p(sc[1]+2*sp,dv);p(tj);p(sc[2]+2*sp,dv);p(tj);p(sc[3]+2*sp,dv);p(dd,true);
        cout<<"  *Brzina ucaca na dan "<<stutc.wDay<<". "<<stutc.wMonth<<". "<<stutc.wYear<<".\n"<<endl;
        spc=sc[0]+sc[1]+sc[2]+sc[3]+sp*8+4;
    }
}

void ispisiDatu(vector<string> &ime, vector<string> &prez, vector<unsigned int> &szp){
    unsigned int j;
    unsigned const int sp=1;
    const char u=179,v=196,du=186,dv=205,gd=187,gl=201,dl=200,dd=188,ts=209,tj=207,ti=182,tz=199,pl=197,tsd=185,tjd=202,tid=185,tzd=204,pld=216;
    const string col[]={"#","Ime i prezime","Brzina*"};
    unsigned int sc[]={
        max(len(col[0]),len(ime.size()+1)),
        max(len(col[1]),mx(ime)+mx(prez)+1),
        max(len(col[2]),mx(szp))
    };
    np("Ulazna data");
    cout<<"\n  Ulazni file: "<<pathToInput<<endl;
    s(1);p(gl);p(sc[0]+2*sp,dv);p(ts);p(sc[1]+2*sp,dv);p(ts);p(sc[2]+2*sp,dv);p(gd,true);
    s(1);p(du);s(sp);s(sc[0]-len(col[0]));p(col[0]);s(sp);p(u);s(sp);s(sc[1]-len(col[1]));p(col[1]);s(sp);p(u);s(sp);s(sc[2]-len(col[2]));p(col[2]);s(sp);p(du,true);
    s(1);p(tzd);p(sc[0]+2*sp,dv);p(pld);p(sc[1]+2*sp,dv);p(pld);p(sc[2]+2*sp,dv);p(tid,true);
    for(j=0;j<ime.size();j++){
        s(1);p(du);s(sp);s(sc[0]-len(j+1));cout<<j+1;s(sp);p(u);
        s(sp);s(sc[1]-len(ime.at(j))-len(prez.at(j))-1);p(ime.at(j));s(1);p(prez.at(j));s(sp);p(u);
        s(sp);cout<<szp.at(j);s(sc[2]-len(szp.at(j)));s(sp);p(du,true);
        if(j+1!=ime.size()){
            s(1);p(tz);p(sc[0]+2*sp,v);p(pl);p(sc[1]+2*sp,v);p(pl);p(sc[2]+2*sp,v);p(ti,true);
        }
    }
    s(1);p(dl);p(sc[0]+2*sp,dv);p(tj);p(sc[1]+2*sp,dv);p(tj);p(sc[2]+2*sp,dv);p(dd,true);
    cout<<"  *stranica/sat na dan "<<stutc.wDay<<". "<<stutc.wMonth<<". "<<stutc.wYear<<".\n"<<endl;
}

void ispisiSpam(bool fajl){
    time_t sad = time(0);
    tm *tmm = localtime(&sad);
    string zaoutputa[]={"Bil hajr!","Fajl napravljen ","Kodirao: Himzo Hasak, Feb 2020."};
    zaoutputa[1].append(itos(tmm->tm_mday)).append(". ").append(itos(tmm->tm_mon+1)).append(". ").append(itos(tmm->tm_year+1900)).append(".");
    if(fajl){
        ufajl.open(pathToOutput,ios::app);
        ss(spf-len(zaoutputa[0]));ufajl<<zaoutputa[0]<<endl;
        ss(spf-len(zaoutputa[1]));ufajl<<zaoutputa[1]<<endl;
        ss(spf-len(zaoutputa[2]));ufajl<<zaoutputa[2]<<endl;
        ufajl.close();
    }
    else{
        s(spc-len(zaoutputa[0]));cout<<zaoutputa[0]<<endl;
        s(spc-len(zaoutputa[1]));cout<<zaoutputa[1]<<endl;
        s(spc-len(zaoutputa[2]));cout<<zaoutputa[2]<<endl;
    }
}

void ispisiZaPonovo(vector<string> &ime, vector<string> &prez, vector<unsigned int> &szp){
    unsigned int j;
    string zaoutputa="\n  Tip:\n  Ovaj fajl mozete koristiti kao input, samo ga preimenujte u ulazno ime.";
    ufajl.open(pathToOutput,ios::app);
    ufajl<<zaoutputa;
    for(j=0;j<20;j++)ufajl<<endl;
    ufajl<<delimeter<<endl;
    for(j=0;j<ime.size();j++)
        ufajl<<ime.at(j)<<" "<<prez.at(j)<<" "<<szp.at(j)<<endl;
    ufajl.close();
}

int main()
{
    SetConsoleTitleA(naslov);
	int str[TOTSTR];
	char izborMenija;
	bool ocitan=false,izracunat=false;

	clock_t t;

	vector<string> imena, prezimena, strZaPrint;
	vector<unsigned int> strNaSat, dodijeljeno, brStrPoUcacu;

    srand(time(NULL));

    while(1){
        displayMenu(izborMenija);
        cout<<" \n\n ";
        if(izborMenija=='1'){
            np("Instrukcije");
            t=clock();
            cout<<instd<<endl<<instk<<"\n\n  Fajl za spremit: "<<pathToInput<<endl;
            t=clock()-t;
            cout<<"\n  Ispis instrukcija zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
        }
        else if(izborMenija=='2'){
            np("Citanje fajla");
            if(ocitan){
                t=clock();
                vecUcitan();
                t=clock()-t;
                cout<<"\n  Ispis zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
                paktc();
                continue;
            }
            t=clock();
            if(citajFile(imena,prezimena,strNaSat)){
                dodijeljeno.resize(strNaSat.size());
                napraviStrPoUcacu(strNaSat,brStrPoUcacu);
                resetStranica(str);
                t=clock()-t;
                cout<<"\n  Citanje fajla uspjesno zavrseno za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
                ocitan=true;
            }
            else cout<<"\n  Greska pri citanju fajla! Mozda fajl ne postoji ("<<pathToInput<<")"<<endl;
        }
        else if(izborMenija=='3'){
            if(ocitan){
                t=clock();
                ispisiDatu(imena,prezimena,strNaSat);
                t=clock()-t;
                cout<<"\n  Ispis zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
            }
            else nijeUcitan();
        }
        else if(izborMenija=='4'){
            np("Racunanje");
            if(izracunat){
                t=clock();
                vecIzracunat();
                t=clock()-t;
                cout<<"\n  Ispis zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
                paktc();
                continue;
            }
            if(ocitan){
                cout<<"\n  Cek...\n  ";
                t=clock();
                theCode(str,dodijeljeno,brStrPoUcacu,strZaPrint);
                t=clock()-t;
                cout<<"\n  Racunanje uspjesno zavrseno za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
                izracunat=true;
            }
            else nijeUcitan();
        }
        else if(izborMenija=='5'){
            if(izracunat){
                t=clock();
                ispisiRezultate(str,dodijeljeno,brStrPoUcacu,imena,prezimena,strZaPrint,false);
                ispisiQuickFact(strNaSat,false);
                ispisiSpam(false);
                t=clock()-t;
                cout<<"\n  Ispis zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
            }
            else nijeIzracunat();
        }
        else if(izborMenija=='6'){
            cout<<"\n  Ispisivanje u fajl...\n\n  Imati na umu da se ovaj fajl moze koristiti i kao ulazni fajl sa brzinama samo ga preimenujte kako treba.\n  ";
            t=clock();
            ispisiRezultate(str,dodijeljeno,brStrPoUcacu,imena,prezimena,strZaPrint,true);
            ispisiQuickFact(strNaSat,true);
            ispisiSpam(true);
            ispisiZaPonovo(imena,prezimena,strNaSat);
            t=clock()-t;
            cout<<"\n  Ispis u fajl zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
        }
        else if(izborMenija=='7'){
            np("O meni bajagi");
            t=clock();
            cout<<omeni<<endl;
            t=clock()-t;
            cout<<"\n  Ispis ovoga o meni zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
        }
        else if(izborMenija=='9'){
            np("Izlaz");
            t=clock();
            cout<<"\n  Hvala vam na koristenju programa.\n  Da Allah olaksa i ukabuli ucenje. Selam!"<<endl;
            t=clock()-t;
            cout<<"\n  Ispis kraja zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
            break;
        }
        else {
            np("Greska");
            t=clock();
            cout<<"\n  Pogresna tipka.\n  Pokusajte opet iz glavnog menija."<<endl;
            t=clock()-t;
            cout<<"\n  Ispis greske zavrsen za: "<<double(t)*1.0/CLOCKS_PER_SEC<<" s"<<endl;
        }
        paktc();
    }
	paktc();
    return 0;
}
