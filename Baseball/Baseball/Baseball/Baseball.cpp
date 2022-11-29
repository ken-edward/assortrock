

// 이제부터 졸라 쉬운 숫자 야구

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string> // 문자열
#include <conio.h>

class Manager
{
public:
	Manager();

public:
	char Target[4] = {}; // 맞춰야 할 정답

public:
	void Progress();
};

Manager::Manager()
{
	// size_t는 부호 없는 정수다.
	// 그래서 반복에서 검사할 때
	// 음수가 돼야 나가는 거면 안 나가진다.
	// 양수 최대값이 되기 때문에.
	for (int i = 0; i < 3; ) // 0 1 2 하고 세 번 돈다.
	{
		int now = rand() % 9 + 1 + '0'; // '0'은 정수 기준 숫자를 문자 기준 숫자로 바꿔준다.
		// 마찬가지로 + 'a', + 'A' 같은 기법도 가능
		// rand() % 9에서 0~8의 나머지가 나오는데, 1을 더하면 1~9가 된다.

		// 기존에 있었는지 검사
		bool check = false;

		if (Target[0] == now ||
			Target[1] == now ||
			Target[2] == now)
		{
			// 하나라도 같으면 걸림
			check = true;
		}

		if (check) // 있으면 다시함.
		{
			continue; // 다음으로 넘김
			// i++; // 카운트 변수 증가 안 함
		}
		else // 대입
		{
			Target[i] = now;

			i++; // 카운트 변수 증가
		}
	}
}

#define KK \
std::cout << "숫자 야구에 오신 것을 환영합니다!" << std::endl << std::endl;\
std::cout << "대충 룰" << std::endl << std::endl;\
std::cout << "대충 치라는 안내문" << std::endl << std::endl;

void Manager::Progress()
{
	// 초기 세팅
	KK;

	int TryCount = 0;

	// 이후 진행
	while (true)
	{
		// Ctrl + R, R 하면 이름 전체 변경 가능 (변수, 함수, 클래스 등)
		std::string Try;
		
		std::cin >> Try;

		// 검사

		bool win = false;

		int Strike = 0;
		int Ball = 0;

		for (int i = 0; i < 3; i++) // 정답 기준 검사
		{
			if (Try[i] == Target[0]) if (i == 0) { Strike++; } else Ball++;
			if (Try[i] == Target[1]) if (i == 1) { Strike++; } else Ball++;
			if (Try[i] == Target[2]) if (i == 2) { Strike++; } else Ball++;
		}

		if (Strike == 3)
		{
			win = true;
		}

		// 계속
		if (win == false)
		{
			TryCount++;

			std::cout << "\n응 아니야~ (시도 횟수: " << TryCount << ")" << std::endl << std::endl;

			// 문자열 버전 함수와
			// 정수 버전 함수를 알아서 호출하고 있는 거다. (<<)
			// 그러면 그냥 정수가 아니고 문자 버전? 으로 출력을 할 거다. (아스키코드 아닐 거임)
			std::cout << "Strike: " << Strike << " Ball: " << Ball << std::endl << std::endl;
		}
		// 대충 축하하고 끝
		else // check 걸릴 때만
		{
			std::cout << "\nㅊ (정답: " << Target << ")" << std::endl << std::endl;

			int 에휴 = _getch();

			break; // 루프문 탈출. 게임 종료.
		}
	}
}


int main()
{
	// 난수 시드 설정
	srand((unsigned int)time(NULL));
	
	Manager manager;

	manager.Progress();

	return 0;
}
