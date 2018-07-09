// Include libs here
#include <sstream>
#include <SFML/Graphics.hpp>

// function
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
    // Create VM object
    sf::VideoMode vm(1920, 1080);
    // low res
    //sf::VideoMode vm(1280, 720);
    // Open a window for the game.
    sf::RenderWindow window(vm, "Timber!!!", sf::Style::Fullscreen);

    // create texture to hold background graphics.
    sf::Texture textureBackground;
    // load a graphic into the texture.
    textureBackground.loadFromFile("./assets/graphics/background.png");
    // create a sprite
    sf::Sprite spriteBackground;
    // attach texture to the sprite
    spriteBackground.setTexture(textureBackground);
    // set the position of the background
    spriteBackground.setPosition(0, 0);

    // create texture to hold tree graphics
    sf::Texture textureTree;
    textureTree.loadFromFile("./assets/graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(1280/2, -150);

    // create teh bee
    sf::Texture textureBee;
    textureBee.loadFromFile("./assets/graphics/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    // is the bee moving?
    bool beeActive = false;
    // how fast is the bee?
    float beeSpeed = 0.0f;

    // create three clouds
    sf::Texture textureCloud;
    textureCloud.loadFromFile("./assets/graphics/cloud.png");
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    sf::Clock clock;

    // time bar
    sf::RectangleShape timeBar;
    float timeBarStartsWith = 400;
    float timeBarHeight = 20;
    timeBar.setSize(sf::Vector2f(timeBarStartsWith, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1280/2) - (timeBarStartsWith/3) / 2, 820);

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartsWith / timeRemaining;

    // track if the game is paused
    bool paused = true;

    // setup some text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;

    // choose a font
    sf::Font font;
    font.loadFromFile("./assets/fonts/KOMIKAP_.ttf");

    // set the font
    messageText.setFont(font);
    scoreText.setFont(font);

    // assign the actual message
    messageText.setString("Press Enter to start!!");
    scoreText.setString("Score = 0");

    // set the text sizes
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // set the text color
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    // Position the text
    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f);
    messageText.setPosition(1280 / 2.0f, 720 / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare 6 Branches
    sf::Texture textureBranch;
    textureBranch.loadFromFile("./assets/graphics/branch.png");
    // set textures for branch count
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        //set origin to center
        branches[i].setOrigin(220, 20);
    }

    // prepare the player
    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("./assets/graphics/player.png");
    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(450, 620);

    // the plauer starts on left
    side playerSide = side::LEFT;

    // prepare the grave stone
    sf::Texture textureRIP;
    textureRIP.loadFromFile("./assets/graphics/rip.png");
    sf::Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(460, 700);

    // prepare the ax
    sf::Texture textureAxe;
    textureAxe.loadFromFile("./assets/graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(570, 730);

    // line axe up with tree;
    const float AXE_POSITION_LEFT = 570;
    const float AXE_POSITION_RIGHT = 875;

    // prepare the flying log
    sf::Texture textureLog;
    textureLog.loadFromFile("./assets/graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(1280/2, 720);

    // usefull log related vars
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    bool acceptInput = false;

    while (window.isOpen()) {
        /**
        * Handle player input
        **/
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  {
                window.close();
            }

            if (event.type == sf::Event::KeyReleased && !paused) {
                acceptInput = true;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            paused = false;
            // reset the time and score
            score = 0;
            timeRemaining = 5;

            for (int i = 1; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
            }

            spriteRIP.setPosition(675, 2000);
            spritePlayer.setPosition(450, 620);
            spriteAxe.setPosition(AXE_POSITION_LEFT,
                spriteAxe.getPosition().y);
            acceptInput = true;
        }
        
        if (acceptInput) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT,
                    spriteAxe.getPosition().y);
                spritePlayer.setPosition(1000, 620);

                updateBranches(score);

                spriteLog.setPosition(1280/2, 720);
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT,
                    spriteAxe.getPosition().y);
                spritePlayer.setPosition(450, 620);

                updateBranches(score);

                spriteLog.setPosition(1280/2, 720);
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
            }
        }

        /**
        * Update the view 
        **/
        if (!paused) {
            sf::Time dt = clock.restart();

            //subtract from the ammountof time remaining
            timeRemaining -= dt.asSeconds();
            // size up the time bar
            timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * 
                timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                // pause the game;
                paused = true;
                // Change the message shown to the player
                messageText.setString("Out of time!!");
                // Re position the text based on it's  size
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                textRect.width / 2.0f,
                textRect.top +
                textRect.height / 2.0f);
                messageText.setPosition(1280 /2.0f, 720 / 2.0f);
            }

            // setup the bee
            if (!beeActive) {
                // how fast is the bee
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;

                // how high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            } else { // move the bee
                spriteBee.setPosition(
                    spriteBee.getPosition().x -
                    (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y
                );
                // has the bee reached the edge of the screen?
                if (spriteBee.getPosition().x < -100) {
                    // reset the bee
                    beeActive = false;
                }
            }

            // Manage the clouds
            if (!cloud1Active) {
                // how fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            } else {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x +
                    (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                );
                // has the bee reached the edge of the screen?
                if (spriteCloud1.getPosition().x > 1920) {
                    // reset the bee
                    cloud1Active = false;
                }
            }

            // Manage the clouds
            if (!cloud2Active) {
                // how fast is the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 150);
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            } else {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x +
                    (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                );
                // has the bee reached the edge of the screen?
                if (spriteCloud2.getPosition().x > 1920) {
                    // reset the bee
                    cloud2Active = false;
                }
            }

            // Manage the clouds
            if (!cloud3Active) {
                // how fast is the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 150);
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            } else {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x +
                    (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                );
                // has the bee reached the edge of the screen?
                if (spriteCloud3.getPosition().x > 1920) {
                    // reset the bee
                    cloud3Active = false;
                }
            }

            // update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT) {
                    // move the sprite to th left side
                    branches[i].setPosition(450, height);
                    branches[i].setRotation(180);
                } else if (branchPositions[i] == side::RIGHT) {
                    // move the sprite to th left side
                    branches[i].setPosition(1150, height);
                    branches[i].setRotation(0);
                } else {
                    // hide the branch
                    branches[i].setPosition(3000, height);
                }
            }

            if (logActive) {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                // has the log reached edge 
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
                    logActive = false;
                    spriteLog.setPosition(1280/2, 720);
                }
            }

            if (branchPositions[5] == playerSide) {
                paused = true;
                acceptInput = false;

                spriteRIP.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
                spritePlayer.setPosition(2000, 660);

                messageText.setString("SQUISHED");

                sf::FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1280/2.0f, 1080/2.0f);
            }

        } // end pause

        /**
        * Draw the view 
        **/ 
        window.clear();

        // Draw the scene
        window.draw(spriteBackground);
        //draw clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        //draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }

        //draw tree
        window.draw(spriteTree);
        //draw the player
        window.draw(spritePlayer);
        // draw axe
        window.draw(spriteAxe);
        window.draw(spriteLog);
    
        window.draw(spriteRIP);
        //draw bee
        window.draw(spriteBee);
        //draw the score
        window.draw(scoreText);
        // draw the timebar
        window.draw(timeBar);
        if(paused) {
            window.draw(messageText);
        }

        // Display the scene
        window.display();
       
    }
    return 0;
}

// function def
void updateBranches(int seed) {
    //move all the branches down one place
    for (int i = NUM_BRANCHES - 1; i > 0; i--) {
        branchPositions[i] = branchPositions[i-1];
    }
    // spawn new branchx
    srand((int)time(0) + seed);
    int r = rand() % 5;
    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}