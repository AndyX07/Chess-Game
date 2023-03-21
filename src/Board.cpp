#include "SDL.h"
#include "Board.h"
#include "bits/stdc++.h"

using namespace std;

Board::Board()
{
    memset(g, 0, sizeof(g));
    window = SDL_CreateWindow( "CHESS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(window, -1, 0);
    brown = {168, 117, 50};
    white = {255, 255, 255, 255};
    green = {32, 252, 3, 255};
    SDL_RenderClear(renderer);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            pieces[i][j] = 'a';
        }
    }
    pieces[0][0]='r';
    pieces[1][0]='n';
    pieces[2][0]='b';
    pieces[3][0]='q';
    pieces[4][0]='k';
    pieces[5][0]='b';
    pieces[6][0]='n';
    pieces[7][0]='r';
    for(int i = 0; i < 8; i++){
        pieces[i][1]='p';
        pieces[i][6]='P';
    }
    pieces[0][7]='R';
    pieces[1][7]='N';
    pieces[2][7]='B';
    pieces[3][7]='Q';
    pieces[4][7]='K';
    pieces[5][7]='B';
    pieces[6][7]='N';
    pieces[7][7]='R';

}

void Board::drawRect(SDL_Color c, int x1, int y1, int w, int h){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_Rect rect;
    rect.x=x1;
    rect.y=y1;
    rect.w = w;
    rect.h = h;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void Board::placePiece( SDL_Surface *img, int x, int y, int w, int h){
    SDL_SetRenderTarget(renderer, NULL);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

void Board::resetG(){
    memset(g, 0, sizeof(g));
}

void Board::generateBoard(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(g[i][j]) drawRect(green, i*75, j*75, 75, 75);
            else if((i+j)%2==0){
                drawRect(white, i*75, j*75, 75, 75);
            }
            else{
                drawRect(brown, i*75, j*75, 75, 75);
            }
        }
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            char c = pieces[i][j];
            if(c!='a'){
                SDL_Surface *img;
                if(islower(c)){
                    if(c=='p'){
                        img = SDL_LoadBMP("bpawn.bmp");
                    }
                    else if(c=='r'){
                        img = SDL_LoadBMP("brook.bmp");
                    }
                    else if(c=='n'){
                        img = SDL_LoadBMP("bknight.bmp");
                    }
                    else if(c=='b'){
                        img = SDL_LoadBMP("bbishop.bmp");
                    }
                    else if(c=='q'){
                        img = SDL_LoadBMP("bqueen.bmp");
                    }
                    else if(c=='k'){
                        img = SDL_LoadBMP("bking.bmp");
                    }
                }
                else{
                    c=tolower(c);
                    if(c=='p'){
                        img = SDL_LoadBMP("wpawn.bmp");
                    }
                    else if(c=='r'){
                        img = SDL_LoadBMP("wrook.bmp");
                    }
                    else if(c=='n'){
                        img = SDL_LoadBMP("wknight.bmp");
                    }
                    else if(c=='b'){
                        img = SDL_LoadBMP("wbishop.bmp");
                    }
                    else if(c=='q'){
                        img = SDL_LoadBMP("wqueen.bmp");
                    }
                    else if(c=='k'){
                        img = SDL_LoadBMP("wking.bmp");
                    }
                }
                placePiece(img, i*75+5, j*75+5, 60, 60);
            }
        }
    }
}

void Board::makeMove(int x, int y, int x1, int y1){
    if(pieces[x1][y1]=='a'){
        swap(pieces[x][y], pieces[x1][y1]);
    }
    else{
        pieces[x1][y1]='a';
        swap(pieces[x][y], pieces[x1][y1]);
    }
}

