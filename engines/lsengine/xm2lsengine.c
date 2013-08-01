/* Converter from XM to Lyndon Sharp's beeper music engine assembly
 * source by Shiru (shiru@mail.ru).  Originial sources (for
 * windows): shiru.untergrund.net/1bit/images/lsengine.zip
 *
 * Adapted for z80asm by Albert Veli.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


unsigned char *xm;


int read_word(int off)
{
   return xm[off]+(xm[off+1]<<8);
}


int read_dword(int off)
{
   return xm[off]+(xm[off+1]<<8)+(xm[off+2]<<16)+(xm[off+3]<<24);
}


void cleanup(void)
{
   free(xm);
}


int main(int argc, char* argv[])
{
   /* const float notes[12]={2093.0f,2217.4f,2349.2f,2489.0f,2637.0f,2793.8f,2960.0f,3136.0f,3322.4f,3520.0f,3729.2f,3951.0f}; */
   /* const float cputime=153.0f; */
   FILE *fp;
   int size;
   int order_len;
   int order_loop;
   int channels;
   int patterns;
   int tempo;
   int bpm;
   int i,j,k,pp,patlen;
   int tag,note,ins,vol,fx,param;
   int row[3][256];
   int speed;
   char name[1024];

   if(argc<2) {
      printf("xm2lsengine converter by Shiru (shiru@mail.ru) 03'11\n");
      printf("Usage: xm2lsengine filename.xm\n");
      exit(0);
   }

   fp=fopen(argv[1],"rb");

   if(!fp) {
      fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
      exit(-1);
   }

   fseek(fp,0,SEEK_END);
   size=ftell(fp);
   fseek(fp,0,SEEK_SET);

   xm=(unsigned char*)malloc(size);
   if (fread(xm,size,1,fp) != 1) {
      fprintf(stderr, "failed to read from %s\n", argv[1]);
      exit(EXIT_FAILURE);
   }
   fclose(fp);

   atexit(cleanup);

   if(memcmp(xm,"Extended Module: ",17)) {
      fprintf(stderr, "Error: Not XM module\n");
      exit(EXIT_FAILURE);
   }

   order_len=read_word(60+4);
   order_loop=read_word(60+6);
   channels=read_word(60+8);
   patterns=read_word(60+10);
   tempo=read_word(60+16);
   bpm=read_word(60+18);

   if(!order_len) {
      fprintf(stderr, "Error: Module should have at least one order position\n");
      exit(EXIT_FAILURE);
   }

   if(channels<2) {
      fprintf(stderr, "Error: Module should have at least two channels\n");
      exit(EXIT_FAILURE);
   }

   strcpy(name,argv[1]);
   name[strlen(argv[1])-2]='a';
   name[strlen(argv[1])-1]='s';
   name[strlen(argv[1])  ]='m';
   name[strlen(argv[1])+1]=0;

   fp=fopen(name,"wt");

   /* fprintf(fp,"module\n"); */

   for(j=0;j<order_len;j++) {
      if(j==order_loop) fprintf(fp,"music.loop:\n");
      fprintf(fp,"\tdw music.p%2.2x\n",xm[60+20+j]);
   }

   fprintf(fp,"\tdw 0\n\tdw music.loop\n");

   for(i=0;i<patterns;i++) {
      pp=60+20+256;
      for(j=0;j<i;j++) pp=pp+read_dword(pp)+read_word(pp+7);

      patlen=read_word(pp+5);
      pp+=read_dword(pp);

      for(j=0;j<patlen;j++) {
         for(k=0;k<channels;k++) {
            if(xm[pp]&0x80) {
               tag=xm[pp++];
               if(tag&0x01) note =xm[pp++]; else note =0;
               if(tag&0x02) ins  =xm[pp++]; else ins  =0;
               if(tag&0x04) vol  =xm[pp++]; else vol  =0;
               if(tag&0x08) fx   =xm[pp++]; else fx   =0;
               if(tag&0x10) param=xm[pp++]; else param=0;

            } else {

               note =xm[pp++];
               ins  =xm[pp++];
               vol  =xm[pp++];
               fx   =xm[pp++];
               param=xm[pp++];
            }
            if(fx==0x0f) {
               if(param<0x20) tempo=param; else bpm=param;
            }
         }
      }

      speed=tempo+1;//very rough conversion

      if(speed<1||speed>255) {
         printf("Warning: Tempo or BPM is out of range (ptn:%2.2x row:%2.2x chn:%i)\n",i,j,k);
      }

      fprintf(fp,"music.p%2.2x:\n",i);
      fprintf(fp,"\tdb $%2.2x,$%2.2x\n",patlen,speed);

      pp=60+20+256;
      for(j=0;j<i;j++) pp=pp+read_dword(pp)+read_word(pp+7);

      patlen=read_word(pp+5);
      pp+=read_dword(pp);

      for(j=0;j<patlen;j++) {
         row[0][j]=-1;
         row[1][j]=-1;
         row[2][j]=0;
      }

      for(j=0;j<patlen;j++) {
         for(k=0;k<channels;k++) {
            if(xm[pp]&0x80) {
               tag=xm[pp++];
               if(tag&0x01) note =xm[pp++]; else note =0;
               if(tag&0x02) ins  =xm[pp++]; else ins  =0;
               if(tag&0x04) vol  =xm[pp++]; else vol  =0;
               if(tag&0x08) fx   =xm[pp++]; else fx   =0;
               if(tag&0x10) param=xm[pp++]; else param=0;
            } else {
               note =xm[pp++];
               ins  =xm[pp++];
               vol  =xm[pp++];
               fx   =xm[pp++];
               param=xm[pp++];
            }
            if(k<2) {
               if(note>0&&note<97&&ins==1) {
                  if(note>=24&&note<24+47) { //24=B-1
                     row[k][j]=note-24+1;
                  } else {
                     printf("Warning: note out of range (ptn:%2.2x row:%2.2x chn:%i)\n",i,j,k);
                  }
               }
               if(note==97) {
                  row[k][j]=0;
               }
            }
            if(ins==2) {
               switch(note) {
               case 49: row[2][j]=1; break;//C-4
               case 51: row[2][j]=2; break;//D-4
               case 53: row[2][j]=3; break;//E-4
               case 54: row[2][j]=4; break;//F-4
               case 56: row[2][j]=5; break;//G-4
               case 58: row[2][j]=6; break;//A-4
               case 60: row[2][j]=7; break;//B-4
               }
            }
         }
      }

      for(j=0;j<patlen;j++) {
         if(row[0][j]<0) {
            if(!j) row[0][j]=0; else row[0][j]=row[0][j-1]&0x3f;
         }
         if(row[1][j]<0) {
            if(!j) row[1][j]=0; else row[1][j]=row[1][j-1]&0x3f;
         }

         row[1][j]|=(row[2][j]&3)<<6;
         row[0][j]|=((row[2][j]>>2)&1)<<7;
      }

      for(j=0;j<patlen;j++) {
         if(!(j&15)) fprintf(fp,"\tdb ");
         fprintf(fp,"$%2.2x%c",row[0][j],(j&15)<15?',':'\n');
      }
      for(j=0;j<patlen;j++) {
         if(!(j&15)) fprintf(fp,"\tdb ");
         fprintf(fp,"$%2.2x%c",row[1][j],(j&15)<15?',':'\n');
      }
   }

   fclose(fp);

   exit(EXIT_SUCCESS);
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
