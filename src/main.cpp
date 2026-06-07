
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
using namespace std;


//variaveis globais
const int larguraMapa = 5000;
const int alturaMapa = 5000;
const int tamanhoCelula = 50;
const int LINHAS = alturaMapa/tamanhoCelula;
const int COLUNAS = larguraMapa/tamanhoCelula;

int MatrizMapa[LINHAS][COLUNAS];

//funcoes
int TaDentro(float x, float y)
{
    int coluna = x / tamanhoCelula;
    int linha = y / tamanhoCelula;

    if(linha < 0 || linha >= LINHAS ||
       coluna < 0 || coluna >= COLUNAS)
    {
        return 0;
    }

    if(MatrizMapa[linha][coluna] == 1)
    {
        return 1;
    }

    return 0;
}


int RoboDentro(Rectangle robo)
{
    // centro de cada lado
    float cimaX = robo.x + robo.width;
    float cimaY = robo.y;

    float direitaX = robo.x + robo.width;
    float direitaY = robo.y + robo.height;

    float baixoX = robo.x +robo.width;
    float baixoY = robo.y + robo.height;

    float esquerdaX = robo.x;
    float esquerdaY = robo.y + robo.height;

    if(TaDentro(cimaX, cimaY))
        return 1;

    if(TaDentro(direitaX, direitaY))
        return 2;

    if(TaDentro(baixoX, baixoY))
        return 3;

    if(TaDentro(esquerdaX, esquerdaY))
        return 4;

    return 0;
}


void DesenharParedes()
{
    for(int y = 0; y < LINHAS; y++)
    {
        for(int x = 0; x < COLUNAS; x++)
        {
            if(MatrizMapa[y][x] == 1)
            {
                DrawRectangle(
                    x * tamanhoCelula,
                    y * tamanhoCelula,
                    tamanhoCelula,
                    tamanhoCelula,
                    WHITE
                );
            }
        }
    }
}



void DefinirMatriz()
{
    // tudo parede
    for(int y = 0; y < LINHAS; y++)
    {
        for(int x = 0; x < COLUNAS; x++)
        {
            MatrizMapa[y][x] = 1;
        }
    }

    const int NUM_COMODOS = 15;

    int centroX[NUM_COMODOS];
    int centroY[NUM_COMODOS];

    // gera cômodos
    for(int i = 0; i < NUM_COMODOS; i++)
    {
        int largura = GetRandomValue(5, 15);
        int altura  = GetRandomValue(5, 15);

        int x = GetRandomValue(1, COLUNAS - largura - 2);
        int y = GetRandomValue(1, LINHAS - altura - 2);

        for(int yy = y; yy < y + altura; yy++)
        {
            for(int xx = x; xx < x + largura; xx++)
            {
                MatrizMapa[yy][xx] = 0;
            }
        }

        centroX[i] = x + largura/2;
        centroY[i] = y + altura/2;
    }

    // liga os cômodos
    for(int i = 0; i < NUM_COMODOS-1; i++)
    {
        int x1 = centroX[i];
        int y1 = centroY[i];

        int x2 = centroX[i+1];
        int y2 = centroY[i+1];

        // corredor horizontal
        int inicio = (x1 < x2) ? x1 : x2;
        int fim    = (x1 < x2) ? x2 : x1;

        for(int x = inicio; x <= fim; x++)
        {
            MatrizMapa[y1][x] = 0;
        }

        // corredor vertical
        inicio = (y1 < y2) ? y1 : y2;
        fim    = (y1 < y2) ? y2 : y1;

        for(int y = inicio; y <= fim; y++)
        {
            MatrizMapa[y][x2] = 0;
        }
    }
}
     
     


void DesenharGrades()
{
    for(int x = 0; x <= larguraMapa; x += tamanhoCelula)
        DrawLine(x, 0, x, alturaMapa, LIGHTGRAY);

    for(int y = 0; y <= alturaMapa; y += tamanhoCelula)
        DrawLine(0, y, larguraMapa, y, LIGHTGRAY);
};

//classes
class Robo{
    public:
    Rectangle Corpo;
    Vector2 pos;
    Vector2 dir;
    float angulo;
    float dt;
    int velocidade;
    float altura;
    float largura;


    void desenhar(){
        DrawRectangleRec(Corpo,RED);
    }


