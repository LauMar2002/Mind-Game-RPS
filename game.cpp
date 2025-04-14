#include "raylib.h"
#include <iostream>
#include <string>
#include <ctime>

enum Choice { NONE = -1, ROCK, PAPER, SCISSORS };
Color backgroundColor = RAYWHITE;
bool muted = false;

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

void PlayGameSound(Sound s) {
    if (!muted) PlaySound(s);
}

std::string DetermineWinner(Choice player, Choice ai, int &playerScore, int &aiScore, Sound winSound, Sound loseSound, Sound drawSound) {
    if (player == ai) {
        PlayGameSound(drawSound);
        return "Draw!";
    }
    if ((player == ROCK && ai == SCISSORS) || (player == PAPER && ai == ROCK) || (player == SCISSORS && ai == PAPER)) {
        playerScore++;
        PlayGameSound(winSound);
        return "You win!";
    }
    aiScore++;
    PlayGameSound(loseSound);
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

        // Define button rectangles
        Rectangle rockBtn = {50, 300, 100, 40};
        Rectangle paperBtn = {250, 300, 100, 40};
        Rectangle scissorsBtn = {450, 300, 100, 40};

        Rectangle muteBtn = {100, 260, 100, 30};
        Rectangle restartBtn = {250, 260, 100, 30};
        Rectangle quitBtn = {400, 260, 100, 30};

        BeginDrawing();
        ClearBackground(backgroundColor);

        DrawText("Choose Rock, Paper or Scissors", 100, 20, 20, DARKGRAY);

        DrawRectangleRec(rockBtn, CheckCollisionPointRec(GetMousePosition(), rockBtn) ? SKYBLUE : LIGHTGRAY);
        DrawText("Rock", 70, 310, 20, BLACK);

        DrawRectangleRec(paperBtn, CheckCollisionPointRec(GetMousePosition(), paperBtn) ? SKYBLUE : LIGHTGRAY);
        DrawText("Paper", 270, 310, 20, BLACK);

        DrawRectangleRec(scissorsBtn, CheckCollisionPointRec(GetMousePosition(), scissorsBtn) ? SKYBLUE : LIGHTGRAY);
        DrawText("Scissors", 460, 310, 20, BLACK);

        DrawRectangleRec(muteBtn, CheckCollisionPointRec(GetMousePosition(), muteBtn) ? GRAY : DARKGRAY);
        DrawText(muted ? "Unmute" : "Mute", 120, 267, 16, WHITE);

        DrawRectangleRec(restartBtn, CheckCollisionPointRec(GetMousePosition(), restartBtn) ? GRAY : DARKGRAY);
        DrawText("Restart", 270, 267, 16, WHITE);

        DrawRectangleRec(quitBtn, CheckCollisionPointRec(GetMousePosition(), quitBtn) ? GRAY : DARKGRAY);
        DrawText("Quit", 435, 267, 16, WHITE);

        DrawText((playerName + ": " + std::to_string(playerScore)).c_str(), 450, 20, 16, BLUE);
        DrawText(("AI Score: " + std::to_string(aiScore)).c_str(), 450, 45, 16, YELLOW);

        if (playerChoice != NONE) {
            DrawText(("You: " + ChoiceToString(playerChoice)).c_str(), 50, 100, 20, BLUE);
            DrawText(("AI: " + ChoiceToString(aiChoice)).c_str(), 50, 130, 20, YELLOW);
            DrawText(result.c_str(), 52, 182, 30, GRAY); // Shadow effect
            DrawText(result.c_str(), 50, 180, 30, WHITE);
        }

        // Retro scanlines
        for (int y = 0; y < 400; y += 4) {
            DrawLine(0, y, 600, y, Fade(BLACK, 0.1f));
        }

        EndDrawing();

        // Game logic
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, quitBtn)) break;
            if (CheckCollisionPointRec(mouse, muteBtn)) muted = !muted;

            if (CheckCollisionPointRec(mouse, restartBtn)) {
                playerScore = 0;
                aiScore = 0;
                playerChoice = NONE;
                aiChoice = NONE;
                lastPlayerChoice = NONE;
                result = "";
                backgroundColor = RAYWHITE;
                PlayGameSound(drawSound);
            } else if (CheckCollisionPointRec(mouse, rockBtn)) {
                playerChoice = ROCK;
            } else if (CheckCollisionPointRec(mouse, paperBtn)) {
                playerChoice = PAPER;
            } else if (CheckCollisionPointRec(mouse, scissorsBtn)) {
                playerChoice = SCISSORS;
            }

            if (playerChoice != NONE) {
                aiChoice = GetSmarterAIChoice(lastPlayerChoice);
                result = DetermineWinner(playerChoice, aiChoice, playerScore, aiScore, winSound, loseSound, drawSound);
                lastPlayerChoice = playerChoice;

                if (result == "You win!") backgroundColor = GREEN;
                else if (result == "You lose!") backgroundColor = RED;
                else backgroundColor = LIGHTGRAY;
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





