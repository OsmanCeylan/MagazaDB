#include <stdio.h>
#include <stdlib.h>

struct urunler{
  int urunNo;
  char urunAd[30];
  char urunKategori[18];
  double urunAlis;
  double urunSatis;
};

struct stoklar{
  int urunNo;
  int magazaID;
  int stokAdet;
  int satilan;
};

struct magazalar{
  int magazaID;
  char magazaAd[20];
  int satis;
  double ciro;
  double kar;

};

void anaMenu ();
int urunSatisi (int);
void magazaoku ();
int magazaid;
void stokDurumu();
void magazaList();
void urunTranferi (int);
void enfazlaKar ();
void sistemdenSil();
void stogaEkleme (int);

int main(void)
{
  printf("Magazalarimiz NO:1 ve 2 ve 3");
  printf("\nLutfen magaza seciniz: ");
  scanf ("%d",&magazaid);
  anaMenu ();
  return 0;
}

void anaMenu(){
  int tercih;
  printf ("1-)Urun Satisi\n"
          "2-)Urunun stok durumunu listeleme\n"
          "3-)Magaza bilgilerinin listelenmesi\n"
          "4-)Magazalar arasi urun transferi\n"
          "5-)En fazla kar edilen urunlerin listesi\n"
          "6-)Magazanin sistemden silinmesi\n"
          "7-)Stoga urun eklenmesi\n Tercihiniz: ");
  scanf ("%d",&tercih);
  switch (tercih){
    case 1:
      urunSatisi(magazaid);
      break;
    case 2:
      stokDurumu ();
      break;
    case 3:
      magazaList();
      break;
    case 4:
      urunTranferi (magazaid);
      break;
    case 5:
      enfazlaKar ();
      break;
    case 6:
      sistemdenSil();
      break;
    case 7:
      stogaEkleme (magazaid);
      break;
    default:
      printf ("\nYanlis girdi\n");
      break;
    }
}

void magazaoku(){
  FILE *cfile;
  cfile=fopen("magazalar.dat","rb");
  struct magazalar magaza={0,"",0,0,0};
  printf ("%10s%10s%15s%10s%10s","Urun no","ad","satis","ciro","kar\n");
  printf ("______________________________________________________\n");
  fread (&magaza,sizeof(struct magazalar),1,cfile);
  while (!feof(cfile)){
      if (magaza.magazaID!=-1)
          printf ("%8d%12s%16d%11lf%10lf\n",magaza.magazaID,magaza.magazaAd,magaza.satis,magaza.ciro,magaza.kar);
      fread (&magaza,sizeof(struct magazalar),1,cfile);
    }
  rewind(cfile);
  fclose(cfile);
}

int urunSatisi(int magazaid){
  int satilanUrun,sutun=0;
  double kar;
  FILE *uruns;
  FILE *stoks;
  FILE *yedek;
  FILE *magazas;
  uruns=fopen("urunler.dat","rb");
  stoks=fopen("stok.dat","r");
  magazas=fopen ("magazalar.dat","rb+");
  yedek=fopen ("yedek.dat","w");
  printf ("Satilan urunun numarasini giriniz: ");
  scanf ("%d",&satilanUrun);
  struct urunler urun={0,"","",0,0};
  struct stoklar stok={0,0,0,0};
  struct magazalar magaza={0,"",0,0,0};
  fread (&urun,sizeof(struct urunler),1,uruns);
  while (!feof(uruns)){
      if (urun.urunNo==satilanUrun){
          printf ("Urun Bulundu ve satildi!\n");
          printf ("%5d%14s%13s%12lf%10lf\n",urun.urunNo,urun.urunAd,urun.urunKategori,urun.urunAlis,urun.urunSatis);
          kar=urun.urunSatis-urun.urunAlis;
          break;
        }
      fread (&urun,sizeof(struct urunler),1,uruns);
    }
  if (urun.urunNo!=satilanUrun){
      printf ("Urun Bulunamadi!\n");
      fclose(stoks);
      fclose(magazas);
      fclose(uruns);
      fclose(yedek);
      remove ("yedek.dat");

    }
  else{
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      while (!feof(stoks)){
          if (satilanUrun==stok.urunNo && stok.magazaID==magazaid){
              stok.satilan++;
              stok.stokAdet--;
            }
          fprintf (yedek,"%d %d %d %d\n",stok);
          fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
        }
      fprintf (yedek,"%d %d %d %d",stok);
      fread(&magaza,sizeof(struct magazalar),1,magazas);
      sutun++;
      while (!feof(magazas)){
          if (magaza.magazaID==magazaid){
              magaza.satis++;
              magaza.kar=kar;
              magaza.ciro+=kar;
              fseek (magazas,(sutun-1)*sizeof(struct magazalar),SEEK_SET);
              fwrite (&magaza,sizeof(struct magazalar),1,magazas);
              break;
            }
          fread(&magaza,sizeof(struct magazalar),1,magazas);
          sutun++;
        }
      fclose(magazas);
      fclose(uruns);
      fclose(stoks);
      fclose(yedek);
      remove ("stok.dat");
      rename ("yedek.dat","stok.dat");
      magazaoku ();
    }
}

