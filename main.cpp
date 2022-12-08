#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;

const int mapX = 80;
const int mapY = 25;
const int replaySize = 100;

struct poziom {
    char mainMap[mapY][mapX];
    int heroX;
    int heroY;
    int chestOneX;
    int chestOneY;
    int chestTwoX;
    int chestTwoY;
    int spotOneX;
    int spotOneY;
    int spotTwoX;
    int spotTwoY;
};

poziom undoStack[replaySize];
poziom redoStack[replaySize];

int undoCounter = 0;
int redoCounter = 0;

void clearScreen() {
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void startScreen() { // funkcja wyswietlajaca ekran powitalny
    system("CLS");
    cout << "\n"
         << "SOKOBAN"
         << "\n";
    cout << "Kamil Ratajczyk"
         << "\n";
    cout << "s193345"
         << "\n";

    Sleep(1000);
    system("CLS");
}

void genBlock(int y, int x, poziom *poziom, int var) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (var == 0) { // generate spot
                poziom->mainMap[y + i][x + j] = 38;
            } else if (var == 1) { // generate chest
                poziom->mainMap[y + i][x + j] = 35;
            } else if (var == 2) { // generate hero
                poziom->mainMap[y + i][x + j] = 135;
            }
        }
    }
}

void fillMap1(poziom *poziom) // funkcja wypełniająca mapę 1
{
    fstream file;

    file.open("blank.txt");

    if (!file) {
        cout << "Pobranie mapy z pliku się nie powiodło";
        exit(0);
    }

    for (int i = 0; i < mapY; i++) {
        for (int j = 0; j < mapX; j++) {
            poziom->mainMap[i][j] = file.get();
        }
    }

    file.close();
}

void genMap1(char tab[mapY][mapX]) // wypisujemy na ekran mapę
{
    clearScreen();

    for (int i = 0; i < mapY; i++) {
        for (int j = 0; j < mapX; j++) {
            cout << tab[i][j];
        }
    }
    cout << "\n\n";
}

int genMenu() {
    system("cls");
    cout << "Wybierz:"
         << "\n";
    cout << "(1) Rozpocznij gre od pierwszego etapu"
         << "\n";
    cout << "(2) Opusc gre"
         << "\n";

    while (true) {
        char znak = getchar();

        if (znak == 49) {
            return 1;
        } else if (znak == 50) {
            exit(0);
        } else {
            cout << "Wybrales niepoprawny znak! Mozesz wybrac 1 lub 2."
                 << "\n";
            Sleep(1000);
            genMenu();
        }
    }
}

void undo(poziom *poziom) {
    if (undoCounter > 1 && redoCounter < replaySize - 1) {
        undoCounter -= 1;

//      redoStack.push(*poziom);
        redoStack[redoCounter % replaySize] = *poziom;//TODO sprawdzic czy to sie kopiuje
        redoCounter++;

//      *poziom = undoStack.top();
//      undoStack.pop();
        *poziom = undoStack[(undoCounter - 1) % replaySize];
        undoCounter--;
    }
}

void redo(poziom *poziom) {
    //TODO dodac czyszczenie redoStack po ruchu?
    if (redoCounter != 0) {
        undoCounter--;

//        undoStack.push(*poziom);
        undoStack[(undoCounter-1)%replaySize] = *poziom;

//        *poziom = redoStack.top();
//        redoStack.pop();
        *poziom = redoStack[redoCounter-1];
        redoCounter--;
    }
}

void action(int *y, int *x, int *b, int *a, poziom *poziom) {
    cout << "Wprowadz znak: ";
    char znak = getch();
    cout << znak;

    //TODO dodac zabezbieczenie przeciwko zapisowi w przypadku, gdy nacisniety niepoprawny klawisz

    if (znak == 'w' || znak == 'W') // instrukcje dla "W"
    {
        if (*y == *b + 3 && *x == *a) {
            *y -= 3;
            *b -= 3;
        } else {
            *y -= 3;
        }
    }

    if (znak == 's' || znak == 'S') // instrukcje dla "S"
    {
        if (*y == *b - 3 && *x == *a) {
            *y += 3;
            *b += 3;
        } else {
            *y += 3;
        }
    }

    if (znak == 'a' || znak == 'A') // instrukcje dla "A"
    {
        if (*y == *b && *x == (*a + 3)) {
            *x -= 3;
            *a -= 3;
        } else {
            *x -= 3;
        }
    }

    if (znak == 'd' || znak == 'D') // instrukcje dla "D"
    {
        if (*y == *b && *x == (*a - 3)) {
            *x += 3;
            *a += 3;
        } else {
            *x += 3;
        }
    }

    if (znak == 'u' || znak == 'U') {
        undo(poziom);
    }

    if (znak == 'r' || znak == 'R') {
        redo(poziom);
    }

}

void saveGameProgress(poziom *poziom) {
//    undoStack.push(poziom);
//    copyGameStates(undoStack[undoCounter], *poziom);

    undoStack[undoCounter % replaySize] = *poziom;
    undoCounter++;
}

void level(int lvl, poziom poziom) {
    if (lvl == 1) {

        poziom.heroX = 48;
        poziom.heroY = 5;
        poziom.chestOneX = 39;
        poziom.chestOneY = 8;
        poziom.chestTwoX = 39;
        poziom.chestTwoY = 11;
        poziom.spotOneX = 27;
        poziom.spotOneY = 5;
        poziom.spotTwoX = 30;
        poziom.spotTwoY = 5;

        fillMap1(&poziom);
        genBlock(poziom.heroY, poziom.heroX, &poziom, 2);
        genBlock(poziom.chestOneY, poziom.chestOneX, &poziom, 1);
        genBlock(poziom.chestTwoY, poziom.chestTwoX, &poziom, 1);
        genBlock(poziom.spotOneY, poziom.spotOneX, &poziom, 0);
        genBlock(poziom.spotTwoY, poziom.spotTwoX, &poziom, 0);
        genMap1(poziom.mainMap);
        saveGameProgress(&poziom);
        for (int i = 0; i < 500; i++) {
            action(&poziom.heroY, &poziom.heroX, &poziom.chestOneY, &poziom.chestOneX, &poziom);
            fillMap1(&poziom);
            genBlock(poziom.heroY, poziom.heroX, &poziom, 2);
            genBlock(poziom.chestOneY, poziom.chestOneX, &poziom, 1);
            genBlock(poziom.chestTwoY, poziom.chestTwoX, &poziom, 1);
            genBlock(poziom.spotOneY, poziom.spotOneX, &poziom, 0);
            genBlock(poziom.spotTwoY, poziom.spotTwoX, &poziom, 0);
            genMap1(poziom.mainMap);
            saveGameProgress(&poziom);
        }
    }
}

int main() {
    poziom poziom;

    // startScreen();

    if (genMenu() == 1) {
        level(1, poziom);
    }

    return 0;
}