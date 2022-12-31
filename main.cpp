#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "headers/coba.hpp"
#include "headers/title.hpp"

using namespace sf;
using namespace std;

string obstacleName;

// int obstacleF(int randomObstacle)
// {
//   enum obstacle
//   {
//     BlackCar,
//     BlueCar,
//     RedCar,
//     RedTruck,
//     BlueTruck,
//     Box,
//     Oil,
//     Rock,
//   };

//   switch (randomObstacle)
//   {
//   case BlackCar:
//     obstacleName = "assets/image/BlackCar.png";
//     break;
//   case BlueCar:
//     obstacleName = "assets/image/BlueCar.png";
//     break;
//   case RedCar:
//     obstacleName = "assets/image/RedCar.png";
//     break;
//   case RedTruck:
//     obstacleName = "assets/image/RedTruck.png";
//     break;
//   case BlueTruck:
//     obstacleName = "assets/image/BlueTruck.png";
//     break;
//   case Box:
//     obstacleName = "assets/image/Box.png";
//     break;
//   case Oil:
//     obstacleName = "assets/image/Oil.png";
//     break;
//   case Rock:
//     obstacleName = "assets/image/Rock.png";
//     break;
//   default:
//     obstacleName = "none";
//     break;
//   }
// }

/*
  normalize berguna untuk pergerakan
  vektor secara diagonal
*/
Vector2f normalize(Vector2f vector)
{
  constexpr auto units_in_last_place = 2;
  auto norm = std::sqrt((vector.x * vector.x) + (vector.y * vector.y));
  // Prevent division by zero
  if (norm <= std::numeric_limits<float>::epsilon() * norm * units_in_last_place || norm < std::numeric_limits<float>::min())
  {
    return sf::Vector2f{};
  }
  return vector / norm;
}

// variabel 3 jalur
int jalur1, jalur2, jalur3;
int randomXpos1;
int randomXpos2;
int randomXpos3;

// fungsi untuk menciptakan random jalur
int randomJalur(int jalur);

