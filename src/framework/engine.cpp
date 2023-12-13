#include "engine.h"
#include "../shapes/arrow.h"

//need this for call back function
Engine* Engine::instance = nullptr;

enum state {play, won, lose};
state screen;

static int arrowNum = 0;
static int currentLevel = 1;

color opaque = {0,0,0,0};

using namespace std;

Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
    instance = this;
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallBack);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.use().setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use().setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    srand(time(NULL));

    vec2 pos = {width/2, height/2};
    vec2 size = {30, 20};
    Direction randDirection;

    //later levels have more arrows
    for(int i = 0; i < 10 + currentLevel * 3; i++)
    {
        randDirection = Direction((rand() % 4));

        arrows.push_back(std::make_unique<Arrow>(shapeShader, pos, vec2{size.x, size.y}, color{1, 0, 0, 1}, randDirection));
        pos.x += xPixel;
    }

    //rects
    bgRect = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{120,120}, color{1,1,1, 1});
    opaqueRect = make_unique<Rect>(shapeShader,vec2{width/2,height/2}, vec2{115,115}, color{0,0,0, 1} );
}

void Engine::processInput() {

    glfwPollEvents();

     //Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //setting screens when conditons are met
    if(currentLevel > 5)
        screen = won;

    if(totalTime - glfwGetTime() < 0)
        screen = lose;

}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);


    int time = totalTime - glfwGetTime();;
    string timeString = "Time:" + to_string(time);
    string levelString = "Level:" + to_string(currentLevel);


    // Set shader to use for all shapes
    shapeShader.use();

    switch(screen) {
        case play:
            //drawing rects and arrows


            bgRect->setUniforms();
            bgRect->draw();

            opaqueRect->setUniforms();
            opaqueRect->draw();

            for(int i = 0; i < arrows.size(); i++)
            {
                arrows[i]->setUniforms();
                arrows[i]->draw();
            }

            fontRenderer->renderText(timeString, (width/2)-80, (height/2)-115, 1, vec3{1, 1, 1});

            //animation
            if(currentLevel == 4){
                for(int i = 0; i < arrows.size(); i++)
                {
                    if(arrowNum != i){
                        arrows[i]->moveY(sin(glfwGetTime() + .90 ));
                    } else {
                        arrows[i]->setPosY(height/2);
                    }
                }
                cout << glfwGetTime();
            }

            if(currentLevel == 5){

                //animation just cause
                for(int i = 0; i < arrows.size(); i++)
                {
                    if(arrowNum != i){
                        if(arrows[i]->getPosY() < height/2 + 150){
                            arrows[i]->moveY(2);
                        }

                        if(arrows[i]->getPosY() == height/2 + 150){
                            arrows[i]->setPosY(height/2 - 200);
                        }

                    } else {
                        arrows[i]->setPosY(height/2);
                    }
                }
            }
            break;
        case won:
            fontRenderer->renderText("You won!", (width/2)-50, (height/2), 1, vec3{0, 1, 0});
            break;
        case lose:
            fontRenderer->renderText("You lose ):", (width/2)-50, (height/2), 1, vec3{1, 0, 1});
            timeString = "Time: 0";
            break;
    }


    fontRenderer->renderText(levelString, (width/2)-120, (height/2)+135, 1.5, vec3{1, 1, 1});


    glfwSwapBuffers(window);
}



bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Engine::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //moving the x value of each arrow
    float movexNum = -instance->xPixel;
    int arrowKeys[4] = {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT};
    pair <Direction, Direction> arrowDirections;

    //store directions of pressed and current arrow
    for(int i = 0; i < 4; i++)
    {
        if(key == arrowKeys[i])
            arrowDirections.first = Direction(i);

    }
    arrowDirections.second = instance->arrows[arrowNum]->getDirection();

    //if the directions match make arrow opaque
    if(arrowDirections.first == arrowDirections.second && action == GLFW_PRESS)
    {
        instance->arrows[arrowNum]->setColor(opaque);
        arrowNum++;

        for(const auto & i : instance->arrows)
        {
            i->moveX(movexNum);
        }
    }


    if(arrowDirections.first != arrowDirections.second && action == GLFW_PRESS) {
       //reset
        instance->arrows.clear();
        instance->initShapes();
        arrowNum = 0;
    }

    //increment level and reset vector (end of level)
    if(arrowNum == instance->arrows.size()){
        arrowNum = 0;
        currentLevel++;
        instance->arrows.clear();
        instance->initShapes();
    }

}
