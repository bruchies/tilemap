/*
 * ATIVIDADE VIVENCIAL M6 – TILEMAP ISOMÉTRICO DIAMOND
 * Bruna Chies e Thais Oliveira
 *
 * Tileset: 7 tiles de 128×64 (areia, grama, terra, lava, água rasa, água funda, rosa)
 * Player:  Messi (player.png)
 *
 * Controles:
 *  - Q/W/E/A/D/Z/X/C : mover (8 direções)
 *  - SPACE / V       : próximo / anterior tile (modo EDITOR)
 *  - M               : alternar EDITOR ↔ PUZZLE
 *  - R               : resetar mapa
 *  - ESC             : fechar
 *
 * Modos:
 *  EDITOR  – movimento livre, SPACE/V trocam o tile atual
 *  PUZZLE  – cada visita avança o tile; tile rosa (6) vira parede e bloqueia
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <cstring>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ── Bitmap font 8×8 ─────────────────────────────
static uint8_t FONT[128][8];

static void initFont()
{
    memset(FONT, 0, sizeof(FONT));
    static const uint8_t dig[10][8] = {
        {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00},
        {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
        {0x3C,0x66,0x06,0x0C,0x18,0x30,0x7E,0x00},
        {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
        {0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00},
        {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
        {0x3C,0x66,0x60,0x7C,0x66,0x66,0x3C,0x00},
        {0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00},
        {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},
        {0x3C,0x66,0x66,0x3E,0x06,0x66,0x3C,0x00},
    };
    for (int i = 0; i < 10; i++) memcpy(FONT['0'+i], dig[i], 8);
    static const uint8_t let[26][8] = {
        {0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00},
        {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
        {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
        {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
        {0x7E,0x60,0x60,0x78,0x60,0x60,0x7E,0x00},
        {0x7E,0x60,0x60,0x78,0x60,0x60,0x60,0x00},
        {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3E,0x00},
        {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
        {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
        {0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00},
        {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},
        {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
        {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},
        {0x66,0x76,0x7E,0x6E,0x66,0x66,0x66,0x00},
        {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
        {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
        {0x3C,0x66,0x66,0x66,0x6E,0x3C,0x06,0x00},
        {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00},
        {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
        {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
        {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
        {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
        {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},
        {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
        {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00},
        {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00},
    };
    for (int i = 0; i < 26; i++) memcpy(FONT['A'+i], let[i], 8);
    static const uint8_t colon[8] = {0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00};
    static const uint8_t slash[8] = {0x02,0x06,0x0C,0x18,0x30,0x60,0x40,0x00};
    static const uint8_t dash[8]  = {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00};
    memcpy(FONT[':'], colon, 8);
    memcpy(FONT['/'], slash, 8);
    memcpy(FONT['-'], dash,  8);
}

// ── Janela ──────────────────────────────────────
static const int SCREEN_W = 1280;
static const int SCREEN_H = 720;

// ── Tileset ─────────────────────────────────────
static const int   TILE_W    = 128;
static const int   TILE_H    = 64;
static const int   NUM_TILES = 7;
static const int   WALL_TILE = NUM_TILES - 1;   // tile rosa = parede no puzzle

// ── Mapa ────────────────────────────────────────
static const int MAP_ROWS = 10;
static const int MAP_COLS = 10;

static int mapInicial[MAP_ROWS][MAP_COLS] = {
    { 5, 5, 5, 5, 5, 5, 4, 4, 4, 4 },
    { 5, 5, 4, 4, 5, 5, 4, 0, 0, 4 },
    { 5, 4, 1, 1, 4, 4, 0, 0, 0, 4 },
    { 5, 4, 1, 2, 1, 4, 0, 1, 0, 4 },
    { 5, 4, 2, 1, 2, 4, 0, 0, 0, 4 },
    { 5, 4, 1, 2, 1, 4, 4, 0, 4, 4 },
    { 5, 4, 1, 1, 4, 4, 4, 4, 4, 4 },
    { 5, 5, 4, 4, 4, 4, 4, 4, 4, 4 },
    { 5, 5, 5, 4, 4, 4, 4, 4, 5, 5 },
    { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
};

static int tileMap[MAP_ROWS][MAP_COLS];

// ── Estado ──────────────────────────────────────
static int  curI       = MAP_ROWS / 2;
static int  curJ       = MAP_COLS / 2;
static bool puzzleMode = false;
static bool keyPressed = false;

// ── Conversão diamond ───────────────────────────
static const float ORIGIN_X = SCREEN_W * 0.5f;
static const float ORIGIN_Y = 30.0f;

static glm::vec2 cellToScreen(int i, int j)
{
    float x = ORIGIN_X + (j - i) * (TILE_W * 0.5f);
    float y = ORIGIN_Y + (j + i) * (TILE_H * 0.5f);
    return glm::vec2(x, y);
}

// ── Textura ──────────────────────────────────────
static GLuint loadTexture(const char* path)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int w, h, ch;
    stbi_set_flip_vertically_on_load(0);
    unsigned char* data = stbi_load(path, &w, &h, &ch, 0);
    if (!data) { fprintf(stderr, "[ERRO] %s\n", path); return 0; }
    GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tex;
}

// ── Shaders ──────────────────────────────────────
static GLuint compileProgram(const char* vs, const char* fs)
{
    auto compile = [](GLenum type, const char* src) -> GLuint {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, NULL);
        glCompileShader(s);
        int ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) { char log[512]; glGetShaderInfoLog(s,512,NULL,log); fprintf(stderr,"Shader: %s\n",log); }
        return s;
    };
    GLuint v = compile(GL_VERTEX_SHADER, vs);
    GLuint f = compile(GL_FRAGMENT_SHADER, fs);
    GLuint p = glCreateProgram();
    glAttachShader(p,v); glAttachShader(p,f);
    glLinkProgram(p);
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

// Shader de tile: UV slice do tileset
static const char* VS_TILE =
    "#version 410\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec2 aTex;\n"
    "out vec2 vTex;\n"
    "uniform mat4 proj;\n"
    "uniform vec2 origin;\n"
    "void main() {\n"
    "    gl_Position = proj * vec4(origin + aPos, 0.0, 1.0);\n"
    "    vTex = aTex;\n"
    "}\n";

static const char* FS_TILE =
    "#version 410\n"
    "in vec2 vTex;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D tex;\n"
    "uniform int tileIdx;\n"
    "uniform int nTiles;\n"
    "void main() {\n"
    "    float fw = 1.0 / float(nTiles);\n"
    "    vec2 uv = vec2(vTex.x * fw + float(tileIdx) * fw, vTex.y);\n"
    "    vec4 c = texture(tex, uv);\n"
    "    if (c.a < 0.05) discard;\n"
    "    FragColor = c;\n"
    "}\n";

// Shader do player (textura simples)
static const char* FS_SPRITE =
    "#version 410\n"
    "in vec2 vTex;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "    vec4 c = texture(tex, vTex);\n"
    "    if (c.a < 0.1) discard;\n"
    "    FragColor = c;\n"
    "}\n";

// Shader de cor sólida (outline do cursor + HUD)
static const char* VS_COLOR =
    "#version 410\n"
    "layout(location=0) in vec2 aPos;\n"
    "uniform mat4 proj;\n"
    "uniform vec2 origin;\n"
    "void main() {\n"
    "    gl_Position = proj * vec4(origin + aPos, 0.0, 1.0);\n"
    "}\n";

static const char* FS_COLOR =
    "#version 410\n"
    "out vec4 FragColor;\n"
    "uniform vec4 color;\n"
    "void main() { FragColor = color; }\n";

// ── HUD: quad unitário e helpers de texto ────────
static GLuint g_hudVAO, g_hudVBO, g_hudEBO;
static GLuint g_progHUD;
static int    g_winW = SCREEN_W, g_winH = SCREEN_H;

static void initHUDQuad()
{
    float v[] = { 0,0, 1,0, 1,1, 0,1 };
    GLuint idx[] = { 0,1,2, 2,3,0 };
    glGenVertexArrays(1, &g_hudVAO); glBindVertexArray(g_hudVAO);
    glGenBuffers(1, &g_hudVBO); glBindBuffer(GL_ARRAY_BUFFER, g_hudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glGenBuffers(1, &g_hudEBO); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_hudEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

static void drawRect(float px, float py, float pw, float ph,
                     float r, float g, float b, float a = 1.0f)
{
    float nx =  (px / g_winW) * 2.0f - 1.0f;
    float ny =  1.0f - ((py + ph) / g_winH) * 2.0f;
    float nw =  (pw / g_winW) * 2.0f;
    float nh =  (ph / g_winH) * 2.0f;
    glUniform2f(glGetUniformLocation(g_progHUD, "uPos"),   nx, ny);
    glUniform2f(glGetUniformLocation(g_progHUD, "uSize"),  nw, nh);
    glUniform4f(glGetUniformLocation(g_progHUD, "uColor"), r, g, b, a);
    glBindVertexArray(g_hudVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void drawChar(float x, float y, float ps, char ch, float r, float g, float b)
{
    uint8_t c = (uint8_t)ch & 0x7F;
    if (c >= 'a' && c <= 'z') c -= 32;
    for (int row = 0; row < 8; row++) {
        uint8_t bits = FONT[c][row];
        for (int col = 0; col < 8; col++)
            if (bits & (0x80u >> col))
                drawRect(x + col*ps, y + row*ps, ps, ps, r, g, b);
    }
}

static void drawText(float x, float y, float ps, const char* text, float r, float g, float b)
{
    for (const char* p = text; *p; ++p, x += 9*ps)
        drawChar(x, y, ps, *p, r, g, b);
}

// ── main ─────────────────────────────────────────
int main()
{
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(SCREEN_W, SCREEN_H,
        "M6 | Tilemap Diamond  [EDITOR]", NULL, NULL);
    if (!win) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 proj = glm::ortho(0.0f, (float)SCREEN_W, (float)SCREEN_H, 0.0f);

    // ── VAO do tile: quad centrado em X, base em Y=0 ──
    // diamond: topo = (0, 0), direita = (TW/2, TH/2), base = (0, TH), esquerda = (-TW/2, TH/2)
    float tileVerts[] = {
        -(float)TILE_W*0.5f, 0.0f,         0.0f, 0.0f,
         (float)TILE_W*0.5f, 0.0f,         1.0f, 0.0f,
         (float)TILE_W*0.5f, (float)TILE_H, 1.0f, 1.0f,
        -(float)TILE_W*0.5f, (float)TILE_H, 0.0f, 1.0f,
    };
    GLuint tileIdx[] = { 0,1,2, 2,3,0 };

    GLuint tileVAO, tileVBO, tileEBO;
    glGenVertexArrays(1, &tileVAO); glBindVertexArray(tileVAO);
    glGenBuffers(1, &tileVBO); glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tileVerts), tileVerts, GL_STATIC_DRAW);
    glGenBuffers(1, &tileEBO); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tileEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIdx), tileIdx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // ── VAO do outline: losango sem preenchimento ──
    float hw = TILE_W * 0.5f, hh = TILE_H * 0.5f;
    float outlineVerts[] = {
        0,   0,          // topo
        hw,  hh,         // direita
        0,   (float)TILE_H, // base
        -hw, hh,         // esquerda
    };
    GLuint outlineVAO, outlineVBO;
    glGenVertexArrays(1, &outlineVAO); glBindVertexArray(outlineVAO);
    glGenBuffers(1, &outlineVBO); glBindBuffer(GL_ARRAY_BUFFER, outlineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(outlineVerts), outlineVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // ── VAO do player ──
    const float PW = 48.0f, PH = 80.0f;
    float sprVerts[] = {
        -PW*0.5f, -PH,   0.0f, 0.0f,
         PW*0.5f, -PH,   1.0f, 0.0f,
         PW*0.5f,  0.0f, 1.0f, 1.0f,
        -PW*0.5f,  0.0f, 0.0f, 1.0f,
    };
    GLuint sprIdx[] = { 0,1,2, 2,3,0 };
    GLuint sprVAO, sprVBO, sprEBO;
    glGenVertexArrays(1, &sprVAO); glBindVertexArray(sprVAO);
    glGenBuffers(1, &sprVBO); glBindBuffer(GL_ARRAY_BUFFER, sprVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprVerts), sprVerts, GL_STATIC_DRAW);
    glGenBuffers(1, &sprEBO); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprIdx), sprIdx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint progTile   = compileProgram(VS_TILE,  FS_TILE);
    GLuint progSprite = compileProgram(VS_TILE,  FS_SPRITE);
    GLuint progColor  = compileProgram(VS_COLOR, FS_COLOR);

    // HUD
    static const char* VS_HUD_SRC =
        "#version 410\n"
        "layout(location=0) in vec2 aPos;\n"
        "uniform vec2 uPos; uniform vec2 uSize;\n"
        "void main() { gl_Position = vec4(uPos + aPos * uSize, 0.0, 1.0); }\n";
    static const char* FS_HUD_SRC =
        "#version 410\n"
        "out vec4 FragColor; uniform vec4 uColor;\n"
        "void main() { FragColor = uColor; }\n";
    g_progHUD = compileProgram(VS_HUD_SRC, FS_HUD_SRC);
    initFont();
    initHUDQuad();

    GLuint texTileset = loadTexture("assets/tileset.png");
    GLuint texPlayer  = loadTexture("assets/player.png");

    // Inicializa mapa
    memcpy(tileMap, mapInicial, sizeof(mapInicial));

    printf("\n========================================\n");
    printf("  M6 | TILEMAP ISOMETRICO DIAMOND\n");
    printf("========================================\n");
    printf("  Q/W/E/A/D/Z/X/C : mover (8 dirs)\n");
    printf("  SPACE / V       : proximo/anterior tile (EDITOR)\n");
    printf("  M               : EDITOR <-> PUZZLE\n");
    printf("  R               : resetar mapa\n");
    printf("  ESC             : fechar\n");
    printf("========================================\n\n");

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(win, GLFW_TRUE);

        // ── Input (debounce) ──
        bool anyKey =
            glfwGetKey(win, GLFW_KEY_Q)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_W)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_E)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_A)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_D)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_Z)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_X)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_C)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_V)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_M)     == GLFW_PRESS ||
            glfwGetKey(win, GLFW_KEY_R)     == GLFW_PRESS;

        if (!keyPressed && anyKey) {
            keyPressed = true;

            // M: alternar modo
            if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS) {
                puzzleMode = !puzzleMode;
                glfwSetWindowTitle(win, puzzleMode
                    ? "M6 | Tilemap Diamond  [PUZZLE — rosa bloqueia]"
                    : "M6 | Tilemap Diamond  [EDITOR — SPACE/V trocam tile]");
            }

            // R: resetar mapa e posição
            if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
                memcpy(tileMap, mapInicial, sizeof(mapInicial));
                curI = MAP_ROWS / 2; curJ = MAP_COLS / 2;
            }

            // SPACE / V: editor — cicla tile
            if (!puzzleMode) {
                if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
                    tileMap[curI][curJ] = (tileMap[curI][curJ] + 1) % NUM_TILES;
                if (glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS)
                    tileMap[curI][curJ] = (tileMap[curI][curJ] + NUM_TILES - 1) % NUM_TILES;
            }

            // Navegação
            int di = 0, dj = 0;
            if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) { di=-1; dj=-1; }  // NW (igual ref)
            if (glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS) { di=+1; dj=+1; }  // SE
            if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { di=+1; dj=-1; }  // SW
            if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) { di=-1; dj=+1; }  // NE
            if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) { di= 0; dj=-1; }  // W
            if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) { di=-1; dj= 0; }  // N
            if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS) { di=+1; dj= 0; }  // S
            if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS) { di= 0; dj=+1; }  // E

            if (di != 0 || dj != 0) {
                int ni = curI + di, nj = curJ + dj;
                if (ni >= 0 && ni < MAP_ROWS && nj >= 0 && nj < MAP_COLS) {
                    if (puzzleMode && tileMap[ni][nj] == WALL_TILE) {
                        // bloqueia
                    } else {
                        curI = ni; curJ = nj;
                        if (puzzleMode)
                            tileMap[curI][curJ] = (tileMap[curI][curJ] + 1) % NUM_TILES;
                    }
                }
            }
        }
        if (!anyKey) keyPressed = false;

        // ── Render ──
        int fbW, fbH;
        glfwGetFramebufferSize(win, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        glClearColor(0.12f, 0.12f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Tiles (painter's algorithm: i crescente → trás para frente)
        glUseProgram(progTile);
        glUniformMatrix4fv(glGetUniformLocation(progTile, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniform1i(glGetUniformLocation(progTile, "tex"),    0);
        glUniform1i(glGetUniformLocation(progTile, "nTiles"), NUM_TILES);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texTileset);
        glBindVertexArray(tileVAO);

        for (int i = 0; i < MAP_ROWS; i++) {
            for (int j = 0; j < MAP_COLS; j++) {
                glm::vec2 pos = cellToScreen(i, j);
                glUniform2f(glGetUniformLocation(progTile, "origin"), pos.x, pos.y);
                glUniform1i(glGetUniformLocation(progTile, "tileIdx"), tileMap[i][j]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        // Outline amarelo no tile do cursor
        glm::vec2 curPos = cellToScreen(curI, curJ);
        glUseProgram(progColor);
        glUniformMatrix4fv(glGetUniformLocation(progColor, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniform2f(glGetUniformLocation(progColor, "origin"), curPos.x, curPos.y);
        glUniform4f(glGetUniformLocation(progColor, "color"),  1.0f, 1.0f, 0.0f, 1.0f);
        glBindVertexArray(outlineVAO);
        glLineWidth(3.0f);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glLineWidth(1.0f);

        // Player centrado no tile do cursor
        glUseProgram(progSprite);
        glUniformMatrix4fv(glGetUniformLocation(progSprite, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniform1i(glGetUniformLocation(progSprite, "tex"), 0);
        // âncora: centro-baixo do tile = pos + (0, TH/2)
        glm::vec2 plPos = curPos + glm::vec2(0.0f, TILE_H * 0.5f);
        glUniform2f(glGetUniformLocation(progSprite, "origin"), plPos.x, plPos.y);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texPlayer);
        glBindVertexArray(sprVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // ── HUD: legenda no canto superior esquerdo ──
        glUseProgram(g_progHUD);
        const float PS = 1.5f, LH = 14.0f, PAD = 6.0f;
        const float BW = 310.0f, BH = puzzleMode ? 5*LH+2*PAD : 6*LH+2*PAD;
        // fundo semi-transparente
        drawRect(8, 8, BW, BH, 0.0f, 0.0f, 0.0f, 0.6f);

        float cy = 8 + PAD;
        if (puzzleMode) {
            drawText(8+PAD, cy, PS, "MODO: PUZZLE", 1.0f, 0.5f, 0.2f); cy += LH;
            drawText(8+PAD, cy, PS, "Q/W/E/A/D/Z/X/C - MOVER", 1.0f, 1.0f, 1.0f); cy += LH;
            drawText(8+PAD, cy, PS, "TILE ROSA - PAREDE/BLOQUEIO", 1.0f, 0.5f, 0.5f); cy += LH;
            drawText(8+PAD, cy, PS, "M - MODO EDITOR", 0.8f, 0.8f, 0.8f); cy += LH;
            drawText(8+PAD, cy, PS, "R - RESETAR MAPA", 0.8f, 0.8f, 0.8f);
        } else {
            drawText(8+PAD, cy, PS, "MODO: EDITOR", 0.3f, 1.0f, 0.5f); cy += LH;
            drawText(8+PAD, cy, PS, "Q/W/E/A/D/Z/X/C - MOVER", 1.0f, 1.0f, 1.0f); cy += LH;
            drawText(8+PAD, cy, PS, "SPACE - PROXIMO TILE", 1.0f, 1.0f, 1.0f); cy += LH;
            drawText(8+PAD, cy, PS, "V - TILE ANTERIOR", 1.0f, 1.0f, 1.0f); cy += LH;
            drawText(8+PAD, cy, PS, "M - MODO PUZZLE", 0.8f, 0.8f, 0.8f); cy += LH;
            drawText(8+PAD, cy, PS, "R - RESETAR MAPA", 0.8f, 0.8f, 0.8f);
        }

        glfwSwapBuffers(win);
    }

    glDeleteVertexArrays(1, &tileVAO);  glDeleteBuffers(1, &tileVBO);  glDeleteBuffers(1, &tileEBO);
    glDeleteVertexArrays(1, &outlineVAO); glDeleteBuffers(1, &outlineVBO);
    glDeleteVertexArrays(1, &sprVAO);   glDeleteBuffers(1, &sprVBO);   glDeleteBuffers(1, &sprEBO);
    glDeleteTextures(1, &texTileset);   glDeleteTextures(1, &texPlayer);
    glDeleteProgram(progTile); glDeleteProgram(progSprite); glDeleteProgram(progColor);

    glfwTerminate();
    return 0;
}
