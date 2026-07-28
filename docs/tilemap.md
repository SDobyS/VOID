# Tilemap

VOID supports tilemaps exported from **Tiled** in JSON format. Only **tile
layers** are rendered.

## Creating a tilemap

```cpp
auto tileset = AssetManager::LoadTexture("tiles", "assets/tileset.png");

Tilemap map(tileset, 32); // 32 = tile size in pixels
```

You can also explicitly provide the number of columns in the tileset:

```cpp
Tilemap map(tileset, 32, 8);
```

## Loading a map

```cpp
if (!map.LoadFromTiledJSON("assets/level.json"))
{
    return;
}
```

The loader automatically reads map width/height, visible tile layers, tile data
and tileset columns.

## Drawing

All visible layers:

```cpp
map.Draw();
```

A single layer:

```cpp
map.DrawLayer("Foreground");
```

## Support matrix

| Feature | Supported |
|---------|:---------:|
| Embedded tilesets | ✅ |
| CSV tile layers | ✅ |
| Multiple visible layers | ✅ |
| External TSX tilesets | ❌ |
| Base64-encoded layers | ❌ |
| Infinite maps | ❌ |
