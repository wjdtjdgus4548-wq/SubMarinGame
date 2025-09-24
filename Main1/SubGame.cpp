#include "SubGame.h"
#include <iostream>
#include <stdio.h>
#include <random>
#include <algorithm>
#include <utility>

Enemy gEnemies[MaxEnemies] = {};
int gEnemyCount = 0;

Ammo gAmmo = { 3, 5, 0, 0 };
bool gRubble[MapHeight][MapWidth] = { false };

void ClearMap(char Map[MapHeight][MapWidth], char fill)
{
	for (int h = 0; h < MapHeight; ++h)
	{
		for (int w = 0; w < MapWidth; ++w)
		{
			Map[h][w] = fill;

		}
	}
}

void BuildSea(char Map[MapHeight][MapWidth])
{
	ClearMap(Map,'.');	
}

void BuildWall(char Map[MapHeight][MapWidth])
{
	const int Wall = MapWidth / 4;
	for (int h = 0; h < MapHeight; ++h)
	{
		
		Map[h][Wall] = '#';

	}
}
void PlayerPosition(char Map[MapHeight][MapWidth], const Player& p)
{
	Map[p.Height][p.Width] = 'P';

}

int PlayerMove(char Map[MapHeight][MapWidth], Player& p)
{
	char Input;
	printf("WASD를 입력해 주세요(Q)는 종료 : ");
	std::cin >> Input;
	if (Input == 'q' || Input == 'Q') return -1;
	if (Input == 'e' || Input == 'E') return 2;

	int NextH = p.Height;
	int NextW = p.Width;

	
	if ((Input == 'w' || Input == 'W') && p.Height > 0)
	{
		NextH = p.Height - 1;
	}
	else if ((Input == 's' || Input == 'S') && p.Height < MapHeight - 1 )
	{
		NextH = p.Height + 1;
	}
	else if ((Input == 'd' || Input == 'D') && p.Width < MapWidth - 1 )
	{
		NextW = p.Width + 1;
	}
	else if ((Input == 'a' || Input == 'A') && p.Width > 0 )
	{
		NextW = p.Width - 1;
	}
	else
	{
		return 0;
	}
	

	if (Map[NextH][NextW] == '#')
	{
		printf("여긴 벽이야 정신점;;");
		return 0;
	}
	p.Height = NextH;
	p.Width = NextW;

	return 1;

}

bool AnyEnemyAlive() {
	for (int i = 0; i < gEnemyCount; ++i)
		if (gEnemies[i].Alive) return true;
	return false;
}

void DrawMap(const char Map[MapHeight][MapWidth])
{
	for (int h = 0; h < MapHeight; h++)
	{
		for (int w = 0; w < MapWidth; w++)
		{
			printf("%c", Map[h][w]);
		}
		printf("\n");
	}
}





int ShotTorpedo(char Map[MapHeight][MapWidth], const Player& p, int RangeX)
{
	int h = p.Height;
	int w = p.Width;

	if (gAmmo.Torpedo <= 0 )
	{
		printf("[어뢰 없음]\n "); return 0;
	}
	gAmmo.Torpedo--;

	printf("어뢰가 날라갈 거리를 입력해 주세요 : ");
	std::cin >> RangeX;
	int steps = Clamp(RangeX, 2, 30);

	for (int s = 0; s < steps; ++s)
	{
		w += 1;
		if (w >= MapWidth)
		{
			w -= 1; break;
		}
	}

	int Kills = 0;
	for (int a = -1; a <= 1; ++a)
	{
		for (int b = -1; b <= 1; ++b)
		{
			int th = h + a, tw = w + b;
			if (th < 0 || th >= MapHeight || tw < 0 || tw >= MapWidth) continue;
			
			gRubble[th][tw] = true;
			for (int i = 0; i < gEnemyCount; i++)
			{
				if (!gEnemies[i].Alive)continue;
				if (gEnemies[i].Height == th && gEnemies[i].Width == tw)
				{
					gEnemies[i].Alive = false;
					++Kills;
				}
			}
			
		}
	}
	return Kills;
}

int ShootSonarRight(char Map[MapHeight][MapWidth], const Player& p, int RangeX)
{
	if (gAmmo.Sonar <= 0)
	{
		printf("소나가 없습니다.");
		return 0;
	}
	gAmmo.Sonar--;

	printf("소나가 날라갈 거리를 입력해 주십시오 : ");
	std::cin >> RangeX;
	int steps = Clamp(RangeX, 2, 30);

	for(int)


}

void EnemySpawn (const char Map[MapHeight][MapWidth], const Player& p)
{	
	int RandomHieght = 0;
	int RandomWidth = 0;

	if (gEnemyCount >= MaxEnemies) return;

	const int Wall = MapWidth / 4;
	const int RightStart = Wall + 1;
	const int WidthSpan = MapWidth - RightStart;

	for (int tires = 0; tires < 100; tires++)
	{
		int h = rand() % MapHeight;
		int w = RightStart + (rand() % WidthSpan);

		if (Map[h][w] == '#') continue;
		if (gRubble[h][w]) continue;

		if (h == p.Height && w == p.Width) continue;

		bool Occupied = false;
		for (int i = 0; i < gEnemyCount; i++)
		{
			if (!gEnemies[i].Alive)continue;
			if (gEnemies[i].Height == h && gEnemies[i].Width == w)
			{
				Occupied = true; break;
			}
		}
		if (Occupied)continue;

		gEnemies[gEnemyCount++] = { h, w, true };
		break;
	}
}

static bool IsInside(int h, int w)
{
	return(0 <= h && h < MapHeight && 0 <= w && w < MapWidth);
}

static bool IsEnemyAt(int h, int w ,int exceptIndex = -1)
{
	for(int i = 0; i < gEnemyCount; ++i)
	{
		if (i == exceptIndex)continue;
		if (!gEnemies[i].Alive)continue;
		if (gEnemies[i].Height == h && gEnemies[i].Width == w)return true;
	}
	return false;

}

static bool CanEnemyEnter(const char Map[MapHeight][MapWidth], const Player& p, int h, int w)
{
	if (!IsInside(h, w)) return false;
	if (Map[h][w] == '#') return false;
	if (gRubble[h][w]) return false;
	if (h == p.Height && w == p.Width) return false;
	if (w < MapWidth / 4 + 1) return false;
	return true;

}

void EnemyFleeOnAction(const char Map[MapHeight][MapWidth], const Player& p, int MaxSteps)
{
	for (int i = 0; i < gEnemyCount; i++)
	{
		if (!gEnemies[i].Alive)continue;

		int steps = rand() % (MaxSteps + 1);
		for (int s = 0; s < steps; s++)
		{
			int dirs[4][2] = { {-1,0}, {1,0} ,{0,1}, {0,-1} };

			for (int k = 0; k < 4; k++)
			{
				int r = rand() % 4;
				std::swap(dirs[k][0], dirs[r][0]);
				std::swap(dirs[k][1], dirs[r][1]);

			}
			bool Moved = false;
			for (int k = 0; k < 4; k++)
			{
				int nh = gEnemies[i].Height + dirs[k][0];
				int nw = gEnemies[i].Width + dirs[k][1];

				if (!CanEnemyEnter(Map, p, nh, nw)) continue;
				if (IsEnemyAt(nh,nw,i))continue;


				gEnemies[i].Height = nh;
				gEnemies[i].Width = nw;
				Moved = true;
				break;

				//죽을거같다...
			}
			if (!Moved) break;
		}
	}
}

