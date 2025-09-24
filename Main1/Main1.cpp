#include "SubGame.h"
#include <iostream>
#include <stdio.h>
#include <time.h>



// main.cpp (또는 네 메인 파일)
#include "SubGame.h"
#include <ctime>    // time
#include <cstdlib>  // srand, rand

int main()
{
    // 1) 랜덤 시드 (딱 한 번)
    srand(static_cast<unsigned>(time(nullptr)));

    // 2) 맵 & 플레이어 준비
    char Map[MapHeight][MapWidth];
    Player p1;
    p1.Height = MapHeight / 2;
    p1.Width = MapWidth / 6;

    // 3) 초기 렌더 + 시작 스폰 1회
    BuildSea(Map);
    BuildWall(Map);
    EnemySpawn(Map, p1);        // ← 시작 시 적 1기 스폰(숨김)
    PlayerPosition(Map, p1);

    // (옵션) 파괴지형 'X' 오버레이
    for (int H = 0; H < MapHeight; ++H)
        for (int W = 0; W < MapWidth; ++W)
            if (gRubble[H][W]) Map[H][W] = 'X';

    DrawMap(Map);
    printf("게임을 시작합니다\n\n");

    // 4) 메인 루프
    bool Running = true;
    int  Turn = 1;

    while (Running)
    {
        // ---- 턴 시작 훅 ----
        if (Turn % 5 == 0) {
            EnemySpawn(Map, p1);   // ← 5턴마다 적 증원 1기
        }

        // 현재 상태 출력(원하면)
        printf("[Turn %d] 무브포인트 3\n", Turn);

        // ---- 이동 입력 루프 ----
        int MovePoints = 3; // 네가 쓰던 값 유지 (원하면 5로 변경)
        for (int i = 0; i < MovePoints; ++i)
        {
            printf("\n당신의 턴 남은 무브포인트 : %d \n", MovePoints - i);
            printf("턴을 종료하려면 E 입력.\n");

            int r = PlayerMove(Map, p1); // -1=게임종료, 2=턴종료, 0/1=무시/성공
            if (r == -1) { Running = false; break; }
            if (r == 2) { printf("턴 종료!\n"); break; }

            // ---- 렌더 ----
            BuildSea(Map);
            BuildWall(Map);
            PlayerPosition(Map, p1);

            // (옵션) 파괴지형 'X' 오버레이
            for (int H = 0; H < MapHeight; ++H)
                for (int W = 0; W < MapWidth; ++W)
                    if (gRubble[H][W]) Map[H][W] = 'X';

            // (디버그용) 적 위치 잠깐 보고 싶으면 아래 4줄 잠깐 켜기
            // for (int ei = 0; ei < gEnemyCount; ++ei)
            //     if (gEnemies[ei].Alive)
            //         Map[gEnemies[ei].Height][gEnemies[ei].Width] = 'E';

            DrawMap(Map);
        }

        if (!Running) break;

        // ---- 턴 종료 훅 ----
        // (소나 타이머 감소/기뢰 폭발 같은 건 나중에 여기서 처리)
        ++Turn;
    }

    printf("게임종료 \n");
    return 0;
}





