#pragma once
#include<stdio.h>
#include<cstdio>
#include <algorithm>

 const int MapHeight = 31;
 const int MapWidth = 41;

 static inline int Clamp(int v, int lo, int hi)
 {
	 return std::min(std::max(v, lo), hi);
 }

 struct Player { int Height; int Width; };


 struct Enemy { int Height; int Width; bool Alive; };


 constexpr int MaxEnemies = 5;
 extern Enemy gEnemies[MaxEnemies];
 extern int   gEnemyCount;

 struct Ammo { int Torpedo; int Sonar; int MineStock; int BombStock; };
 extern Ammo  gAmmo;

 extern bool  gRubble[MapHeight][MapWidth];

void ClearMap(char Map[MapHeight][MapWidth], char fill);
void BuildSea(char Map[MapHeight][MapWidth]);
void BuildWall(char Map[MapHeight][MapWidth]);
void PlayerPosition(char Map[MapHeight][MapWidth], const Player& p);
int PlayerMove(char Map[MapHeight][MapWidth], Player& p);
void DrawMap(const char Map[MapHeight][MapWidth]);
 int  ShotTorpedo(char Map[MapHeight][MapWidth],const Player& p,int rangeX);
bool AnyEnemyAlive();
void EnemySpawn(const char Map[MapHeight][MapWidth], const Player& p);
void EnemyFleeOnAction(const char Map[MapHeight][MapWidth],const Player& p,int maxSteps);
extern int gSonar[MapHeight][MapWidth];

int ShootSonarRight(const Player& p);
void TickSonar();
void ApplySonarOverlay(char Map[MapHeight][MapWidth], int RangeX);