int main()
{
  // Membuat tampilan window game
  RenderWindow window(VideoMode(480, 720), "Game Car 2D Fun", Style::Close);
  window.setFramerateLimit(60);
  srand(time(0));

  // membuat struct Texture,
  struct Textures
  {
    Texture player;
    Texture enemy1;
    Texture enemy2;
    Texture enemy3;
    Texture enemy4;
    Texture background;
    Texture road;
    Texture gameOver;
  } textures;

  // char randomObstacle = rand() % (maxXPos - minXPos + 1) + minXPos;
  // int randomObstacle;
  // srand(time(0));
  // randomObstacle = rand() % 9;
  // obstacleF(randomObstacle);

  // player
  textures.player.loadFromFile("assets/image/BlueCar.png");

  // enemy
  textures.enemy1.loadFromFile("assets/image/BlueTruck.png");
  textures.enemy2.loadFromFile("assets/image/RedTruck.png");
  textures.enemy3.loadFromFile("assets/image/OrangeTruck.png");
  textures.enemy4.loadFromFile("assets/image/RedCar.png");

  // road & background
  textures.road.loadFromFile("assets/image/Road.png");
  textures.background.loadFromFile("assets/image/Background.png");

  struct objekGame
  {
    int score = 0;
    int frames = 0;
    Font font;
    // gameState state = pause;
    Sprite player;
    Sprite road;
    Sprite back1;
    Sprite back2;
    Sprite obs1;
    Sprite obs2;
    Sprite obs3;
    Sprite gameOver;
    // Text textScore;
    Text pressUp;
  } game;

  game.font.loadFromFile("assets/fonts/Poppins-Regular.ttf");

  // player
  game.player.setTexture(textures.player);
  game.player.setPosition(480 / 2, 720);
  game.player.setOrigin(textures.player.getSize().x / 2, textures.player.getSize().y);

  // road
  game.road.setTexture(textures.road);
  game.road.setPosition(480 / 2, 720);
  game.road.setOrigin(textures.road.getSize().x / 2, textures.road.getSize().y);

  // background
  game.back1.setTexture(textures.background);
  game.back2.setTexture(textures.background);
  game.back1.setPosition((480 - (textures.background.getSize().x)) / 2, 0);
  game.back2.setPosition((480 - (textures.background.getSize().x)) / 2, (-720 + 1));
  int backspeed = 6;

  // variabel untuk mengatur 3 posisi arah Y dari obstacle
  int minYObs1 = 0;
  int maxYObs1 = 100;
  int minYObs2 = 120;
  int maxYObs2 = 220;
  int minYObs3 = 230;
  int maxYObs3 = 400;

  /*
  melakukan random posisi Y dari variabel diatas.
  ini berguna untuk menghasilkan jarak yang
  berbeda setiap obstacle dari sumbu Y.
  */
  srand(time(0));
  int randomYpos1 = rand() % (maxYObs1 - minYObs1 + 1) + minYObs1;
  int randomYpos2 = rand() % (maxYObs2 - minYObs1 + 1) + minYObs2;
  int randomYpos3 = rand() % (maxYObs3 - minYObs1 + 1) + minYObs3;

  // variabel untuk mengatur 3 posisi arah X dari obstacle
  int minXPos1 = 100;
  int maxXPos1 = 150;
  int minXPos2 = 200;
  int maxXPos2 = 240;
  int minXPos3 = 290;
  int maxXPos3 = 340;

  /*
   melakukan random posisi X dari variabel diatas.
   ini berguna untuk menghasilkan jarak yang
   berbeda setiap obstacle dari sumbu X.
   */
  srand(time(0));
  randomXpos1 = rand() % (maxXPos1 - minXPos1 + 1) + minXPos1;
  randomXpos2 = rand() % (maxXPos2 - minXPos2 + 1) + minXPos2;
  randomXpos3 = rand() % (maxXPos3 - minXPos3 + 1) + minXPos3;

  /*
  melakukan random jalur, dengan memanggil fungsi randomJalur().
  ini berguna agar obstacle tidak selalu berada,
  pada jalur yang sama.
  */
  srand(time(0));
  int jalur = rand() % 6;
  randomJalur(jalur);

  // obstacle
  game.obs1.setTexture(textures.enemy1);
  game.obs1.setPosition(jalur1, randomYpos1 * (-1));
  game.obs2.setTexture(textures.enemy2);
  game.obs2.setPosition(jalur2, randomYpos2 * (-1));
  game.obs3.setTexture(textures.enemy3);
  game.obs3.setPosition(jalur3, randomYpos3 * (-1));

  // game over
  Text gameOverText("Game Over!", game.font, 32);
  gameOverText.setPosition(150, 720 / 2);

  // reset game
  sf::Text ResetGameText("Press Space to Reset the game", game.font, 20);
  ResetGameText.setPosition(80, 400);

  game.pressUp.setFont(game.font);
  game.pressUp.setFillColor(Color::White);
  game.pressUp.setCharacterSize(30);
  game.pressUp.setPosition(480, 0);

  bool gameOver = false;
  bool obstaclePassed = false;

  Vector2f direction;
  float speed = 3;

  while (window.isOpen())
  {
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();

      if (event.type == Event::KeyPressed)
      {
        if (event.key.code == Keyboard::Left)
          direction.x = -2.0;

        if (event.key.code == Keyboard::Right)
          direction.x = 2.0;

        if (event.key.code == Keyboard::Up)
          direction.y = -2.0;

        if (event.key.code == Keyboard::Down)
          direction.y = 2.0;

        direction = normalize(direction) * speed;
      }

      if (event.type == Event::KeyReleased)
      {
        if (event.key.code == Keyboard::Left || event.key.code == Keyboard::Right)
          direction.x = 0;

        if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
          direction.y = 0;
      }
    }

    // Update posisi player
    game.player.move(direction);

    // jika player menabrak obstacle, maka gameover.
    if (game.player.getGlobalBounds().intersects(game.obs1.getGlobalBounds()))
      gameOver = true;

    if (game.player.getGlobalBounds().intersects(game.obs2.getGlobalBounds()))
      gameOver = true;

    if (game.player.getGlobalBounds().intersects(game.obs3.getGlobalBounds()))
      gameOver = true;

    /*
    jika player berhasil melewati salah satu obstacle,
    maka score akan bertambah 1 dan obstacle bisa di panggil kembali.
    */
    if (game.obs1.getPosition().y > game.player.getPosition().y && !obstaclePassed)
    {
      game.score++;
      obstaclePassed = true;
    }

    if (game.obs2.getPosition().y > game.player.getPosition().y && !obstaclePassed)
    {
      game.score++;
      obstaclePassed = true;
    }

    if (game.obs3.getPosition().y > game.player.getPosition().y && !obstaclePassed)
    {
      game.score++;
      obstaclePassed = true;
    }

    // berguna agar obstacle bergerak
    game.obs1.move(0, 2);
    game.obs2.move(0, 2);
    game.obs3.move(0, 2);

    /*
    jika salah satu obstacle telah melewati window atau keluar dari window.
    maka akan di lakukan random jalur dengan memanggil fungsi randomJalur.
    agar jalur dari obstacle tidak sama dengan yang sebelumnya.
    */
    if (game.obs1.getPosition().y > 720 || game.obs2.getPosition().y > 720 || game.obs3.getPosition().y > 720)
    {
      srand(time(0));
      int jalur = rand() % 6;
      randomJalur(jalur);

      /*
      menampilkan obstacle kembali setelah melakukan,
      random jalur untuk obstacle.
      */
      if (game.obs1.getPosition().y > 720)
      {
        obstaclePassed = false;
        game.obs1.setPosition(jalur1, randomYpos1 * (-1));
      }

      if (game.obs2.getPosition().y > 720)
      {
        obstaclePassed = false;
        game.obs2.setPosition(jalur2, randomXpos2 * (-1));
      }

      if (game.obs3.getPosition().y > 720)
      {
        obstaclePassed = false;
        game.obs3.setPosition(jalur3, randomXpos3 * (-1));
      }
    }

    /*
    membuat batas jalan.
    agar player tidak keluar dari jalan,
    serta tidak melebihi batas window bawah & atas.
    */

    // Batas kiri
    if (game.player.getPosition().x < 120)
      game.player.setPosition(120, (game.player.getPosition().y));

    // Batas Kanan
    if (game.player.getPosition().x > 360)
      game.player.setPosition(360, (game.player.getPosition().y));

    // Batas Atas
    if (game.player.getPosition().y < (textures.player.getSize().y))
      game.player.setPosition((game.player.getPosition().x), (textures.player.getSize().y));

    // Batas Bawah
    if (game.player.getPosition().y > 720)
      game.player.setPosition((game.player.getPosition().x), 720);

    string scoreString = to_string(game.score);

    Text scoreText(scoreString, game.font, 32);
    scoreText.setPosition(10, 10);

    // Update game.
    if (game.back1.getPosition().y > 720)
      game.back1.setPosition((480 - (textures.background.getSize().x)) / 2, (-720 + 1));
    if (game.back2.getPosition().y > 720)
      game.back2.setPosition((480 - (textures.background.getSize().x)) / 2, (-720 + 1));

    game.back1.move(0, backspeed);
    game.back2.move(0, backspeed);

    // menampilkan objek dalam window.
    window.draw(game.back1);
    window.draw(game.back2);
    window.draw(game.road);
    window.draw(game.obs1);
    window.draw(game.obs2);
    window.draw(game.obs3);
    window.draw(game.player);
    window.draw(scoreText);

    // game Over
    if (gameOver)
    {
      window.clear(Color::Black);
      window.draw(gameOverText);
      window.draw(ResetGameText);
      if (event.key.code == Keyboard::Space)
      {
        game.score = 0;
        game.frames = 0;
        game.player.setPosition(480 / 2, 720);
        game.obs1.setPosition(480 / 2, 0);
        gameOver = false;
      }
    }

    window.display();
  }

  return 0;
}

