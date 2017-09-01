//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
const float RADIAN=0.01744444444;

TForm1 *Form1;
Graphics::TBitmap*ZadniyBuffer;
float ShipPosX,ShipPosY;
float ShipAngle;
float ShipSpeed;
bool LEFT,RIGHT,UP;
TList*Bullets;
TList*Asteroids;
class Bullet
{
public:
float x,y,dx,dy;
void CreateBullet()
        {
        x=ShipPosX;y=ShipPosY;
        dx=10*cos(RADIAN*ShipAngle);
        dy=10*sin(RADIAN*ShipAngle);
        }
void Draw()
        {
        ZadniyBuffer->Canvas->Pen->Color=clRed;
        ZadniyBuffer->Canvas->MoveTo(x,y);
        ZadniyBuffer->Canvas->LineTo(x+dx,y+dy);
        }
};

class CRock
{
public:
float x,y;
int Figure[50];
float Angle;
float Spin;
float XSpeed,YSpeed;
int Tip;
void CreateAsteroid(int t)
        {
        Tip=t;

        XSpeed=random(4)+(random(9)/10.0)-2;
        YSpeed=random(4)+(random(9)/10.0)-2;


        Spin=random(10)-5;
        for(int i=0;i<20;i++)
                {
                if(Tip==1)Figure[i]=random(10)+25;
                if(Tip==2)Figure[i]=random(10)+10;
                if(Tip==3)Figure[i]=random(5)+5;
                }
        }
void Move()
        {
        x+=XSpeed;y+=YSpeed;
        Angle+=Spin;
        if(x>440)x=-40;if(x<-40)x=440;
        if(y>440)y=-40;if(y<-40)y=440;
        }
void Draw()
        {
         ZadniyBuffer->Canvas->Pen->Color=clYellow;
        float tAng=0;
                int tx=cos(RADIAN*(Angle+tAng))*Figure[0]+x;
                int ty=sin(RADIAN*(Angle+tAng))*Figure[0]+y;
                ZadniyBuffer->Canvas->MoveTo(tx,ty);
        for(int i=0;i<20;i++,tAng+=19)
                {
                int tx=cos(RADIAN*(Angle+tAng))*Figure[i]+x;
                int ty=sin(RADIAN*(Angle+tAng))*Figure[i]+y;
                ZadniyBuffer->Canvas->LineTo(tx,ty);
                }
        }
};

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
randomize();
ZadniyBuffer=new Graphics::TBitmap();
ZadniyBuffer->Width =400;
ZadniyBuffer->Height=400;
ZadniyBuffer->Canvas->Pen->Color=clWhite;
Bullets  =new TList();
Asteroids=new TList();
LEFT=false;RIGHT=false;
ShipPosX=200;ShipPosY=200;
ShipAngle=0;
ShipSpeed=0;
for(int i=0;i<4;i++)
{
CRock*a=new CRock();
a->x=random(400);
a->y=40;
a->CreateAsteroid(1);
Asteroids->Add((void*)a);
}
}
//---------------------------------------------------------------------------
void TForm1::CLS()
{
        ZadniyBuffer->Canvas->Brush->Color=clBlack;
        ZadniyBuffer->Canvas->FillRect(Rect(0,0,400,400));
}

void __fastcall TForm1::CopyToScreen()
{
        Canvas->Draw(0,0,ZadniyBuffer);
}


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
if(LEFT)ShipAngle-=3;
if(RIGHT)ShipAngle+=3;
MoveShip();
CLS();
DrawShip();
MoveAndDrawBullets();
MoveAndDrawAsteroids();
ZadniyBuffer->Canvas->Font->Color=clWhite;
ZadniyBuffer->Canvas->TextOutA(5,5,"Астероидов : "+IntToStr(Asteroids->Count));
CopyToScreen();



if(IsShipBreaked())
{
Timer1->Enabled=false;
ShowMessage("Корабль взорван");
StartNew();
Timer1->Enabled=true;
}

if(Asteroids->Count==0)
{
Timer1->Enabled=false;
ShowMessage("Уровень пройден");
StartNew();
Timer1->Enabled=true;
}
}
//---------------------------------------------------------------------------