void stokDurumu(){
  int urunno;
  FILE *uruns;
  FILE *magazas;
  FILE *stoks;
  uruns=fopen("urunler.dat","rb");
  magazas=fopen("magazalar.dat","rb");
  stoks=fopen("stok.dat","rb");
  printf ("Stok durumuna bakilacak urunun numarasini giriniz: ");
  scanf("%d",&urunno);
  struct urunler urun={0,0,0,0};
  struct magazalar magaza={0,"",0,0,0};
  struct stoklar stok={0,0,0,0};
  fread(&urun,sizeof(struct urunler),1,uruns);
  while (!feof(uruns)){
      if (urun.urunNo==urunno)
          break;
     fread(&urun,sizeof(struct urunler),1,uruns);
    }
  if (urun.urunNo!=urunno){
      printf ("Urun bulunamadi\n");
      fclose(uruns);
      fclose(magazas);
      fclose(stoks);
    }
  else{
    printf("\n%10s%14s%15s%15s","Urun No","Urun Ad","Magaza Ad","Stok Adeti\n");
    printf("______________________________________________________\n");
    fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
    while (!feof(stoks)){
       if (urun.urunNo==stok.urunNo){
           fread(&magaza,sizeof(struct magazalar),1,magazas);
           printf ("%8d%13s%18s%11d\n",urun.urunNo,urun.urunAd,magaza.magazaAd,stok.stokAdet);
         }
       fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
       }
    }
  fclose(stoks);
  fclose(magazas);
  fclose(uruns);
}

void magazaList (){
  FILE *magazas;
  int i=1,j,k,z=0;
  double dizi[10],yedek;
  magazas=fopen("magazalar.dat","rb");
  struct magazalar magaza={0,"",0,0,0};
  fread (&magaza,sizeof(struct magazalar),1,magazas);
  dizi[0]=magaza.ciro;
  while (!feof(magazas)){
      fread (&magaza,sizeof(struct magazalar),1,magazas);
      dizi[i]=magaza.ciro;
      i++;
    }
  j=i;
  for (k=0;k<j-1;k++){
      for (i=0;i<j-1;i++){
      if (dizi[i+1]>dizi[i]){
          yedek=dizi[i];
          dizi[i]=dizi[i+1];
          dizi[i+1]=yedek;
        }
    }
}
  rewind(magazas);
  printf ("%13s%14s%14s%13s%14s","Magaza ID","Magaza AD","Satis Adet","Ciro","Kar\n");
  printf ("_______________________________________________________________________\n");
  k=0;
  for (i=0;i<j;i++){
      fread (&magaza,sizeof(struct magazalar),1,magazas);
      while (!feof(magazas)){
          if (magaza.ciro==dizi[i] && magaza.magazaID!=-1 ){
              printf ("%9d%18s%9d%20lf%14lf\n",magaza.magazaID,magaza.magazaAd,magaza.satis,magaza.ciro,magaza.kar);
              k++;
            }
          fread (&magaza,sizeof(struct magazalar),1,magazas);
        }
      if (k==3)
        return;
      rewind(magazas);
    }
  fclose(magazas);
}

