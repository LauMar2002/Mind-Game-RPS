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

std::string DetermineWinner(Choice player, Choice ai) {
    if (player == ai) return "Draw!";
    if ((player == ROCK && ai == SCISSORS) || 
        (player == PAPER && ai == ROCK) || 
        (player == SCISSORS && ai == PAPER)) return "You win!";
    return "You lose!";
}

int main() {
    InitWindow(600, 400, "Rock Paper Scissors");
    SetTargetFPS(60);

    Choice playerChoice = NONE;
    Choice aiChoice = NONE;
    std::string result = "";

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, {50, 300, 100, 40})) playerChoice = ROCK;
            else if (CheckCollisionPointRec(mouse, {250, 300, 100, 40})) playerChoice = PAPER;
            else if (CheckCollisionPointRec(mouse, {450, 300, 100, 40})) playerChoice = SCISSORS;

            if (playerChoice != NONE) {
                aiChoice = GetRandomChoice();
                result = DetermineWinner(playerChoice, aiChoice);
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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