void __fastcall TForm1::DrawShip()
{
       ZadniyBuffer->Canvas->Pen->Color=clWhite;
       int tA=0;
       int tx=cos(RADIAN*(ShipAngle+tA))*10+ShipPosX;
       int ty=sin(RADIAN*(ShipAngle+tA))*10+ShipPosY;
       ZadniyBuffer->Canvas->MoveTo(tx,ty);
       int l;
       for(int i=0;i<4;i++,tA+=120)
                {
                if((i==0)||(i==3))l=15;else l=10;
                int tx=cos(RADIAN*(ShipAngle+tA))*l+ShipPosX;
                int ty=sin(RADIAN*(ShipAngle+tA))*l+ShipPosY;
                ZadniyBuffer->Canvas->LineTo(tx,ty);
                }
}


void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==VK_LEFT) LEFT=true;
if(Key==VK_RIGHT)RIGHT=true;
if(Key==VK_UP)UP=true;
if(Key==VK_SPACE)
        {
        Bullet*b=new Bullet();
        b->CreateBullet();
        Bullets->Add((void*)b);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==VK_LEFT) LEFT=false;
if(Key==VK_RIGHT)RIGHT=false;
if(Key==VK_UP)UP=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::MoveShip()
{
        if(UP)
        {
        ShipSpeed+=0.01;
        if(ShipSpeed>2)ShipSpeed=4;
        }
        else
        {
        ShipSpeed-=0.01;
        if(ShipSpeed<0)ShipSpeed=0;
        }
        ShipPosX+=cos(RADIAN*ShipAngle)*ShipSpeed;
        ShipPosY+=sin(RADIAN*ShipAngle)*ShipSpeed;
        if(ShipPosX>410)ShipPosX=-10;
        if(ShipPosX<-10)ShipPosX=410;
        if(ShipPosY>408)ShipPosY=-10;
        if(ShipPosY<-10)ShipPosY=410;
}

void __fastcall TForm1::MoveAndDrawBullets()
{

        Bullet*b;
        for(int i=0;i<Bullets->Count;i++)
                {
                b=(Bullet*)Bullets->Items[i];
                b->x += b->dx;
                b->y += b->dy;
                if((b->x>410)||(b->x<-10)||(b->y>410)||(b->y<-10))
                     {
                     delete b;
                     Bullets->Delete(i);
                     b=NULL;
                     }

                if(b!=NULL)b->Draw();
                }
}

TForm1::MoveAndDrawAsteroids()
{
        CRock*r,*r2;
        Bullet*b;
        for(int i=0;i<Asteroids->Count;i++)
        {
        r=(CRock*)Asteroids->Items[i];
               if(r!=NULL)
                for(int j=0;j<Bullets->Count;j++)
                {
                b=(Bullet*)Bullets->Items[j];
                int distance=sqrt( (r->x-b->x)*(r->x-b->x) + (r->y-b->y)*(r->y-b->y));
                if(((r->Tip==1)&&(distance<30))||((r->Tip==2)&&(distance<15))||((r->Tip==3)&&(distance<5)))
                    {
                    Asteroids->Delete(i);
                    Bullets->Delete(j);

                    if(r->Tip<3)
                    {
                    r2=new CRock();
                    r2->x=r->x;
                    r2->y=r->y;
                    r2->CreateAsteroid(r->Tip+1);
                    Asteroids->Add((void*)r2);

                    r2=new CRock();
                    r2->x=r->x;
                    r2->y=r->y;
                    r2->CreateAsteroid(r->Tip+1);
                    Asteroids->Add((void*)r2);
                    delete r;
                    r=NULL;
                    }
                    }
                }
        if(r!=NULL)
        {
        r->Move();
        r->Draw();
        }
        }
return 0;
}

bool __fastcall TForm1::IsShipBreaked()
{
      CRock*r,*r2;
      for(int i=0;i<Asteroids->Count;i++)
        {
        r=(CRock*)Asteroids->Items[i];
        int distance=sqrt((r->x-ShipPosX)*(r->x-ShipPosX)+(r->y-ShipPosY)*(r->y-ShipPosY));
        if(((r->Tip==1)&&(distance<30))||((r->Tip==2)&&(distance<25))||((r->Tip==3)&&(distance<15)))
        return true;
        }
return false;
}

void __fastcall TForm1::StartNew()
{

LEFT=false;RIGHT=false;
ShipPosX=200;ShipPosY=200;
ShipAngle=0;
ShipSpeed=0;
Asteroids->Clear();
Bullets->Clear();
for(int i=0;i<4;i++)
{
CRock*a=new CRock();
a->x=random(400);
a->y=40;
a->CreateAsteroid(1);
Asteroids->Add((void*)a);
}
}