void urunTranferi(int id){
  int i=0,j=0,urunNumara,yedek;
  int dizi[50];
  FILE *stoks;
  FILE *gecici;
  gecici=fopen ("gecici.dat","w");
  stoks=fopen ("stok.dat","r+");
  struct stoklar stok={0,0,0,0};
  fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
  if (id==stok.magazaID){
      if (stok.stokAdet==0){
          printf ("%d inci magazada %d .urun bitmis\n",stok.magazaID,stok.urunNo,i++);
          urunNumara=stok.urunNo;
        }
    }
  while (!feof(stoks)){
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      if (id==stok.magazaID){
          if (stok.stokAdet==0){
              printf ("%d inci magazada %d .urun bitmis\n",stok.magazaID,stok.urunNo,i++);
              urunNumara=stok.urunNo;
            }
        }
     }
  if (i==0){
      printf ("Bu magazada hala urunler var!Stoklar boslamamis!\n");
      fclose(gecici);
      fclose(stoks);
      remove ("gecici.dat");
    }
  if (i!=0){
      i=0;
      rewind(stoks);
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      if (stok.urunNo==urunNumara){
          dizi[j++]=stok.stokAdet;
          i++;
        }
      while (!feof(stoks)){
          fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
          if (stok.urunNo==urunNumara){
              dizi[j++]=stok.stokAdet;
              i++;
            }
        }
      j=i;
      for (i=0;i<j-1;i++){
          if (dizi[i]<dizi[i+1]){
              yedek=dizi[i];
              dizi[i]=dizi[i+1];
              dizi[i+1]=yedek;
            }
        }
      rewind(stoks);
      printf ("En cok %d kadar var yarisi stogu tukenmis magazaya aktariliyor!\n",dizi[0]);
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      while (!feof(stoks)){
          if (stok.urunNo==urunNumara && stok.stokAdet==dizi[0])
              stok.stokAdet=dizi[0]/2;
          if (stok.urunNo==urunNumara && stok.stokAdet==0)
              stok.stokAdet=dizi[0]/2;
          fprintf (gecici,"%d %d %d %d\n",stok);
          fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
        }
      fprintf (gecici,"%d %d %d %d",stok);
      fclose(gecici);
      fclose(stoks);
      remove ("stok.dat");
      rename ("gecici.dat","stok.dat");
      printf ("Islem tamamlandi\n!");
    }
}

void enfazlaKar(){
  double dizi[50][3];
  double temp1,temp2,temp3;
  int i=0,j=0;
  int satis=0;
  char urunad[30];
  FILE *uruns;
  FILE *stoks;
  uruns=fopen ("urunler.dat","rb");
  stoks=fopen ("stok.dat","r");
  struct urunler urun={0,"","",0,0};
  struct stoklar stok={0,0,0,0};
  fread (&urun,sizeof(struct urunler),1,uruns);
  while (!feof(uruns)){
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      while (!feof(stoks)){
          if (stok.urunNo==urun.urunNo){
             satis+=stok.satilan;
            }
          fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
        }
      if (stok.urunNo==urun.urunNo)
         satis+=stok.satilan;
      dizi[i][j]=urun.urunNo;
      dizi[i][j+1]=satis;
      dizi[i][j+2]=satis*(urun.urunSatis-urun.urunAlis);
      fread (&urun,sizeof(struct urunler),1,uruns);
      rewind(stoks);
      i++;j=0;satis=0;
    }
  rewind(uruns);
  for (j=0;j<6;j++){
      for (i=0;i<10;i++){
          if (dizi[i][2]<dizi[i+1][2]){
              temp1=dizi[i][0];temp2=dizi[i][1];temp3=dizi[i][2];
              dizi[i][0]=dizi[i+1][0];dizi[i][1]=dizi[i+1][1];dizi[i][2]=dizi[i+1][2];
              dizi[i+1][0]=temp1;dizi[i+1][1]=temp2;dizi[i+1][2]=temp3;
            }
        }
    }
  printf ("\n%10s%13s%16s%16s","Urun ID","Urun AD","Toplam SATIS","Toplam KAR\n");
  printf ("_________________________________________________________________\n");
  for (i=0;i<10;i++){
     fread (&urun,sizeof(struct urunler),1,uruns);
     while (!feof(uruns)){
         if (urun.urunNo==(int)dizi[i][0]){
             for (j=0;j<strlen(urun.urunAd);j++)
                 urunad[j]=urun.urunAd[j];
             urunad[j]='\0';
             if ((int)dizi[i][0]!=0)
                 printf ("%7d%16s%11d%20lf \n",(int)dizi[i][0],urunad,(int)dizi[i][1],dizi[i][2]);
           }
         fread (&urun,sizeof(struct urunler),1,uruns);
       }
     rewind(uruns);
    }
  fclose(uruns);
  fclose(stoks);
}

