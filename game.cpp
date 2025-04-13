#include "raylib.h"
#include <iostream>
#include <string>
#include <ctime>

enum Choice { NONE = -1, ROCK, PAPER, SCISSORS };

Color backgroundColor = RAYWHITE;
Color targetBackgroundColor = RAYWHITE;

Choice GetRandomChoice() {
    return static_cast<Choice>(GetRandomValue(0, 2));
}

Choice GetSmarterAIChoice(Choice previousPlayerChoice) {
    switch (previousPlayerChoice) {
        case ROCK: return PAPER;
        case PAPER: return SCISSORS;
        case SCISSORS: return ROCK;
        default: return GetRandomChoice();
    }
}

std::string ChoiceToString(Choice choice) {
    switch (choice) {
        case ROCK: return "Rock";
        case PAPER: return "Paper";
        case SCISSORS: return "Scissors";
        default: return "None";
    }
}

std::string DetermineWinner(Choice player, Choice ai, int &playerScore, int &aiScore, Sound winSound, Sound loseSound, Sound drawSound){
    if (player == ai) {
        PlaySound(drawSound);
        return "Draw!";
    }
    if ((player == ROCK && ai == SCISSORS) || 
        (player == PAPER && ai == ROCK) || 
        (player == SCISSORS && ai == PAPER)) {
        playerScore++;
        PlaySound(winSound);
        return "You win!";
    }
    aiScore++;
    PlaySound(loseSound);
    return "You lose!";
}

int main() {
    InitWindow(600, 400, "Mind Game RPS");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    Choice playerChoice = NONE;
    Choice aiChoice = NONE;
    Choice lastPlayerChoice = NONE;

    std::string result = "";
    std::string playerName = "";
    bool nameEntered = false;
    char nameBuffer[20] = "";

    int playerScore = 0;
    int aiScore = 0;

    InitAudioDevice();
    Sound winSound = LoadSound("win.wav");
    Sound loseSound = LoadSound("lose.wav");
    Sound drawSound = LoadSound("draw.wav");

    while (!WindowShouldClose()) {
        // === PLAYER NAME INPUT FIRST ===
        if (!nameEntered) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (strlen(nameBuffer) < 19)) {
                    int len = strlen(nameBuffer);
                    nameBuffer[len] = (char)key;
                    nameBuffer[len + 1] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = strlen(nameBuffer);
                if (len > 0) nameBuffer[len - 1] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && strlen(nameBuffer) > 0) {
                playerName = nameBuffer;
                nameEntered = true;
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Enter your name:", 180, 150, 20, DARKGRAY);
            DrawRectangle(180, 180, 240, 40, LIGHTGRAY);
            DrawText(nameBuffer, 190, 190, 20, BLACK);
            DrawText("Press ENTER to start", 170, 240, 20, GRAY);
            EndDrawing();
            continue;
        }

        // === HOVER COLORS ===
        Vector2 mouse = GetMousePosition();
        Color rockColor = LIGHTGRAY;
        Color paperColor = LIGHTGRAY;
        Color scissorsColor = LIGHTGRAY;

        if (CheckCollisionPointRec(mouse, {50, 300, 100, 40})) rockColor = GRAY;
        if (CheckCollisionPointRec(mouse, {250, 300, 100, 40})) paperColor = GRAY;
        if (CheckCollisionPointRec(mouse, {450, 300, 100, 40})) scissorsColor = GRAY;

        // === Animate Background Color ===
        backgroundColor.r += (targetBackgroundColor.r - backgroundColor.r) / 10;
        backgroundColor.g += (targetBackgroundColor.g - backgroundColor.g) / 10;
        backgroundColor.b += (targetBackgroundColor.b - backgroundColor.b) / 10;
        backgroundColor.a += (targetBackgroundColor.a - backgroundColor.a) / 10;

        // === GAME DRAWING ===
        BeginDrawing();
        ClearBackground(backgroundColor);

        DrawText("Choose Rock, Paper or Scissors", 100, 20, 20, DARKGRAY);

        DrawRectangle(50, 300, 100, 40, rockColor);
        DrawText("Rock", 70, 310, 20, BLACK);

        DrawRectangle(250, 300, 100, 40, paperColor);
        DrawText("Paper", 270, 310, 20, BLACK);

        DrawRectangle(450, 300, 100, 40, scissorsColor);
        DrawText("Scissors", 460, 310, 20, BLACK);

        DrawRectangle(250, 260, 100, 30, DARKGRAY);
        DrawText("Restart", 270, 267, 16, WHITE);

        DrawText((playerName + ": " + std::to_string(playerScore)).c_str(), 450, 20, 16, BLUE);
        DrawText(("AI Score: " + std::to_string(aiScore)).c_str(), 450, 45, 16, RED);

        if (playerChoice != NONE) {
            DrawText(("You: " + ChoiceToString(playerChoice)).c_str(), 50, 100, 20, BLUE);
            DrawText(("AI: " + ChoiceToString(aiChoice)).c_str(), 50, 130, 20, RED);
            DrawText(result.c_str(), 50, 180, 30, DARKGREEN);
        }

        EndDrawing();

        // === GAME LOGIC ===
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, {250, 260, 100, 30})) {
                playerScore = 0;
                aiScore = 0;
                playerChoice = NONE;
                aiChoice = NONE;
                lastPlayerChoice = NONE;
                result = "";
                targetBackgroundColor = RAYWHITE;
                PlaySound(drawSound);
            }
            else if (CheckCollisionPointRec(mouse, {50, 300, 100, 40})) {
                playerChoice = ROCK;
            }
            else if (CheckCollisionPointRec(mouse, {250, 300, 100, 40})) {
                playerChoice = PAPER;
            }
            else if (CheckCollisionPointRec(mouse, {450, 300, 100, 40})) {
                playerChoice = SCISSORS;
            }

            if (playerChoice != NONE) {
                aiChoice = GetSmarterAIChoice(lastPlayerChoice);
                result = DetermineWinner(playerChoice, aiChoice, playerScore, aiScore, winSound, loseSound, drawSound);
                lastPlayerChoice = playerChoice;

                // Update target background color
                if (result == "You win!") targetBackgroundColor = GREEN;
                else if (result == "You lose!") targetBackgroundColor = RED;
                else if (result == "Draw!") targetBackgroundColor = LIGHTGRAY;
            }
        }
    }

    UnloadSound(winSound);
    UnloadSound(loseSound);
    UnloadSound(drawSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

