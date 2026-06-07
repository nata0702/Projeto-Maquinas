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
    float cimaX = robo.x + robo.width/2;
    float cimaY = robo.y;

    float direitaX = robo.x + robo.width;
    float direitaY = robo.y + robo.height/2;

    float baixoX = robo.x + robo.width/2;
    float baixoY = robo.y + robo.height;

    float esquerdaX = robo.x;
    float esquerdaY = robo.y + robo.height/2;

    if(TaDentro(cimaX, cimaY))
        return 1;

    if(TaDentro(direitaX, direitaY))
        return 2;

    if(TaDentro(baixoX, baixoY))
        return 3;

    if(TaDentro(esquerdaX, esquerdaY))
        return 4;

    return 0;
};


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
            if(MatrizMapa[y][x]==2)
            {
                DrawRectangle(
                x * tamanhoCelula,
                y * tamanhoCelula,
                tamanhoCelula,
                tamanhoCelula,
                GREEN
                );
            };
        }
    }
};


void DefinirMatriz()
{
    
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

    
    for(int i = 0; i < NUM_COMODOS-1; i++)
    {
        int x1 = centroX[i];
        int y1 = centroY[i];

        int x2 = centroX[i+1];
        int y2 = centroY[i+1];

        
        int inicio = (x1 < x2) ? x1 : x2;
        int fim    = (x1 < x2) ? x2 : x1;

        for(int x = inicio; x <= fim; x++)
        {
            MatrizMapa[y1][x] = 0;
        }

       
        inicio = (y1 < y2) ? y1 : y2;
        fim    = (y1 < y2) ? y2 : y1;

        for(int y = inicio; y <= fim; y++)
        {
            MatrizMapa[y][x2] = 0;
        }
    }
};
 

void DestacarMouse(Camera2D camera)
{
    static int ultimaLinha = -1;
    static int ultimaColuna = -1;

    // volta a célula antiga para 0
    if(ultimaLinha != -1 &&
       MatrizMapa[ultimaLinha][ultimaColuna] == 2)
    {
        MatrizMapa[ultimaLinha][ultimaColuna] = 0;
    }

    Vector2 mouseMundo = GetScreenToWorld2D(GetMousePosition(), camera);

    int coluna = mouseMundo.x / tamanhoCelula;
    int linha  = mouseMundo.y / tamanhoCelula;

    if(linha >= 0 && linha < LINHAS &&
       coluna >= 0 && coluna < COLUNAS)
    {
        if(MatrizMapa[linha][coluna] == 0)
        {
            MatrizMapa[linha][coluna] = 2;

            ultimaLinha = linha;
            ultimaColuna = coluna;
        }
        else
        {
            ultimaLinha = -1;
            ultimaColuna = -1;
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
        dt = GetFrameTime();

        float dx = 0;
        float dy = 0;

        if(IsKeyDown(KEY_D)) dx += velocidade * dt;
        if(IsKeyDown(KEY_A)) dx -= velocidade * dt;
        if(IsKeyDown(KEY_S)) dy += velocidade * dt;
        if(IsKeyDown(KEY_W)) dy -= velocidade * dt;

        // Movimento em X
        pos.x += dx;
        Corpo.x = pos.x;

        if(RoboDentro(Corpo) != 0)
        {
            pos.x -= dx;
            Corpo.x = pos.x;
        }

        // Movimento em Y
        pos.y += dy;
        Corpo.y = pos.y;

        if(RoboDentro(Corpo) != 0)
        {
            pos.y -= dy;
            Corpo.y = pos.y;
        }

        Corpo.x = pos.x;
        Corpo.y = pos.y;
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
    RoboDeCarga.velocidade=300;
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
        


  
    camera.target.x=RoboDeCarga.pos.x;
    camera.target.y=RoboDeCarga.pos.y;
   
    
    //loop do jogo
    while(!WindowShouldClose()){
        float velocidadeCamera = 300.0f;
        float velocidadeZoom = 3.0f;
        float dt = GetFrameTime();
        float tempo =+ dt;


        
        
        if(IsKeyDown(KEY_UP) && camera.zoom<3){
            camera.zoom+=velocidadeZoom*dt;
        }
        if(IsKeyDown(KEY_DOWN) && camera.zoom>0.3){
            camera.zoom-=velocidadeZoom*dt;
        }



        if(IsKeyDown(KEY_SPACE)){
            estado=1;
        }





        if(estado==0){
            //funcao que recebe a posicao do mouse, o vetor mapa ja definido, 
            DestacarMouse(camera);
            if(IsKeyDown(KEY_W)){
                camera.target.y-=velocidadeCamera*dt;
            }
            if(IsKeyDown(KEY_A)){
                camera.target.x-=velocidadeCamera*dt;
            }
            if(IsKeyDown(KEY_S)){
                camera.target.y+=velocidadeCamera*dt;
            }
            if(IsKeyDown(KEY_D)){
                camera.target.x+=velocidadeCamera*dt;
            }

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
            camera.target.x=RoboDeCarga.pos.x;
            camera.target.y=RoboDeCarga.pos.y;
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