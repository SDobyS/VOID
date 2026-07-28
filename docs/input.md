# Input

The Input module completely hides SDL from the public API. Input state is updated
once every frame.

## Keyboard

Held key:

```cpp
if (Input::IsKeyDown(KEY_W))
{
}
```

Pressed this frame:

```cpp
if (Input::IsKeyPressed(KEY_SPACE))
{
}
```

Released this frame:

```cpp
if (Input::IsKeyReleased(KEY_ESCAPE))
{
}
```

## Mouse

Button state:

```cpp
if (Input::IsMouseButtonDown(MOUSE_BUTTON_LEFT))
{
}
```

Mouse position:

```cpp
glm::vec2 mouse = Input::GetMousePosition();
```