    void update(){
    
        if(RoboDentro(Corpo)==0){
                dt = GetFrameTime();
            if(pos.x<5000-largura && pos.x>0){
                pos.x+=velocidade*dir.x*dt;
            }
            else{
                pos.x-=velocidade*dir.x*dt;
                dir.x*=-1;
            }
            
            if(pos.y>0 && pos.y<5000-largura){
                pos.y+=velocidade*dir.y*dt;
            }
            else{
                pos.y-=velocidade*dir.y*dt;
                dir.y*=-1;
            }
        }
        else if(RoboDentro(Corpo)==1){
            pos.y-=velocidade*dt*dir.y;
            dir.y*=-1;
        }
         else if(RoboDentro(Corpo)==2){
            pos.x-=velocidade*dt*dir.x;
            dir.x*=-1;
        }
         else if(RoboDentro(Corpo)==3){
            pos.y-=velocidade*dt*dir.y;
            dir.y*=-1;
        }
        else if(RoboDentro(Corpo)==4){
            pos.x-=velocidade*dt*dir.x;
            dir.x*=-1;
        }

        printf("\n dir.y = %f\n dir.x = %f", dir.y, dir.x);
        
        
        Corpo.x=pos.x;
        Corpo.y=pos.y;
    }

};



int main(void)
{
    int estado = 0;
    SetRandomSeed(time(NULL));
    //mapa
    DefinirMatriz();


    //janela
    int larguraJanela=1280;
    int alturaJanela=800;
    InitWindow(larguraJanela,alturaJanela,"SEILAPORRA");
    SetTargetFPS(60);

    

    //variaveis do robo
    Robo RoboDeCarga;
    RoboDeCarga.altura=20.0f;
    RoboDeCarga.largura=20.0f;
    RoboDeCarga.angulo= GetRandomValue(0, 10000) / 10000.0f * 2 * PI;
    RoboDeCarga.velocidade=500;
    RoboDeCarga.dir={cos(RoboDeCarga.angulo),-sin(RoboDeCarga.angulo)};

    do{
        RoboDeCarga.pos=
        {
        (float)GetRandomValue(0, 5000 - RoboDeCarga.largura),
        (float)GetRandomValue(0, 5000 - RoboDeCarga.altura)
        };
        RoboDeCarga.Corpo={RoboDeCarga.pos.x,  RoboDeCarga.pos.y, RoboDeCarga.largura  ,RoboDeCarga.altura};
    }while(RoboDentro(RoboDeCarga.Corpo)!=0);        

    
    
    //camera

    Camera2D camera = {0};

    camera.target = RoboDeCarga.pos;

    camera.offset =
    {
        larguraJanela/2.0f,
        alturaJanela/2.0f
    };

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
        


  

   
    
    //loop do jogo
    while(!WindowShouldClose()){
    float velocidadeCamera = 500.0f;
    float dt = GetFrameTime();
    float tempo =+ dt;




    if(IsKeyDown(KEY_SPACE)){
        camera.target.x=RoboDeCarga.pos.x;
        camera.target.y=RoboDeCarga.pos.y;
        estado=1;
    }


    if(IsKeyDown(KEY_D))
        camera.target.x += velocidadeCamera * dt;

    if(IsKeyDown(KEY_A))
        camera.target.x -= velocidadeCamera * dt;

    if(IsKeyDown(KEY_W))
        camera.target.y -= velocidadeCamera * dt;

    if(IsKeyDown(KEY_S))
        camera.target.y += velocidadeCamera * dt;
    
    if(IsKeyDown(KEY_UP) && camera.zoom<2)
        camera.zoom += 1.0f*dt;

    if(IsKeyDown(KEY_DOWN) && camera.zoom>0.2)
        camera.zoom -= 1.0f*dt;

    


        if(estado==0){
            BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(camera);

            DesenharGrades();
            DesenharParedes();
            RoboDeCarga.desenhar();

            EndMode2D();
            DrawText("CLIQUE ESPAÇO PARA DEFINIR O DESTINO", larguraJanela/7, alturaJanela/20, 40, RED);
            EndDrawing();


        }
        
        if(estado==1){
            RoboDeCarga.update();
            BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(camera);

            DesenharGrades();
            DesenharParedes();
            RoboDeCarga.desenhar();

            EndMode2D();
            
            EndDrawing();

        }

    

    }
    
    
    
    return 0;
}