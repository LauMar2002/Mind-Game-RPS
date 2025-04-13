#include "raylib.h"
#include <iostream>
#include <string>
#include <ctime>

enum Choice { NONE = -1, ROCK, PAPER, SCISSORS };

std::string ChoiceToString(Choice choice) {
    switch (choice) {
        case ROCK: return "Rock";
        case PAPER: return "Paper";
        case SCISSORS: return "Scissors";
        default: return "None";
    }
}

Choice GetRandomChoice() {
    return static_cast<Choice>(GetRandomValue(0, 2));
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
    SetTargetFPS(60);

    Choice playerChoice = NONE;
    Choice aiChoice = NONE;
    std::string result = "";
    int playerScore = 0;
    int aiScore = 0;

    InitAudioDevice();

Sound winSound = LoadSound("win.wav");
Sound loseSound = LoadSound("lose.wav");
Sound drawSound = LoadSound("draw.wav");

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, {50, 300, 100, 40})) playerChoice = ROCK;
            else if (CheckCollisionPointRec(mouse, {250, 300, 100, 40})) playerChoice = PAPER;
            else if (CheckCollisionPointRec(mouse, {450, 300, 100, 40})) playerChoice = SCISSORS;

            if (playerChoice != NONE) {
                aiChoice = GetRandomChoice();
                result = DetermineWinner(playerChoice, aiChoice, playerScore, aiScore, winSound, loseSound, drawSound);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Choose Rock, Paper or Scissors", 100, 20, 20, DARKGRAY);

        DrawRectangle(50, 300, 100, 40, LIGHTGRAY);
        DrawText("Rock", 70, 310, 20, BLACK);

        DrawRectangle(250, 300, 100, 40, LIGHTGRAY);
        DrawText("Paper", 270, 310, 20, BLACK);

        DrawRectangle(450, 300, 100, 40, LIGHTGRAY);
        DrawText("Scissors", 460, 310, 20, BLACK);

        if (playerChoice != NONE) {
            DrawText(("You: " + ChoiceToString(playerChoice)).c_str(), 50, 100, 20, BLUE);
            DrawText(("AI: " + ChoiceToString(aiChoice)).c_str(), 50, 130, 20, RED);
            DrawText(result.c_str(), 50, 180, 30, DARKGREEN);
        }

        // Show scores

        DrawText(("Your Score: " + std::to_string(playerScore)).c_str(), 450, 20, 16, BLUE);
        DrawText(("AI Score: " + std::to_string(aiScore)).c_str(), 450, 45, 16, RED);

        // Draw Restart button
    DrawRectangle(250, 260, 100, 30, DARKGRAY);
    DrawText("Restart", 270, 267, 16, WHITE);

       // Check Restart click
       if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
    
        // Check Restart FIRST
        if (CheckCollisionPointRec(mouse, {250, 260, 100, 30})) {
            playerScore = 0;
            aiScore = 0;
            playerChoice = NONE;
            aiChoice = NONE;
            result = "";
            PlaySound(drawSound);
        }
        // Only check choices if NOT clicking restart
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
            aiChoice = GetRandomChoice();
            result = DetermineWinner(playerChoice, aiChoice, playerScore, aiScore, winSound, loseSound, drawSound);
        }
    }
}

        EndDrawing();
    }


    CloseWindow();
    UnloadSound(winSound);
    UnloadSound(loseSound);
    UnloadSound(drawSound);
    CloseAudioDevice();

    return 0;
}

