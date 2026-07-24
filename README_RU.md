# VOID

<p align="center">

<img src="https://img.shields.io/badge/C%2B%2B-20-blue">
<img src="https://img.shields.io/badge/OpenGL-4.6-success">
<img src="https://img.shields.io/badge/SDL-3-orange">
<img src="https://img.shields.io/badge/GLAD-2-red">
<img src="https://img.shields.io/badge/CMake-3.25+-blueviolet">
<img src="https://img.shields.io/badge/License-MIT-yellow">

</p>

🇬🇧 English | 🇷🇺 Русский

Лёгкий игровой движок, написанный на современном C++ с использованием SDL3 и OpenGL.

Проект создан с нуля для изучения программирования графики, техник рендеринга и архитектуры игровых движков без использования готовых игровых движков.

## Возможности

- Управление окнами и событиями через SDL3
- OpenGL 4.6 Core Profile
- GLAD 2 загрузчик генерируется автоматически через CMake
- Современный шейдерный пайплайн с загрузкой внешних GLSL-файлов
- Vertex Buffer Objects (VBO), Vertex Array Objects (VAO) и Element Buffer Objects (EBO)
- Загрузка текстур через stb_image 
- Автоматическая запасная текстура (фиолетово-чёрный checkerboard) при отсутствии файла
- Фильтрация текстур, оптимизированная под пиксель-арт (GL_NEAREST)
- Интеграция Dear ImGui через FetchContent для отладочного интерфейса
- Динамическая коррекция соотношения сторон (aspect ratio)
- Автоматическое управление зависимостями через CMake FetchContent

## Требования

- Visual Studio 2022+
- CMake 3.25+
- Git
- Python 3+

## Сборка

Настройка:

```bash
cmake --preset windows-msvc-debug
```

Сборка:

```bash
cmake --build build
```

## Пример

<p align="center">
  <img src="docs/example.png" width="900">
</p>

_В данный момент движок отображает первый треугольник через современный OpenGL-пайплайн._

## Как это работает

Движок создаёт окно SDL3 и инициализирует контекст OpenGL 4.6 Core Profile.

GLAD загружает функции OpenGL во время выполнения программы.

Шейдеры загружаются из внешних GLSL-файлов, компилируются, связываются в шейдерную программу и используются для отрисовки геометрии.

## Структура проекта

```text
.
├── assets/
│   ├── shaders/
│   │   ├── triangle.vert
│   │   └── triangle.frag
│   └── textures/
│       └── test.png
├── docs/
│   ├── cat.gif
│   └── example.png
├── src/
│   ├── stb.cpp
│   └── main.cpp
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
├── README.md
└── README_RU.md
```

## Дорожная карта

- [x] Инициализация SDL3
- [x] Создание OpenGL контекста
- [x] Интеграция GLAD 2
- [x] Загрузка и компиляция шейдеров
- [x] Отрисовка первого треугольника
- [x] Vertex Buffer Objects (VBO)
- [x] Vertex Array Objects (VAO)
- [x] Element Buffer Objects (EBO)
- [x] Загрузка текстур + система fallback
- [x] Инициализация Dear ImGui
- [x] Динамическая обработка соотношения сторон
- [ ] Класс Sprite
- [ ] Класс Texture
- [ ] Класс Shader
- [ ] Ортографическая камера
- [ ] Batch Renderer
- [ ] Sprite Renderer
- [ ] Рендеринг текста (шрифты)
- [ ] Менеджер ввода
- [ ] Менеджер ассетов
- [ ] Система сцен
- [ ] Entity Component System (ECS)
- [ ] Tilemap
- [ ] Система анимаций
- [ ] Система частиц
- [ ] Пост-обработка
- [ ] И многое другое...

## О проекте

VOID — это персональный проект по разработке графического движка, направленный на изучение современного OpenGL и создание собственного движка с нуля.

Проект делает акцент на понимании графического пайплайна, а не на использовании готовых рендеринговых фреймворков.

## Лицензия

MIT

---

<p align="center">
  <img src="docs/cat.gif" width="800">
</p>