sistemdenSil (){
  int silinen,sayac=0,i=0;
  magazaoku ();
  printf ("Silmek istediginiz magazanin id sini giriniz: ");
  scanf ("%d",&silinen);
  FILE *magazas;
  FILE *stoks;
  FILE *yedek;
  yedek=fopen("yedek.dat","w");
  stoks=fopen("stok.dat","r+");
  magazas=fopen ("magazalar.dat","rb+");
  struct magazalar magaza={0,"",0,0,0};
  struct stoklar stok={0,0,0,0};
  fread(&magaza,sizeof(struct magazalar),1,magazas);
  sayac++;
  while (!feof(magazas)){
      if (magaza.magazaID==silinen){
          fseek (magazas,(sayac-1)*sizeof(struct magazalar),SEEK_SET);
          magaza.magazaID=-1;magaza.magazaAd==' ';magaza.satis=0;magaza.ciro=0;magaza.kar=0;
          fwrite (&magaza,sizeof(struct magazalar),1,magazas);
          fseek(magazas,(sayac)*sizeof(struct magazalar),SEEK_SET);
          i++;
        }
     fread(&magaza,sizeof(struct magazalar),1,magazas);
     sayac++;
    }
  if (i==0){
      printf ("Magza bulunamadi!\n");
      fclose(magazas);
      fclose(yedek);
      fclose(stoks);
      remove("yedek.dat");
    }
  else{
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      while (!feof(stoks)){
          if (stok.magazaID!=silinen)
              fprintf (yedek,"%d %d %d %d\n",stok);
          fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
        }
      if (stok.magazaID!=silinen)
          fprintf (yedek,"%d %d %d %d",stok);
      fclose(stoks);
      fclose(yedek);
      remove("stok.dat");
      rename ("yedek.dat","stok.dat");
      fclose(magazas);
      magazaoku ();
    }
}

void stogaEkleme(int magID){
  int urunno,adet,i;
  FILE *stoks;
  FILE *yedek;
  struct stoklar stok={0,0,0,0};
  do{
      i=0;
      yedek=fopen("yedek.dat","w");
      stoks=fopen("stok.dat","r+");
      printf ("\nStoga eklemek istediginiz urunun urun numarasini giriniz(CIKIS icin 0 yaz): ");
      scanf("%d",&urunno);
      if (urunno==0)
          break;
      printf ("\nKac tane eklemek istersiniz: ");
      scanf ("%d",&adet);
      fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
      while (!feof(stoks)){
          if (stok.urunNo==urunno && stok.magazaID==magID){
              stok.stokAdet+=adet;
              i++;
            }
          fprintf (yedek,"%d %d %d %d\n",stok);
          fscanf (stoks,"%d%d%d%d",&stok.urunNo,&stok.magazaID,&stok.stokAdet,&stok.satilan);
        }
      if (stok.urunNo==urunno && stok.magazaID==magID){
          stok.stokAdet+=adet;
          i++;
        }
      fprintf (yedek,"%d %d %d %d",stok);
      fclose(yedek);
      fclose(stoks);
      if (i==0){
          printf ("\nUrun bulunamadi!");
          remove ("yedek.dat");
        }
      else {
          printf ("\nStoklar guncellendi!");
          remove("stok.dat");
          rename ("yedek.dat","stok.dat");
        }
    }while (urunno!=0);
  fclose (yedek);
  remove ("yedek.dat");
}