void Board::mouse(SDL_MouseButtonEvent e){
        bool ismove = 0;
        int x = e.x/75;
        int y = e.y/75;
        if(pieces[x][y]!='a'){
            if(getColor(x, y)==turn){
                for(int i = 0; i < 8; i++){
                    for(int j = 0; j < 8; j++){
                        if(possible(x, y, i, j, pieces[x][y], 1)){
                            g[i][j]=1;
                            ismove=1;
                        }
                    }
                }
                if(!ismove) return;
                generateBoard();
                SDL_Event e;
                bool quit = 0;
                while(!quit){
                    while(SDL_PollEvent(&e)!=0){
                        if(e.type==SDL_MOUSEBUTTONUP){
                            //cout << "!" << endl;
                            int x1 = e.button.x/75;
                            int y1 = e.button.y/75;
                            if(g[x1][y1]){
                                makeMove(x, y, x1, y1);
                                resetG();
                                if(y1==0&&pieces[x1][y1]=='P')pieces[x1][y1]='Q';
                                if(y1==7&&pieces[x1][y1]=='p')pieces[x1][y1]='q';
                                generateBoard();
                                quit = 1;
                                turn = !turn;
                                break;
                            }
                        }
                    }
                }
            }
        }
}

bool Board::pawnPossible(int x, int y, int x1, int y1, bool b){
    if(x1>=0&&y1>=0&&x1<8&&y1<8&&(x!=x1||y!=y1)){
        char c = pieces[x1][y1];
        if(!b){
            //black
            if(isupper(c)&&y1-y==1&&abs(x1-x)==1){
                return 1;
            }
            if(y==1){
                if(x1==x){
                    if(y1-y==2&&c=='a'&&pieces[x][y1-1]=='a'){
                        return 1;
                    }
                }
            }
            if(x1==x&&y1-y==1&&c=='a'){
                return 1;
            }
        }
        else{
            //white
            if(islower(c)&&c!='a'&&y-y1==1&&abs(x1-x)==1){
                return 1;
            }
            if(y==6){
                if(x1==x){
                    if(y-y1==2&&c=='a'&&pieces[x][y-1]=='a'){
                        return 1;
                    }
                }
            }
            if(x1==x&&y-y1==1&&c=='a'){
                return 1;
            }
        }
    }
    return 0;
}

