# Atividade Vivencial M6 – Tilemap Isométrico Diamond

**Disciplina:** Processamento Gráfico  
**Aluna:** Bruna Chies  

---

## Sobre o Projeto

Nessa atividade foi desenvolvido um tilemap isométrico no modelo **diamond** com navegação por teclado, dois modos de interação e um personagem (Messi, estilo Minecraft) que indica a posição atual no mapa.

O mapa é uma grade 10×10 renderizada em projeção ortográfica paralela com `1 unidade = 1 pixel`. Cada tile é um losango de 128×64 pixels (proporção 2:1), extraído de um tileset horizontal com 7 tiles.

---

## Modos de Jogo

### Editor
Movimento livre pelo mapa. As teclas `SPACE` e `V` trocam o tile da posição atual para frente ou para trás no tileset — funciona como um editor de mapa em tempo real.

### Puzzle
Cada vez que o personagem visita um tile, ele avança automaticamente para o próximo tipo. O último tile (rosa, índice 6) vira uma **parede** que bloqueia o movimento — o objetivo é navegar pelo mapa sem ficar preso.

---

## Como o Diamond Funciona

A conversão de coordenadas de matriz (i, j) para posição na tela segue a fórmula do modelo diamond:

```cpp
x = ORIGIN_X + (j - i) * (TILE_W / 2)
y = ORIGIN_Y + (j + i) * (TILE_H / 2)
```

Os tiles são desenhados do menor `i` para o maior (de trás para frente), garantindo a sobreposição correta pelo algoritmo do pintor.

---

## Tileset

O tileset contém 7 tiles lado a lado em `assets/tileset.png` (896×64 px). Cada tile ocupa `1/7` da largura em UV:

| Índice | Tipo |
|---|---|
| 0 | Areia |
| 1 | Grama |
| 2 | Terra |
| 3 | Lava |
| 4 | Água rasa |
| 5 | Água funda |
| 6 | Rosa (parede no puzzle) |

No fragment shader, o frame correto é selecionado com:
```glsl
float fw = 1.0 / float(nTiles);
vec2 uv = vec2(TexCoord.x * fw + float(tileIdx) * fw, TexCoord.y);
```

---

## Controles

| Tecla | Ação |
|---|---|
| `Q` `W` `E` | NW / N / NE |
| `A` `D` | W / E |
| `Z` `X` `C` | SW / S / SE |
| `SPACE` | Próximo tile (modo Editor) |
| `V` | Tile anterior (modo Editor) |
| `M` | Alternar Editor ↔ Puzzle |
| `R` | Resetar mapa |
| `ESC` | Fechar |

---

## Tecnologias

- **OpenGL 4.1 Core Profile** — renderização
- **GLFW** — janela e input
- **GLEW** — extensões OpenGL
- **GLM** — projeção ortográfica (`glm::ortho`)
- **stb_image** — carregamento de PNG

Projeção usada:
```cpp
glm::ortho(0.0f, (float)SCREEN_W, (float)SCREEN_H, 0.0f)
// Y para baixo, 1 unidade = 1 pixel
```

---

## Como Compilar e Executar

### Dependências (macOS)

```bash
brew install cmake glfw glew glm
```

### Compilação

```bash
cd M6_Atividade_Vivencial_Bruna_Chies
cmake -B build
cmake --build build
./build/tilemap
```

---

## Estrutura do Projeto

```
tilemap/
├── src/
│   ├── main.cpp        ← código principal
│   └── stb_image.h     ← carregamento de imagens
├── assets/
│   ├── tileset.png     ← 7 tiles isométricos (896×64)
│   └── player.png      ← sprite do Messi (144×256)
└── CMakeLists.txt
```
