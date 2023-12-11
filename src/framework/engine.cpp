#include "engine.h"
#include "../shapes/arrow.h"

Engine* Engine::instance = nullptr;
enum state {start, play, over};
state screen;
static int arrowNum = 0;

color opaque = {0,0,0,0};

// Colors
color originalFill, hoverFill, pressFill;

Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
    instance = this;

    originalFill = {1, 0, 0, 1};
    hoverFill.vec = originalFill.vec + vec4{0.5, 0.5, 0.5, 0};
    pressFill.vec = originalFill.vec - vec4{0.5, 0.5, 0.5, 0};
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

    glfwSetKeyCallback(window, key_callback);


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
    // red spawn button centered in the top left corner
    float x = 200;
    float y= 100;
    float pivot_x = x;
    float pivot_y = y + 80.0f;

//    arrow = make_unique<Arrow>(shapeShader, vec2{width/2,height/2}, vec2{200, 100}, color{1, 0, 0, 1});

    vec2 pos = {0, height/2};
    vec2 size = {30, 20};

    for(int i = 0; i < 5; i++)
    {

        //hover squares are opacity 0 until hovered
        level1.push_back(std::make_unique<Arrow>(shapeShader, pos, vec2{size.x + 5, size.y + 5}, color{1, 0, 0, 1}, DOWN));
        pos.x += 50;
    }

    level1.push_back(std::make_unique<Arrow>(shapeShader, pos, vec2{size.x + 5, size.y + 5}, color{1, 0, 0, 1}, UP));
    pos.x +=50;
    level1.push_back(std::make_unique<Arrow>(shapeShader, pos, vec2{size.x + 5, size.y + 5}, color{1, 0, 0, 1}, RIGHT));
    pos.x +=50;
    level1.push_back(std::make_unique<Arrow>(shapeShader, pos, vec2{size.x + 5, size.y + 5}, color{1, 0, 0, 1}, LEFT));

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


    if(keys[GLFW_KEY_UP] && level1[arrowNum]->getDirection() == UP){
        level1[arrowNum]->setColor(opaque);
        arrowNum++;
    }

    cout << arrowNum;

    bool downPressed = keys[GLFW_KEY_DOWN] == GLFW_PRESS;

//    if( downPressedLastFrame != downPressed && level1[arrowNum]->getDirection() == DOWN){
//
//    }
//
//
//    downPressedLastFrame = keys[GLFW_KEY_DOWN];
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;



}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

//    arrow->setUniforms();
//    arrow->draw();

    for(int i = 0; i < level1.size(); i++)
    {
        level1[i]->setUniforms();
        level1[i]->draw();
    }


    glfwSwapBuffers(window);
}



bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        cout << "YO" << endl;
        //need instance to interact in static function
        instance->level1[arrowNum]->setColor(opaque);
        arrowNum++;
    }`
}