int randomJalur(int jalur)
{
  if (jalur == 0)
  {
    jalur1 = randomXpos1;
    jalur2 = randomXpos2;
    jalur3 = randomXpos3;
  }
  else if (jalur == 1)
  {
    jalur1 = randomXpos1;
    jalur2 = randomXpos3;
    jalur3 = randomXpos2;
  }
  else if (jalur == 2)
  {
    jalur1 = randomXpos2;
    jalur2 = randomXpos3;
    jalur3 = randomXpos1;
  }
  else if (jalur == 3)
  {
    jalur1 = randomXpos2;
    jalur2 = randomXpos1;
    jalur3 = randomXpos3;
  }
  else if (jalur == 4)
  {
    jalur1 = randomXpos3;
    jalur2 = randomXpos1;
    jalur3 = randomXpos2;
  }
  else if (jalur == 5)
  {
    jalur1 = randomXpos3;
    jalur2 = randomXpos2;
    jalur3 = randomXpos1;
  }

  return jalur1, jalur2, jalur3;
}

/*
    Kelompok kami membuat program dengan konsep game balap 2 Dimensi

    Program ini dibuat dengan kreativitas anggota kelompok. game ini dibuat dengan tidak
    meniru atau mengambil contoh dari source code dari internet.

    Kelompok kami membuat konsep tentang game balap 2 Dimensi.
    kemudian kami mencari video tutorial untuk membangun game ini.

    kami mencari video berasarkan kata kunci,
    misalnya "pergerakan vektor dalam SFML" , "normalize vektor dalam SFML", dll.

*/