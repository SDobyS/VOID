# VOID

<p align="center">

<img src="https://img.shields.io/badge/C%2B%2B-20-blue">
<img src="https://img.shields.io/badge/OpenGL-4.6-success">
<img src="https://img.shields.io/badge/SDL-3-orange">
<img src="https://img.shields.io/badge/GLAD-2-red">
<img src="https://img.shields.io/badge/CMake-3.25+-blueviolet">
<img src="https://img.shields.io/badge/License-MIT-yellow">

</p>

🇬🇧 [English](README.md) | 🇷🇺 Русский

Лёгкий игровой движок, написанный на современном C++ с использованием SDL3 и OpenGL.

Проект создаётся полностью с нуля для изучения компьютерной графики, современных техник рендеринга и архитектуры игровых движков без использования готовых игровых движков. Он предоставляет **глобальный API в стиле Raylib**, делая разработку 2D-игр простой, понятной и удобной.

## ✨ Возможности

- **Глобальный API в стиле Raylib**: простой игровой цикл (`InitWindow`, `BeginDrawing`, `EndDrawing`, `IsKeyDown` и др.)
- **Модульная архитектура**: разделение движка на Window, Input, Graphics и Utils
- **Современный графический конвейер**: OpenGL 4.6 Core Profile с загрузчиком GLAD 2 (генерируется автоматически через CMake)
- **Объектно-ориентированные абстракции**: классы `Shader`, `Texture`, `Mesh` (VAO/VBO/EBO) и `VertexArray`
- **Встроенный 2D Renderer**: отрисовка квадов через отдельный рендерер
- **Система спрайтов**: хранение текстуры, позиции, размера, цвета и поворота
- **Ортографическая камера**: система координат Y-Down на базе GLM с автоматической коррекцией соотношения сторон
- **Современная система шейдеров**: внешние GLSL-файлы со встроенными uniform-переменными (`u_Model`, `u_ViewProj`, `u_Texture`)
- **Система ввода**: абстракция клавиатуры и мыши, полностью скрывающая SDL от игрового кода
- **Загрузка текстур**: через `stb_image` с автоматической резервной текстурой (фиолетово-чёрная шахматная сетка)
- **Оптимизация для Pixel Art**: фильтрация текстур `GL_NEAREST` по умолчанию
- **Интерфейс отладки**: интеграция Dear ImGui через CMake FetchContent
- **Инструменты разработчика**: цветной логгер с временными метками и вывод информации о системе и видеокарте
- **Автоматическое управление зависимостями**: CMake FetchContent без необходимости использовать vcpkg

## 🛠 Требования

- Visual Studio 2022+
- CMake 3.25+
- Git
- Python 3+

## 🚀 Сборка

Конфигурация:

```bash
cmake --preset windows-msvc-debug
```

Сборка:

```bash
cmake --build build
```

## 🖼 Пример

<p align="center">
  <img src="docs/example_1.png" width="900">
</p>

<p align="center">
  <img src="docs/example_2.png" width="900">
</p>

_На данный момент движок умеет отображать текстурированные 2D-спрайты с использованием собственного Renderer, Sprite и Orthographic Camera._

## ⚙️ Как это работает

VOID предоставляет лёгкий API, вдохновлённый Raylib, а внутри использует современные классы C++20 для инкапсуляции ресурсов и модульной архитектуры движка.

Движок создаёт окно SDL3, инициализирует контекст OpenGL 4.6 Core Profile и загружает функции OpenGL через GLAD во время выполнения. Вместо тяжёлого класса `Application` используются простые глобальные функции для управления окном, рендерингом и вводом.

Внутри движок состоит из следующих модулей:

- Window
- Shader
- Texture
- VertexArray
- Mesh
- Renderer
- Sprite
- OrthographicCamera
- Input
- ImGuiLayer

Игровой код подключает только один заголовочный файл:

```cpp
#include "void.h"
```

Игровой слой никогда не взаимодействует напрямую с SDL3, OpenGL, GLAD или stb_image, благодаря чему публичный API остаётся чистым и простым в использовании.

## 📂 Структура проекта

```text
.
├── assets/
│   └── engine/
│       ├── shaders/
│       │   ├── default.frag
│       │   └── default.vert
│       └── textures/
│           └── test.png
│
├── docs/
│   ├── cat.gif
│   ├── example_1.png
│   └── example_2.png
│
├── src/
│   ├── engine/
│   │   ├── graphics/
│   │   │   ├── Camera/
│   │   │   │   ├── OrthographicCamera.cpp
│   │   │   │   └── OrthographicCamera.h
│   │   │   │
│   │   │   ├── Mesh.cpp
│   │   │   ├── Mesh.h
│   │   │   ├── Renderer.cpp
│   │   │   ├── Renderer.h
│   │   │   ├── Shader.cpp
│   │   │   ├── Shader.h
│   │   │   ├── Sprite.cpp
│   │   │   ├── Sprite.h
│   │   │   ├── Texture.cpp
│   │   │   ├── Texture.h
│   │   │   ├── VertexArray.cpp
│   │   │   └── VertexArray.h
│   │   │
│   │   ├── utils/
│   │   │   ├── ConsoleColors.h
│   │   │   ├── Log.cpp
│   │   │   ├── Log.h
│   │   │   ├── SystemInfo.cpp
│   │   │   ├── SystemInfo.h
│   │   │   ├── Time.cpp
│   │   │   └── Time.h
│   │   │
│   │   ├── ImGuiLayer.cpp
│   │   ├── ImGuiLayer.h
│   │   ├── Input.cpp
│   │   ├── Input.h
│   │   ├── Window.cpp
│   │   ├── Window.h
│   │   ├── stb.cpp
│   │   └── void.h
│
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
├── README.md
└── README_RU.md
```

## 🚧 Roadmap

- [x] Инициализация SDL3
- [x] Контекст OpenGL 4.6
- [x] Интеграция GLAD2
- [x] Загрузка шейдеров
- [x] Первый отображённый треугольник
- [x] VAO / VBO / EBO
- [x] Загрузка текстур
- [x] Резервная текстура (Fallback)
- [x] Интеграция ImGui
- [x] Ортографическая камера
- [x] Система спрайтов
- [x] Абстракция Shader
- [x] Абстракция Texture
- [x] Абстракция Mesh
- [x] Абстракция VertexArray
- [x] Модульная архитектура движка
- [x] Конфигурация окна
- [x] Система ввода
- [x] Глобальный API в стиле Raylib
- [ ] Batch Renderer
- [ ] Отрисовка шрифтов
- [ ] Менеджер ресурсов
- [ ] Система сцен
- [ ] Tilemap
- [ ] Система анимации
- [ ] Система частиц
- [ ] Постобработка
- [ ] И многое другое...

## 📖 О проекте

VOID — это персональный проект по разработке игрового движка, целью которого является изучение современной компьютерной графики, архитектуры игровых движков и рендеринга в реальном времени путём создания всех компонентов с нуля.

Основная идея проекта — разобраться во всех этапах графического конвейера, не полагаясь на существующие игровые движки. В долгосрочной перспективе VOID должен стать лёгким, модульным и удобным 2D-движком для пиксельных игр с API, вдохновлённым Raylib, и современной реализацией на C++20.

## 📜 Лицензия

MIT

---

<p align="center">
  <img src="docs/cat.gif" width="800">
</p>