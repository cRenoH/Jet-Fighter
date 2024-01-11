#include <random>
#include <sl.h>
#include "Entities.h"
#include <random>
#include <vector>
#include <string>


using namespace std;

bool isCollision(MyPlane p, RocketEnmy r);

int main()
{
	slWindow(600, 1200, "Jet Fighter", false);

	int text = slLoadFont("amk.ttf");
	int imgplant = slLoadTexture("kon.png");
	int imgmenu = slLoadTexture("anjas.png");
	int imgroket = slLoadTexture("rudal.png");
	int imgbackground = slLoadTexture("sky.png");
	//int lagumenu = slLoadWAV("lagu.wav");
	int backsoundgmply = slLoadWAV("backsound.wav");
	int soundeffectw = slLoadWAV("noss.wav");
	int imgcloud = slLoadTexture("cloud.png");


	random_device rd;
	int score = 0;
	double speed = 0;
	double speedDelay = 5;
	double delayMin = 0.1;
	int maxspeed = 5;

	ImgBackground imgback;
	imgback.posX = 300;
	imgback.posY1 = 0;
	imgback.posY2 = 1200;
	imgback.velY = 4.5;

	BackgroundMenu backmenu;
	backmenu.posX = 300;
	backmenu.posY1 = 0;
	backmenu.posY2 = 2000;
	backmenu.velY = 4.5;

	MyPlane myplane;
	myplane.posX = 300;
	myplane.posY = 300;
	myplane.velX = 0;
	myplane.velY = 0;
	myplane.radius = 27;

	RocketEnmy rocketenmy;
	rocketenmy.posX = 300;
	rocketenmy.posY = 1250;
	rocketenmy.velX = 0;
	rocketenmy.velY = -8;
	rocketenmy.radius = 27;

	vector<Cloud>cloud;
	for (int i = 0; i < 7; ++i)
	{
		double y = (rd() % 3 + 1) * 100;
		double x = 1200 + i * 128;
		cloud.push_back({ y,x,15});
	}
	bool gameover = false;
	bool mainMenu = true;
	while (!slShouldClose())
	{
		slSetFont(text, 25);
		slSprite(imgbackground, imgback.posX, imgback.posY1, 2000, 2000);

		if (mainMenu)
		{
			
			slSprite(imgmenu, 300, 400, 600, 1200);
			slSetFont(text, 30);
			slSetTextAlign(SL_ALIGN_CENTER);
			slText(300, 400, "tekan w, a, s, d");
			slText(300, 430, "Untuk bergerak");
			slText(300, 470, "Tekan 'space' untuk memulai");
			if (slGetKey(' '))
			{
				mainMenu = false;
			}
		}

		else if (!gameover)
		{
			slSprite(imgplant, myplane.posX, myplane.posY, 150, 150);
			slSprite(imgroket, rocketenmy.posX, rocketenmy.posY, 40, 115);
			for (const Cloud& c : cloud)
				slSprite(imgcloud, c.posX, c.posY, 90, 90);

			imgback.posY1 -= imgback.velY;
			imgback.posY2 -= imgback.velY;

			slSetTextAlign(SL_ALIGN_LEFT);
			slText(20, 75, std::to_string(score).c_str());


			if (imgback.posY1 <= -250)
			{
				imgback.posY1 = imgback.posY2 - 180;
			}

			if (imgback.posY2 <= -50)
			{
				imgback.posY2 = imgback.posY1 + 1210;
			}

			//border pesawat
			if (myplane.posX < 63)
				myplane.posX = 63;

			if (myplane.posX > 537)
				myplane.posX = 537;

			if (myplane.posY < 60)
				myplane.posY = 60;

			if (myplane.posY > 1100)
				myplane.posY = 1100;




			//input move
			bool isMovingup = slGetKey('W') || ('S');
			bool ismovingkr = slGetKey('A') || ('D');

			if (isMovingup)
			{
				if (slGetKey('W'))
				{
					myplane.posY += 4;
				}
				else if (slGetKey('S'))
				{
					myplane.posY += -4;
				}
			}
			if (ismovingkr)
			{
				if (slGetKey('A'))
				{
					myplane.posX += -4;
				}
				else if (slGetKey('D'))
				{
					myplane.posX += 4;
				}
			}
		
			if (rocketenmy.posX <= 30 && score <= maxspeed)
			{
				rocketenmy.velY--;
			}
			if (rocketenmy.posY <= -30)
			{
				score++;
				rocketenmy.posY = 1250;
				rocketenmy.posX = (rd() % 39 + 1) * 10;

				if (rocketenmy.posX <= 100)
					rocketenmy.posX = 100;

				if (rocketenmy.posX >= 500)
					rocketenmy.posX = 500;
			}
			rocketenmy.posY += rocketenmy.velY;
			if (isCollision(myplane, rocketenmy))
				gameover = true;
		}
		else
		{
			slSoundStopAll();
			slSetForeColor(1, 0.5, 0.5, 1);
			slSetTextAlign(SL_ALIGN_CENTER);
			slText(300, 420, "Game Over");
			slText(300, 400, "Press 'space' to restart");
			slText(300, 380, "HighScore:");
			slText(370, 380, std::to_string(score).c_str());
			if (slGetKey(' '))
			{
				gameover = false;
				myplane.posX = 300;
				myplane.posY = 300;
				rocketenmy.posY = 1250;
				score = 0;
				rocketenmy.velY = -8;
				slSetForeColor(1, 1, 1, 1);
			}
		}

		slRender();
	}
	

	slClose();
	return 0;
}

bool isCollision(MyPlane p, RocketEnmy r)
{
	double dx = r.posX - p.posX;
	double dy = r.posY - p.posY;
	double distance = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	if (distance <= p.radius + r.radius)
		return true;
	return false;
}
