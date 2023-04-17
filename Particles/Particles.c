#include <raylib.h>
#include <time.h>
#include <math.h>

#define WIDTH 1280
#define HEIGHT 720
#define FRICTION 0.5

typedef struct{
    Vector2 position, velocity;
    int type;
}Particle;


Particle particles[1000];

void rule(int i, int target, int target2, float attraction){
    for(int l = 0; l < 1000; l++){
        if(particles[i].type == target && particles[l].type == target2){
            float dx = particles[i].position.x - particles[l].position.x;
            float dy = particles[i].position.y - particles[l].position.y;
            float d = sqrt(dx * dx + dy * dy);
            if(d > 0 && d < 80){
                float f = attraction * 1/d;
                particles[i].velocity.x += f * dx;
                particles[i].velocity.y += f * dy;
            }
        }
    }
}

int main(){
    srand(time(NULL));
    
    bool spaceDown = false;
    
    int currentRule = 0;
    float values[16]; /* Holds the rules force */
    for(int i = 0; i < 16; i++) values[i] = 0;
    
    Color colors[4];
    colors[0] =   (Color){255,0,0,255};
    colors[1] =   (Color){0,255,0,255};
    colors[2] =   (Color){0,0,255,255};
    colors[3] = (Color){255,0,255,255};
    
    /* Randomize Position */
    for(int i = 1000; i > 0; i--){
        particles[i - 1].position = (Vector2){rand() % WIDTH, rand() % HEIGHT};
        particles[i - 1].type = rand() % 4;
    }
    
    SetTargetFPS(240);
    InitWindow(WIDTH, HEIGHT, "Particles");
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        /* Add Forces */
        for(int i = 0; i < 1000; i++){
            /* Rules */
            rule(i, 0, 0, values[0]);
            rule(i, 0, 1, values[1]);
            rule(i, 0, 2, values[2]);
            rule(i, 0, 3, values[3]);
            
            rule(i, 1, 0, values[4]);
            rule(i, 1, 1, values[5]);
            rule(i, 1, 2, values[6]);
            rule(i, 1, 3, values[7]);
            
            rule(i, 2, 0, values[8]);
            rule(i, 2, 1, values[9]);
            rule(i, 2, 2, values[10]);
            rule(i, 2, 3, values[11]);
            
            rule(i, 3, 0, values[12]);
            rule(i, 3, 1, values[13]);
            rule(i, 3, 2, values[14]);
            rule(i, 3, 3, values[15]);
            
            /* Move */
            particles[i].velocity.x *= FRICTION;
            particles[i].velocity.y *= FRICTION;
            particles[i].position.x += particles[i].velocity.x;
            particles[i].position.y += particles[i].velocity.y;
            
            /* Clamp Pos */
            if(particles[i].position.x <= 0 || particles[i].position.x >= WIDTH) particles[i].velocity.x *= -1;
            if(particles[i].position.y <= 0 || particles[i].position.y >= HEIGHT) particles[i].velocity.y *= -1;
        }
        
        /* Change Selected Rule */
        if(IsKeyDown(KEY_SPACE) && spaceDown == false){
            currentRule++;
            spaceDown = true;
        }
        if(!IsKeyDown(KEY_SPACE)) spaceDown = false;
        
        if(currentRule > 15) currentRule = 0;
        
        if(IsKeyDown(KEY_RIGHT)) values[currentRule] += 0.01;
        if(IsKeyDown(KEY_LEFT)) values[currentRule] -= 0.01;
        if(values[currentRule] > 4) values[currentRule] = 4;
        if(values[currentRule] < -4) values[currentRule] = -4;
        
        
        /* Draw Particles */
        for(int i = 1000; i > 0; i--) DrawCircle(particles[i].position.x, particles[i].position.y, 5, colors[particles[i].type]);
        
        /* Show Rules */
        int t = 0;
        DrawRectangle(0,0,105, 330, (Color){0,0,0,220});
        for(int i = 0; i < 16; i++){
            if(currentRule == i){
                DrawText(TextFormat(": %i", (int)round(values[i] * 100)), 65, i * 20, 20, WHITE);
                DrawText("o", 10, i * 20, 20, colors[(int)round((i / 4) + 0.5) - 1]);
                DrawText("-", 25, i * 20, 20, WHITE);
                DrawText("o", 40, i * 20, 20, colors[t]);
            }else{
                DrawText(TextFormat(": %i", (int)round(values[i] * 100)), 65, i * 20, 20, GRAY);
                DrawText("o", 10, i * 20, 20, colors[(int)round((i / 4) + 0.5) - 1]);
                DrawText("-", 25, i * 20, 20, GRAY);
                DrawText("o", 40, i * 20, 20, colors[t]);
                
            }
            t++;
            if(t > 3){
                t = 0;
            }
        }
        
        if(IsKeyDown(KEY_R)) for(int i = 0; i < 16; i++) values[i] = (float)((rand() % (400 + 1 - -400)) + -400) / 100;
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}