bool Board::rookPossible(int x, int y, int x1, int y1, bool b){
    if(x1>=0&&y1>=0&&x1<8&&y1<8&&(x!=x1||y!=y1)){
        char c = pieces[x1][y1];
        if(x==x1){
            if(c=='a'||(b==0&&isupper(c))||(b==1&&islower(c))){
                if(y1>y){
                    for(int i = y+1; i<y1; i++){
                        if(pieces[x][i]!='a') return 0;
                    }
                    return 1;
                }
                else{
                    for(int i = y-1; i>y1; i--){
                        if(pieces[x][i]!='a')return 0;
                    }
                    return 1;
                }
            }
        }
        if(y==y1){
            if(c=='a'||(b==0&&isupper(c))||(b==1&&islower(c))){
                if(x1>x){
                    for(int i = x+1; i<x1; i++){
                        if(pieces[i][y]!='a') return 0;
                    }
                    return 1;
                }
                else{
                    for(int i = x-1; i>x1; i--){
                        if(pieces[i][y]!='a')return 0;
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

bool Board::knightPossible(int x, int y, int x1, int y1, bool b){
    if(x1>=0&&y1>=0&&x1<8&&y1<8&&(x!=x1||y!=y1)){
        char c = pieces[x1][y1];
        if(c=='a'||(b==0&&isupper(c))||(b==1&&islower(c))){
            if((abs(x-x1)==1&&abs(y-y1)==2)||(abs(x-x1)==2&&abs(y-y1)==1)){
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

bool Board::bishopPossible(int x, int y, int x1, int y1, bool b){
    if(x1>=0&&y1>=0&&x1<8&&y1<8&&(x!=x1||y!=y1)){
        if((b==1&&isupper(pieces[x1][y1])))return 0;
        if((b==0&&pieces[x1][y1]!='a'&&islower(pieces[x1][y1]))) return 0;
        char c = pieces[x1][y1];
        if(x+y==x1+y1 || x-x1==y-y1){
            int xc, yc;
            if(x1>x){
                xc=1;
            }
            else xc=-1;
            if(y1>y) yc=1;
            else yc=-1;
            int i = x, j = y;
            while(0<=i&&0<=j&&i<=7&&j<=7){
                i+=xc;
                j+=yc;
                if(x1==i&&y1==j)return 1;
                if(pieces[i][j]!='a'){
                    return 0;
                }
            }
            return 1;
        }
        else{
            return 0;
        }
    }
    return 0;
}

bool Board::queenPossible(int x, int y, int x1, int y1, bool b){
    if(bishopPossible(x, y, x1, y1, b)||rookPossible(x, y, x1, y1, b))return 1;
    return 0;
}

bool Board::kingPossible(int x, int y, int x1, int y1, bool b){
    if(x1>=0&&y1>=0&&x1<8&&y1<8){
        if(pieces[x1][y1]=='a'||b!=getColor(x1, y1)){
            int d1 = abs(x-x1);
            int d2 = abs(y-y1);
            if(d1==0&&d2==1)return 1;
            else if(d1==1&&d2==0)return 1;
            else if(d1==1&&d2==1)return 1;
            else return 0;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

bool Board::possible(int x, int y, int x1, int y1, char c, bool m){
    bool b;
    bool res;
    char originalPiece = pieces[x1][y1];
    if(isupper(c)){
        b=1;
    }
    else{
        b=0;
    }
    c=tolower(c);
    if(c=='p'){
        res = pawnPossible(x, y, x1, y1, b);
    }
    else if(c=='r'){
        res = rookPossible(x, y, x1, y1, b);
    }
    else if(c=='n'){
        res = knightPossible(x, y, x1, y1, b);
    }
    else if(c=='b'){
        res = bishopPossible(x, y, x1, y1, b);
    }
    else if(c=='k'){
        res = kingPossible(x, y, x1, y1, b);
    }
    else if(c=='q'){
        res = queenPossible(x, y, x1, y1, b);
    }
    if(!m){
        return res;
    }
    if(!res)return 0;
    makeMove(x, y, x1, y1);
    if(check(b)){
        makeMove(x1, y1, x, y);
        pieces[x1][y1] = originalPiece;
        return 0;
    }
    makeMove(x1, y1, x, y);
    pieces[x1][y1] = originalPiece;
    return 1;
}

bool Board::getColor(int x, int y){
    return isupper(pieces[x][y]);
}

bool Board::check(bool b){
    int x, y;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(tolower(pieces[i][j])=='k'&&getColor(i, j)==b){
                x=i, y=j;
                break;
            }
        }
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(x==i&&y==j)continue;
            if(pieces[i][j]!='a'){
                if(possible(i, j, x, y, pieces[i][j], 0)){
                    return 1;
                }
            }
        }
    }
    return 0;
}

bool Board::checkMate(bool b){
    int x, y;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(tolower(pieces[i][j])=='k' && getColor(i, j)==b){
                x = i;
                y = j;
                break;
            }
        }
    }
    if(!check(b)) return 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(pieces[i][j]!='a'&&getColor(i, j)==b){
                for(int k = 0; k < 8; k++){
                    for(int l = 0; l < 8; l++){
                        if(possible(i, j, k, l, pieces[i][j], 1)){
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

bool Board::staleMate(bool b){
    if(!check(b)){
        int x, y;
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(tolower(pieces[i][j])=='k' && getColor(i, j)==b){
                    x = i;
                    y = j;
                    break;
                }
            }
        }
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(pieces[i][j]!='a'&&getColor(i, j)==b){
                    for(int k = 0; k < 8; k++){
                        for(int l = 0; l < 8; l++){
                            if(possible(i, j, k, l, pieces[i][j], 1)){
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

void Board::close(){
    SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

Board::~Board()
{
    //dtor
}
